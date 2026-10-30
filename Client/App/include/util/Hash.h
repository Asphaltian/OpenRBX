#ifndef UTIL_HASH_H
#define UTIL_HASH_H

#include "decomp.h"

#include <string>

namespace RBX {

// SIZE 0x01
class Hash
{
public:
	static unsigned int hash(const std::string& p_unk0x04);
	static unsigned int hash(const void* p_unk0x04, unsigned int p_unk0x08);
	static void hashAppend(unsigned int& p_unk0x04, unsigned int p_unk0x08);
	static void hashAppend(unsigned int& p_unk0x04, const void* p_unk0x08, unsigned int p_unk0x0c);
};

DECOMP_SIZE_ASSERT(Hash, 0x01)

} // namespace RBX

#endif // UTIL_HASH_H
