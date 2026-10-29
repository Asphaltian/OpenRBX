#include "ViewBase.h"
#include "network/API.h"
#include "reflection/type.h"
#include "script/ScriptContext.h"
#include "security/SecurityContext.h"
#include "util/ContentProvider.h"
#include "util/Profiling.h"
#include "util/Sound.h"
#include "util/standardout.h"
#include "v8datamodel/DataModel.h"
#include "v8datamodel/factoryregistration.h"

#include <G3D/System.h>
#include <G3D/format.h>
#include <boost/shared_ptr.hpp>
#include <fstream>
#include <iostream>
#include <lua.h>
#include <map>
#include <queue>
#include <shlobj.h>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <vector>

namespace {

const int maxTableDepth = 10;

struct LuaValue
{
	int type;
	std::string value;
	std::vector<LuaValue> table;

	LuaValue() : type(LUA_TNIL) {}
};

class StandardOutLog : public RBX::Listener<RBX::StandardOut, RBX::StandardOutMessage>
{
public:
	std::queue<RBX::StandardOutMessage> messages;

	StandardOutLog() : standardOut(RBX::StandardOut::singleton())
	{
		boost::mutex::scoped_lock lock(standardOut->sync);
		RBX::startListening(standardOut, this);
	}

	virtual ~StandardOutLog()
	{
		boost::mutex::scoped_lock lock(standardOut->sync);
		RBX::stopListening(standardOut, this);
	}

protected:
	virtual void onEvent(const RBX::StandardOut* source, RBX::StandardOutMessage event)
	{
		boost::mutex::scoped_lock lock(messagesMutex);

		messages.push(event);

		if (messages.size() > 1000) {
			messages.pop();
		}
	}

private:
	boost::shared_ptr<RBX::StandardOut> standardOut;

public:
	boost::mutex messagesMutex;
};

std::string escape(const std::string& text)
{
	std::string result;
	for (size_t i = 0; i < text.size(); ++i) {
		unsigned char c = text[i];
		if (c == '\\') {
			result += "\\\\";
		}
		else if (c == '\n') {
			result += "\\n";
		}
		else if (c == '\r') {
			result += "\\r";
		}
		else if (c == '\t') {
			result += "\\t";
		}
		else if (c < 0x20 || c >= 0x7f) {
			result += G3D::format("\\x%02x", c);
		}
		else {
			result += c;
		}
	}
	return result;
}

class Reader
{
public:
	explicit Reader(std::istream& in) : in(in) {}

	bool command(std::string& name, std::vector<std::string>& args)
	{
		std::string line;
		while (std::getline(in, line)) {
			if (line.empty()) {
				continue;
			}
			std::istringstream words(line);
			words >> name;
			args.clear();
			std::string word;
			while (words >> word) {
				args.push_back(word);
			}
			return true;
		}
		return false;
	}

	std::string field(const char* expected)
	{
		std::string name;
		std::vector<std::string> args;
		if (!command(name, args) || name != expected) {
			throw std::runtime_error(std::string("expected ") + expected);
		}
		return args.empty() ? std::string() : args[0];
	}

	std::string block(const char* expected)
	{
		size_t length = atoi(field(expected).c_str());
		std::string data(length, '\0');
		if (length > 0) {
			in.read(&data[0], length);
		}
		return data;
	}

