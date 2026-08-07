#ifndef HUMANOID_BALANCING_H
#define HUMANOID_BALANCING_H

#include "decomp.h"
#include "humanoid/Humanoid.h"
#include "util/HitTestFilter.h"

#include <G3D/Vector3.h>
#include <boost/shared_ptr.hpp>

namespace RBX {

class PartInstance;

// SIZE 0x28
class Balancing : public Humanoid::State, public HitTestFilter
{
private:
	PartInstance* findFloor(bool existingFloor);
	PartInstance* tryFloor(const G3D::Ray& ray, float distance);
	void balance(const float stepDt, const G3D::Vector3& yAxis);

	const float kP; // 0x0c
	const float kD; // 0x10

protected:
	static const float maxTorqueComponent();
	static const float fallCutoff();
	static const float maxClimbDistance();

	virtual void onComputeForce(const float dt);                       // vtable+0x08
	virtual State* onStep(const float stepDt, Controller& controller); // vtable+0x0c
	virtual Result filterResult(const Primitive* testMe) const;        // HitTestFilter vtable+0x00

	G3D::Vector3 getYAxis() const;

	boost::shared_ptr<PartInstance> floorPart; // 0x14
	G3D::Vector3 floorTouchInWorld;            // 0x1c

public:
	Balancing(Humanoid* humanoid, const float kP, const float kD);
};

DECOMP_SIZE_ASSERT(Balancing, 0x28)

} // namespace RBX

#endif // HUMANOID_BALANCING_H
