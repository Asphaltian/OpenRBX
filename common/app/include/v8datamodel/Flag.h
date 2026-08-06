#ifndef V8DATAMODEL_FLAG_H
#define V8DATAMODEL_FLAG_H

#include "decomp.h"
#include "v8datamodel/BrickColor.h"
#include "v8datamodel/Tool.h"
#include "v8tree/Instance.h"

namespace RBX {

extern const char sFlag[];

// SIZE 0x260
class Flag : public DescribedCreatable<Flag, Tool, sFlag>
{
public:
	BrickColor getTeamColor() const { return teamColor; }

	void setTeamColor(BrickColor value);

private:
	undefined m_unk0x230[0x254 - 0x230]; // 0x230
	BrickColor teamColor;                // 0x254
};

DECOMP_SIZE_ASSERT(Flag, 0x260)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x1021c3e0
// RBX::`dynamic initializer for 'prop_Color''
// SYNTHETIC: WEBSERVICE 0x10223d60
// RBX::`dynamic atexit destructor for 'prop_Color''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_FLAG_H
