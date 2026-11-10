#include "LuaWriter.h"

#include <cctype>

// STUB: ROBLOX 0x004742e0
int FUN_004742e0(const std::string& p_unk0x04, const std::string& p_unk0x08)
{
	std::string::const_iterator p = p_unk0x04.begin();
	std::string::const_iterator p2 = p_unk0x08.begin();

	while (p != p_unk0x04.end() && p2 != p_unk0x08.end()) {
		if (toupper(*p) != toupper(*p2)) {
			return toupper(*p) < toupper(*p2) ? -1 : 1;
		}

		++p;
		++p2;
	}

	return p_unk0x08.size() == p_unk0x04.size() ? 0 : p_unk0x04.size() < p_unk0x08.size() ? -1 : 1;
}
