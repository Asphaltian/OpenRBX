#ifndef LDRAWCOMMAND_H
#define LDRAWCOMMAND_H

#include "LuaObjectWriter.h"
#include "decomp.h"

#include <ostream>
#include <string>

namespace RBX {
namespace LDraw2Lua {

class LDraw2RobloxMapRoot;
struct Unknown00473110;

// VTABLE: ROBLOX 0x0080ffdc
// SIZE 0x90
class LDrawCommand : public LuaObjectWriter
{
public:
	LDrawCommand(LDraw2RobloxMapRoot* p_unk0x04);

	virtual ~LDrawCommand();
	virtual bool VTable0x04(std::ostream& stream, std::string p_unk0x08); // vtable+0x04

	// SYNTHETIC: ROBLOX 0x00473310
	// RBX::LDraw2Lua::LDrawCommand::`scalar deleting destructor'

	bool FUN_00473330(std::ostream& stream, const Unknown00473110* p_unk0x08);
	bool FUN_00473500(std::ostream& stream);
	bool FUN_00473550(std::ostream& stream, std::string p_unk0x08);
	bool FUN_00473780(std::ostream& stream, std::string p_unk0x08);

	LDraw2RobloxMapRoot* m_unk0x04; // 0x04
	int m_unk0x08;                  // 0x08
	int m_unk0x0c;                  // 0x0c
	double m_unk0x10[12];           // 0x10
	std::string m_unk0x70;          // 0x70
};

DECOMP_SIZE_ASSERT(LDrawCommand, 0x90)

} // namespace LDraw2Lua
} // namespace RBX

#endif // LDRAWCOMMAND_H
