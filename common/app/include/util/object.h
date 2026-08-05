#ifndef UTIL_OBJECT_H
#define UTIL_OBJECT_H

#include "decomp.h"
#include "util/Name.h"

#include <boost/shared_ptr.hpp>
#include <stdlib.h>

namespace RBX {

class Instance;

// VTABLE: WEBSERVICE 0x1022e290
// SIZE 0x04
class Object
{
public:
	virtual ~Object() {} // vtable+0x00
};

DECOMP_SIZE_ASSERT(Object, 0x04)

// clang-format off
// TEMPLATE: WEBSERVICE 0x1006c2e0
// RBX::shared_from<RBX::Instance>
// clang-format on
template <class T>
boost::shared_ptr<T> shared_from(T* r)
{
	return r != NULL ? boost::shared_static_cast<T>(r->shared_from_this()) : boost::shared_ptr<T>();
}

template <class T>
class Creatable : public Object
{
public:
	class Deleter
	{
	public:
		void operator()(T* instance) const
		{
			T::predelete(instance);
			delete instance;
		}
	};

	template <class U>
	static boost::shared_ptr<U> create()
	{
		return boost::shared_ptr<U>(new U(), Deleter());
	}

	template <class U, class A>
	static boost::shared_ptr<U> create(A argument)
	{
		return boost::shared_ptr<U>(new U(argument), Deleter());
	}

protected:
	Creatable() {}
	virtual ~Creatable() {}

	static void* operator new(size_t size) { return malloc(size); }

	static void operator delete(void* p) { free(p); }
};

template <class T, class Base, const char* sName>
class FactoryProduct : public Base
{
public:
	class Creator
	{
	public:
		virtual boost::shared_ptr<Object> create() const { return Creatable<Instance>::create<T>(); }
	};

