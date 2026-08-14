#ifndef V8DATAMODEL_ACCOUTREMENT_H
#define V8DATAMODEL_ACCOUTREMENT_H

#include "decomp.h"
#include "util/Handle.h"
#include "util/IRenderable.h"
#include "v8datamodel/Accoutrement.h"
#include "v8datamodel/IEquipable.h"
#include "v8tree/Instance.h"

namespace RBX {

extern const char sHat[];

extern const char sAccoutrement[];

// SIZE 0x1a0
class Accoutrement : public DescribedCreatable<Accoutrement, Instance, sAccoutrement>,
					 public IEquipable,
					 public IRenderable
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

	// FUNCTION: WEBSERVICE 0x100db430
	int getBackendAccoutrementState() const { return backendAccoutrementState; }

	void setBackendAccoutrementState(int value);

private:
	undefined m_unk0x118[0x11c - 0x118]; // 0x118
	int backendAccoutrementState;        // 0x11c
	undefined m_unk0x120[0x1a0 - 0x120]; // 0x120
};

DECOMP_SIZE_ASSERT(Accoutrement, 0x1a0)

// SIZE 0x1a0
class Hat : public DescribedCreatable<Hat, Accoutrement, sHat>
{
};

DECOMP_SIZE_ASSERT(Hat, 0x1a0)

// clang-format off
// STUB: WEBSERVICE 0x1021c870
// RBX::`dynamic initializer for 'prop_BackendAccoutrementState''
// SYNTHETIC: WEBSERVICE 0x10223f40
// RBX::`dynamic atexit destructor for 'prop_BackendAccoutrementState''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_ACCOUTREMENT_H
