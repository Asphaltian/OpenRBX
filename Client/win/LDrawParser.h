#ifndef LDRAWPARSER_H
#define LDRAWPARSER_H

#include "placeholder.h"

#include <ostream>
#include <string>

namespace RBX {
namespace LDraw2Lua {

class LDrawParser
{
public:
	LDrawParser(const char* p_unk0x04);

	virtual ~LDrawParser();

	void FUN_00473a80(std::ostream& stream);

	std::string m_unk0x04;
};

} // namespace LDraw2Lua
} // namespace RBX

#endif // LDRAWPARSER_H
