#include "v8datamodel/RootInstance.h"

namespace RBX {

// STUB: WEBSERVICE 0x100c0eb0
G3D::Vector3 RootInstance::computeIdeInsertPoint() const
{
	STUB(0x100c0eb0);
	return G3D::Vector3::zero();
}

// STUB: WEBSERVICE 0x100c1020
ControllerTypeArray RootInstance::computeControllersUsed() const
{
	STUB(0x100c1020);
	return ControllerTypeArray();
}

// STUB: WEBSERVICE 0x100c1250
RootInstance::~RootInstance()
{
	STUB(0x100c1250);
}

// STUB: WEBSERVICE 0x100c22b0
RootInstance::RootInstance() : ControllersUsed(this, &RootInstance::computeControllersUsed)
{
	STUB(0x100c22b0);
}

} // namespace RBX
