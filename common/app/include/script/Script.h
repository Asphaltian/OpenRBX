#ifndef SCRIPT_SCRIPT_H
#define SCRIPT_SCRIPT_H

#include "decomp.h"
#include "v8tree/Instance.h"

namespace RBX {

extern char sScript[];

extern char sLocalScript[];
// SIZE 0x128
class Script : public DescribedCreatable<Script, Instance, sScript>
{
private:
	undefined m_unk0x0f8[0x128 - 0x0f8]; // 0x0f8
};

DECOMP_SIZE_ASSERT(Script, 0x128)

// SIZE 0x128
class LocalScript : public DescribedCreatable<LocalScript, Script, sLocalScript>
{
};

DECOMP_SIZE_ASSERT(LocalScript, 0x128)

// clang-format off
// STUB: WEBSERVICE 0x10068bf0
// RBX::Script::~Script
// clang-format on

} // namespace RBX

#endif // SCRIPT_SCRIPT_H
