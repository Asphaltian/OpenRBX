#include "v8world/MultiJoint.h"

#include "decomp.h"

namespace RBX {

// STUB: WEBSERVICE 0x1011ee20
RotateJoint::~RotateJoint()
{
}

// STUB: WEBSERVICE 0x1011ee30
void RotateJoint::removeFromKernel()
{
	rotateConnector = NULL;
	MultiJoint::removeFromKernel();
}

// FUNCTION: WEBSERVICE 0x1011fcd0
MultiJoint::~MultiJoint()
{
}

// FUNCTION: WEBSERVICE 0x1011fce0
void MultiJoint::putInKernel(Kernel* kernel)
{
	IPipelined::putInKernel(kernel);
}

// FUNCTION: WEBSERVICE 0x1011fd40
Point* MultiJoint::getPoint(int index) const
{
	return point[index];
}

// STUB: WEBSERVICE 0x1011fd50
void MultiJoint::removeFromKernel()
{
	STUB(0x1011fd50);
}

// STUB: WEBSERVICE 0x1011fdf0
bool MultiJoint::isBroken() const
{
	STUB(0x1011fdf0);
	return false;
}

} // namespace RBX
