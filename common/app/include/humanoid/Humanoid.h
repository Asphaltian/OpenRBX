#ifndef HUMANOID_HUMANOID_H
#define HUMANOID_HUMANOID_H

#include "decomp.h"
#include "util/Handle.h"
#include "util/IControllable.h"
#include "util/IRenderable.h"
#include "util/Name.h"
#include "util/RunStateOwner.h"
#include "v8datamodel/ICharacterSubject.h"
#include "v8datamodel/PartInstance.h"
#include "v8kernel/Connector.h"
#include "v8tree/Instance.h"

#include <boost/shared_ptr.hpp>
#include <memory>

namespace RBX {

class Body;
class Controller;
class World;

extern char sHumanoid[];

// SIZE 0x1f0
class Humanoid : public DescribedCreatable<Humanoid, Instance, sHumanoid>,
				 public Connector,
				 public IControllable,
				 public IRenderable,
				 public ICharacterSubject,
				 public Listener<RunService, Stepped>
{

public:
	Humanoid();
	virtual ~Humanoid();

	virtual void computeForce(float dt, bool throttling);
	virtual bool isControllable() const;
	virtual const CoordinateFrame getLocation() const;

	virtual bool shouldRender2d() const;
	virtual bool shouldRender3dAdorn() const;
	virtual void render2d(Adorn* adorn);
	virtual void render3dAdorn(Adorn* adorn);

	virtual ContactManager* getContactManager();
	virtual void getIgnorePrims(std::vector<const Primitive*>& prims);
	virtual void tellCameraNear(float distance);
	G3D::Vector3 getIntendedMovementVector(bool ignoreSit);
	virtual G3D::Vector3 getIntendedMovementVector();

	G3D::Vector3 updateWalkDirection();
	virtual float getIntendedRotationAboutYAxis();
	virtual void cameraSetWalkOrientation(float angle, bool value);

	virtual void onEvent(const RunService* source, Stepped event);

public:
	// SIZE 0x08
	class __declspec(novtable) State : public INamed
	{
	public:
		virtual ~State() {} // vtable+0x04

		virtual void onComputeForce(const float stepDt) = 0;                   // vtable+0x08
		virtual State* onStep(const float stepDt, Controller& controller) = 0; // vtable+0x0c

		// FUNCTION: WEBSERVICE 0x100a0710 FOLDED
		virtual float getIntendedRotationAboutYAxis() const // vtable+0x10
		{
			return 0;
		}

	protected:
		State(Humanoid* humanoid) : humanoid(humanoid) {}

		Humanoid* const humanoid; // 0x04
	};

	// FUNCTION: WEBSERVICE 0x100a0750
	float getMaxHealth() const { return maxHealth; }

	void setMaxHealth(float value);
	// FUNCTION: WEBSERVICE 0x100a0790
	float getWalkRotationalVelocity() const { return walkRotationalVelocity; }

	void setWalkRotationalVelocity(const float& value);
	// FUNCTION: WEBSERVICE 0x100a07b0
	bool getJump() const { return jump; }

	void setJump(bool value);
	// FUNCTION: WEBSERVICE 0x100a07c0
	bool getSit() const { return sit; }

	void setSit(bool value);

	PartInstance* getTorso() const;
	PartInstance* getLeftLeg() const;
	PartInstance* getRightLeg() const;

	Primitive* getTorsoPrimitive() const;
	Primitive* getLeftLegPrimitive();
	Primitive* getRightLegPrimitive();

	Body* getTorsoBody();
	Body* getRootBody();

private:
	float health;                             // 0x16c
	float maxHealth;                          // 0x170
	float walkRotationalVelocity;             // 0x174
	undefined m_unk0x178[0x1ac - 0x178];      // 0x178
	bool jump : 1;                            // 0x1ac
	bool imDead : 1;                          // 0x1ac
	bool hadHeadJoint : 1;                    // 0x1ac
	bool sit : 1;                             // 0x1ac
	boost::shared_ptr<PartInstance> head;     // 0x1b0
	boost::shared_ptr<PartInstance> torso;    // 0x1b8
	boost::shared_ptr<PartInstance> leftLeg;  // 0x1c0
	boost::shared_ptr<PartInstance> rightLeg; // 0x1c8
	boost::shared_ptr<PartInstance> rightArm; // 0x1d0
	boost::shared_ptr<PartInstance> leftArm;  // 0x1d8
	World* world;                             // 0x1e0
	std::auto_ptr<State> currentState;        // 0x1e4
};

DECOMP_SIZE_ASSERT(Humanoid, 0x1f0)
DECOMP_SIZE_ASSERT(Humanoid::State, 0x08)

// clang-format off
// STUB: WEBSERVICE 0x1021b310
// RBX::`dynamic initializer for 'propWalkRotationalVelocity''
// STUB: WEBSERVICE 0x1021b370
// RBX::`dynamic initializer for 'propJump''
// STUB: WEBSERVICE 0x1021b430
// RBX::`dynamic initializer for 'propSit''
// STUB: WEBSERVICE 0x1021b4f0
// RBX::`dynamic initializer for 'propMaxHealth''
// clang-format on

} // namespace RBX

#endif // HUMANOID_HUMANOID_H
