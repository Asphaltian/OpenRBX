#include "v8datamodel/GlobalSettings.h"

#include "util/FileSystem.h"
#include "util/Name.h"
#include "v8xml/SerializerV2.h"
#include "v8xml/XmlSerializer.h"

#include <fstream>
#include <memory>

namespace RBX {

const char sGlobalSettings[] = "GlobalSettings";

boost::recursive_mutex GlobalSettings::mutex;

// FUNCTION: ROBLOX 0x00581c60
static std::string FUN_00581c60()
{
	std::string fileName = FileSystem::getUserAppDataDirectory(true, NULL);

	if (fileName.size() > 0) {
		fileName += "\\GlobalSettings7.xml";
	}

	return fileName;
}

// FUNCTION: ROBLOX 0x00582140
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

// FUNCTION: WEBSERVICE 0x100b7370
// FUNCTION: ROBLOX 0x00582820
GlobalSettings::GlobalSettings()
{
	setName("Global Settings");
}

// FUNCTION: WEBSERVICE 0x100b74f0
// FUNCTION: ROBLOX 0x005829a0
boost::shared_ptr<GlobalSettings> GlobalSettings::singleton()
{
	boost::recursive_mutex::scoped_lock lock(mutex);

	static boost::shared_ptr<GlobalSettings> sing = Creatable<Instance>::create<GlobalSettings>();

	return sing;
}

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sGlobalSettings>();
template void RBX::Name::callDoDeclare<RBX::sGlobalSettings>();
