#ifndef V8KERNEL_BODY_H
#define V8KERNEL_BODY_H

#include "decomp.h"
#include "util/PV.h"
#include "util/Velocity.h"

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
	undefined m_unk0x00[0x08 - 0x00]; // 0x00
	Body* parent;                     // 0x08
	undefined m_unk0x0c[0x84 - 0x0c]; // 0x0c
	int stateIndex;                   // 0x84
	PV pv;                            // 0x88
};

DECOMP_SIZE_ASSERT(Body, 0xd0)

} // namespace RBX

#endif // V8KERNEL_BODY_H
