#ifndef V8KERNEL_SIMBODY_H
#define V8KERNEL_SIMBODY_H

#include "placeholder.h"
#include "util/PV.h"
#include "util/Quaternion.h"

#include <G3D/Vector3.h>

namespace RBX {

class Body;

using G3D::Vector3;

class SimBody
{
public:
	SimBody(Body* body);
	~SimBody();

	void accumulateForceCofm(const Vector3& value)
	{
		updateIfDirty();

		force += value;
	}

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

	Body* body;
	bool dirty;
	PV pv;
	Quaternion qOrientation;
	Vector3 angMomentum;
	Vector3 momentRecip;
	float massRecip;
	float constantForceY;
	Vector3 force;
	Vector3 torque;
};

inline void SimBody::resetAccumulators()
{
	updateIfDirty();
	clearAccumulators();
}

inline void SimBody::accumulateForce(const Vector3& _force, const Vector3& worldPos)
{
	updateIfDirty();

	force += _force;
	torque += (worldPos - pv.position.translation).cross(_force);
}

} // namespace RBX

#endif // V8KERNEL_SIMBODY_H
