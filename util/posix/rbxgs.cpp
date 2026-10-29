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

#include <boost/shared_ptr.hpp>
#include <fstream>
#include <iostream>
#include <shlobj.h>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <vector>

class ConsoleLog : public RBX::Listener<RBX::StandardOut, RBX::StandardOutMessage>
{
public:
	ConsoleLog() : standardOut(RBX::StandardOut::singleton())
	{
		boost::mutex::scoped_lock lock(standardOut->sync);
		RBX::startListening(standardOut, this);
	}

	virtual ~ConsoleLog()
	{
		boost::mutex::scoped_lock lock(standardOut->sync);
		RBX::stopListening(standardOut, this);
	}

protected:
	virtual void onEvent(const RBX::StandardOut* source, RBX::StandardOutMessage event)
	{
		FILE* out = event.type >= RBX::MESSAGE_WARNING ? stderr : stdout;
		fprintf(out, "%s\n", event.message.c_str());
		fflush(out);
	}

private:
	boost::shared_ptr<RBX::StandardOut> standardOut;
};

static void initRoblox()
{
	RBX::Profiling::init(true);
	RBX::StandardOut::singleton()->print(RBX::MESSAGE_INFO, "Intializing Roblox Web Service");

	RBX::ContentProvider::singleton();

	char path[MAX_PATH] = "";
	if (SHGetFolderPathAndSubDir(NULL, CSIDL_COMMON_APPDATA, NULL, 0, "Roblox\\content\\", path) == S_OK) {
		RBX::ContentProvider::singleton().setAssetFolder(path);
	}

	static RBX::FactoryRegistrator registerFactoryObjects;

	RBX::Soundscape::SoundService::soundDisabled = true;
	RBX::ViewBase::getShadowsEnabled() = false;

	RBX::Network::API::init("0, 3, 676, 0");
}

static std::string readScript(const char* path)
{
	std::stringstream code;
	if (path) {
		std::ifstream file(path, std::ios::in | std::ios::binary);
		if (!file) {
			throw std::runtime_error(std::string("cannot open ") + path);
		}
		code << file.rdbuf();
	}
	else {
		code << std::cin.rdbuf();
	}
	return code.str();
}

static std::string describe(const RBX::Reflection::Value& value)
{
	if (value.isType<bool>()) {
		return value.get<bool>() ? "true" : "false";
	}
	try {
		return value.get<std::string>();
	}
	catch (std::exception&) {
		return value.type().name.c_str();
	}
}

int main(int argc, char** argv)
{
	ConsoleLog log;

	try {
		initRoblox();

		std::string code = readScript(argc > 1 ? argv[1] : NULL);
		boost::shared_ptr<RBX::DataModel> dataModel = RBX::DataModel::createDataModel();

		std::auto_ptr<std::vector<RBX::Reflection::Value> > results;
		{
			RBX::DataModel::Lock lock(dataModel);
			RBX::ScriptContext* context = dataModel->create<RBX::ScriptContext>();
			results = context->executeInNewThread(
				RBX::Security::TrustedWebService,
				code.c_str(),
				argc > 1 ? argv[1] : "stdin",
				std::vector<RBX::Reflection::Value>()
			);
		}

		for (size_t i = 0; i < results->size(); ++i) {
			printf("%s\n", describe((*results)[i]).c_str());
		}

		RBX::DataModel::closeDataModel(dataModel, true);
	}
	catch (std::exception& e) {
		fprintf(stderr, "%s\n", e.what());
		return 1;
	}

	return 0;
}
