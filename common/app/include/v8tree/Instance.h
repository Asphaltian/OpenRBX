#ifndef V8TREE_INSTANCE_H
#define V8TREE_INSTANCE_H

#include "decomp.h"
#include "reflection/property.h"
#include "reflection/reflection.h"
#include "reflection/signal.h"
#include "util/Association.h"
#include "util/Events.h"
#include "util/Guid.h"
#include "util/Utilities.h"
#include "util/object.h"
#include "v8xml/XmlElement.h"

#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

namespace RBX {

using boost::shared_ptr;

class IReferenceBinder;
class Instance;
class Name;
class ServiceProvider;

struct ChildAdded
{
};
struct ChildRemoved
{
};
struct DescendentAdded
{
};
struct DescendentRemoving
{
};
// SIZE 0xc
struct AncestorChanged
{
	Instance* const child;     // 0x00
	Instance* const oldParent; // 0x04
	Instance* const newParent; // 0x08

	AncestorChanged(Instance* child, Instance* oldParent, Instance* newParent)
		: child(child), oldParent(oldParent), newParent(newParent)
	{
	}
};
class PropertyChanged
{
};

template <class T>
class AbstractFactoryProduct : public Creatable<T>
{
public:
	virtual ~AbstractFactoryProduct() {}
};

extern const char sInstance[];

class Instance : public GuidItem<Instance>,
				 public AbstractFactoryProduct<Instance>,
				 public Reflection::Described<Instance, sInstance, Reflection::DescribedBase>,
				 public Debugable,
				 public Notifier<Instance, ChildAdded>,
				 public Notifier<Instance, ChildRemoved>,
				 public Notifier<Instance, DescendentAdded>,
				 public Notifier<Instance, DescendentRemoving>,
				 public Notifier<Instance, AncestorChanged>,
				 public Notifier<Instance, PropertyChanged>,
				 public boost::enable_shared_from_this<Instance>,
				 public boost::noncopyable
{
public:
	Instance();
	Instance(const char* name);
	virtual ~Instance(); // vtable+0x00

	virtual void setName(const std::string& value); // vtable+0x04
	// FUNCTION: WEBSERVICE 0x100c74e0 FOLDED
	virtual bool askAddChild(const Instance* instance) const { return false; } // vtable+0x08

	// FUNCTION: WEBSERVICE 0x100c74e0 FOLDED
	virtual bool askSetParent(const Instance* instance) const { return false; } // vtable+0x0c
	virtual void onAncestorChanged(const AncestorChanged& event);               // vtable+0x10
	virtual void onDescendentAdded(Instance* instance);                         // vtable+0x14
	virtual void onDescendentRemoving(const shared_ptr<Instance>& instance);    // vtable+0x18
	virtual void onChildAdded(Instance* child) {}                               // vtable+0x1c

	virtual void onChildRemoving(Instance* child) {} // vtable+0x20

	virtual void onChildRemoved(Instance* child) {} // vtable+0x24

	virtual void onLastChildRemoved() {}                                            // vtable+0x28
	virtual void readProperty(const XmlElement* element, IReferenceBinder& binder); // vtable+0x2c
	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider) {
	} // vtable+0x30
	virtual shared_ptr<Instance> createChild(const Name& className);            // vtable+0x34
	virtual XmlElement* write();                                                // vtable+0x38
	virtual void onChildChanged(Instance* child, const PropertyChanged& event); // vtable+0x3c

	static Reflection::SignalDesc<Instance, void(const Reflection::PropertyDescriptor*)> event_propertyChanged;

	Instance* getParent() const { return parent; }

	Instance* findFirstChildByName(const std::string& name) const;

	template <class T>
	T* findFirstChildOfType() const;

	const std::string& getName() const { return name; }

	const CopyOnWrite<std::vector<shared_ptr<Instance> > >& getChildren() const { return children; }

	// FUNCTION: WEBSERVICE 0x10048320
	unsigned int numChildren() const { return children.read() != NULL ? children.read()->size() : 0; }

	// STUB: WEBSERVICE 0x100469f0
	DECOMP_NOINLINE void raisePropertyChanged(const Reflection::PropertyDescriptor& descriptor)
	{
		STUB(0x100469f0);

		const PropertyChanged event;

		if (event_propertyChanged.findSignalInstance(reinterpret_cast<const Reflection::SignalSource*>(this)) != NULL) {
			if (parent != NULL) {
				parent->onChildChanged(this, event);
			}
		}
	}

