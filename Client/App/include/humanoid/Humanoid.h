#ifndef HUMANOID_HUMANOID_H
#define HUMANOID_HUMANOID_H

#include "placeholder.h"
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

class Accoutrement;
class Body;
class Controller;
class JointInstance;
class ModelInstance;
class Weld;
class World;

extern char sHumanoid[];

class Humanoid : public DescribedCreatable<Humanoid, Instance, sHumanoid>,
				 public Connector,
				 public IControllable,
				 public IRenderable,
				 public ICharacterSubject,
				 public Listener<RunService, Stepped>
{
	void resetTimer();
	void getPrimitives(std::vector<Primitive*>&);
	void getAccoutrements(std::vector<boost::shared_ptr<Accoutrement> >&);

public:
	enum WalkMode
	{
		HAS_PART = 0,
		HAS_POINT = 1,
		CLICK_TO_MOVE = 2,
		DIRECTION_MOVE = 3
	};

	class State : public INamed
	{
	public:
		virtual ~State() {}

		virtual void onComputeForce(const float stepDt) = 0;
		virtual State* onStep(const float stepDt, Controller& controller) = 0;

		virtual float getIntendedRotationAboutYAxis() const { return 0; }

	protected:
		State(Humanoid* humanoid) : humanoid(humanoid) {}

		Humanoid* const humanoid;
	};

	static Reflection::SignalDesc<Humanoid, void()> event_Died;
	static Reflection::SignalDesc<Humanoid, void(float)> event_Running;
	static Reflection::SignalDesc<Humanoid, void(float)> event_Climbing;
	static Reflection::SignalDesc<Humanoid, void(bool)> event_Jumping;
	static Reflection::SignalDesc<Humanoid, void(bool)> event_FreeFalling;
	static Reflection::SignalDesc<Humanoid, void(bool)> event_GettingUp;
	static Reflection::SignalDesc<Humanoid, void(bool)> event_FallingDown;
	static Reflection::SignalDesc<Humanoid, void(bool)> event_Seated;

	virtual ~Humanoid();

	static Humanoid* modelIsCharacter(const Instance* instance);
	static Humanoid* getLocalHumanoidFromContext(const Instance* context);
	static PartInstance* getLocalHeadFromContext(const Instance* context);
	static ModelInstance* getCharacterFromHumanoid(Humanoid* humanoid);
	static PartInstance* getHeadFromCharacter(const ModelInstance* character);
	static Weld* getGrip(const Instance*);
	static const float walkSpeed();
	static void renderWaypoint(Adorn* adorn, const G3D::Vector3& waypoint);

	Body* getTorsoBody();
	Body* getRootBody();

	void setHealth(float value);
	float getHealth() const { return health; }

	void setMaxHealth(float value);
	float getMaxHealth() const { return maxHealth; }

	void takeDamage(float value);
	float relativeHealth() const;

	void setWalkDirection(const G3D::Vector3& value);
	G3D::Vector3 getWalkDirection() const { return walkDirection; }

	void setWalkRotationalVelocity(const float& value);
	float getWalkRotationalVelocity() const { return walkRotationalVelocity; }

	void setWalkToPoint(const G3D::Vector3& value);
	const G3D::Vector3& getWalkToPoint() const { return walkToPoint; }

	void setWalkToPart(PartInstance* value);
	PartInstance* getWalkToPart() const { return walkToPart.get(); }

	void setJump(bool value);
	bool getJump() const { return jump; }

	void setSit(bool value);
	bool getSit() const { return sit; }
	bool canSit() const { return !sit && currentState->getName() == "Running"; }
	World* getWorld() const { return world; }

	const G3D::Vector3& getTargetPoint() const { return targetPoint; }

	void moveTo(const G3D::Vector3& worldPosition, PartInstance* part);
	void moveTo2(G3D::Vector3 worldPosition, boost::shared_ptr<Instance> part);

	G3D::Vector3 updateWalkDirection();
	void buildJoints();
	float getTorsoHeading() const;

	Primitive* getHeadPrimitive()
	{
		PartInstance* part = getHead();

		return part != NULL ? part->getPrimitive() : NULL;
	}
	Primitive* getTorsoPrimitive() const;
	Primitive* getLeftLegPrimitive();
	Primitive* getRightLegPrimitive();
	Primitive* getLeftArmPrimitive();
	Primitive* getRightArmPrimitive();

	PartInstance* getHead() const;
	PartInstance* getTorso() const;
	G3D::CoordinateFrame getTopOfHead() const;
	PartInstance* getLeftLeg() const;
	PartInstance* getRightLeg() const;
	PartInstance* getLeftArm() const;
	PartInstance* getRightArm() const;
	G3D::CoordinateFrame getRightArmGrip() const;
	PartInstance* findRightArm() const;
	JointInstance* findRightShoulder() const;
	void setTargetPoint(const G3D::Vector3& value);
	void setTorso(PartInstance* value);
	void setLeftLeg(PartInstance* value);
	void setRightLeg(PartInstance* value);

private:
	void setState(State* state);
	void checkForJointDeath();
	bool hasWalkToPoint(G3D::Vector3& worldPosition) const;

	virtual bool shouldRender2d() const;
	virtual bool shouldRender3dAdorn() const;
	virtual void render2d(Adorn* adorn);
	virtual void render3dAdorn(Adorn* adorn);
	void renderMultiplayer(Adorn* adorn, const G3D::GCamera& camera);
	virtual bool isControllable() const;
	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);
	virtual void onAncestorChanged(const AncestorChanged& event);
	virtual bool askSetParent(const Instance* instance) const;
	virtual void onEvent(const RunService* source, Stepped event);
	void onLocalHumanoidEnteringWorkspace();
	virtual void computeForce(float dt, bool throttling);
	virtual const CoordinateFrame getLocation() const;
	virtual ContactManager* getContactManager();
	virtual void tellCameraNear(float distance);
	virtual float getIntendedRotationAboutYAxis();
	virtual void getCameraIgnorePrimitives(std::vector<const Primitive*>& primitives);
	virtual void cameraSetWalkOrientation(float rad, bool noAdjust);
	void onChangedHealth(const Reflection::PropertyDescriptor&);

public:
	virtual void getIgnorePrims(std::vector<const Primitive*>& ignore);

	Humanoid();

private:
	G3D::Vector3 getIntendedMovementVector(bool setWalkMode);
	virtual G3D::Vector3 getIntendedMovementVector();

private:
	float health;
	float maxHealth;
	float walkRotationalVelocity;
	int walkTimer;
	WalkMode walkMode;
	shared_ptr<PartInstance> walkToPart;
	G3D::Vector3 walkToPoint;
	G3D::Vector3 walkDirection;
	G3D::Vector3 targetPoint;
	bool jump : 1;
	bool imDead : 1;
	bool hadHeadJoint : 1;
	bool sit : 1;
	mutable boost::shared_ptr<PartInstance> head;
	mutable boost::shared_ptr<PartInstance> torso;
	mutable boost::shared_ptr<PartInstance> leftLeg;
	mutable boost::shared_ptr<PartInstance> rightLeg;
	mutable boost::shared_ptr<PartInstance> rightArm;
	mutable boost::shared_ptr<PartInstance> leftArm;
	World* world;
	std::auto_ptr<State> currentState;
};

} // namespace RBX

#endif // HUMANOID_HUMANOID_H
