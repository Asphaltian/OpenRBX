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

} // namespace Network
} // namespace RBX

#endif // NETWORK_API_H
