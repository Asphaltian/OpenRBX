#include "v8kernel/Body.h"

#include "decomp.h"

namespace RBX {

// STUB: WEBSERVICE 0x1009ad20
void Body::updatePV() const
{
	STUB(0x1009ad20);
}

// STUB: WEBSERVICE 0x10104be0
void Body::setVelocity(const Velocity& velocity)
{
	STUB(0x10104be0);
}

// STUB: WEBSERVICE 0x10104c60
void Body::setMass(float mass)
{
	STUB(0x10104c60);
}

// STUB: WEBSERVICE 0x10104cc0
void Body::setMoment(const Matrix3& moment)
{
	STUB(0x10104cc0);
}

// STUB: WEBSERVICE 0x10104db0
void Body::setMeInParent(const CoordinateFrame& value)
{
	STUB(0x10104db0);
}

// STUB: WEBSERVICE 0x10104e60
void Body::setMeInParent(Link* link)
{
	STUB(0x10104e60);
}

// STUB: WEBSERVICE 0x10104ee0
void Body::setCoordinateFrame(const CoordinateFrame& value)
{
	STUB(0x10104ee0);
}

// STUB: WEBSERVICE 0x10104f60
Vector3 Body::getBranchCofmPos() const
{
	STUB(0x10104f60);
	return Vector3::zero();
}

// STUB: WEBSERVICE 0x10105390
void Body::setParent(Body* value)
{
	STUB(0x10105390);
}

// STUB: WEBSERVICE 0x101054a0
Body::Body()
{
	STUB(0x101054a0);
}

// STUB: WEBSERVICE 0x101055a0
Body::~Body()
{
	STUB(0x101055a0);
}

} // namespace RBX