	LuaValue value()
	{
		std::string name;
		std::vector<std::string> args;
		if (!command(name, args)) {
			throw std::runtime_error("expected value");
		}

		LuaValue result;
		if (name == "number" || name == "boolean" || name == "string") {
			result.type = name == "number" ? LUA_TNUMBER : name == "boolean" ? LUA_TBOOLEAN : LUA_TSTRING;
			size_t length = atoi(args.at(0).c_str());
			result.value.assign(length, '\0');
			if (length > 0) {
				in.read(&result.value[0], length);
			}
		}
		else if (name == "table") {
			result.type = LUA_TTABLE;
			int count = atoi(args.at(0).c_str());
			for (int i = 0; i < count; ++i) {
				result.table.push_back(value());
			}
		}
		return result;
	}

private:
	std::istream& in;
};

struct Script
{
	std::string name;
	std::vector<LuaValue> arguments;
	std::string code;
};

void convert(const LuaValue& source, RBX::Reflection::Value& dest, int depth)
{
	switch (source.type) {
	case LUA_TNUMBER:
		dest = source.value;
		dest.convert<double>();
		break;

	case LUA_TBOOLEAN:
		dest = source.value;
		dest.convert<bool>();
		break;

	case LUA_TSTRING:
		dest = source.value;
		break;

	case LUA_TTABLE: {
		if (depth == maxTableDepth) {
			break;
		}
		dest = std::vector<RBX::Reflection::Value>(source.table.size());

		std::vector<RBX::Reflection::Value>& table = dest.convert<std::vector<RBX::Reflection::Value> >();
		for (size_t i = 0; i < source.table.size(); ++i) {
			convert(source.table[i], table[i], depth + 1);
		}
		break;
	}

	default:
		break;
	}
}

void writeValue(std::ostream& out, const RBX::Reflection::Value& source, int depth, const std::string& indent)
{
	if (source.isType<void>()) {
		out << indent << "nil\n";
	}
	else if (source.isType<int>() || source.isType<float>() || source.isType<double>()) {
		out << indent << "number " << escape(source.get<std::string>()) << "\n";
	}
	else if (source.isType<bool>()) {
		out << indent << "boolean " << escape(source.get<std::string>()) << "\n";
	}
	else if (source.isType<std::string>() || source.isType<RBX::ContentId>()) {
		out << indent << "string " << escape(source.get<std::string>()) << "\n";
	}
	else if (depth < maxTableDepth && source.isType<std::vector<RBX::Reflection::Value> >()) {
		const std::vector<RBX::Reflection::Value>& table = source.cast<const std::vector<RBX::Reflection::Value>&>();
		out << indent << "table " << table.size() << "\n";
		for (size_t i = 0; i < table.size(); ++i) {
			writeValue(out, table[i], depth + 1, indent + "  ");
		}
	}
	else {
		out << indent << "nil\n";
	}
}

class WebService
{
public:
	explicit WebService(std::ostream& out) : out(out) {}

	bool openJob(const std::string& id, const Script& script, double timeout)
	{
		try {
			Job job;
			job.dataModel = RBX::DataModel::createDataModel();
			job.expiration = G3D::System::getLocalTime() + timeout;
			jobs[id] = job;
			RBX::StandardOut::singleton()->print(RBX::MESSAGE_INFO, "Opened Job %s", id.c_str());
		}
		catch (std::exception& e) {
			reportException(e);
			return false;
		}

		return execute(id, script);
	}

	bool execute(const std::string& id, const Script& script)
	{
		try {
			std::string code = script.code;
			if (RBX::ContentProvider::isHttpUrl(code)) {
				std::auto_ptr<std::istream> stream = RBX::ContentProvider::singleton().getContent(RBX::ContentId(code));
				std::stringstream content;
				content << stream->rdbuf();
				code = content.str();
			}

			boost::shared_ptr<RBX::DataModel> dataModel = getJob(id).dataModel;

			std::auto_ptr<std::vector<RBX::Reflection::Value> > tuple;
			{
				std::vector<RBX::Reflection::Value> args(script.arguments.size());
				for (size_t i = 0; i < script.arguments.size(); ++i) {
					convert(script.arguments[i], args[i], 1);
				}

				RBX::DataModel::Lock lock(dataModel);
				RBX::ScriptContext* context = dataModel->create<RBX::ScriptContext>();
				tuple = context->executeInNewThread(
					RBX::Security::TrustedWebService, code.c_str(), script.name.c_str(), args
				);
			}

			out << "count " << tuple->size() << "\n";
			for (size_t i = 0; i < tuple->size(); ++i) {
				writeValue(out, (*tuple)[i], 1, "");
			}
		}
		catch (std::exception& e) {
			reportException(e);
			return false;
		}

		return true;
	}

	void batchJob(const std::string& id, const Script& script, double timeout)
	{
		if (openJob(id, script, timeout)) {
			closeJob(id);
		}
	}

	void closeJob(const std::string& id)
	{
		try {
			std::map<std::string, Job>::iterator iter = jobs.find(id);
			if (iter != jobs.end()) {
				boost::shared_ptr<RBX::DataModel> dataModel = iter->second.dataModel;
				jobs.erase(iter);
				RBX::DataModel::closeDataModel(dataModel, true);
				RBX::StandardOut::singleton()->print(RBX::MESSAGE_INFO, "Closed Job %s", id.c_str());
			}
		}
		catch (std::exception& e) {
			reportException(e);
		}
	}

	void closeAllJobs()
	{
		try {
			for (std::map<std::string, Job>::iterator iter = jobs.begin(); iter != jobs.end(); ++iter) {
				RBX::DataModel::closeDataModel(iter->second.dataModel, true);
				RBX::StandardOut::singleton()->print(RBX::MESSAGE_INFO, "Closed Job %s", iter->first.c_str());
			}
			jobs.clear();
		}
		catch (std::exception& e) {
			reportException(e);
		}
	}

