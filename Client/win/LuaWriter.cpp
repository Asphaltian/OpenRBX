#include "LuaWriter.h"

#include "LDrawCommand.h"

#include <cctype>

namespace RBX {
namespace LDraw2Lua {

// FUNCTION: ROBLOX 0x00473e60
LuaWriter::LuaWriter(const char* p_unk0x04) : m_unk0x04(p_unk0x04)
{
}

// FUNCTION: ROBLOX 0x00473ec0
LuaWriter::~LuaWriter()
{
	size_t count = m_unk0x20.size();
	for (size_t i = 0; i < count; i++) {
		delete m_unk0x20.at(i);
	}

	m_unk0x20.clear();
}

// FUNCTION: ROBLOX 0x00474020
void LuaWriter::FUN_00474020(std::ostream& stream)
{
	if (m_unk0x20.size() > 0) {
		stream << "-- " << "Get the current workspace" << "\n";
		stream << "local w = game:service(\"Workspace\")" << "\n";
		stream << "\n";
		stream << "-- " << "Create a new model" << "\n";
		stream << "m = Instance.new(\"Model\")" << "\n";
		stream << "m.Name = \"" << m_unk0x04.c_str() << "\"" << "\n";
		stream << "m.Parent = w" << "\n";
		stream << "\n";

		size_t count = m_unk0x20.size();
		for (size_t i = 0; i < count; i++) {
			m_unk0x20.at(i)->VTable0x04(stream, "m");
		}
	}
}

// FUNCTION: ROBLOX 0x00474210
void LuaWriter::FUN_00474210(LDrawCommand* p_unk0x04)
{
	m_unk0x20.push_back(p_unk0x04);
}

} // namespace LDraw2Lua
} // namespace RBX

// FUNCTION: ROBLOX 0x004742e0
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

	size_t size = p_unk0x04.size();
	size_t size2 = p_unk0x08.size();
	return size2 == size ? 0 : size < size2 ? -1 : 1;
}
