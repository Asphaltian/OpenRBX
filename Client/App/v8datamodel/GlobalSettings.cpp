#include "v8datamodel/GlobalSettings.h"

#include "util/Name.h"

namespace RBX {

const char sGlobalSettings[] = "GlobalSettings";

boost::recursive_mutex GlobalSettings::mutex;

// STUB: ROBLOX 0x00582140
void GlobalSettings::saveState()
{
	STUB(0x582140);
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
