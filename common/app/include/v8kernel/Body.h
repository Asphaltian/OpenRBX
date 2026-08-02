#ifndef V8KERNEL_BODY_H
#define V8KERNEL_BODY_H

#include "decomp.h"
#include "util/IndexArray.h"
#include "util/PV.h"
#include "util/Velocity.h"
#include "v8kernel/Cofm.h"

#include <G3D/CoordinateFrame.h>
#include <G3D/Matrix3.h>
#include <G3D/Vector3.h>

namespace RBX {

class Link;

using G3D::CoordinateFrame;
using G3D::Matrix3;
using G3D::Vector3;

// SIZE 0xd0
class Body
{
public:
	Body();
	~Body();

	void updatePV() const;

	Body* getParent() const { return parent; }

	float getMass() const { return mass; }

	float getBranchMass() const { return cofm != NULL ? cofm->getMass() : mass; }

	int numChildren() const { return children.size(); }

	Body* getChild(int index) const { return children[index]; }

	const Vector3& getPos() const
	{
		updatePV();
		return pv.position.translation;
	}

	Matrix3 getIWorldAtPoint(const Vector3& point) const;
	Matrix3 getBranchIWorldAtPoint(const Vector3& point) const;

	int getStateIndex() const
	{
		updatePV();
		return stateIndex;
	}

	const CoordinateFrame& getCoordinateFrame() const
	{
		updatePV();
		return pv.position;
	}

	static int getNextStateIndex();

	void advanceStateIndex();

	Vector3 getBranchCofmPos() const;

	void setParent(Body* value);
	void setMeInParent(Link* link);
	void setMeInParent(const CoordinateFrame& value);

	void setCoordinateFrame(const CoordinateFrame& value);
	void setVelocity(const Velocity& velocity);
	void setMass(float mass);
	void setMoment(const Matrix3& moment);

private:
	int& getIndex() const { return index; }

	undefined m_unk0x00[0x04 - 0x00];           // 0x00
	Body* root;                                 // 0x04
	Body* parent;                               // 0x08
	mutable int index;                          // 0x0c
	IndexArray<Body, &Body::getIndex> children; // 0x10
	Cofm* cofm;                                 // 0x1c
	undefined m_unk0x20[0x80 - 0x20];           // 0x20
	float mass;                                 // 0x80
	int stateIndex;                             // 0x84
	PV pv;                                      // 0x88
};

DECOMP_SIZE_ASSERT(Body, 0xd0)

} // namespace RBX

#endif // V8KERNEL_BODY_H
