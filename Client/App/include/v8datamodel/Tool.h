#ifndef V8DATAMODEL_TOOL_H
#define V8DATAMODEL_TOOL_H

#include "decomp.h"
#include "util/ILocation.h"
#include "util/IRenderable.h"
#include "util/ISelectable3d.h"
#include "v8datamodel/Hopper.h"
#include "v8datamodel/IEquipable.h"
#include "v8tree/Instance.h"

namespace RBX {

extern const char sTool[];

// SIZE 0x238
class Tool : public DescribedCreatable<Tool, BackpackItem, sTool>,
			 public IEquipable,
			 public IRenderable,
			 virtual public ILocation,
			 virtual public ISelectable3d
{
public:
	virtual ~Tool();

private:
	virtual const CoordinateFrame getLocation() const;

public:
	enum ToolState
	{
		NOTHING = 0,
		HAS_HANDLE = 1,
		IN_WORKSPACE = 2,
		IN_CHARACTER = 3,
		HAS_TORSO = 4,
		EQUIPPED = 5,
		ACTIVATED = 6
	};

	// FUNCTION: WEBSERVICE 0x100c9db0
	int getBackendToolState() const { return backendToolState; }

	void setBackendToolState(int value);

private:
	void setBackendToolStateNoReplicate(int value);

	int backendToolState;                // 0x18c
	undefined m_unk0x190[0x230 - 0x190]; // 0x190
};

DECOMP_SIZE_ASSERT(Tool, 0x238)

// clang-format off
// FUNCTION: WEBSERVICE 0x1021c240
// RBX::`dynamic initializer for 'prop_BackendToolState''
// SYNTHETIC: WEBSERVICE 0x10223cd0
// RBX::`dynamic atexit destructor for 'prop_BackendToolState''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_TOOL_H