	void readProperties(const XmlElement* element, IReferenceBinder& binder);
	void readChild(const XmlElement* element, IReferenceBinder& binder);
	void readChildren(const XmlElement* element, IReferenceBinder& binder);
	void writeChildren(XmlElement* element);

	bool contains(const Instance* instance) const;

	bool isAncestorOf(const Instance* descendent) const
	{
		while (descendent != 0) {
			descendent = descendent->parent;

			if (descendent == this) {
				return true;
			}
		}

		return false;
	}

	bool isDescendentOf(const Instance* ancestor) const
	{
		const Instance* walk = parent;

		while (ancestor != walk) {
			if (walk == 0) {
				return false;
			}

			walk = walk->parent;
		}

		return true;
	}

private:
	friend class Creatable<Instance>::Deleter;

	void predelete();
	static void predelete(Instance* instance);

	Association<Instance> assoc;                                      // 0xbc
	Instance* parent;                                                 // 0xcc
	CopyOnWrite<std::vector<boost::shared_ptr<Instance> > > children; // 0xd0
	std::string name;                                                 // 0xd8
	bool archivable;                                                  // 0xf4
};

DECOMP_SIZE_ASSERT(Instance, 0xf8)

template <class T, class Base, const char* sName>
class DescribedNonCreatable : public Reflection::Described<T, sName, NonFactoryProduct<Base, sName> >
{
public:
	DescribedNonCreatable() {}

	template <class U>
	DescribedNonCreatable(U* joint) : Reflection::Described<T, sName, NonFactoryProduct<Base, sName> >(joint)
	{
	}
};

