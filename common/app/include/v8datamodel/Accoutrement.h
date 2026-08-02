#ifndef V8DATAMODEL_ACCOUTREMENT_H
#define V8DATAMODEL_ACCOUTREMENT_H

#include "decomp.h"
#include "util/Handle.h"
#include "v8datamodel/Accoutrement.h"
#include "v8tree/Instance.h"

namespace RBX {

extern const char sHat[];

extern const char sAccoutrement[];

// SIZE 0x1a0
class Accoutrement : public DescribedCreatable<Accoutrement, Instance, sAccoutrement>
{
public:
	enum AccoutrementState
	{
		NOTHING = 0,
		HAS_HANDLE = 1,
		IN_WORKSPACE = 2,
		IN_CHARACTER = 3,
		EQUIPPED = 4
	};

	void setBackendAccoutrementState(AccoutrementState value);

private:
	undefined m_unk0x0f8[0x11c - 0x0f8];        // 0x0f8
	AccoutrementState backendAccoutrementState; // 0x11c
	undefined m_unk0x120[0x1a0 - 0x120];        // 0x120
};

DECOMP_SIZE_ASSERT(Accoutrement, 0x1a0)

// SIZE 0x1a0
class Hat : public DescribedCreatable<Hat, Accoutrement, sHat>
{
};

DECOMP_SIZE_ASSERT(Hat, 0x1a0)

} // namespace RBX

#endif // V8DATAMODEL_ACCOUTREMENT_H
