#include "util/Hash.h"

namespace RBX {

// FUNCTION: ROBLOX 0x006665d0
void Hash::hashAppend(unsigned int& p_unk0x04, const void* p_unk0x08, unsigned int p_unk0x0c)
{
	const unsigned char* data = static_cast<const unsigned char*>(p_unk0x08);
	unsigned int a = 63689;

	for (unsigned int i = 0; i < p_unk0x0c; i++) {
		p_unk0x04 = p_unk0x04 * a + data[i];
		a = a * 378551;
	}

	p_unk0x04 &= 0x7fffffff;
}

unsigned int Hash::hash(const void* p_unk0x04, unsigned int p_unk0x08)
{
	unsigned int result = 0;
	hashAppend(result, p_unk0x04, p_unk0x08);
	return result;
}

// FUNCTION: ROBLOX 0x00666630
unsigned int Hash::hash(const std::string& p_unk0x04)
{
	return hash(p_unk0x04.c_str(), p_unk0x04.size());
}

} // namespace RBX
