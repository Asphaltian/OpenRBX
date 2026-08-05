#ifndef V8DATAMODEL_PVINSTANCE_H
#define V8DATAMODEL_PVINSTANCE_H

#include "decomp.h"
#include "v8tree/Instance.h"

namespace RBX {

// SIZE 0x170
class PVInstance : public Instance
{
public:
	// FUNCTION: WEBSERVICE 0x100990b0
	bool getShowControllerFlag() const { return showControllerFlag; }

	void setShowControllerFlag(bool value);

private:
	undefined m_unk0x0f8[0x160 - 0x0f8]; // 0x0f8
	bool showControllerFlag;             // 0x160
	undefined m_unk0x161[0x170 - 0x161]; // 0x161
};

DECOMP_SIZE_ASSERT(PVInstance, 0x170)

// clang-format off
// STUB: WEBSERVICE 0x1021a6f0
// RBX::`dynamic initializer for 'prop_ControllerFlagShown''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_PVINSTANCE_H
