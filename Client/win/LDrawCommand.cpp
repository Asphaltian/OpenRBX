#include "LDrawCommand.h"

#include "LDraw2RobloxMapRoot.h"

#include <cstdlib>

namespace RBX {
namespace LDraw2Lua {

LuaObjectWriter::~LuaObjectWriter()
{
}

LDrawCommand::LDrawCommand(LDraw2RobloxMapRoot* p_unk0x04) : m_unk0x04(p_unk0x04)
{
	m_unk0x08 = 0;
	m_unk0x0c = 0;
	m_unk0x10[0] = 0.0;
	m_unk0x10[1] = 0.0;
	m_unk0x10[2] = 0.0;
	m_unk0x10[3] = 0.0;
	m_unk0x10[4] = 0.0;
	m_unk0x10[5] = 0.0;
	m_unk0x10[6] = 0.0;
	m_unk0x10[7] = 0.0;
	m_unk0x10[8] = 0.0;
	m_unk0x10[9] = 0.0;
	m_unk0x10[10] = 0.0;
	m_unk0x10[11] = 0.0;

	std::string text("");
}

LDrawCommand::~LDrawCommand()
{
}

bool LDrawCommand::FUN_00473330(std::ostream& stream, const Unknown00473110* p_unk0x08)
{
	stream << "-- " << "Part Description -------------------------------------" << "\n";
	stream << "part = Instance.new(\"Part\")" << "\n";

	std::string name = m_unk0x70.substr(0, m_unk0x70.find_first_of(".", 0));
	stream << "part.Name = \"" << name.c_str() << "\"" << "\n";

	int shape = p_unk0x08->m_unk0x00;
	stream << "part.Shape = " << shape << "\n";

	if (shape == 0 || shape == 2) {
		stream << "part.FormFactor = 0  -- symmetric" << "\n";
	}
	else {
		stream << "part.FormFactor = " << p_unk0x08->m_unk0x04 << "\n";
	}

	stream << "part.Size = Vector3.new(" << p_unk0x08->m_unk0x08.c_str() << ")" << "\n";
	return true;
}

bool LDrawCommand::FUN_00473500(std::ostream& stream)
{
	stream << "part.BrickColor = BrickColor.new( " << m_unk0x04->FUN_00471710(m_unk0x0c) << " )";
	stream << "\n";
	return true;
}

bool LDrawCommand::FUN_00473550(std::ostream& stream, std::string p_unk0x08)
{
	double offset = atof(p_unk0x08.c_str());
	double x = m_unk0x10[0] / 20.0;
	double y = m_unk0x10[1] / -20.0 - offset;
	double z = m_unk0x10[2] / -20.0;
	double r00 = m_unk0x10[3];
	double r01 = m_unk0x10[4];
	double r02 = m_unk0x10[5];
	double r10 = m_unk0x10[6];
	double r11 = m_unk0x10[7];
	double r12 = m_unk0x10[8];
	double r20 = m_unk0x10[9];
	double r21 = m_unk0x10[10];
	double r22 = m_unk0x10[11];

	stream << "part.CFrame = CFrame.new(" << x << "," << y << "," << z << "," << r00 << "," << r01 << "," << r02 << ","
		   << r10 << "," << r11 << "," << r12 << "," << r20 << "," << r21 << "," << r22 << ")";
	stream << "\n";
	return false;
}

bool LDrawCommand::FUN_00473780(std::ostream& stream, std::string p_unk0x08)
{
	stream << "part.Parent = " << p_unk0x08.c_str();
	stream << "\n";
	stream << "part:MakeJoints()";
	stream << "\n";
	return false;
}

bool LDrawCommand::VTable0x04(std::ostream& stream, std::string p_unk0x08)
{
	if (m_unk0x08 == 1) {
		LDraw2RobloxPartMap* map = m_unk0x04->FUN_00471790(m_unk0x70);
		if (map) {
			const Unknown00473110* part = map->FUN_00472ae0();
			if (part) {
				FUN_00473330(stream, part);
				FUN_00473500(stream);
				FUN_00473550(stream, part->m_unk0x24);
				FUN_00473780(stream, p_unk0x08);
				stream << "\n";
				return true;
			}
		}
	}

	return false;
}

} // namespace LDraw2Lua
} // namespace RBX
