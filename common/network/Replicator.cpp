#include "network/Replicator.h"

#include "util/Name.h"

namespace RBX {
namespace Network {

const char sReplicator[] = "NetworkReplicator";

} // namespace Network
} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::Network::sReplicator>();
template void RBX::Name::callDoDeclare<RBX::Network::sReplicator>();
