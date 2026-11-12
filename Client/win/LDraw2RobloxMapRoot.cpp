#include "LDraw2RobloxMapRoot.h"

#include "LuaWriter.h"
#include "util/Name.h"
#include "v8xml/XmlElement.h"
#include "v8xml/XmlSerializer.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <memory>

namespace RBX {
namespace LDraw2Lua {

static const RBX::Name& g_unk0x00955318 = RBX::Name::declare("LDraw2RobloxMap", -1);
static const RBX::Name& g_unk0x0095531c = RBX::Name::declare("schema_version", -1);
static const RBX::Name& g_unk0x00955320 = RBX::Name::declare("color_map", -1);
static const RBX::Name& g_unk0x00955324 = RBX::Name::declare("color", -1);
static const RBX::Name& g_unk0x00955328 = RBX::Name::declare("ldraw_code", -1);
static const RBX::Name& g_unk0x0095532c = RBX::Name::declare("roblox_code", -1);
static const RBX::Name& g_unk0x00955330 = RBX::Name::declare("part_map", -1);
static const RBX::Name& g_unk0x00955334 = RBX::Name::declare("part", -1);
static const RBX::Name& g_unk0x00955338 = RBX::Name::declare("name", -1);
static const RBX::Name& g_unk0x0095533c = RBX::Name::declare("description", -1);
static const RBX::Name& g_unk0x00955340 = RBX::Name::declare("ldraw_filename", -1);
static const RBX::Name& g_unk0x00955344 = RBX::Name::declare("mapping_fidelity", -1);
static const RBX::Name& g_unk0x00955348 = RBX::Name::declare("roblox_part", -1);
static const RBX::Name& g_unk0x0095534c = RBX::Name::declare("shape", -1);
static const RBX::Name& g_unk0x00955350 = RBX::Name::declare("form_factor", -1);
static const RBX::Name& g_unk0x00955354 = RBX::Name::declare("size_vect3", -1);
static const RBX::Name& g_unk0x00955358 = RBX::Name::declare("y_offset", -1);

// FUNCTION: ROBLOX 0x004711c0
int LDraw2RobloxMapRoot::FUN_004711c0(std::string p_unk0x04)
{
	if (FUN_004742e0(p_unk0x04, "exact") == 0) {
		return 0;
	}

	if (FUN_004742e0(p_unk0x04, "close") == 0) {
		return 1;
	}

	return 2;
}

// FUNCTION: ROBLOX 0x004712c0
int LDraw2RobloxMapRoot::FUN_004712c0(std::string p_unk0x04)
{
	if (FUN_004742e0(p_unk0x04, "ball") == 0) {
		return 0;
	}

	if (FUN_004742e0(p_unk0x04, "block") == 0) {
		return 1;
	}

	if (FUN_004742e0(p_unk0x04, "cylinder") == 0) {
		return 2;
	}

	return 999;
}

// FUNCTION: ROBLOX 0x00471430
int LDraw2RobloxMapRoot::FUN_00471430(std::string p_unk0x04)
{
	if (FUN_004742e0(p_unk0x04, "symmetric") == 0) {
		return 0;
	}

	if (FUN_004742e0(p_unk0x04, "brick") == 0) {
		return 1;
	}

	return 2;
}

// FUNCTION: ROBLOX 0x004716b0
LDraw2RobloxColorMap* LDraw2RobloxMapRoot::FUN_004716b0(int p_unk0x04)
{
	std::map<int, LDraw2RobloxColorMap*>::iterator iter = m_unk0x3c.find(p_unk0x04);
	if (iter != m_unk0x3c.end()) {
		return iter->second;
	}

	return NULL;
}

// FUNCTION: ROBLOX 0x00471710
int LDraw2RobloxMapRoot::FUN_00471710(int p_unk0x04)
{
	LDraw2RobloxColorMap* color = FUN_004716b0(p_unk0x04);
	if (color) {
		return color->m_unk0x08;
	}

	return 1;
}

// FUNCTION: ROBLOX 0x00471730
LDraw2RobloxPartMap* LDraw2RobloxMapRoot::FUN_00471730(const std::string& p_unk0x04)
{
	std::map<std::string, LDraw2RobloxPartMap*>::iterator iter = m_unk0x48.find(p_unk0x04);
	if (iter != m_unk0x48.end()) {
		return iter->second;
	}

	return NULL;
}

// FUNCTION: ROBLOX 0x00471790
LDraw2RobloxPartMap* LDraw2RobloxMapRoot::FUN_00471790(const std::string& p_unk0x04)
{
	std::string key = p_unk0x04;
	std::transform(key.begin(), key.end(), key.begin(), tolower);
	return FUN_00471730(key);
}

// FUNCTION: ROBLOX 0x00471b90
void LDraw2RobloxMapRoot::FUN_00471b90(LDraw2RobloxPartMap* p_unk0x04)
{
	std::string key = p_unk0x04->FUN_00471130();
	std::transform(key.begin(), key.end(), key.begin(), tolower);

	LDraw2RobloxPartMap* part = FUN_00471730(key);
	if (part) {
		delete part;
	}

	m_unk0x48[key] = p_unk0x04;
}

// FUNCTION: ROBLOX 0x00471cb0
LDraw2RobloxMapRoot::~LDraw2RobloxMapRoot()
{
	for (std::map<int, LDraw2RobloxColorMap*>::const_iterator color = m_unk0x3c.begin(); color != m_unk0x3c.end();
		 ++color) {
		delete color->second;
	}

	for (std::map<std::string, LDraw2RobloxPartMap*>::const_iterator part = m_unk0x48.begin(); part != m_unk0x48.end();
		 ++part) {
		delete part->second;
	}
}

// STUB: ROBLOX 0x00471e70
LDraw2RobloxMapRoot::LDraw2RobloxMapRoot(std::string p_unk0x04) : m_unk0x04(p_unk0x04)
{
	std::ifstream stream(m_unk0x04.c_str());
	TextXmlParser parser(stream.rdbuf());
	std::auto_ptr<XmlElement> root = parser.parse();

	const XmlElement* element = root->findFirstChildByTag(g_unk0x0095531c);
	if (element) {
		element->getValue(m_unk0x20);
	}

	element = root->findFirstChildByTag(g_unk0x00955320);
	if (element) {
		element = element->findFirstChildByTag(g_unk0x00955324);
		std::string text;

		while (element) {
			int count = 0;
			element->findFirstChildByTag(g_unk0x00955338);

			const XmlElement* ldraw = element->findFirstChildByTag(g_unk0x00955328);
			int ldrawCode = -1;
			int robloxCode = -1;
			if (ldraw) {
				count++;
				ldraw->getValue(text);
				ldrawCode = atoi(text.c_str());
			}

			const XmlElement* roblox = element->findFirstChildByTag(g_unk0x0095532c);
			if (roblox) {
				count++;
				roblox->getValue(text);
				robloxCode = atoi(text.c_str());
			}

			if (count == 2) {
				LDraw2RobloxColorMap* map = new LDraw2RobloxColorMap(ldrawCode, robloxCode);
				LDraw2RobloxColorMap* old = FUN_004716b0(map->m_unk0x04);
				if (old) {
					delete old;
				}

				int code = map->m_unk0x04;
				m_unk0x3c[code] = map;
			}

			element = element->nextSibling();
		}
	}

	const XmlElement* parts = root->findFirstChildByTag(g_unk0x00955330);
	if (parts) {
		const XmlElement* part = parts->findFirstChildByTag(g_unk0x00955334);
		while (part) {
			int count = 0;

			const XmlElement* value = part->findFirstChildByTag(g_unk0x00955338);
			std::string name("");
			if (value) {
				count++;
				value->getValue(name);
			}

			value = part->findFirstChildByTag(g_unk0x0095533c);
			std::string description("");
			if (value) {
				count++;
				value->getValue(description);
			}

			value = part->findFirstChildByTag(g_unk0x00955340);
			std::string ldrawFilename("");
			if (value) {
				count++;
				value->getValue(ldrawFilename);
			}

			value = part->findFirstChildByTag(g_unk0x00955344);
			std::string mappingFidelity("");
			int fidelity = 2;
			if (value) {
				count++;
				value->getValue(mappingFidelity);
				fidelity = FUN_004711c0(mappingFidelity);
			}

			LDraw2RobloxPartMap* partMap = new LDraw2RobloxPartMap(name, description, ldrawFilename, fidelity);

			const XmlElement* robloxPart = part->findFirstChildByTag(g_unk0x00955348);
			if (robloxPart) {
				count++;

				while (robloxPart) {
					int partCount = 0;

					const XmlElement* item = robloxPart->findFirstChildByTag(g_unk0x0095534c);
					std::string text("");
					int shape = 999;
					if (item) {
						partCount++;
						item->getValue(text);
						shape = FUN_004712c0(text);
					}

					item = robloxPart->findFirstChildByTag(g_unk0x00955350);
					text = "";
					int formFactor = 0;
					if (item) {
						partCount++;
						item->getValue(text);
						formFactor = FUN_00471430(text);
					}

					item = robloxPart->findFirstChildByTag(g_unk0x00955354);
					std::string size("");
					if (item) {
						partCount++;
						item->getValue(size);
					}

					item = robloxPart->findFirstChildByTag(g_unk0x00955358);
					std::string yOffset("");
					if (item) {
						partCount++;
						item->getValue(yOffset);
					}

					if (partCount == 4) {
						partMap->FUN_00473100(Unknown00473110(shape, formFactor, size, yOffset));
					}

					robloxPart = robloxPart->nextSibling();
				}
			}

			if (count == 5) {
				FUN_00471b90(partMap);
			}

			part = part->nextSibling();
		}
	}
}

} // namespace LDraw2Lua
} // namespace RBX
