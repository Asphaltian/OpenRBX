#include "LDrawParser.h"

#include "LDraw2RobloxMapRoot.h"
#include "LDrawCommand.h"
#include "LuaWriter.h"
#include "util/ContentProvider.h"

#include <fstream>
#include <memory>

namespace RBX {
namespace LDraw2Lua {

static std::auto_ptr<LDraw2RobloxMapRoot> g_unk0x0095535c;

LDrawParser::~LDrawParser()
{
}

LDrawParser::LDrawParser(const char* p_unk0x04) : m_unk0x04(p_unk0x04)
{
	if (!g_unk0x0095535c.get()) {
		g_unk0x0095535c.reset(
			new LDraw2RobloxMapRoot(RBX::ContentProvider::singleton().getAssetFile("fonts\\LDraw2RobloxMap.xml"))
		);
	}
}

void LDrawParser::FUN_00473a80(std::ostream& stream)
{
	stream << "-- " << "Roblox LDraw2Lua Conversion" << "\n";
	stream << "-- " << "FileName: " << m_unk0x04.c_str() << "\n";
	stream << "\n";

	LuaWriter writer("LDrawModel");

	{
		std::ifstream input(m_unk0x04.c_str());

		while (!input.eof()) {
			char line[2048];
			int type;
			input >> type;

			if (type == 1) {
				LDrawCommand* command = new LDrawCommand(g_unk0x0095535c.get());
				command->m_unk0x08 = 1;

				int color = -1;
				input >> color;
				command->m_unk0x0c = color;

				double value = 0.0;
				input >> value;
				command->m_unk0x10[0] = value;
				input >> value;
				command->m_unk0x10[1] = value;
				input >> value;
				command->m_unk0x10[2] = value;
				input >> value;
				command->m_unk0x10[3] = value;
				input >> value;
				command->m_unk0x10[4] = value;
				input >> value;
				command->m_unk0x10[5] = value;
				input >> value;
				command->m_unk0x10[6] = value;
				input >> value;
				command->m_unk0x10[7] = value;
				input >> value;
				command->m_unk0x10[8] = value;
				input >> value;
				command->m_unk0x10[9] = value;
				input >> value;
				command->m_unk0x10[10] = value;
				input >> value;
				command->m_unk0x10[11] = value;

				input.getline(line, 2000);
				std::string text(line);
				command->setFileName(text.substr(text.find_first_not_of(" ", 0), text.length()));
				writer.FUN_00474210(command);
			}
			else {
				input.getline(line, 2000);
			}
		}
	}

	writer.FUN_00474020(stream);
}

} // namespace LDraw2Lua
} // namespace RBX
