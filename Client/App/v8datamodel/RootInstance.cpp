#include "v8datamodel/RootInstance.h"

namespace RBX {

char sRootInstance[] = "RootInstance";

// STUB: WEBSERVICE 0x100c0d50
void RootInstance::setInsertPoint(const G3D::Vector3& point)
{
	STUB(0x100c0d50);
}

// STUB: WEBSERVICE 0x100c0eb0
G3D::Vector3 RootInstance::computeIdeInsertPoint() const
{
	STUB(0x100c0eb0);
	return G3D::Vector3::zero();
}

// FUNCTION: WEBSERVICE 0x100c1020
ControllerTypeArray RootInstance::computeControllersUsed() const
{
	ControllerTypeArray result;

	for (unsigned int i = 0; i < numChildren(); i++) {
		const PVInstance* pv = dynamic_cast<const PVInstance*>(getChild(i));

		if (pv != NULL && pv->isControllable()) {
			result.setController(pv->getControllerType(), true);
		}
	}

	return result;
}

// STUB: WEBSERVICE 0x100c1250
RootInstance::~RootInstance()
{
	STUB(0x100c1250);
}

// STUB: WEBSERVICE 0x100c1640
void RootInstance::moveToPoint(PVInstance* pv, G3D::Vector3 point)
{
	STUB(0x100c1640);
}

// STUB: WEBSERVICE 0x100c22b0
RootInstance::RootInstance() : ControllersUsed(this, &RootInstance::computeControllersUsed)
{
	STUB(0x100c22b0);
}

} // namespace RBX
