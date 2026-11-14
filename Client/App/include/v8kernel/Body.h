#ifndef V8KERNEL_BODY_H
#define V8KERNEL_BODY_H

#include "placeholder.h"
#include "util/IndexArray.h"
#include "util/Math.h"
#include "util/PV.h"
#include "util/Velocity.h"
#include "v8kernel/Cofm.h"
#include "v8kernel/KernelIndex.h"
#include "v8kernel/SimBody.h"

#include <G3D/CoordinateFrame.h>
#include <G3D/Matrix3.h>
#include <G3D/Vector3.h>

namespace RBX {

class Kernel;
class KernelData;
class Link;

using G3D::CoordinateFrame;
using G3D::Matrix3;
using G3D::Vector3;

class Body : public KernelIndex
{
public:
	Body();
	~Body();

private:
	void updatePV() const;

public:
	Body* getParent() const { return parent; }
	Link* getLink() const;
	const Body* getRootConst() const;

	Body* getRoot() { return root; }

	float getMass() const { return mass; }

	float getBranchMass() const { return cofm != NULL ? cofm->getMass() : mass; }

	int numChildren() const { return children.size(); }

	Body* getChild(int index) const { return children[index]; }

	const Vector3& getCofmOffset() const { return cofm != NULL ? cofm->getCofmInBody() : Vector3::zero(); }
	G3D::CoordinateFrame getMeInDescendant(const Body*) const;
	G3D::CoordinateFrame getMeInRoot() const;

	const Vector3& getPos() const
	{
		updatePV();
		return pv.position.translation;
	}

	Matrix3 getIBody() const { return moment; }

	Matrix3 getBranchIBody() const { return cofm != NULL ? cofm->getMoment() : moment; }

	Vector3 getIBodyV3() const { return Math::toDiagonal(getIBody()); }
	Vector3 getBranchIBodyV3() const { return Math::toDiagonal(getBranchIBody()); }

	const Vector3& getBranchForce() const
	{
		const SimBody* simBody = getRootSimBody();

		return simBody != NULL ? simBody->getForce() : Vector3::zero();
	}

	const Vector3& getBranchTorque() const
	{
		const SimBody* simBody = getRootSimBody();

		return simBody != NULL ? simBody->getTorque() : Vector3::zero();
	}

	const Velocity& getBranchVelocity() const
	{
		const SimBody* simBody = getRootSimBody();

		return simBody != NULL ? simBody->getPV().velocity : Velocity::zero();
	}

	Matrix3 getIWorld() const { return Math::momentToWorldSpace(getIBody(), getPV().position.rotation); }

	Matrix3 getBranchIWorld() const { return Math::momentToWorldSpace(getBranchIBody(), getPV().position.rotation); }

	Matrix3 getIWorldAtPoint(const Vector3& point) const;

	float kineticEnergy() const;
	float potentialEnergy() const;

	void step(float dt, bool throttling);
	bool cofmIsClean();

	CoordinateFrame getBranchCofmCoordinateFrame() const;

	const Velocity& getVelocity() const
	{
		updatePV();
		return pv.velocity;
	}
	Matrix3 getBranchIWorldAtPoint(const Vector3& point) const;

	const PV& getPV() const
	{
		updatePV();
		return pv;
	}

	void setPv(const PV& value);

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

	static Body* getWorldBody();

	static int getNextStateIndex();

	void advanceStateIndex();
	void makeStateDirty();

	void makeCofmDirty();

	Vector3 getBranchCofmPos() const;

	void accumulateForceAtBranchCofm(const Vector3& force);
	void accumulateForce(const Vector3& force, const Vector3& worldPos);
	void accumulateTorque(const Vector3& torque);
	void resetAccumulators();

	const bool getCanThrottle() const { return canThrottle; }

	void setParent(Body* value);
	void setMeInParent(Link* value);
	void setMeInParent(const CoordinateFrame& value);

	void setCoordinateFrame(const CoordinateFrame& value);
	void setVelocity(const Velocity& velocity);
	void setCanThrottle(bool value) { canThrottle = value; }
	void setMass(float value);
	void setMoment(const Matrix3& value);

private:
	friend class Kernel;
	friend class KernelData;

	void onChildAdded(Body* child);
	void onChildRemoved(Body* child);
	const Body* calcRootConst() const;

	void resetRoot(Body* value);
	bool validateParentCofmDirty();
	const G3D::CoordinateFrame& getMeInParent() const;

	Body* calcRoot();

	SimBody* getRootSimBody() { return root->simBody; }
	const SimBody* getRootSimBody() const { return root->simBody; }

	int& getIndex() { return index; }

	int& getKernelIndex() { return kernelIndex; }

	Body* root;
	Body* parent;
	mutable int index;
	IndexArray<Body, &Body::getIndex> children;
	Cofm* cofm;
	SimBody* simBody;
	bool canThrottle;
	Link* link;
	CoordinateFrame meInParent;
	Matrix3 moment;
	float mass;
	mutable int stateIndex;
	mutable PV pv;
};

inline void Body::accumulateForceAtBranchCofm(const Vector3& force)
{
	SimBody* simBody = getRootSimBody();

	if (simBody != NULL) {
		simBody->accumulateForceCofm(force);
	}
}

inline void Body::accumulateForce(const Vector3& force, const Vector3& worldPos)
{
	SimBody* simBody = getRootSimBody();

	if (simBody != NULL) {
		simBody->accumulateForce(force, worldPos);
	}
}

inline void Body::accumulateTorque(const Vector3& torque)
{
	SimBody* simBody = getRootSimBody();

	if (simBody != NULL) {
		simBody->accumulateTorque(torque);
	}
}

inline void Body::resetAccumulators()
{
	SimBody* simBody = getRootSimBody();
	if (simBody != NULL) {
		simBody->resetAccumulators();
	}
}

inline Body* Body::calcRoot()
{
	return parent != NULL ? parent->calcRoot() : this;
}

} // namespace RBX

#endif // V8KERNEL_BODY_H
