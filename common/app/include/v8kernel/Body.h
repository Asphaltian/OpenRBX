#ifndef V8KERNEL_BODY_H
#define V8KERNEL_BODY_H

#include "decomp.h"
#include "util/PV.h"
#include "util/Velocity.h"

#include <G3D/CoordinateFrame.h>
#include <G3D/Matrix3.h>

namespace RBX {

using G3D::CoordinateFrame;
using G3D::Matrix3;

// SIZE 0xd0
class Body
{
public:
	Body();
	~Body();

	void updatePV() const;

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

	void setCoordinateFrame(const CoordinateFrame& value);
	void setVelocity(const Velocity& velocity);
	void setMass(float mass);
	void setMoment(const Matrix3& moment);

private:
	undefined m_unk0x00[0x84 - 0x00]; // 0x00
	int stateIndex;                   // 0x84
	PV pv;                            // 0x88
};

DECOMP_SIZE_ASSERT(Body, 0xd0)

} // namespace RBX

#endif // V8KERNEL_BODY_H
