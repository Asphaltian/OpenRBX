#ifndef V8DATAMODEL_CHARACTERAPPEARANCE_H
#define V8DATAMODEL_CHARACTERAPPEARANCE_H

#include "decomp.h"
#include "util/TextureId.h"
#include "v8tree/Instance.h"

namespace RBX {

extern const char sSkin[];

extern const char sBodyColors[];

extern const char sShirt[];

extern const char sShirtGraphic[];
// SIZE 0xf8
class __declspec(novtable) CharacterAppearance : public Instance
{
};

DECOMP_SIZE_ASSERT(CharacterAppearance, 0xf8)

// SIZE 0x110
class BodyColors : public DescribedCreatable<BodyColors, CharacterAppearance, sBodyColors>
{
private:
	undefined m_unk0x0f8[0x110 - 0x0f8]; // 0x0f8
};

DECOMP_SIZE_ASSERT(BodyColors, 0x110)

// SIZE 0x118
class ShirtGraphic : public DescribedCreatable<ShirtGraphic, CharacterAppearance, sShirtGraphic>
{
private:
	TextureId graphic; // 0x0f8
};

DECOMP_SIZE_ASSERT(ShirtGraphic, 0x118)

// SIZE 0xfc
class Skin : public DescribedCreatable<Skin, CharacterAppearance, sSkin>
{
private:
	undefined m_unk0x0f8[0xfc - 0x0f8]; // 0x0f8
};

DECOMP_SIZE_ASSERT(Skin, 0xfc)

// SIZE 0x124
class Shirt : public DescribedCreatable<Shirt, ShirtGraphic, sShirt>
{
private:
	undefined m_unk0x118[0x124 - 0x118]; // 0x118
};

DECOMP_SIZE_ASSERT(Shirt, 0x124)

// clang-format off
// FUNCTION: WEBSERVICE 0x100e0930
// RBX::ShirtGraphic::~ShirtGraphic
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_CHARACTERAPPEARANCE_H
