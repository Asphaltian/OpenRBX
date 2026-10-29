#include "v8datamodel/GlobalSettings.h"

#include "util/Name.h"

namespace RBX {

const char sGlobalSettings[] = "GlobalSettings";

boost::recursive_mutex GlobalSettings::mutex;

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