template <class T, class Base, const char* sName>
// clang-format off
	// STUB: WEBSERVICE 0x10043190
	// RBX::DescribedCreatable<RBX::Lighting,RBX::Instance,&RBX::sLighting>::~DescribedCreatable<RBX::Lighting,RBX::Instance,&RBX::sLighting>
	// STUB: WEBSERVICE 0x100459e0
	// RBX::DescribedCreatable<RBX::Lighting,RBX::Instance,&RBX::sLighting>::DescribedCreatable<RBX::Lighting,RBX::Instance,&RBX::sLighting>
	// STUB: WEBSERVICE 0x100543a0
	// RBX::DescribedCreatable<RBX::RunService,RBX::Instance,&RBX::sRunService>::~DescribedCreatable<RBX::RunService,RBX::Instance,&RBX::sRunService>
	// STUB: WEBSERVICE 0x100563e0
	// RBX::DescribedCreatable<RBX::RunService,RBX::Instance,&RBX::sRunService>::DescribedCreatable<RBX::RunService,RBX::Instance,&RBX::sRunService>
	// STUB: WEBSERVICE 0x1005d000
	// RBX::DescribedCreatable<RBX::Selection,RBX::Instance,&RBX::sSelection>::~DescribedCreatable<RBX::Selection,RBX::Instance,&RBX::sSelection>
	// STUB: WEBSERVICE 0x1005e3f0
	// RBX::DescribedCreatable<RBX::Selection,RBX::Instance,&RBX::sSelection>::DescribedCreatable<RBX::Selection,RBX::Instance,&RBX::sSelection>
	// STUB: WEBSERVICE 0x10061210
	// RBX::DescribedCreatable<RBX::ScriptContext,RBX::Instance,&RBX::sScriptContext>::~DescribedCreatable<RBX::ScriptContext,RBX::Instance,&RBX::sScriptContext>
	// STUB: WEBSERVICE 0x10065660
	// RBX::DescribedCreatable<RBX::ScriptContext,RBX::Instance,&RBX::sScriptContext>::DescribedCreatable<RBX::ScriptContext,RBX::Instance,&RBX::sScriptContext>
	// TEMPLATE: WEBSERVICE 0x10068be0
	// RBX::DescribedCreatable<RBX::Script,RBX::Instance,&RBX::sScript>::~DescribedCreatable<RBX::Script,RBX::Instance,&RBX::sScript>
	// STUB: WEBSERVICE 0x10068ff0
	// RBX::DescribedCreatable<RBX::LocalScript,RBX::Script,&RBX::sLocalScript>::~DescribedCreatable<RBX::LocalScript,RBX::Script,&RBX::sLocalScript>
	// STUB: WEBSERVICE 0x10069880
	// RBX::DescribedCreatable<RBX::Script,RBX::Instance,&RBX::sScript>::DescribedCreatable<RBX::Script,RBX::Instance,&RBX::sScript><char const *>
	// STUB: WEBSERVICE 0x10069be0
	// RBX::DescribedCreatable<RBX::LocalScript,RBX::Script,&RBX::sLocalScript>::DescribedCreatable<RBX::LocalScript,RBX::Script,&RBX::sLocalScript>
	// TEMPLATE: WEBSERVICE 0x1008c6f0
	// RBX::DescribedCreatable<RBX::Message,RBX::Instance,&RBX::sMessage>::~DescribedCreatable<RBX::Message,RBX::Instance,&RBX::sMessage>
	// STUB: WEBSERVICE 0x1008cb80
	// RBX::DescribedCreatable<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue>::~DescribedCreatable<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue>
	// STUB: WEBSERVICE 0x1008de10
	// RBX::DescribedCreatable<RBX::Hint,RBX::Message,&RBX::sHint>::DescribedCreatable<RBX::Hint,RBX::Message,&RBX::sHint>
	// STUB: WEBSERVICE 0x1008e2c0
	// RBX::DescribedCreatable<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue>::DescribedCreatable<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue>
	// TEMPLATE: WEBSERVICE 0x100a10f0
	// RBX::DescribedCreatable<RBX::Humanoid,RBX::Instance,&RBX::sHumanoid>::~DescribedCreatable<RBX::Humanoid,RBX::Instance,&RBX::sHumanoid>
	// STUB: WEBSERVICE 0x100a3b60
	// RBX::DescribedCreatable<RBX::Humanoid,RBX::Instance,&RBX::sHumanoid>::DescribedCreatable<RBX::Humanoid,RBX::Instance,&RBX::sHumanoid>
	// STUB: WEBSERVICE 0x100b3b30
	// RBX::DescribedCreatable<RBX::DebugSettings,RBX::GlobalSettings::Item,&RBX::sDebugSettings>::~DescribedCreatable<RBX::DebugSettings,RBX::GlobalSettings::Item,&RBX::sDebugSettings>
	// STUB: WEBSERVICE 0x100b4f20
	// RBX::DescribedCreatable<RBX::DebugSettings,RBX::GlobalSettings::Item,&RBX::sDebugSettings>::DescribedCreatable<RBX::DebugSettings,RBX::GlobalSettings::Item,&RBX::sDebugSettings>
	// STUB: WEBSERVICE 0x100be000
	// RBX::DescribedCreatable<RBX::Team,RBX::Instance,&RBX::sTeam>::~DescribedCreatable<RBX::Team,RBX::Instance,&RBX::sTeam>
	// STUB: WEBSERVICE 0x100be560
	// RBX::DescribedCreatable<RBX::Team,RBX::Instance,&RBX::sTeam>::DescribedCreatable<RBX::Team,RBX::Instance,&RBX::sTeam>
	// STUB: WEBSERVICE 0x100c2ee0
	// RBX::DescribedCreatable<RBX::Camera,RBX::Instance,&RBX::sCamera>::~DescribedCreatable<RBX::Camera,RBX::Instance,&RBX::sCamera>
	// STUB: WEBSERVICE 0x100c4030
	// RBX::DescribedCreatable<RBX::Camera,RBX::Instance,&RBX::sCamera>::DescribedCreatable<RBX::Camera,RBX::Instance,&RBX::sCamera>
	// STUB: WEBSERVICE 0x100c5b70
	// RBX::DescribedCreatable<RBX::ControllerService,RBX::Instance,&RBX::sControllerService>::~DescribedCreatable<RBX::ControllerService,RBX::Instance,&RBX::sControllerService>
	// STUB: WEBSERVICE 0x100c6a90
	// RBX::DescribedCreatable<RBX::ControllerService,RBX::Instance,&RBX::sControllerService>::DescribedCreatable<RBX::ControllerService,RBX::Instance,&RBX::sControllerService>
	// STUB: WEBSERVICE 0x100d87f0
	// RBX::DescribedCreatable<RBX::Snap,RBX::AutoJoint,&RBX::sSnap>::~DescribedCreatable<RBX::Snap,RBX::AutoJoint,&RBX::sSnap>
	// STUB: WEBSERVICE 0x100d8800
	// RBX::DescribedCreatable<RBX::Motor,RBX::AutoJoint,&RBX::sMotor>::~DescribedCreatable<RBX::Motor,RBX::AutoJoint,&RBX::sMotor>
	// STUB: WEBSERVICE 0x100d9440
	// RBX::DescribedCreatable<RBX::Snap,RBX::AutoJoint,&RBX::sSnap>::DescribedCreatable<RBX::Snap,RBX::AutoJoint,&RBX::sSnap><RBX::Joint *>
	// STUB: WEBSERVICE 0x100d94e0
	// RBX::DescribedCreatable<RBX::Weld,RBX::AutoJoint,&RBX::sWeld>::DescribedCreatable<RBX::Weld,RBX::AutoJoint,&RBX::sWeld><RBX::Joint *>
	// STUB: WEBSERVICE 0x100d95c0
	// RBX::DescribedCreatable<RBX::Glue,RBX::AutoJoint,&RBX::sGlue>::DescribedCreatable<RBX::Glue,RBX::AutoJoint,&RBX::sGlue><RBX::GlueJoint *>
	// STUB: WEBSERVICE 0x100d96d0
	// RBX::DescribedCreatable<RBX::Rotate,RBX::AutoJoint,&RBX::sRotate>::DescribedCreatable<RBX::Rotate,RBX::AutoJoint,&RBX::sRotate><RBX::Joint *>
	// STUB: WEBSERVICE 0x100d9790
	// RBX::DescribedCreatable<RBX::RotateP,RBX::AutoJoint,&RBX::sRotateP>::DescribedCreatable<RBX::RotateP,RBX::AutoJoint,&RBX::sRotateP><RBX::Joint *>
	// STUB: WEBSERVICE 0x100d9870
	// RBX::DescribedCreatable<RBX::RotateV,RBX::AutoJoint,&RBX::sRotateV>::DescribedCreatable<RBX::RotateV,RBX::AutoJoint,&RBX::sRotateV><RBX::Joint *>
	// STUB: WEBSERVICE 0x100d9970
	// RBX::DescribedCreatable<RBX::Motor,RBX::AutoJoint,&RBX::sMotor>::DescribedCreatable<RBX::Motor,RBX::AutoJoint,&RBX::sMotor><RBX::MotorJoint *>
	// STUB: WEBSERVICE 0x100da960
	// RBX::DescribedCreatable<RBX::Teams,RBX::Instance,&RBX::sTeams>::~DescribedCreatable<RBX::Teams,RBX::Instance,&RBX::sTeams>
	// STUB: WEBSERVICE 0x100db0d0
	// RBX::DescribedCreatable<RBX::Teams,RBX::Instance,&RBX::sTeams>::DescribedCreatable<RBX::Teams,RBX::Instance,&RBX::sTeams>
	// STUB: WEBSERVICE 0x100db930
	// RBX::DescribedCreatable<RBX::Accoutrement,RBX::Instance,&RBX::sAccoutrement>::~DescribedCreatable<RBX::Accoutrement,RBX::Instance,&RBX::sAccoutrement>
	// STUB: WEBSERVICE 0x100dc280
	// RBX::DescribedCreatable<RBX::Hat,RBX::Accoutrement,&RBX::sHat>::~DescribedCreatable<RBX::Hat,RBX::Accoutrement,&RBX::sHat>
	// STUB: WEBSERVICE 0x100dcc10
	// RBX::DescribedCreatable<RBX::Accoutrement,RBX::Instance,&RBX::sAccoutrement>::DescribedCreatable<RBX::Accoutrement,RBX::Instance,&RBX::sAccoutrement>
	// STUB: WEBSERVICE 0x100dd680
	// RBX::DescribedCreatable<RBX::Hat,RBX::Accoutrement,&RBX::sHat>::DescribedCreatable<RBX::Hat,RBX::Accoutrement,&RBX::sHat>
	// STUB: WEBSERVICE 0x100de230
	// RBX::DescribedCreatable<RBX::GameSettings,RBX::GlobalSettings::Item,&RBX::sGameSettings>::~DescribedCreatable<RBX::GameSettings,RBX::GlobalSettings::Item,&RBX::sGameSettings>
	// STUB: WEBSERVICE 0x100de4c0
	// RBX::DescribedCreatable<RBX::GameSettings,RBX::GlobalSettings::Item,&RBX::sGameSettings>::DescribedCreatable<RBX::GameSettings,RBX::GlobalSettings::Item,&RBX::sGameSettings>
	// STUB: WEBSERVICE 0x100e0920
	// RBX::DescribedCreatable<RBX::ShirtGraphic,RBX::CharacterAppearance,&RBX::sShirtGraphic>::~DescribedCreatable<RBX::ShirtGraphic,RBX::CharacterAppearance,&RBX::sShirtGraphic>
	// STUB: WEBSERVICE 0x100e0a80
	// RBX::DescribedCreatable<RBX::Skin,RBX::CharacterAppearance,&RBX::sSkin>::~DescribedCreatable<RBX::Skin,RBX::CharacterAppearance,&RBX::sSkin>
	// STUB: WEBSERVICE 0x100e0b50
	// RBX::DescribedCreatable<RBX::BodyColors,RBX::CharacterAppearance,&RBX::sBodyColors>::~DescribedCreatable<RBX::BodyColors,RBX::CharacterAppearance,&RBX::sBodyColors>
	// STUB: WEBSERVICE 0x100e0d50
	// RBX::DescribedCreatable<RBX::Shirt,RBX::ShirtGraphic,&RBX::sShirt>::~DescribedCreatable<RBX::Shirt,RBX::ShirtGraphic,&RBX::sShirt>
	// STUB: WEBSERVICE 0x100e11c0
	// RBX::DescribedCreatable<RBX::ShirtGraphic,RBX::CharacterAppearance,&RBX::sShirtGraphic>::DescribedCreatable<RBX::ShirtGraphic,RBX::CharacterAppearance,&RBX::sShirtGraphic>
	// STUB: WEBSERVICE 0x100e12b0
	// RBX::DescribedCreatable<RBX::BodyColors,RBX::CharacterAppearance,&RBX::sBodyColors>::DescribedCreatable<RBX::BodyColors,RBX::CharacterAppearance,&RBX::sBodyColors>
	// STUB: WEBSERVICE 0x100e13a0
	// RBX::DescribedCreatable<RBX::Skin,RBX::CharacterAppearance,&RBX::sSkin>::DescribedCreatable<RBX::Skin,RBX::CharacterAppearance,&RBX::sSkin>
	// STUB: WEBSERVICE 0x100e1840
	// RBX::DescribedCreatable<RBX::Shirt,RBX::ShirtGraphic,&RBX::sShirt>::DescribedCreatable<RBX::Shirt,RBX::ShirtGraphic,&RBX::sShirt>
	// STUB: WEBSERVICE 0x100e2100
	// RBX::DescribedCreatable<RBX::Explosion,RBX::Instance,&RBX::sExplosion>::~DescribedCreatable<RBX::Explosion,RBX::Instance,&RBX::sExplosion>
	// STUB: WEBSERVICE 0x100e2c00
	// RBX::DescribedCreatable<RBX::Explosion,RBX::Instance,&RBX::sExplosion>::DescribedCreatable<RBX::Explosion,RBX::Instance,&RBX::sExplosion>
	// STUB: WEBSERVICE 0x100e46f0
	// RBX::DescribedCreatable<RBX::VelocityMotor,RBX::JointInstance,&RBX::sVelocityMotor>::~DescribedCreatable<RBX::VelocityMotor,RBX::JointInstance,&RBX::sVelocityMotor>
	// STUB: WEBSERVICE 0x100e4ea0
	// RBX::DescribedCreatable<RBX::Hole,RBX::Feature,&RBX::sHole>::~DescribedCreatable<RBX::Hole,RBX::Feature,&RBX::sHole>
	// STUB: WEBSERVICE 0x100e4eb0
	// RBX::DescribedCreatable<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature>::~DescribedCreatable<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature>
	// STUB: WEBSERVICE 0x100e6c20
	// RBX::DescribedCreatable<RBX::VelocityMotor,RBX::JointInstance,&RBX::sVelocityMotor>::DescribedCreatable<RBX::VelocityMotor,RBX::JointInstance,&RBX::sVelocityMotor><RBX::MotorJoint *>
	// STUB: WEBSERVICE 0x100e7730
	// RBX::DescribedCreatable<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature>::DescribedCreatable<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature>
	// STUB: WEBSERVICE 0x100e77d0
	// RBX::DescribedCreatable<RBX::Hole,RBX::Feature,&RBX::sHole>::DescribedCreatable<RBX::Hole,RBX::Feature,&RBX::sHole>
	// STUB: WEBSERVICE 0x100e8050
	// RBX::DescribedCreatable<RBX::FlagStandService,RBX::Instance,&RBX::sFlagStandService>::~DescribedCreatable<RBX::FlagStandService,RBX::Instance,&RBX::sFlagStandService>
	// STUB: WEBSERVICE 0x100e89f0
	// RBX::DescribedCreatable<RBX::FlagStandService,RBX::Instance,&RBX::sFlagStandService>::DescribedCreatable<RBX::FlagStandService,RBX::Instance,&RBX::sFlagStandService>
	// STUB: WEBSERVICE 0x100e9460
	// RBX::DescribedCreatable<RBX::ForceField,RBX::Instance,&RBX::sForceField>::~DescribedCreatable<RBX::ForceField,RBX::Instance,&RBX::sForceField>
	// STUB: WEBSERVICE 0x100e96e0
	// RBX::DescribedCreatable<RBX::ForceField,RBX::Instance,&RBX::sForceField>::DescribedCreatable<RBX::ForceField,RBX::Instance,&RBX::sForceField>
	// STUB: WEBSERVICE 0x100e9d30
	// RBX::DescribedCreatable<RBX::GeometryService,RBX::Instance,&RBX::sGeometryService>::DescribedCreatable<RBX::GeometryService,RBX::Instance,&RBX::sGeometryService><char *>
	// STUB: WEBSERVICE 0x100ec040
	// RBX::DescribedCreatable<RBX::Rocket,RBX::BodyMover,&RBX::sRocket>::~DescribedCreatable<RBX::Rocket,RBX::BodyMover,&RBX::sRocket>
	// STUB: WEBSERVICE 0x100ec050
	// RBX::DescribedCreatable<RBX::BodyGyro,RBX::BodyMover,&RBX::sBodyGyro>::~DescribedCreatable<RBX::BodyGyro,RBX::BodyMover,&RBX::sBodyGyro>
	// STUB: WEBSERVICE 0x100ece60
	// RBX::DescribedCreatable<RBX::BodyGyro,RBX::BodyMover,&RBX::sBodyGyro>::DescribedCreatable<RBX::BodyGyro,RBX::BodyMover,&RBX::sBodyGyro><char *>
	// STUB: WEBSERVICE 0x100ecf60
	// RBX::DescribedCreatable<RBX::BodyPosition,RBX::BodyMover,&RBX::sBodyPosition>::DescribedCreatable<RBX::BodyPosition,RBX::BodyMover,&RBX::sBodyPosition><char *>
	// STUB: WEBSERVICE 0x100ed010
	// RBX::DescribedCreatable<RBX::BodyVelocity,RBX::BodyMover,&RBX::sBodyVelocity>::DescribedCreatable<RBX::BodyVelocity,RBX::BodyMover,&RBX::sBodyVelocity><char *>
	// STUB: WEBSERVICE 0x100ed1b0
	// RBX::DescribedCreatable<RBX::BodyForce,RBX::BodyMover,&RBX::sBodyForce>::DescribedCreatable<RBX::BodyForce,RBX::BodyMover,&RBX::sBodyForce><char *>
	// STUB: WEBSERVICE 0x100ed380
	// RBX::DescribedCreatable<RBX::BodyThrust,RBX::BodyMover,&RBX::sBodyThrust>::DescribedCreatable<RBX::BodyThrust,RBX::BodyMover,&RBX::sBodyThrust><char *>
	// STUB: WEBSERVICE 0x100eddb0
	// RBX::DescribedCreatable<RBX::Rocket,RBX::BodyMover,&RBX::sRocket>::DescribedCreatable<RBX::Rocket,RBX::BodyMover,&RBX::sRocket><char *>
	// STUB: WEBSERVICE 0x100ee970
	// RBX::DescribedCreatable<RBX::Message,RBX::Instance,&RBX::sMessage>::DescribedCreatable<RBX::Message,RBX::Instance,&RBX::sMessage><char const *>
	// STUB: WEBSERVICE 0x100f8d50
	// RBX::DescribedCreatable<RBX::Visit,RBX::Instance,&RBX::sVisit>::~DescribedCreatable<RBX::Visit,RBX::Instance,&RBX::sVisit>
	// STUB: WEBSERVICE 0x100f94e0
	// RBX::DescribedCreatable<RBX::Visit,RBX::Instance,&RBX::sVisit>::DescribedCreatable<RBX::Visit,RBX::Instance,&RBX::sVisit>
	// STUB: WEBSERVICE 0x100fa020
	// RBX::DescribedCreatable<RBX::DebrisService,RBX::Instance,&RBX::sDebrisService>::~DescribedCreatable<RBX::DebrisService,RBX::Instance,&RBX::sDebrisService>
	// STUB: WEBSERVICE 0x100fac30
	// RBX::DescribedCreatable<RBX::DebrisService,RBX::Instance,&RBX::sDebrisService>::DescribedCreatable<RBX::DebrisService,RBX::Instance,&RBX::sDebrisService><char *>
	// STUB: WEBSERVICE 0x100fb690
	// RBX::DescribedCreatable<RBX::TimerService,RBX::Instance,&RBX::sTimerService>::~DescribedCreatable<RBX::TimerService,RBX::Instance,&RBX::sTimerService>
	// STUB: WEBSERVICE 0x100fbc60
	// RBX::DescribedCreatable<RBX::TimerService,RBX::Instance,&RBX::sTimerService>::DescribedCreatable<RBX::TimerService,RBX::Instance,&RBX::sTimerService>
	// STUB: WEBSERVICE 0x100fc3f0
	// RBX::DescribedCreatable<RBX::Seat,RBX::PartInstance,&RBX::sSeat>::~DescribedCreatable<RBX::Seat,RBX::PartInstance,&RBX::sSeat>
	// STUB: WEBSERVICE 0x100fcfd0
	// RBX::DescribedCreatable<RBX::Seat,RBX::PartInstance,&RBX::sSeat>::DescribedCreatable<RBX::Seat,RBX::PartInstance,&RBX::sSeat>
	// STUB: WEBSERVICE 0x100fda80
	// RBX::DescribedCreatable<RBX::SpawnerService,RBX::Instance,&RBX::sSpawnerService>::~DescribedCreatable<RBX::SpawnerService,RBX::Instance,&RBX::sSpawnerService>
	// STUB: WEBSERVICE 0x100feae0
	// RBX::DescribedCreatable<RBX::SpawnerService,RBX::Instance,&RBX::sSpawnerService>::DescribedCreatable<RBX::SpawnerService,RBX::Instance,&RBX::sSpawnerService>
	// STUB: WEBSERVICE 0x100ffb30
	// RBX::DescribedCreatable<RBX::Texture,RBX::Decal,&RBX::sTexture>::~DescribedCreatable<RBX::Texture,RBX::Decal,&RBX::sTexture>
	// STUB: WEBSERVICE 0x101002f0
	// RBX::DescribedCreatable<RBX::Texture,RBX::Decal,&RBX::sTexture>::DescribedCreatable<RBX::Texture,RBX::Decal,&RBX::sTexture>
	// STUB: WEBSERVICE 0x1012cc20
	// RBX::DescribedCreatable<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape>::~DescribedCreatable<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape>
	// STUB: WEBSERVICE 0x1012d890
	// RBX::DescribedCreatable<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape>::DescribedCreatable<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape>
