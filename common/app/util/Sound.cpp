#include "util/Sound.h"

#include "util/Name.h"
#include "v8datamodel/GameSettings.h"

char sStockSound[] = "StockSound";

namespace RBX {
namespace Soundscape {

char sSoundService[] = "SoundService";

char sSoundChannel[] = "Sound";

// STUB: WEBSERVICE 0x10080830
void SoundChannel::onEvent(const RunService* source, Heartbeat event)
{
	STUB(0x10080830);
}

} // namespace Soundscape
} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sGameSettings>();
template const RBX::Name& RBX::Name::doDeclare<sStockSound>();
template void RBX::Name::callDoDeclare<RBX::sGameSettings>();
template void RBX::Name::callDoDeclare<sStockSound>();
