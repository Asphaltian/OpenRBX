#ifndef LDRAWPARSER_H
#define LDRAWPARSER_H

#include "decomp.h"

#include <ostream>
#include <string>

namespace RBX {
namespace LDraw2Lua {

// VTABLE: ROBLOX 0x00810138
// SIZE 0x20
class LDrawParser
{
public:
	LDrawParser(const char* p_unk0x04);

	virtual ~LDrawParser();

	// SYNTHETIC: ROBLOX 0x00473a50
	// RBX::LDraw2Lua::LDrawParser::`scalar deleting destructor'

	void FUN_00473a80(std::ostream& stream);

	std::string m_unk0x04; // 0x04
};

DECOMP_SIZE_ASSERT(LDrawParser, 0x20)

} // namespace LDraw2Lua
} // namespace RBX

// SYNTHETIC: ROBLOX 0x007e5220
// RBX::LDraw2Lua::`dynamic initializer for 'g_unk0x0095535c''

// SYNTHETIC: ROBLOX 0x007f00c0
// RBX::LDraw2Lua::`dynamic atexit destructor for 'g_unk0x0095535c''

#endif // LDRAWPARSER_H
