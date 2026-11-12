#ifndef UNKNOWN00409EF0_H
#define UNKNOWN00409EF0_H

#include "decomp.h"

#include <string>

// SIZE 0x1c
class Unknown00409ef0
{
public:
	Unknown00409ef0(const char* host);
	~Unknown00409ef0();

	std::string FUN_00409f70(const char* url, const char* ticket);

private:
	std::string m_unk0x00; // 0x00
};

DECOMP_SIZE_ASSERT(Unknown00409ef0, 0x1c)

#endif // UNKNOWN00409EF0_H
