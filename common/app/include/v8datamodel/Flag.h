#ifndef V8DATAMODEL_FLAG_H
#define V8DATAMODEL_FLAG_H

#include "decomp.h"
#include "v8datamodel/BrickColor.h"
#include "v8tree/Instance.h"

namespace RBX {

extern const char sFlag[];

// SIZE 0x260
class Flag : public Instance
{
public:
	BrickColor getTeamColor() const { return teamColor; }

	void setTeamColor(BrickColor value);

private:
	undefined m_unk0x0f8[0x254 - 0x0f8]; // 0x0f8
	BrickColor teamColor;                // 0x254
	undefined m_unk0x258[0x260 - 0x258]; // 0x258
};

DECOMP_SIZE_ASSERT(Flag, 0x260)

// clang-format off
// STUB: WEBSERVICE 0x1021c3e0
// RBX::`dynamic initializer for 'prop_Color''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_FLAG_H