	// clang-format off
	// STUB: WEBSERVICE 0x10007b20
	// RBX::FactoryProduct<RBX::Lighting,RBX::Instance,&RBX::sLighting>::Creator::create
	// STUB: WEBSERVICE 0x10007be0
	// RBX::FactoryProduct<RBX::RunService,RBX::Instance,&RBX::sRunService>::Creator::create
	// STUB: WEBSERVICE 0x10007d60
	// RBX::FactoryProduct<RBX::Selection,RBX::Instance,&RBX::sSelection>::Creator::create
	// STUB: WEBSERVICE 0x10007e20
	// RBX::FactoryProduct<RBX::ScriptContext,RBX::Instance,&RBX::sScriptContext>::Creator::create
	// STUB: WEBSERVICE 0x10007ee0
	// RBX::FactoryProduct<RBX::Script,RBX::Instance,&RBX::sScript>::Creator::create
	// STUB: WEBSERVICE 0x10007fa0
	// RBX::FactoryProduct<RBX::LocalScript,RBX::Script,&RBX::sLocalScript>::Creator::create
	// TEMPLATE: WEBSERVICE 0x10042700
	// RBX::FactoryProduct<RBX::Lighting,RBX::Instance,&RBX::sLighting>::~FactoryProduct<RBX::Lighting,RBX::Instance,&RBX::sLighting>
	// TEMPLATE: WEBSERVICE 0x10042760
	// RBX::FactoryProduct<RBX::Lighting,RBX::Instance,&RBX::sLighting>::FactoryProduct<RBX::Lighting,RBX::Instance,&RBX::sLighting>
	// STUB: WEBSERVICE 0x100429e0
	// RBX::FactoryProduct<RBX::Lighting,RBX::Instance,&RBX::sLighting>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x10043c80
	// RBX::FactoryProduct<RBX::Lighting,RBX::Instance,&RBX::sLighting>::getClassName
	// TEMPLATE: WEBSERVICE 0x10054250
	// RBX::FactoryProduct<RBX::RunService,RBX::Instance,&RBX::sRunService>::~FactoryProduct<RBX::RunService,RBX::Instance,&RBX::sRunService>
	// TEMPLATE: WEBSERVICE 0x100542a0
	// RBX::FactoryProduct<RBX::RunService,RBX::Instance,&RBX::sRunService>::FactoryProduct<RBX::RunService,RBX::Instance,&RBX::sRunService>
	// TEMPLATE: WEBSERVICE 0x100550d0
	// RBX::FactoryProduct<RBX::RunService,RBX::Instance,&RBX::sRunService>::getClassName
	// STUB: WEBSERVICE 0x100564d0
	// RBX::FactoryProduct<RBX::RunService,RBX::Instance,&RBX::sRunService>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x1005bb30
	// RBX::FactoryProduct<RBX::Humanoid,RBX::Instance,&RBX::sHumanoid>::Creator::create
	// TEMPLATE: WEBSERVICE 0x1005ce00
	// RBX::FactoryProduct<RBX::Selection,RBX::Instance,&RBX::sSelection>::~FactoryProduct<RBX::Selection,RBX::Instance,&RBX::sSelection>
	// TEMPLATE: WEBSERVICE 0x1005ce50
	// RBX::FactoryProduct<RBX::Selection,RBX::Instance,&RBX::sSelection>::FactoryProduct<RBX::Selection,RBX::Instance,&RBX::sSelection>
	// STUB: WEBSERVICE 0x1005cec0
	// RBX::FactoryProduct<RBX::Selection,RBX::Instance,&RBX::sSelection>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x1005d5c0
	// RBX::FactoryProduct<RBX::Selection,RBX::Instance,&RBX::sSelection>::getClassName
	// TEMPLATE: WEBSERVICE 0x1005ec80
	// RBX::FactoryProduct<RBX::ScriptContext,RBX::Instance,&RBX::sScriptContext>::~FactoryProduct<RBX::ScriptContext,RBX::Instance,&RBX::sScriptContext>
	// TEMPLATE: WEBSERVICE 0x1005eeb0
	// RBX::FactoryProduct<RBX::ScriptContext,RBX::Instance,&RBX::sScriptContext>::FactoryProduct<RBX::ScriptContext,RBX::Instance,&RBX::sScriptContext>
	// TEMPLATE: WEBSERVICE 0x10062f50
	// RBX::FactoryProduct<RBX::ScriptContext,RBX::Instance,&RBX::sScriptContext>::getClassName
	// STUB: WEBSERVICE 0x10064700
	// RBX::FactoryProduct<RBX::DebugSettings,RBX::GlobalSettings::Item,&RBX::sDebugSettings>::Creator::create
	// STUB: WEBSERVICE 0x10065740
	// RBX::FactoryProduct<RBX::ScriptContext,RBX::Instance,&RBX::sScriptContext>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x10068970
	// RBX::FactoryProduct<RBX::Script,RBX::Instance,&RBX::sScript>::~FactoryProduct<RBX::Script,RBX::Instance,&RBX::sScript>
	// STUB: WEBSERVICE 0x10068aa0
	// RBX::FactoryProduct<RBX::Script,RBX::Instance,&RBX::sScript>::FactoryProduct<RBX::Script,RBX::Instance,&RBX::sScript><char const *>
	// TEMPLATE: WEBSERVICE 0x10068b10
	// RBX::FactoryProduct<RBX::Script,RBX::Instance,&RBX::sScript>::`scalar deleting destructor'
	// STUB: WEBSERVICE 0x10068e10
	// RBX::FactoryProduct<RBX::LocalScript,RBX::Script,&RBX::sLocalScript>::~FactoryProduct<RBX::LocalScript,RBX::Script,&RBX::sLocalScript>
	// TEMPLATE: WEBSERVICE 0x10068e80
	// RBX::FactoryProduct<RBX::LocalScript,RBX::Script,&RBX::sLocalScript>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x10069070
	// RBX::FactoryProduct<RBX::Script,RBX::Instance,&RBX::sScript>::getClassName
	// TEMPLATE: WEBSERVICE 0x10069090
	// RBX::FactoryProduct<RBX::LocalScript,RBX::Script,&RBX::sLocalScript>::getClassName
	// TEMPLATE: WEBSERVICE 0x10069270
	// RBX::FactoryProduct<RBX::Team,RBX::Instance,&RBX::sTeam>::Creator::create
	// TEMPLATE: WEBSERVICE 0x10069b80
	// RBX::FactoryProduct<RBX::LocalScript,RBX::Script,&RBX::sLocalScript>::FactoryProduct<RBX::LocalScript,RBX::Script,&RBX::sLocalScript>
	// TEMPLATE: WEBSERVICE 0x1006d8d0
	// RBX::FactoryProduct<RBX::ControllerService,RBX::Instance,&RBX::sControllerService>::Creator::create
	// TEMPLATE: WEBSERVICE 0x1006d980
	// RBX::FactoryProduct<RBX::Camera,RBX::Instance,&RBX::sCamera>::Creator::create
	// TEMPLATE: WEBSERVICE 0x100766a0
	// RBX::FactoryProduct<RBX::Snap,RBX::AutoJoint,&RBX::sSnap>::Creator::create
	// TEMPLATE: WEBSERVICE 0x10076910
	// RBX::FactoryProduct<RBX::Weld,RBX::AutoJoint,&RBX::sWeld>::Creator::create
	// TEMPLATE: WEBSERVICE 0x10076b80
	// RBX::FactoryProduct<RBX::Glue,RBX::AutoJoint,&RBX::sGlue>::Creator::create
	// TEMPLATE: WEBSERVICE 0x10076df0
	// RBX::FactoryProduct<RBX::Rotate,RBX::AutoJoint,&RBX::sRotate>::Creator::create
	// TEMPLATE: WEBSERVICE 0x10077060
	// RBX::FactoryProduct<RBX::RotateP,RBX::AutoJoint,&RBX::sRotateP>::Creator::create
	// TEMPLATE: WEBSERVICE 0x100772d0
	// RBX::FactoryProduct<RBX::RotateV,RBX::AutoJoint,&RBX::sRotateV>::Creator::create
	// STUB: WEBSERVICE 0x10077540
	// RBX::FactoryProduct<RBX::Motor,RBX::AutoJoint,&RBX::sMotor>::Creator::create
	// TEMPLATE: WEBSERVICE 0x10077a20
	// RBX::FactoryProduct<RBX::Teams,RBX::Instance,&RBX::sTeams>::Creator::create
	// TEMPLATE: WEBSERVICE 0x10077c90
	// RBX::FactoryProduct<RBX::Accoutrement,RBX::Instance,&RBX::sAccoutrement>::Creator::create
	// TEMPLATE: WEBSERVICE 0x10077f00
	// RBX::FactoryProduct<RBX::Hat,RBX::Accoutrement,&RBX::sHat>::Creator::create
	// TEMPLATE: WEBSERVICE 0x100815c0
	// RBX::FactoryProduct<RBX::GameSettings,RBX::GlobalSettings::Item,&RBX::sGameSettings>::Creator::create
	// TEMPLATE: WEBSERVICE 0x100899b0
	// RBX::FactoryProduct<RBX::ShirtGraphic,RBX::CharacterAppearance,&RBX::sShirtGraphic>::Creator::create
	// TEMPLATE: WEBSERVICE 0x10089c20
	// RBX::FactoryProduct<RBX::Shirt,RBX::ShirtGraphic,&RBX::sShirt>::Creator::create
	// TEMPLATE: WEBSERVICE 0x10089e90
	// RBX::FactoryProduct<RBX::BodyColors,RBX::CharacterAppearance,&RBX::sBodyColors>::Creator::create
	// TEMPLATE: WEBSERVICE 0x1008a100
	// RBX::FactoryProduct<RBX::Skin,RBX::CharacterAppearance,&RBX::sSkin>::Creator::create
	// TEMPLATE: WEBSERVICE 0x1008a370
	// RBX::FactoryProduct<RBX::Explosion,RBX::Instance,&RBX::sExplosion>::Creator::create
	// TEMPLATE: WEBSERVICE 0x1008a5e0
	// RBX::FactoryProduct<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature>::Creator::create
	// TEMPLATE: WEBSERVICE 0x1008a850
	// RBX::FactoryProduct<RBX::Hole,RBX::Feature,&RBX::sHole>::Creator::create
	// STUB: WEBSERVICE 0x1008aac0
	// RBX::FactoryProduct<RBX::VelocityMotor,RBX::JointInstance,&RBX::sVelocityMotor>::Creator::create
	// TEMPLATE: WEBSERVICE 0x1008afa0
	// RBX::FactoryProduct<RBX::FlagStandService,RBX::Instance,&RBX::sFlagStandService>::Creator::create
	// TEMPLATE: WEBSERVICE 0x1008b210
	// RBX::FactoryProduct<RBX::ForceField,RBX::Instance,&RBX::sForceField>::Creator::create
	// TEMPLATE: WEBSERVICE 0x1008b480
	// RBX::FactoryProduct<RBX::GeometryService,RBX::Instance,&RBX::sGeometryService>::Creator::create
	// TEMPLATE: WEBSERVICE 0x1008b6f0
	// RBX::FactoryProduct<RBX::BodyGyro,RBX::BodyMover,&RBX::sBodyGyro>::Creator::create
	// TEMPLATE: WEBSERVICE 0x1008b960
	// RBX::FactoryProduct<RBX::BodyForce,RBX::BodyMover,&RBX::sBodyForce>::Creator::create
	// TEMPLATE: WEBSERVICE 0x1008bbd0
	// RBX::FactoryProduct<RBX::BodyThrust,RBX::BodyMover,&RBX::sBodyThrust>::Creator::create
	// TEMPLATE: WEBSERVICE 0x1008be40
	// RBX::FactoryProduct<RBX::BodyPosition,RBX::BodyMover,&RBX::sBodyPosition>::Creator::create
	// TEMPLATE: WEBSERVICE 0x1008c0b0
	// RBX::FactoryProduct<RBX::BodyVelocity,RBX::BodyMover,&RBX::sBodyVelocity>::Creator::create
	// TEMPLATE: WEBSERVICE 0x1008c320
	// RBX::FactoryProduct<RBX::Rocket,RBX::BodyMover,&RBX::sRocket>::Creator::create
	// TEMPLATE: WEBSERVICE 0x1008c590
	// RBX::FactoryProduct<RBX::Message,RBX::Instance,&RBX::sMessage>::Creator::create
	// TEMPLATE: WEBSERVICE 0x1008c640
	// RBX::FactoryProduct<RBX::Message,RBX::Instance,&RBX::sMessage>::~FactoryProduct<RBX::Message,RBX::Instance,&RBX::sMessage>
	// FUNCTION: WEBSERVICE 0x1008c690
	// RBX::FactoryProduct<RBX::Message,RBX::Instance,&RBX::sMessage>::`scalar deleting destructor'
	// STUB: WEBSERVICE 0x1008c800
	// RBX::FactoryProduct<RBX::Hint,RBX::Message,&RBX::sHint>::FactoryProduct<RBX::Hint,RBX::Message,&RBX::sHint>
	// STUB: WEBSERVICE 0x1008c940
	// RBX::FactoryProduct<RBX::Hint,RBX::Message,&RBX::sHint>::~FactoryProduct<RBX::Hint,RBX::Message,&RBX::sHint>
	// TEMPLATE: WEBSERVICE 0x1008c9a0
	// RBX::FactoryProduct<RBX::Message,RBX::Instance,&RBX::sMessage>::getClassName
	// TEMPLATE: WEBSERVICE 0x1008c9c0
	// RBX::FactoryProduct<RBX::Hint,RBX::Message,&RBX::sHint>::getClassName
	// TEMPLATE: WEBSERVICE 0x1008c9e0
	// RBX::FactoryProduct<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue>::~FactoryProduct<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue>
	// TEMPLATE: WEBSERVICE 0x1008ca30
	// RBX::FactoryProduct<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue>::FactoryProduct<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue>
	// STUB: WEBSERVICE 0x1008caa0
	// RBX::FactoryProduct<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x1008cc40
	// RBX::FactoryProduct<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue>::getClassName
	// TEMPLATE: WEBSERVICE 0x1008ce90
	// RBX::FactoryProduct<RBX::Visit,RBX::Instance,&RBX::sVisit>::Creator::create
	// TEMPLATE: WEBSERVICE 0x1008d100
	// RBX::FactoryProduct<RBX::DebrisService,RBX::Instance,&RBX::sDebrisService>::Creator::create
	// TEMPLATE: WEBSERVICE 0x1008d370
	// RBX::FactoryProduct<RBX::TimerService,RBX::Instance,&RBX::sTimerService>::Creator::create
	// TEMPLATE: WEBSERVICE 0x1008d5e0
	// RBX::FactoryProduct<RBX::Seat,RBX::PartInstance,&RBX::sSeat>::Creator::create
	// TEMPLATE: WEBSERVICE 0x1008dac0
	// RBX::FactoryProduct<RBX::SpawnerService,RBX::Instance,&RBX::sSpawnerService>::Creator::create
	// STUB: WEBSERVICE 0x1008e4b0
	// RBX::FactoryProduct<RBX::Hint,RBX::Message,&RBX::sHint>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x1008e8a0
	// RBX::FactoryProduct<RBX::Hint,RBX::Message,&RBX::sHint>::Creator::create
	// TEMPLATE: WEBSERVICE 0x1008e950
	// RBX::FactoryProduct<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue>::Creator::create
	// TEMPLATE: WEBSERVICE 0x100a09b0
	// RBX::FactoryProduct<RBX::Humanoid,RBX::Instance,&RBX::sHumanoid>::~FactoryProduct<RBX::Humanoid,RBX::Instance,&RBX::sHumanoid>
	// TEMPLATE: WEBSERVICE 0x100a0a70
	// RBX::FactoryProduct<RBX::Humanoid,RBX::Instance,&RBX::sHumanoid>::FactoryProduct<RBX::Humanoid,RBX::Instance,&RBX::sHumanoid>
	// STUB: WEBSERVICE 0x100a0b40
	// RBX::FactoryProduct<RBX::Humanoid,RBX::Instance,&RBX::sHumanoid>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x100a1bb0
	// RBX::FactoryProduct<RBX::Humanoid,RBX::Instance,&RBX::sHumanoid>::getClassName
	// TEMPLATE: WEBSERVICE 0x100ad0d0
	// RBX::FactoryProduct<RBX::Texture,RBX::Decal,&RBX::sTexture>::Creator::create
	// STUB: WEBSERVICE 0x100b3810
	// RBX::FactoryProduct<RBX::DebugSettings,RBX::GlobalSettings::Item,&RBX::sDebugSettings>::~FactoryProduct<RBX::DebugSettings,RBX::GlobalSettings::Item,&RBX::sDebugSettings>
	// STUB: WEBSERVICE 0x100b38c0
	// RBX::FactoryProduct<RBX::DebugSettings,RBX::GlobalSettings::Item,&RBX::sDebugSettings>::`scalar deleting destructor'
	// STUB: WEBSERVICE 0x100b38e0
	// RBX::FactoryProduct<RBX::DebugSettings,RBX::GlobalSettings::Item,&RBX::sDebugSettings>::FactoryProduct<RBX::DebugSettings,RBX::GlobalSettings::Item,&RBX::sDebugSettings>
	// STUB: WEBSERVICE 0x100b3bc0
	// RBX::FactoryProduct<RBX::DebugSettings,RBX::GlobalSettings::Item,&RBX::sDebugSettings>::getClassName
	// TEMPLATE: WEBSERVICE 0x100bdef0
	// RBX::FactoryProduct<RBX::Team,RBX::Instance,&RBX::sTeam>::~FactoryProduct<RBX::Team,RBX::Instance,&RBX::sTeam>
	// TEMPLATE: WEBSERVICE 0x100bdf40
	// RBX::FactoryProduct<RBX::Team,RBX::Instance,&RBX::sTeam>::FactoryProduct<RBX::Team,RBX::Instance,&RBX::sTeam>
	// STUB: WEBSERVICE 0x100bdfa0
	// RBX::FactoryProduct<RBX::Team,RBX::Instance,&RBX::sTeam>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x100be220
	// RBX::FactoryProduct<RBX::Team,RBX::Instance,&RBX::sTeam>::getClassName
	// TEMPLATE: WEBSERVICE 0x100c2710
	// RBX::FactoryProduct<RBX::Camera,RBX::Instance,&RBX::sCamera>::~FactoryProduct<RBX::Camera,RBX::Instance,&RBX::sCamera>
	// TEMPLATE: WEBSERVICE 0x100c2770
	// RBX::FactoryProduct<RBX::Camera,RBX::Instance,&RBX::sCamera>::FactoryProduct<RBX::Camera,RBX::Instance,&RBX::sCamera>
	// TEMPLATE: WEBSERVICE 0x100c3690
	// RBX::FactoryProduct<RBX::Camera,RBX::Instance,&RBX::sCamera>::getClassName
	// STUB: WEBSERVICE 0x100c4100
	// RBX::FactoryProduct<RBX::Camera,RBX::Instance,&RBX::sCamera>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x100c59a0
	// RBX::FactoryProduct<RBX::ControllerService,RBX::Instance,&RBX::sControllerService>::~FactoryProduct<RBX::ControllerService,RBX::Instance,&RBX::sControllerService>
	// TEMPLATE: WEBSERVICE 0x100c59f0
	// RBX::FactoryProduct<RBX::ControllerService,RBX::Instance,&RBX::sControllerService>::FactoryProduct<RBX::ControllerService,RBX::Instance,&RBX::sControllerService>
	// STUB: WEBSERVICE 0x100c5ab0
	// RBX::FactoryProduct<RBX::ControllerService,RBX::Instance,&RBX::sControllerService>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x100c5e20
	// RBX::FactoryProduct<RBX::ControllerService,RBX::Instance,&RBX::sControllerService>::getClassName
	// STUB: WEBSERVICE 0x100d8300
	// RBX::FactoryProduct<RBX::Snap,RBX::AutoJoint,&RBX::sSnap>::~FactoryProduct<RBX::Snap,RBX::AutoJoint,&RBX::sSnap>
	// TEMPLATE: WEBSERVICE 0x100d8370
	// RBX::FactoryProduct<RBX::Snap,RBX::AutoJoint,&RBX::sSnap>::getClassName
	// STUB: WEBSERVICE 0x100d8390
	// RBX::FactoryProduct<RBX::Weld,RBX::AutoJoint,&RBX::sWeld>::~FactoryProduct<RBX::Weld,RBX::AutoJoint,&RBX::sWeld>
	// TEMPLATE: WEBSERVICE 0x100d8410
	// RBX::FactoryProduct<RBX::Weld,RBX::AutoJoint,&RBX::sWeld>::getClassName
	// STUB: WEBSERVICE 0x100d8430
	// RBX::FactoryProduct<RBX::Glue,RBX::AutoJoint,&RBX::sGlue>::~FactoryProduct<RBX::Glue,RBX::AutoJoint,&RBX::sGlue>
	// TEMPLATE: WEBSERVICE 0x100d84c0
	// RBX::FactoryProduct<RBX::Glue,RBX::AutoJoint,&RBX::sGlue>::getClassName
	// STUB: WEBSERVICE 0x100d84e0
	// RBX::FactoryProduct<RBX::Rotate,RBX::AutoJoint,&RBX::sRotate>::~FactoryProduct<RBX::Rotate,RBX::AutoJoint,&RBX::sRotate>
	// TEMPLATE: WEBSERVICE 0x100d8550
	// RBX::FactoryProduct<RBX::Rotate,RBX::AutoJoint,&RBX::sRotate>::getClassName
	// STUB: WEBSERVICE 0x100d8570
	// RBX::FactoryProduct<RBX::RotateP,RBX::AutoJoint,&RBX::sRotateP>::~FactoryProduct<RBX::RotateP,RBX::AutoJoint,&RBX::sRotateP>
	// TEMPLATE: WEBSERVICE 0x100d8610
	// RBX::FactoryProduct<RBX::RotateP,RBX::AutoJoint,&RBX::sRotateP>::getClassName
	// STUB: WEBSERVICE 0x100d8630
	// RBX::FactoryProduct<RBX::RotateV,RBX::AutoJoint,&RBX::sRotateV>::~FactoryProduct<RBX::RotateV,RBX::AutoJoint,&RBX::sRotateV>
	// TEMPLATE: WEBSERVICE 0x100d86b0
	// RBX::FactoryProduct<RBX::RotateV,RBX::AutoJoint,&RBX::sRotateV>::getClassName
	// STUB: WEBSERVICE 0x100d86d0
	// RBX::FactoryProduct<RBX::Motor,RBX::AutoJoint,&RBX::sMotor>::~FactoryProduct<RBX::Motor,RBX::AutoJoint,&RBX::sMotor>
	// FUNCTION: WEBSERVICE 0x100d8750
	// RBX::FactoryProduct<RBX::Motor,RBX::AutoJoint,&RBX::sMotor>::getClassName
	// STUB: WEBSERVICE 0x100d8770
	// RBX::FactoryProduct<RBX::Snap,RBX::AutoJoint,&RBX::sSnap>::`scalar deleting destructor'
	// STUB: WEBSERVICE 0x100d8790
	// RBX::FactoryProduct<RBX::Weld,RBX::AutoJoint,&RBX::sWeld>::`scalar deleting destructor'
	// STUB: WEBSERVICE 0x100d87b0
	// RBX::FactoryProduct<RBX::Glue,RBX::AutoJoint,&RBX::sGlue>::`scalar deleting destructor'
	// STUB: WEBSERVICE 0x100d9100
	// RBX::FactoryProduct<RBX::Snap,RBX::AutoJoint,&RBX::sSnap>::FactoryProduct<RBX::Snap,RBX::AutoJoint,&RBX::sSnap><RBX::Joint *>
	// STUB: WEBSERVICE 0x100d9170
	// RBX::FactoryProduct<RBX::Weld,RBX::AutoJoint,&RBX::sWeld>::FactoryProduct<RBX::Weld,RBX::AutoJoint,&RBX::sWeld><RBX::Joint *>
	// STUB: WEBSERVICE 0x100d91e0
	// RBX::FactoryProduct<RBX::Glue,RBX::AutoJoint,&RBX::sGlue>::FactoryProduct<RBX::Glue,RBX::AutoJoint,&RBX::sGlue><RBX::GlueJoint *>
	// STUB: WEBSERVICE 0x100d9250
	// RBX::FactoryProduct<RBX::Rotate,RBX::AutoJoint,&RBX::sRotate>::FactoryProduct<RBX::Rotate,RBX::AutoJoint,&RBX::sRotate><RBX::Joint *>
	// STUB: WEBSERVICE 0x100d92c0
	// RBX::FactoryProduct<RBX::RotateP,RBX::AutoJoint,&RBX::sRotateP>::FactoryProduct<RBX::RotateP,RBX::AutoJoint,&RBX::sRotateP><RBX::Joint *>
	// STUB: WEBSERVICE 0x100d9330
	// RBX::FactoryProduct<RBX::RotateV,RBX::AutoJoint,&RBX::sRotateV>::FactoryProduct<RBX::RotateV,RBX::AutoJoint,&RBX::sRotateV><RBX::Joint *>
	// STUB: WEBSERVICE 0x100d93a0
	// RBX::FactoryProduct<RBX::Motor,RBX::AutoJoint,&RBX::sMotor>::FactoryProduct<RBX::Motor,RBX::AutoJoint,&RBX::sMotor><RBX::MotorJoint *>
	// STUB: WEBSERVICE 0x100d9950
	// RBX::FactoryProduct<RBX::Motor,RBX::AutoJoint,&RBX::sMotor>::`scalar deleting destructor'
	// STUB: WEBSERVICE 0x100d9f70
	// RBX::FactoryProduct<RBX::Rotate,RBX::AutoJoint,&RBX::sRotate>::`scalar deleting destructor'
	// STUB: WEBSERVICE 0x100da0e0
	// RBX::FactoryProduct<RBX::RotateP,RBX::AutoJoint,&RBX::sRotateP>::`scalar deleting destructor'
	// STUB: WEBSERVICE 0x100da260
	// RBX::FactoryProduct<RBX::RotateV,RBX::AutoJoint,&RBX::sRotateV>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100da830
	// RBX::FactoryProduct<RBX::Teams,RBX::Instance,&RBX::sTeams>::~FactoryProduct<RBX::Teams,RBX::Instance,&RBX::sTeams>
	// FUNCTION: WEBSERVICE 0x100da880
	// RBX::FactoryProduct<RBX::Teams,RBX::Instance,&RBX::sTeams>::FactoryProduct<RBX::Teams,RBX::Instance,&RBX::sTeams>
	// STUB: WEBSERVICE 0x100da8e0
	// RBX::FactoryProduct<RBX::Teams,RBX::Instance,&RBX::sTeams>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x100dabc0
	// RBX::FactoryProduct<RBX::Teams,RBX::Instance,&RBX::sTeams>::getClassName
	// FUNCTION: WEBSERVICE 0x100db590
	// RBX::FactoryProduct<RBX::Accoutrement,RBX::Instance,&RBX::sAccoutrement>::~FactoryProduct<RBX::Accoutrement,RBX::Instance,&RBX::sAccoutrement>
	// FUNCTION: WEBSERVICE 0x100db5e0
	// RBX::FactoryProduct<RBX::Accoutrement,RBX::Instance,&RBX::sAccoutrement>::FactoryProduct<RBX::Accoutrement,RBX::Instance,&RBX::sAccoutrement>
	// STUB: WEBSERVICE 0x100dbdf0
	// RBX::FactoryProduct<RBX::Hat,RBX::Accoutrement,&RBX::sHat>::~FactoryProduct<RBX::Hat,RBX::Accoutrement,&RBX::sHat>
	// TEMPLATE: WEBSERVICE 0x100dc290
	// RBX::FactoryProduct<RBX::Accoutrement,RBX::Instance,&RBX::sAccoutrement>::getClassName
	// TEMPLATE: WEBSERVICE 0x100dc2b0
	// RBX::FactoryProduct<RBX::Hat,RBX::Accoutrement,&RBX::sHat>::getClassName
	// STUB: WEBSERVICE 0x100dcce0
	// RBX::FactoryProduct<RBX::Accoutrement,RBX::Instance,&RBX::sAccoutrement>::`scalar deleting destructor'
	// STUB: WEBSERVICE 0x100dcec0
	// RBX::FactoryProduct<RBX::Hat,RBX::Accoutrement,&RBX::sHat>::FactoryProduct<RBX::Hat,RBX::Accoutrement,&RBX::sHat>
	// STUB: WEBSERVICE 0x100dd950
	// RBX::FactoryProduct<RBX::Hat,RBX::Accoutrement,&RBX::sHat>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100de130
	// RBX::FactoryProduct<RBX::GameSettings,RBX::GlobalSettings::Item,&RBX::sGameSettings>::~FactoryProduct<RBX::GameSettings,RBX::GlobalSettings::Item,&RBX::sGameSettings>
	// STUB: WEBSERVICE 0x100de1b0
	// RBX::FactoryProduct<RBX::GameSettings,RBX::GlobalSettings::Item,&RBX::sGameSettings>::`scalar deleting destructor'
	// STUB: WEBSERVICE 0x100de1d0
	// RBX::FactoryProduct<RBX::GameSettings,RBX::GlobalSettings::Item,&RBX::sGameSettings>::FactoryProduct<RBX::GameSettings,RBX::GlobalSettings::Item,&RBX::sGameSettings>
	// TEMPLATE: WEBSERVICE 0x100de2e0
	// RBX::FactoryProduct<RBX::GameSettings,RBX::GlobalSettings::Item,&RBX::sGameSettings>::getClassName
	// FUNCTION: WEBSERVICE 0x100e04a0
	// RBX::FactoryProduct<RBX::ShirtGraphic,RBX::CharacterAppearance,&RBX::sShirtGraphic>::~FactoryProduct<RBX::ShirtGraphic,RBX::CharacterAppearance,&RBX::sShirtGraphic>
	// FUNCTION: WEBSERVICE 0x100e0530
	// RBX::FactoryProduct<RBX::BodyColors,RBX::CharacterAppearance,&RBX::sBodyColors>::~FactoryProduct<RBX::BodyColors,RBX::CharacterAppearance,&RBX::sBodyColors>
	// FUNCTION: WEBSERVICE 0x100e05a0
	// RBX::FactoryProduct<RBX::Skin,RBX::CharacterAppearance,&RBX::sSkin>::~FactoryProduct<RBX::Skin,RBX::CharacterAppearance,&RBX::sSkin>
	// STUB: WEBSERVICE 0x100e0640
	// RBX::FactoryProduct<RBX::ShirtGraphic,RBX::CharacterAppearance,&RBX::sShirtGraphic>::FactoryProduct<RBX::ShirtGraphic,RBX::CharacterAppearance,&RBX::sShirtGraphic>
	// STUB: WEBSERVICE 0x100e06a0
	// RBX::FactoryProduct<RBX::BodyColors,RBX::CharacterAppearance,&RBX::sBodyColors>::FactoryProduct<RBX::BodyColors,RBX::CharacterAppearance,&RBX::sBodyColors>
	// STUB: WEBSERVICE 0x100e0700
	// RBX::FactoryProduct<RBX::Skin,RBX::CharacterAppearance,&RBX::sSkin>::FactoryProduct<RBX::Skin,RBX::CharacterAppearance,&RBX::sSkin>
	// STUB: WEBSERVICE 0x100e0c40
	// RBX::FactoryProduct<RBX::Shirt,RBX::ShirtGraphic,&RBX::sShirt>::~FactoryProduct<RBX::Shirt,RBX::ShirtGraphic,&RBX::sShirt>
	// STUB: WEBSERVICE 0x100e0cb0
	// RBX::FactoryProduct<RBX::Shirt,RBX::ShirtGraphic,&RBX::sShirt>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x100e0cd0
	// RBX::FactoryProduct<RBX::ShirtGraphic,RBX::CharacterAppearance,&RBX::sShirtGraphic>::getClassName
	// TEMPLATE: WEBSERVICE 0x100e0cf0
	// RBX::FactoryProduct<RBX::Shirt,RBX::ShirtGraphic,&RBX::sShirt>::getClassName
	// TEMPLATE: WEBSERVICE 0x100e0d10
	// RBX::FactoryProduct<RBX::BodyColors,RBX::CharacterAppearance,&RBX::sBodyColors>::getClassName
	// TEMPLATE: WEBSERVICE 0x100e0d30
	// RBX::FactoryProduct<RBX::Skin,RBX::CharacterAppearance,&RBX::sSkin>::getClassName
	// STUB: WEBSERVICE 0x100e1290
	// RBX::FactoryProduct<RBX::ShirtGraphic,RBX::CharacterAppearance,&RBX::sShirtGraphic>::`scalar deleting destructor'
	// STUB: WEBSERVICE 0x100e1380
	// RBX::FactoryProduct<RBX::BodyColors,RBX::CharacterAppearance,&RBX::sBodyColors>::`scalar deleting destructor'
	// STUB: WEBSERVICE 0x100e16b0
	// RBX::FactoryProduct<RBX::Skin,RBX::CharacterAppearance,&RBX::sSkin>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100e17e0
	// RBX::FactoryProduct<RBX::Shirt,RBX::ShirtGraphic,&RBX::sShirt>::FactoryProduct<RBX::Shirt,RBX::ShirtGraphic,&RBX::sShirt>
	// FUNCTION: WEBSERVICE 0x100e1b80
	// RBX::FactoryProduct<RBX::Explosion,RBX::Instance,&RBX::sExplosion>::~FactoryProduct<RBX::Explosion,RBX::Instance,&RBX::sExplosion>
	// FUNCTION: WEBSERVICE 0x100e1bd0
	// RBX::FactoryProduct<RBX::Explosion,RBX::Instance,&RBX::sExplosion>::FactoryProduct<RBX::Explosion,RBX::Instance,&RBX::sExplosion>
	// TEMPLATE: WEBSERVICE 0x100e2280
	// RBX::FactoryProduct<RBX::Explosion,RBX::Instance,&RBX::sExplosion>::getClassName
	// STUB: WEBSERVICE 0x100e2ce0
	// RBX::FactoryProduct<RBX::Explosion,RBX::Instance,&RBX::sExplosion>::`scalar deleting destructor'
	// STUB: WEBSERVICE 0x100e3c40
	// RBX::FactoryProduct<RBX::VelocityMotor,RBX::JointInstance,&RBX::sVelocityMotor>::~FactoryProduct<RBX::VelocityMotor,RBX::JointInstance,&RBX::sVelocityMotor>
	// STUB: WEBSERVICE 0x100e3d70
	// RBX::FactoryProduct<RBX::VelocityMotor,RBX::JointInstance,&RBX::sVelocityMotor>::FactoryProduct<RBX::VelocityMotor,RBX::JointInstance,&RBX::sVelocityMotor><RBX::MotorJoint *>
	// STUB: WEBSERVICE 0x100e4860
	// RBX::FactoryProduct<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature>::~FactoryProduct<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature>
	// STUB: WEBSERVICE 0x100e4900
	// RBX::FactoryProduct<RBX::Hole,RBX::Feature,&RBX::sHole>::~FactoryProduct<RBX::Hole,RBX::Feature,&RBX::sHole>
	// TEMPLATE: WEBSERVICE 0x100e4ee0
	// RBX::FactoryProduct<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature>::getClassName
	// TEMPLATE: WEBSERVICE 0x100e4f00
	// RBX::FactoryProduct<RBX::Hole,RBX::Feature,&RBX::sHole>::getClassName
	// STUB: WEBSERVICE 0x100e4f20
	// RBX::FactoryProduct<RBX::VelocityMotor,RBX::JointInstance,&RBX::sVelocityMotor>::getClassName
	// STUB: WEBSERVICE 0x100e6d00
	// RBX::FactoryProduct<RBX::VelocityMotor,RBX::JointInstance,&RBX::sVelocityMotor>::`scalar deleting destructor'
	// STUB: WEBSERVICE 0x100e7350
	// RBX::FactoryProduct<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature>::FactoryProduct<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature>
	// STUB: WEBSERVICE 0x100e73c0
	// RBX::FactoryProduct<RBX::Hole,RBX::Feature,&RBX::sHole>::FactoryProduct<RBX::Hole,RBX::Feature,&RBX::sHole>
	// STUB: WEBSERVICE 0x100e7890
	// RBX::FactoryProduct<RBX::Hole,RBX::Feature,&RBX::sHole>::`scalar deleting destructor'
	// STUB: WEBSERVICE 0x100e7ab0
	// RBX::FactoryProduct<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100e7be0
	// RBX::FactoryProduct<RBX::FlagStandService,RBX::Instance,&RBX::sFlagStandService>::~FactoryProduct<RBX::FlagStandService,RBX::Instance,&RBX::sFlagStandService>
	// FUNCTION: WEBSERVICE 0x100e7c30
	// RBX::FactoryProduct<RBX::FlagStandService,RBX::Instance,&RBX::sFlagStandService>::FactoryProduct<RBX::FlagStandService,RBX::Instance,&RBX::sFlagStandService>
	// STUB: WEBSERVICE 0x100e7d00
	// RBX::FactoryProduct<RBX::FlagStandService,RBX::Instance,&RBX::sFlagStandService>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x100e83b0
	// RBX::FactoryProduct<RBX::FlagStandService,RBX::Instance,&RBX::sFlagStandService>::getClassName
	// FUNCTION: WEBSERVICE 0x100e92a0
	// RBX::FactoryProduct<RBX::ForceField,RBX::Instance,&RBX::sForceField>::~FactoryProduct<RBX::ForceField,RBX::Instance,&RBX::sForceField>
	// FUNCTION: WEBSERVICE 0x100e92f0
	// RBX::FactoryProduct<RBX::ForceField,RBX::Instance,&RBX::sForceField>::FactoryProduct<RBX::ForceField,RBX::Instance,&RBX::sForceField>
	// TEMPLATE: WEBSERVICE 0x100e9550
	// RBX::FactoryProduct<RBX::ForceField,RBX::Instance,&RBX::sForceField>::getClassName
	// STUB: WEBSERVICE 0x100e97d0
	// RBX::FactoryProduct<RBX::ForceField,RBX::Instance,&RBX::sForceField>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100e9a10
	// RBX::FactoryProduct<RBX::GeometryService,RBX::Instance,&RBX::sGeometryService>::~FactoryProduct<RBX::GeometryService,RBX::Instance,&RBX::sGeometryService>
	// STUB: WEBSERVICE 0x100e9a60
	// RBX::FactoryProduct<RBX::GeometryService,RBX::Instance,&RBX::sGeometryService>::FactoryProduct<RBX::GeometryService,RBX::Instance,&RBX::sGeometryService><char *>
	// STUB: WEBSERVICE 0x100e9ac0
	// RBX::FactoryProduct<RBX::GeometryService,RBX::Instance,&RBX::sGeometryService>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x100e9b70
	// RBX::FactoryProduct<RBX::GeometryService,RBX::Instance,&RBX::sGeometryService>::getClassName
	// STUB: WEBSERVICE 0x100eb370
	// RBX::FactoryProduct<RBX::BodyGyro,RBX::BodyMover,&RBX::sBodyGyro>::~FactoryProduct<RBX::BodyGyro,RBX::BodyMover,&RBX::sBodyGyro>
	// STUB: WEBSERVICE 0x100eb400
	// RBX::FactoryProduct<RBX::BodyForce,RBX::BodyMover,&RBX::sBodyForce>::~FactoryProduct<RBX::BodyForce,RBX::BodyMover,&RBX::sBodyForce>
	// STUB: WEBSERVICE 0x100eb470
	// RBX::FactoryProduct<RBX::BodyThrust,RBX::BodyMover,&RBX::sBodyThrust>::~FactoryProduct<RBX::BodyThrust,RBX::BodyMover,&RBX::sBodyThrust>
	// STUB: WEBSERVICE 0x100eb520
	// RBX::FactoryProduct<RBX::BodyPosition,RBX::BodyMover,&RBX::sBodyPosition>::~FactoryProduct<RBX::BodyPosition,RBX::BodyMover,&RBX::sBodyPosition>
	// STUB: WEBSERVICE 0x100eb5d0
	// RBX::FactoryProduct<RBX::BodyVelocity,RBX::BodyMover,&RBX::sBodyVelocity>::~FactoryProduct<RBX::BodyVelocity,RBX::BodyMover,&RBX::sBodyVelocity>
	// STUB: WEBSERVICE 0x100eb670
	// RBX::FactoryProduct<RBX::Rocket,RBX::BodyMover,&RBX::sRocket>::~FactoryProduct<RBX::Rocket,RBX::BodyMover,&RBX::sRocket>
	// STUB: WEBSERVICE 0x100eb730
	// RBX::FactoryProduct<RBX::BodyGyro,RBX::BodyMover,&RBX::sBodyGyro>::`scalar deleting destructor'
	// STUB: WEBSERVICE 0x100eb750
	// RBX::FactoryProduct<RBX::BodyThrust,RBX::BodyMover,&RBX::sBodyThrust>::`scalar deleting destructor'
	// STUB: WEBSERVICE 0x100eb770
	// RBX::FactoryProduct<RBX::BodyPosition,RBX::BodyMover,&RBX::sBodyPosition>::`scalar deleting destructor'
	// STUB: WEBSERVICE 0x100eb7f0
	// RBX::FactoryProduct<RBX::Rocket,RBX::BodyMover,&RBX::sRocket>::FactoryProduct<RBX::Rocket,RBX::BodyMover,&RBX::sRocket><char *>
	// STUB: WEBSERVICE 0x100eb870
	// RBX::FactoryProduct<RBX::BodyGyro,RBX::BodyMover,&RBX::sBodyGyro>::FactoryProduct<RBX::BodyGyro,RBX::BodyMover,&RBX::sBodyGyro><char *>
	// STUB: WEBSERVICE 0x100eb8f0
	// RBX::FactoryProduct<RBX::BodyPosition,RBX::BodyMover,&RBX::sBodyPosition>::FactoryProduct<RBX::BodyPosition,RBX::BodyMover,&RBX::sBodyPosition><char *>
	// STUB: WEBSERVICE 0x100eb970
	// RBX::FactoryProduct<RBX::BodyVelocity,RBX::BodyMover,&RBX::sBodyVelocity>::FactoryProduct<RBX::BodyVelocity,RBX::BodyMover,&RBX::sBodyVelocity><char *>
	// STUB: WEBSERVICE 0x100eb9f0
	// RBX::FactoryProduct<RBX::BodyForce,RBX::BodyMover,&RBX::sBodyForce>::FactoryProduct<RBX::BodyForce,RBX::BodyMover,&RBX::sBodyForce><char *>
	// STUB: WEBSERVICE 0x100eba70
	// RBX::FactoryProduct<RBX::BodyThrust,RBX::BodyMover,&RBX::sBodyThrust>::FactoryProduct<RBX::BodyThrust,RBX::BodyMover,&RBX::sBodyThrust><char *>
	// TEMPLATE: WEBSERVICE 0x100ec060
	// RBX::FactoryProduct<RBX::BodyGyro,RBX::BodyMover,&RBX::sBodyGyro>::getClassName
	// TEMPLATE: WEBSERVICE 0x100ec080
	// RBX::FactoryProduct<RBX::BodyForce,RBX::BodyMover,&RBX::sBodyForce>::getClassName
	// TEMPLATE: WEBSERVICE 0x100ec0a0
	// RBX::FactoryProduct<RBX::BodyThrust,RBX::BodyMover,&RBX::sBodyThrust>::getClassName
	// TEMPLATE: WEBSERVICE 0x100ec0c0
	// RBX::FactoryProduct<RBX::BodyPosition,RBX::BodyMover,&RBX::sBodyPosition>::getClassName
	// TEMPLATE: WEBSERVICE 0x100ec0e0
	// RBX::FactoryProduct<RBX::BodyVelocity,RBX::BodyMover,&RBX::sBodyVelocity>::getClassName
	// TEMPLATE: WEBSERVICE 0x100ec100
	// RBX::FactoryProduct<RBX::Rocket,RBX::BodyMover,&RBX::sRocket>::getClassName
	// STUB: WEBSERVICE 0x100ed820
	// RBX::FactoryProduct<RBX::BodyVelocity,RBX::BodyMover,&RBX::sBodyVelocity>::`scalar deleting destructor'
	// STUB: WEBSERVICE 0x100ed9c0
	// RBX::FactoryProduct<RBX::BodyForce,RBX::BodyMover,&RBX::sBodyForce>::`scalar deleting destructor'
	// STUB: WEBSERVICE 0x100edeb0
	// RBX::FactoryProduct<RBX::Rocket,RBX::BodyMover,&RBX::sRocket>::`scalar deleting destructor'
	// STUB: WEBSERVICE 0x100ee470
	// RBX::FactoryProduct<RBX::Message,RBX::Instance,&RBX::sMessage>::FactoryProduct<RBX::Message,RBX::Instance,&RBX::sMessage><char const *>
	// FUNCTION: WEBSERVICE 0x100f8b90
	// RBX::FactoryProduct<RBX::Visit,RBX::Instance,&RBX::sVisit>::~FactoryProduct<RBX::Visit,RBX::Instance,&RBX::sVisit>
	// FUNCTION: WEBSERVICE 0x100f8c40
	// RBX::FactoryProduct<RBX::Visit,RBX::Instance,&RBX::sVisit>::FactoryProduct<RBX::Visit,RBX::Instance,&RBX::sVisit>
	// TEMPLATE: WEBSERVICE 0x100f9080
	// RBX::FactoryProduct<RBX::Visit,RBX::Instance,&RBX::sVisit>::getClassName
	// STUB: WEBSERVICE 0x100f95b0
	// RBX::FactoryProduct<RBX::Visit,RBX::Instance,&RBX::sVisit>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100f9df0
	// RBX::FactoryProduct<RBX::DebrisService,RBX::Instance,&RBX::sDebrisService>::~FactoryProduct<RBX::DebrisService,RBX::Instance,&RBX::sDebrisService>
	// STUB: WEBSERVICE 0x100f9f20
	// RBX::FactoryProduct<RBX::DebrisService,RBX::Instance,&RBX::sDebrisService>::FactoryProduct<RBX::DebrisService,RBX::Instance,&RBX::sDebrisService><char *>
	// TEMPLATE: WEBSERVICE 0x100fa220
	// RBX::FactoryProduct<RBX::DebrisService,RBX::Instance,&RBX::sDebrisService>::getClassName
	// STUB: WEBSERVICE 0x100fad10
	// RBX::FactoryProduct<RBX::DebrisService,RBX::Instance,&RBX::sDebrisService>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100fb580
	// RBX::FactoryProduct<RBX::TimerService,RBX::Instance,&RBX::sTimerService>::~FactoryProduct<RBX::TimerService,RBX::Instance,&RBX::sTimerService>
	// FUNCTION: WEBSERVICE 0x100fb5d0
	// RBX::FactoryProduct<RBX::TimerService,RBX::Instance,&RBX::sTimerService>::FactoryProduct<RBX::TimerService,RBX::Instance,&RBX::sTimerService>
	// STUB: WEBSERVICE 0x100fb630
	// RBX::FactoryProduct<RBX::TimerService,RBX::Instance,&RBX::sTimerService>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x100fb880
	// RBX::FactoryProduct<RBX::TimerService,RBX::Instance,&RBX::sTimerService>::getClassName
	// STUB: WEBSERVICE 0x100fc040
	// RBX::FactoryProduct<RBX::Seat,RBX::PartInstance,&RBX::sSeat>::~FactoryProduct<RBX::Seat,RBX::PartInstance,&RBX::sSeat>
	// STUB: WEBSERVICE 0x100fc2a0
	// RBX::FactoryProduct<RBX::Seat,RBX::PartInstance,&RBX::sSeat>::`scalar deleting destructor'
	// STUB: WEBSERVICE 0x100fc2c0
	// RBX::FactoryProduct<RBX::Seat,RBX::PartInstance,&RBX::sSeat>::FactoryProduct<RBX::Seat,RBX::PartInstance,&RBX::sSeat>
	// TEMPLATE: WEBSERVICE 0x100fc820
	// RBX::FactoryProduct<RBX::Seat,RBX::PartInstance,&RBX::sSeat>::getClassName
	// FUNCTION: WEBSERVICE 0x100fd610
	// RBX::FactoryProduct<RBX::SpawnerService,RBX::Instance,&RBX::sSpawnerService>::~FactoryProduct<RBX::SpawnerService,RBX::Instance,&RBX::sSpawnerService>
	// FUNCTION: WEBSERVICE 0x100fd660
	// RBX::FactoryProduct<RBX::SpawnerService,RBX::Instance,&RBX::sSpawnerService>::FactoryProduct<RBX::SpawnerService,RBX::Instance,&RBX::sSpawnerService>
	// TEMPLATE: WEBSERVICE 0x100fdd50
	// RBX::FactoryProduct<RBX::SpawnerService,RBX::Instance,&RBX::sSpawnerService>::getClassName
	// STUB: WEBSERVICE 0x100febb0
	// RBX::FactoryProduct<RBX::SpawnerService,RBX::Instance,&RBX::sSpawnerService>::`scalar deleting destructor'
	// STUB: WEBSERVICE 0x100ffaa0
	// RBX::FactoryProduct<RBX::Texture,RBX::Decal,&RBX::sTexture>::~FactoryProduct<RBX::Texture,RBX::Decal,&RBX::sTexture>
	// TEMPLATE: WEBSERVICE 0x100ffb10
	// RBX::FactoryProduct<RBX::Texture,RBX::Decal,&RBX::sTexture>::getClassName
	// FUNCTION: WEBSERVICE 0x10100110
	// RBX::FactoryProduct<RBX::Texture,RBX::Decal,&RBX::sTexture>::FactoryProduct<RBX::Texture,RBX::Decal,&RBX::sTexture>
	// STUB: WEBSERVICE 0x101004b0
	// RBX::FactoryProduct<RBX::Texture,RBX::Decal,&RBX::sTexture>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x10129f20
	// RBX::FactoryProduct<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape>::Creator::create
	// FUNCTION: WEBSERVICE 0x1012c970
	// RBX::FactoryProduct<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape>::~FactoryProduct<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape>
	// FUNCTION: WEBSERVICE 0x1012c9c0
	// RBX::FactoryProduct<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape>::FactoryProduct<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape>
	// STUB: WEBSERVICE 0x1012ca90
	// RBX::FactoryProduct<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x1012cef0
	// RBX::FactoryProduct<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape>::getClassName
	// clang-format on
	FactoryProduct() {}

	template <class U>
	FactoryProduct(U* joint) : Base(joint)
	{
	}

	const Creator& getCreator() const { return creator; }

	static const Name& className() { return Name::declare<sName>(); }

	virtual const Name& getClassName() const { return className(); }

protected:
	virtual ~FactoryProduct() {} // vtable+0x00

private:
	static Creator creator;
};

template <class T, class Base, const char* sName>
typename FactoryProduct<T, Base, sName>::Creator FactoryProduct<T, Base, sName>::creator;

template <class Base, const char* sName>
class NonFactoryProduct : public Base
{
public:
	NonFactoryProduct() {}

	template <class U>
	NonFactoryProduct(U* joint) : Base(joint)
	{
	}

	static const Name& className() { return Name::declare<sName>(); }

	virtual const Name& getClassName() const { return className(); }
};

} // namespace RBX

#endif // UTIL_OBJECT_H
