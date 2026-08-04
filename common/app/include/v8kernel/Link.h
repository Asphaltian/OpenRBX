#ifndef V8KERNEL_LINK_H
#define V8KERNEL_LINK_H

#include "decomp.h"

#include <G3D/CoordinateFrame.h>

namespace RBX {

class Body;

using G3D::CoordinateFrame;

// SIZE 0xcc
class __declspec(novtable) Link
{
public:
	Link();
	~Link();

	void reset(const CoordinateFrame& parentC, const CoordinateFrame& childC);

	const CoordinateFrame& getChildInParent() const;

	Body* getBody() { return body; }

protected:
	friend class Body;

	virtual void computeChildInParent(CoordinateFrame& answer) const = 0; // vtable+0x00

	void dirty();

	void setBody(Body* value) { body = value; }

	Body* body;                            // 0x04
	CoordinateFrame parentCoord;           // 0x08
	CoordinateFrame childCoord;            // 0x38
	CoordinateFrame childCoordInverse;     // 0x68
	mutable CoordinateFrame childInParent; // 0x98
	mutable int stateIndex;                // 0xc8
};

DECOMP_SIZE_ASSERT(Link, 0xcc)

// VTABLE: WEBSERVICE 0x102480b4
// SIZE 0xd0
class RevoluteLink : public Link
{
public:
	RevoluteLink() : jointAngle(0.0f) {}

	void setJointAngle(float value)
	{
		jointAngle = value;
		dirty();
	}

private:
	virtual void computeChildInParent(CoordinateFrame& answer) const; // vtable+0x00

	float jointAngle; // 0xcc
};

DECOMP_SIZE_ASSERT(RevoluteLink, 0xd0)

} // namespace RBX

#endif // V8KERNEL_LINK_H
