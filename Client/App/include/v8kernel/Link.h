#ifndef V8KERNEL_LINK_H
#define V8KERNEL_LINK_H

#include "placeholder.h"

#include <G3D/CoordinateFrame.h>

namespace RBX {

class Body;

using G3D::CoordinateFrame;

class __declspec(novtable) Link
{
public:
	Link();
	~Link();

	void reset(const CoordinateFrame& parentC, const CoordinateFrame& childC);

	const CoordinateFrame& getChildInParent() const;

	Body* getBody() const { return body; }

protected:
	friend class Body;

	virtual void computeChildInParent(CoordinateFrame& answer) const = 0;

	void dirty();

	void setBody(Body* value) { body = value; }

	Body* body;
	CoordinateFrame parentCoord;
	CoordinateFrame childCoord;
	CoordinateFrame childCoordInverse;
	mutable CoordinateFrame childInParent;
	mutable int stateIndex;
};

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
	virtual void computeChildInParent(CoordinateFrame& answer) const;

	float jointAngle;
};

} // namespace RBX

#endif // V8KERNEL_LINK_H
