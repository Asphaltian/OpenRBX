#ifndef NETWORK_API_H
#define NETWORK_API_H

#include "decomp.h"

#include <string>

namespace RBX {
namespace Network {

// SIZE 0x1
class API
{
public:
	static std::string version;

	static void init(const char* version);
};

DECOMP_SIZE_ASSERT(API, 0x1)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x1021f140
// `dynamic initializer for 'RBX::Network::API::version''
// SYNTHETIC: WEBSERVICE 0x10225d10
// `dynamic atexit destructor for 'RBX::Network::API::version''
// clang-format on

} // namespace Network
} // namespace RBX

#endif // NETWORK_API_H
