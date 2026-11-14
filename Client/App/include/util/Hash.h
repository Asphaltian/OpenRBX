#ifndef UTIL_HASH_H
#define UTIL_HASH_H

#include "placeholder.h"

#include <string>

namespace RBX {

class Hash
{
public:
	static unsigned int hash(const std::string& p_unk0x04);
	static unsigned int hash(const void* p_unk0x04, unsigned int p_unk0x08);
	static void hashAppend(unsigned int& p_unk0x04, unsigned int p_unk0x08);
	static void hashAppend(unsigned int& p_unk0x04, const void* p_unk0x08, unsigned int p_unk0x0c);
};

} // namespace RBX

#endif // UTIL_HASH_H