	void getAllJobs()
	{
		out << "count " << jobs.size() << "\n";
		for (std::map<std::string, Job>::iterator iter = jobs.begin(); iter != jobs.end(); ++iter) {
			out << "string " << escape(iter->first) << "\n";
		}
	}

	void getStandardOutMessages(StandardOutLog& log, size_t maxCount)
	{
		boost::mutex::scoped_lock lock(log.messagesMutex);

		while (log.messages.size() > maxCount) {
			log.messages.pop();
		}

		out << "count " << log.messages.size() << "\n";
		while (!log.messages.empty()) {
			const RBX::StandardOutMessage& message = log.messages.front();
			out << "message " << message.type << " " << escape(message.message) << "\n";
			log.messages.pop();
		}
	}

private:
	struct Job
	{
		boost::shared_ptr<RBX::DataModel> dataModel;
		double expiration;
	};

	Job& getJob(const std::string& id)
	{
		std::map<std::string, Job>::iterator iter = jobs.find(id);
		if (iter == jobs.end()) {
			throw std::runtime_error(G3D::format("Job %s not found", id.c_str()));
		}
		return iter->second;
	}

	void reportException(const std::exception& e) { out << "fault " << escape(e.what()) << "\n"; }

	std::ostream& out;
	std::map<std::string, Job> jobs;
};

void initRoblox()
{
	RBX::Profiling::init(true);
	RBX::StandardOut::singleton()->print(RBX::MESSAGE_INFO, "Intializing Roblox Web Service");

	RBX::ContentProvider::singleton();

	char path[MAX_PATH] = "";
	if (const char* content = getenv("RBXGS_CONTENT")) {
		RBX::ContentProvider::singleton().setAssetFolder(content);
	}
	else if (SHGetFolderPathAndSubDir(NULL, CSIDL_COMMON_APPDATA, NULL, 0, "Roblox\\content\\", path) != S_OK) {
		RBX::StandardOut::singleton()->print(
			RBX::MESSAGE_ERROR, "Unable to find CSIDL_COMMON_APPDATA\\Roblox\\content"
		);
	}
	else {
		RBX::ContentProvider::singleton().setAssetFolder(path);
	}

	static RBX::FactoryRegistrator registerFactoryObjects;

	RBX::Soundscape::SoundService::soundDisabled = true;
	RBX::ViewBase::getShadowsEnabled() = false;

	RBX::Network::API::init("0, 3, 676, 0");
}

Script readScript(Reader& reader)
{
	Script script;
	script.name = reader.field("name");
	int count = atoi(reader.field("args").c_str());
	for (int i = 0; i < count; ++i) {
		script.arguments.push_back(reader.value());
	}
	script.code = reader.block("script");
	return script;
}

} // namespace

int main(int argc, char** argv)
{
	std::ifstream file;
	if (argc > 1) {
		file.open(argv[1], std::ios::in | std::ios::binary);
		if (!file) {
			fprintf(stderr, "cannot open %s\n", argv[1]);
			return 1;
		}
	}

	std::ofstream results;
	if (argc > 2) {
		results.open(argv[2], std::ios::out | std::ios::binary);
		if (!results) {
			fprintf(stderr, "cannot open %s\n", argv[2]);
			return 1;
		}
	}

	std::istream& in = argc > 1 ? static_cast<std::istream&>(file) : std::cin;
	std::ostream& out = argc > 2 ? static_cast<std::ostream&>(results) : std::cout;

	StandardOutLog log;

	initRoblox();
	WebService service(out);
	Reader reader(in);

	std::string name;
	std::vector<std::string> args;
	for (int index = 0; reader.command(name, args); ++index) {
		out << "=== " << index << " " << name << "\n";

		if (name == "open") {
			Script script = readScript(reader);
			service.openJob(args.at(0), script, atof(args.at(1).c_str()));
		}
		else if (name == "execute") {
			Script script = readScript(reader);
			service.execute(args.at(0), script);
		}
		else if (name == "batch") {
			Script script = readScript(reader);
			service.batchJob(args.at(0), script, atof(args.at(1).c_str()));
		}
		else if (name == "close") {
			service.closeJob(args.at(0));
		}
		else if (name == "closeall") {
			service.closeAllJobs();
		}
		else if (name == "jobs") {
			service.getAllJobs();
		}
		else if (name == "sleep") {
			G3D::System::sleep(atoi(args.at(0).c_str()) / 1000.0);
		}
		else if (name == "messages") {
			service.getStandardOutMessages(log, atoi(args.at(0).c_str()));
		}
		else {
			fprintf(stderr, "unknown command %s\n", name.c_str());
			return 1;
		}

		out.flush();
	}

	service.closeAllJobs();
	return 0;
}
