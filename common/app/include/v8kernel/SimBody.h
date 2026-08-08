#ifndef V8KERNEL_SIMBODY_H
#define V8KERNEL_SIMBODY_H

#include "decomp.h"
#include "util/PV.h"
#include "util/Quaternion.h"

#include <G3D/Vector3.h>

namespace RBX {

class Body;

using G3D::Vector3;

// SIZE 0x98
class SimBody
{
public:
	SimBody(Body* body);
	~SimBody();

	void accumulateForce(const Vector3& _force, const Vector3& worldPos);

	void accumulateTorque(const Vector3& value)
	{
		updateIfDirty();

		torque += value;
	}

	void step(float dt);

	PV getOwnerPV();

	const PV& getPV() const { return pv; }

	const Vector3& getForce() const { return force; }
	const Vector3& getTorque() const { return torque; }

	void makeDirty() { dirty = true; }

	bool getDirty() const { return dirty; }

	void resetAccumulators();

private:
	void update();

	void clearAccumulators()
	{
		force = Vector3(0.0f, constantForceY, 0.0f);
		torque = Vector3(0.0f, 0.0f, 0.0f);
	}

	void updateIfDirty()
	{
		if (dirty) {
			update();
		}
	}

	Body* body;              // 0x00
	bool dirty;              // 0x04
	PV pv;                   // 0x08
	Quaternion qOrientation; // 0x50
	Vector3 angMomentum;     // 0x60
	Vector3 momentRecip;     // 0x6c
	float massRecip;         // 0x78
	float constantForceY;    // 0x7c
	Vector3 force;           // 0x80
	Vector3 torque;          // 0x8c
};

DECOMP_SIZE_ASSERT(SimBody, 0x98)

// FUNCTION: WEBSERVICE 0x101049a0
inline void SimBody::resetAccumulators()
{
	updateIfDirty();
	clearAccumulators();
}

// FUNCTION: WEBSERVICE 0x100e1a80
inline void SimBody::accumulateForce(const Vector3& _force, const Vector3& worldPos)
{
	updateIfDirty();

	force += _force;
	torque += (worldPos - pv.position.translation).cross(_force);
}

} // namespace RBX

#endif // V8KERNEL_SIMBODY_H
