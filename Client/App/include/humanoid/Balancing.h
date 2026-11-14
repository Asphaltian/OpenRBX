#ifndef HUMANOID_BALANCING_H
#define HUMANOID_BALANCING_H

#include "humanoid/Humanoid.h"
#include "placeholder.h"
#include "util/HitTestFilter.h"

#include <G3D/Vector3.h>
#include <boost/shared_ptr.hpp>

namespace RBX {

class PartInstance;

class Balancing : public Humanoid::State, public HitTestFilter
{
private:
	PartInstance* findFloor(bool existingFloor);
	PartInstance* tryFloor(const G3D::Ray& ray, float maxDistance);
	void balance(const float stepDt, const G3D::Vector3& yAxis);

	const float kP;
	const float kD;

protected:
	static const float maxTorqueComponent() { return 4000.0f; }
	static const float fallCutoff() { return 0.85f; }
	static const float maxClimbDistance();

	virtual void onComputeForce(const float dt);
	virtual State* onStep(const float stepDt, Controller& controller);
	virtual Result filterResult(const Primitive* testMe) const; // HitTestFilter vtable+0x00

	G3D::Vector3 getYAxis() const;

	boost::shared_ptr<PartInstance> floorPart;
	G3D::Vector3 floorTouchInWorld;

public:
	Balancing(Humanoid* humanoid, const float kP, const float kD);
};

} // namespace RBX

#endif // HUMANOID_BALANCING_H
