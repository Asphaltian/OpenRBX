#include "v8datamodel/GlobalSettings.h"

#include "util/FileSystem.h"
#include "util/Name.h"
#include "util/StandardOut.h"
#include "v8xml/SerializerV2.h"
#include "v8xml/XmlSerializer.h"

#include <fstream>
#include <memory>

namespace RBX {

const char sGlobalSettings[] = "GlobalSettings";

boost::recursive_mutex GlobalSettings::mutex;

static std::string FUN_00581c60()
{
	std::string fileName = FileSystem::getUserAppDataDirectory(true, NULL);

	if (fileName.size() > 0) {
		fileName += "\\GlobalSettings7.xml";
	}

	return fileName;
}

void GlobalSettings::eraseSettingsStore()
{
	DeleteFile(FUN_00581c60().c_str());
}

void GlobalSettings::loadState()
{
	std::string fileName = FUN_00581c60();

	if (fileName.size() == 0) {
		return;
	}

	try {
		std::ifstream stream(fileName.c_str(), std::ios_base::in | std::ios_base::binary);
		TextXmlParser parser(stream.rdbuf());
		std::auto_ptr<XmlElement> root(parser.parse());

		boost::recursive_mutex::scoped_lock lock(mutex);
		MergeBinder binder;
		readChildren(root.get(), binder);
		binder.resolveRefs();
	}
	catch (std::exception& e) {
		StandardOut::singleton()->print(MESSAGE_INFO, "Exception thrown in loadState %s", e.what());
	}
}

void GlobalSettings::saveState()
{
	std::string fileName = FUN_00581c60();

	if (fileName.size() == 0) {
		return;
	}

	std::auto_ptr<XmlElement> root(SerializerV2::newRootElement());
	writeChildren(root.get());

	std::ofstream stream(fileName.c_str(), std::ios_base::out | std::ios_base::binary);
	TextXmlWriter writer(stream);
	writer.serialize(root.get());
}

GlobalSettings::GlobalSettings()
{
	setName("Global Settings");
}

boost::shared_ptr<GlobalSettings> GlobalSettings::singleton()
{
	boost::recursive_mutex::scoped_lock lock(mutex);

	static boost::shared_ptr<GlobalSettings> sing = Creatable<Instance>::create<GlobalSettings>();

	return sing;
}

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sGlobalSettings>();
template void RBX::Name::callDoDeclare<RBX::sGlobalSettings>();