// clang-format on
class DescribedCreatable : public Reflection::Described<T, sName, FactoryProduct<T, Base, sName> >
{
public:
	DescribedCreatable() {}

	template <class U>
	DescribedCreatable(U* joint) : Reflection::Described<T, sName, FactoryProduct<T, Base, sName> >(joint)
	{
	}
};

// clang-format off
// SYNTHETIC: WEBSERVICE 0x10218730
// `dynamic initializer for 'RBX::Instance::event_propertyChanged''
// SYNTHETIC: WEBSERVICE 0x102217c0
// `dynamic atexit destructor for 'RBX::Instance::event_propertyChanged''
// clang-format on

// clang-format off
// TEMPLATE: WEBSERVICE 0x1005aff0
// RBX::Instance::findFirstChildOfType<RBX::Humanoid>
// clang-format on
template <class T>
T* Instance::findFirstChildOfType() const
{
	if (getChildren().read() != NULL) {
		std::vector<shared_ptr<Instance> >::const_iterator end = getChildren().read()->end();

		for (std::vector<shared_ptr<Instance> >::const_iterator iter = getChildren().read()->begin(); iter != end;
			 ++iter) {
			T* child = dynamic_cast<T*>(iter->get());

			if (child != NULL) {
				return child;
			}
		}
	}

	return NULL;
}

} // namespace RBX

#endif // V8TREE_INSTANCE_H
