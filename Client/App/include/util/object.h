#ifndef UTIL_OBJECT_H
#define UTIL_OBJECT_H

#include "decomp.h"
#include "util/Name.h"

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <map>
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
// TEMPLATE: WEBSERVICE 0x100c5e40
// RBX::shared_from<RBX::Instance const>
// TEMPLATE: WEBSERVICE 0x1016ef10
// RBX::shared_from<RBX::GuiItem>
// clang-format on
template <class T>
boost::shared_ptr<T> shared_from(T* r)
{
	return r != NULL ? boost::shared_static_cast<T>(r->shared_from_this()) : boost::shared_ptr<T>();
}

template <class T, class Base>
boost::shared_ptr<T> shared_from_dynamic_cast(boost::enable_shared_from_this<Base>* r)
{
	return r != NULL ? boost::shared_dynamic_cast<T>(r->shared_from_this()) : boost::shared_ptr<T>();
}

template <class T>
class Creatable : public Object
{
public:
	class Deleter
	{
	public:
		void operator()(T* instance)
		{
			T::predelete(instance);
			delete instance;
		}
	};

	// clang-format off
	// FUNCTION: WEBSERVICE 0x10009450
	// RBX::Creatable<RBX::Instance>::create<RBX::Lighting>
	// FUNCTION: WEBSERVICE 0x100094e0
	// RBX::Creatable<RBX::Instance>::create<RBX::RunService>
	// TEMPLATE: WEBSERVICE 0x10009570
	// RBX::Creatable<RBX::Instance>::create<RBX::ModelInstance>
	// FUNCTION: WEBSERVICE 0x10009600
	// RBX::Creatable<RBX::Instance>::create<RBX::Selection>
	// FUNCTION: WEBSERVICE 0x10009690
	// RBX::Creatable<RBX::Instance>::create<RBX::ScriptContext>
	// FUNCTION: WEBSERVICE 0x10009720
	// RBX::Creatable<RBX::Instance>::create<RBX::Script>
	// FUNCTION: WEBSERVICE 0x100097b0
	// RBX::Creatable<RBX::Instance>::create<RBX::LocalScript>
	// FUNCTION: WEBSERVICE 0x1005bab0
	// RBX::Creatable<RBX::Instance>::create<RBX::Humanoid>
	// TEMPLATE: WEBSERVICE 0x10062870
	// RBX::Creatable<RBX::Instance>::create<RBX::DebugSettings>
	// TEMPLATE: WEBSERVICE 0x100657f0
	// RBX::Creatable<RBX::Instance>::create<RBX::Stats::Item,char const *>
	// TEMPLATE: WEBSERVICE 0x100659d0
	// RBX::Creatable<RBX::Instance>::create<RBX::Stats::StatsService>
	// TEMPLATE: WEBSERVICE 0x100666f0
	// RBX::Creatable<RBX::Instance>::create<RBX::Stats::TypedStatsItem<bool>,bool const *>
	// TEMPLATE: WEBSERVICE 0x10066770
	// RBX::Creatable<RBX::Instance>::create<RBX::Stats::TypedStatsItem<int>,boost::function0<int,std::allocator<boost::function_base> > >
	// TEMPLATE: WEBSERVICE 0x100691f0
	// RBX::Creatable<RBX::Instance>::create<RBX::Team>
	// TEMPLATE: WEBSERVICE 0x1006bd80
	// RBX::Creatable<RBX::Instance>::create<RBX::Camera>
	// TEMPLATE: WEBSERVICE 0x1006be50
	// RBX::Creatable<RBX::Instance>::create<RBX::ControllerService>
	// TEMPLATE: WEBSERVICE 0x1006bed0
	// RBX::Creatable<RBX::Instance>::create<RBX::StarterPackService>
	// TEMPLATE: WEBSERVICE 0x10076620
	// RBX::Creatable<RBX::Instance>::create<RBX::Snap>
	// TEMPLATE: WEBSERVICE 0x10076890
	// RBX::Creatable<RBX::Instance>::create<RBX::Weld>
	// TEMPLATE: WEBSERVICE 0x10076b00
	// RBX::Creatable<RBX::Instance>::create<RBX::Glue>
	// TEMPLATE: WEBSERVICE 0x10076d70
	// RBX::Creatable<RBX::Instance>::create<RBX::Rotate>
	// TEMPLATE: WEBSERVICE 0x10076fe0
	// RBX::Creatable<RBX::Instance>::create<RBX::RotateP>
	// TEMPLATE: WEBSERVICE 0x10077250
	// RBX::Creatable<RBX::Instance>::create<RBX::RotateV>
	// TEMPLATE: WEBSERVICE 0x100779a0
	// RBX::Creatable<RBX::Instance>::create<RBX::Teams>
	// STUB: WEBSERVICE 0x10077c10
	// RBX::Creatable<RBX::Instance>::create<RBX::Accoutrement>
	// STUB: WEBSERVICE 0x10077e80
	// RBX::Creatable<RBX::Instance>::create<RBX::Hat>
	// TEMPLATE: WEBSERVICE 0x1007e750
	// RBX::Creatable<RBX::Instance>::create<RBX::GameSettings>
	// TEMPLATE: WEBSERVICE 0x10089930
	// RBX::Creatable<RBX::Instance>::create<RBX::ShirtGraphic>
	// TEMPLATE: WEBSERVICE 0x10089ba0
	// RBX::Creatable<RBX::Instance>::create<RBX::Shirt>
	// TEMPLATE: WEBSERVICE 0x10089e10
	// RBX::Creatable<RBX::Instance>::create<RBX::BodyColors>
	// TEMPLATE: WEBSERVICE 0x1008a080
	// RBX::Creatable<RBX::Instance>::create<RBX::Skin>
	// TEMPLATE: WEBSERVICE 0x1008a2f0
	// RBX::Creatable<RBX::Instance>::create<RBX::Explosion>
	// TEMPLATE: WEBSERVICE 0x1008a560
	// RBX::Creatable<RBX::Instance>::create<RBX::MotorFeature>
	// TEMPLATE: WEBSERVICE 0x1008a7d0
	// RBX::Creatable<RBX::Instance>::create<RBX::Hole>
	// TEMPLATE: WEBSERVICE 0x1008af20
	// RBX::Creatable<RBX::Instance>::create<RBX::FlagStandService>
	// TEMPLATE: WEBSERVICE 0x1008b190
	// RBX::Creatable<RBX::Instance>::create<RBX::ForceField>
	// TEMPLATE: WEBSERVICE 0x1008b400
	// RBX::Creatable<RBX::Instance>::create<RBX::GeometryService>
	// TEMPLATE: WEBSERVICE 0x1008b670
	// RBX::Creatable<RBX::Instance>::create<RBX::BodyGyro>
	// TEMPLATE: WEBSERVICE 0x1008b8e0
	// RBX::Creatable<RBX::Instance>::create<RBX::BodyForce>
	// TEMPLATE: WEBSERVICE 0x1008bb50
	// RBX::Creatable<RBX::Instance>::create<RBX::BodyThrust>
	// TEMPLATE: WEBSERVICE 0x1008bdc0
	// RBX::Creatable<RBX::Instance>::create<RBX::BodyPosition>
	// TEMPLATE: WEBSERVICE 0x1008c030
	// RBX::Creatable<RBX::Instance>::create<RBX::BodyVelocity>
	// TEMPLATE: WEBSERVICE 0x1008c2a0
	// RBX::Creatable<RBX::Instance>::create<RBX::Rocket>
	// TEMPLATE: WEBSERVICE 0x1008c510
	// RBX::Creatable<RBX::Instance>::create<RBX::Message>
	// TEMPLATE: WEBSERVICE 0x1008ce10
	// RBX::Creatable<RBX::Instance>::create<RBX::Visit>
	// TEMPLATE: WEBSERVICE 0x1008d080
	// RBX::Creatable<RBX::Instance>::create<RBX::DebrisService>
	// TEMPLATE: WEBSERVICE 0x1008d2f0
	// RBX::Creatable<RBX::Instance>::create<RBX::TimerService>
	// FUNCTION: WEBSERVICE 0x1008d560
	// RBX::Creatable<RBX::Instance>::create<RBX::Seat>
	// TEMPLATE: WEBSERVICE 0x1008da40
	// RBX::Creatable<RBX::Instance>::create<RBX::SpawnerService>
	// TEMPLATE: WEBSERVICE 0x1008e7a0
	// RBX::Creatable<RBX::Instance>::create<RBX::Hint>
	// TEMPLATE: WEBSERVICE 0x1008e820
	// RBX::Creatable<RBX::Instance>::create<RBX::ObjectValue>
	// TEMPLATE: WEBSERVICE 0x100b7470
	// RBX::Creatable<RBX::Instance>::create<RBX::GlobalSettings>
	// TEMPLATE: WEBSERVICE 0x100ad050
	// RBX::Creatable<RBX::Instance>::create<RBX::Texture>
	// TEMPLATE: WEBSERVICE 0x10077730
	// RBX::Creatable<RBX::Instance>::create<RBX::Backpack>
	// TEMPLATE: WEBSERVICE 0x100b2810
	// RBX::Creatable<RBX::Instance>::create<RBX::Stats::Item>
	// TEMPLATE: WEBSERVICE 0x100b2890
	// RBX::Creatable<RBX::Instance>::create<ProfilingItem,RBX::Profiling::Profiler const *>
	// TEMPLATE: WEBSERVICE 0x100b2ba0
	// RBX::Creatable<RBX::Instance>::create<RBX::Stats::TypedMemItem,unsigned int const *>
	// TEMPLATE: WEBSERVICE 0x10069460
	// RBX::Creatable<RBX::Instance>::create<RBX::Network::Player>
	// TEMPLATE: WEBSERVICE 0x100a1d00
	// RBX::shared_from_dynamic_cast<RBX::PartInstance,RBX::Instance>
	// TEMPLATE: WEBSERVICE 0x100dda40
	// RBX::Creatable<RBX::Instance>::create<RBX::Snap,RBX::Joint *>
	// TEMPLATE: WEBSERVICE 0x100ddac0
	// RBX::Creatable<RBX::Instance>::create<RBX::Weld,RBX::Joint *>
	// TEMPLATE: WEBSERVICE 0x100ddb40
	// RBX::Creatable<RBX::Instance>::create<RBX::Glue,RBX::Joint *>
	// TEMPLATE: WEBSERVICE 0x100ddbc0
	// RBX::Creatable<RBX::Instance>::create<RBX::Rotate,RBX::Joint *>
	// TEMPLATE: WEBSERVICE 0x100ddc40
	// RBX::Creatable<RBX::Instance>::create<RBX::RotateP,RBX::Joint *>
	// TEMPLATE: WEBSERVICE 0x100ddcc0
	// RBX::Creatable<RBX::Instance>::create<RBX::RotateV,RBX::Joint *>
	// TEMPLATE: WEBSERVICE 0x100f5c50
	// RBX::Creatable<RBX::Instance>::create<RBX::Value<int,&RBX::sIntValue> >
	// TEMPLATE: WEBSERVICE 0x100f5cd0
	// RBX::Creatable<RBX::Instance>::create<RBX::Value<bool,&RBX::sBoolValue> >
	// TEMPLATE: WEBSERVICE 0x100f5d50
	// RBX::Creatable<RBX::Instance>::create<RBX::Value<double,&RBX::sDoubleValue> >
	// TEMPLATE: WEBSERVICE 0x100f5dd0
	// RBX::Creatable<RBX::Instance>::create<RBX::Value<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,&RBX::sStringValue> >
	// TEMPLATE: WEBSERVICE 0x100f5e50
	// RBX::Creatable<RBX::Instance>::create<RBX::Value<G3D::Vector3,&RBX::sVector3Value> >
	// TEMPLATE: WEBSERVICE 0x100f5ed0
	// RBX::Creatable<RBX::Instance>::create<RBX::Value<G3D::CoordinateFrame,&RBX::sCFrameValue> >
	// TEMPLATE: WEBSERVICE 0x100f5f50
	// RBX::Creatable<RBX::Instance>::create<RBX::Value<G3D::Color3,&RBX::sColor3Value> >
	// TEMPLATE: WEBSERVICE 0x100f5fd0
	// RBX::Creatable<RBX::Instance>::create<RBX::Value<RBX::BrickColor,&RBX::sBrickColorValue> >
	// TEMPLATE: WEBSERVICE 0x1018b8e0
	// RBX::Creatable<RBX::Instance>::create<RBX::NetworkSettings>
	// TEMPLATE: WEBSERVICE 0x10129ea0
	// RBX::Creatable<RBX::Instance>::create<RBX::SpecialShape>
	// clang-format on
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

// SIZE 0x4
class __declspec(novtable) ICreator
{
public:
	virtual boost::shared_ptr<Object> create() const = 0; // vtable+0x00
};

DECOMP_SIZE_ASSERT(ICreator, 0x4)

// clang-format off
// TEMPLATE: WEBSERVICE 0x10047dc0
// RBX::AbstractFactoryProduct<RBX::Instance>::~AbstractFactoryProduct<RBX::Instance>
// FUNCTION: WEBSERVICE 0x10048490
// RBX::AbstractFactoryProduct<RBX::Instance>::`scalar deleting destructor'
// clang-format on
template <class T>
class AbstractFactoryProduct : public Creatable<T>
{
protected:
	// TEMPLATE: WEBSERVICE 0x10009a60
	// RBX::AbstractFactoryProduct<RBX::Instance>::getCreators
	static std::map<const Name*, const ICreator*>& getCreators()
	{
		static std::map<const Name*, const ICreator*> creators;
		return creators;
	}

public:
	virtual const Name& getClassName() const = 0; // vtable+0x04

	virtual ~AbstractFactoryProduct() {}
};

template <class T, class Base, const char* sName>
class FactoryProduct : public Base
{
public:
	class Creator : public ICreator
	{
	public:
		virtual boost::shared_ptr<Object> create() const { return Creatable<Instance>::create<T>(); }

		const Name& getClassName() const { return Name::declare<sName>(); }

		Creator() { FactoryProduct::getCreators()[&getClassName()] = this; }

		~Creator() { FactoryProduct::getCreators().erase(&getClassName()); }
	};

	// clang-format off
	// FUNCTION: WEBSERVICE 0x10007b20
	// RBX::FactoryProduct<RBX::Lighting,RBX::Instance,&RBX::sLighting>::Creator::create
	// FUNCTION: WEBSERVICE 0x10007be0
	// RBX::FactoryProduct<RBX::RunService,RBX::Instance,&RBX::sRunService>::Creator::create
	// TEMPLATE: WEBSERVICE 0x10007ca0
	// RBX::FactoryProduct<RBX::ModelInstance,RBX::PVInstance,&RBX::sModel>::Creator::create
	// FUNCTION: WEBSERVICE 0x10007d60
	// RBX::FactoryProduct<RBX::Selection,RBX::Instance,&RBX::sSelection>::Creator::create
	// FUNCTION: WEBSERVICE 0x10007e20
	// RBX::FactoryProduct<RBX::ScriptContext,RBX::Instance,&RBX::sScriptContext>::Creator::create
	// FUNCTION: WEBSERVICE 0x10007ee0
	// RBX::FactoryProduct<RBX::Script,RBX::Instance,&RBX::sScript>::Creator::create
	// FUNCTION: WEBSERVICE 0x10007fa0
	// RBX::FactoryProduct<RBX::LocalScript,RBX::Script,&RBX::sLocalScript>::Creator::create
	// TEMPLATE: WEBSERVICE 0x10042700
	// RBX::FactoryProduct<RBX::Lighting,RBX::Instance,&RBX::sLighting>::~FactoryProduct<RBX::Lighting,RBX::Instance,&RBX::sLighting>
	// TEMPLATE: WEBSERVICE 0x10042760
	// RBX::FactoryProduct<RBX::Lighting,RBX::Instance,&RBX::sLighting>::FactoryProduct<RBX::Lighting,RBX::Instance,&RBX::sLighting>
	// FUNCTION: WEBSERVICE 0x100429e0
	// RBX::FactoryProduct<RBX::Lighting,RBX::Instance,&RBX::sLighting>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x10054250
	// RBX::FactoryProduct<RBX::RunService,RBX::Instance,&RBX::sRunService>::~FactoryProduct<RBX::RunService,RBX::Instance,&RBX::sRunService>
	// TEMPLATE: WEBSERVICE 0x100542a0
	// RBX::FactoryProduct<RBX::RunService,RBX::Instance,&RBX::sRunService>::FactoryProduct<RBX::RunService,RBX::Instance,&RBX::sRunService>
	// FUNCTION: WEBSERVICE 0x100564d0
	// RBX::FactoryProduct<RBX::RunService,RBX::Instance,&RBX::sRunService>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x10059c90
	// RBX::FactoryProduct<RBX::ModelInstance,RBX::PVInstance,&RBX::sModel>::~FactoryProduct<RBX::ModelInstance,RBX::PVInstance,&RBX::sModel>
	// TEMPLATE: WEBSERVICE 0x10059f00
	// RBX::FactoryProduct<RBX::ModelInstance,RBX::PVInstance,&RBX::sModel>::FactoryProduct<RBX::ModelInstance,RBX::PVInstance,&RBX::sModel><char const *>
	// TEMPLATE: WEBSERVICE 0x1005b540
	// RBX::FactoryProduct<RBX::ModelInstance,RBX::PVInstance,&RBX::sModel>::getClassName
	// TEMPLATE: WEBSERVICE 0x1005bb30
	// RBX::FactoryProduct<RBX::Humanoid,RBX::Instance,&RBX::sHumanoid>::Creator::create
	// TEMPLATE: WEBSERVICE 0x1005bfe0
	// RBX::DescribedCreatable<RBX::ModelInstance,RBX::PVInstance,&RBX::sModel>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x1005ce00
	// RBX::FactoryProduct<RBX::Selection,RBX::Instance,&RBX::sSelection>::~FactoryProduct<RBX::Selection,RBX::Instance,&RBX::sSelection>
	// TEMPLATE: WEBSERVICE 0x1005ce50
	// RBX::FactoryProduct<RBX::Selection,RBX::Instance,&RBX::sSelection>::FactoryProduct<RBX::Selection,RBX::Instance,&RBX::sSelection>
	// FUNCTION: WEBSERVICE 0x1005cec0
	// RBX::FactoryProduct<RBX::Selection,RBX::Instance,&RBX::sSelection>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x1005ec80
	// RBX::FactoryProduct<RBX::ScriptContext,RBX::Instance,&RBX::sScriptContext>::~FactoryProduct<RBX::ScriptContext,RBX::Instance,&RBX::sScriptContext>
	// TEMPLATE: WEBSERVICE 0x1005eeb0
	// RBX::FactoryProduct<RBX::ScriptContext,RBX::Instance,&RBX::sScriptContext>::FactoryProduct<RBX::ScriptContext,RBX::Instance,&RBX::sScriptContext>
	// FUNCTION: WEBSERVICE 0x10064700
	// RBX::FactoryProduct<RBX::DebugSettings,RBX::GlobalSettings::Item,&RBX::sDebugSettings>::Creator::create
	// FUNCTION: WEBSERVICE 0x10065740
	// RBX::FactoryProduct<RBX::ScriptContext,RBX::Instance,&RBX::sScriptContext>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x10068970
	// RBX::FactoryProduct<RBX::Script,RBX::Instance,&RBX::sScript>::~FactoryProduct<RBX::Script,RBX::Instance,&RBX::sScript>
	// FUNCTION: WEBSERVICE 0x10068aa0
	// RBX::FactoryProduct<RBX::Script,RBX::Instance,&RBX::sScript>::FactoryProduct<RBX::Script,RBX::Instance,&RBX::sScript><char const *>
	// FUNCTION: WEBSERVICE 0x10068e10
	// RBX::FactoryProduct<RBX::LocalScript,RBX::Script,&RBX::sLocalScript>::~FactoryProduct<RBX::LocalScript,RBX::Script,&RBX::sLocalScript>
	// FUNCTION: WEBSERVICE 0x10068e80
	// RBX::FactoryProduct<RBX::LocalScript,RBX::Script,&RBX::sLocalScript>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x10069270
	// RBX::FactoryProduct<RBX::Team,RBX::Instance,&RBX::sTeam>::Creator::create
	// TEMPLATE: WEBSERVICE 0x1006dca0
	// RBX::FactoryProduct<RBX::StarterPackService,RBX::Hopper,&RBX::sStarterPackService>::Creator::create
	// TEMPLATE: WEBSERVICE 0x100777b0
	// RBX::FactoryProduct<RBX::Backpack,RBX::Hopper,&RBX::sBackpack>::Creator::create
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
	// FUNCTION: WEBSERVICE 0x10077540
	// RBX::FactoryProduct<RBX::Motor,RBX::AutoJoint,&RBX::sMotor>::Creator::create
	// TEMPLATE: WEBSERVICE 0x10077a20
	// RBX::FactoryProduct<RBX::Teams,RBX::Instance,&RBX::sTeams>::Creator::create
	// TEMPLATE: WEBSERVICE 0x10077c90
	// RBX::FactoryProduct<RBX::Accoutrement,RBX::Instance,&RBX::sAccoutrement>::Creator::create
	// TEMPLATE: WEBSERVICE 0x10077f00
	// RBX::FactoryProduct<RBX::Hat,RBX::Accoutrement,&RBX::sHat>::Creator::create
	// TEMPLATE: WEBSERVICE 0x1007ce50
	// RBX::FactoryProduct<RBX::Soundscape::SoundChannel,RBX::Instance,&RBX::Soundscape::sSoundChannel>::~FactoryProduct<RBX::Soundscape::SoundChannel,RBX::Instance,&RBX::Soundscape::sSoundChannel>
	// TEMPLATE: WEBSERVICE 0x1007d040
	// RBX::FactoryProduct<RBX::Soundscape::SoundChannel,RBX::Instance,&RBX::Soundscape::sSoundChannel>::FactoryProduct<RBX::Soundscape::SoundChannel,RBX::Instance,&RBX::Soundscape::sSoundChannel>
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
	// FUNCTION: WEBSERVICE 0x1008aac0
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
	// FUNCTION: WEBSERVICE 0x1008c800
	// RBX::FactoryProduct<RBX::Hint,RBX::Message,&RBX::sHint>::FactoryProduct<RBX::Hint,RBX::Message,&RBX::sHint>
	// FUNCTION: WEBSERVICE 0x1008c940
	// RBX::FactoryProduct<RBX::Hint,RBX::Message,&RBX::sHint>::~FactoryProduct<RBX::Hint,RBX::Message,&RBX::sHint>
	// TEMPLATE: WEBSERVICE 0x1008c9e0
	// RBX::FactoryProduct<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue>::~FactoryProduct<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue>
	// TEMPLATE: WEBSERVICE 0x1008ca30
	// RBX::FactoryProduct<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue>::FactoryProduct<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue>
	// FUNCTION: WEBSERVICE 0x1008caa0
	// RBX::FactoryProduct<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue>::`scalar deleting destructor'
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
	// FUNCTION: WEBSERVICE 0x1008e4b0
	// RBX::FactoryProduct<RBX::Hint,RBX::Message,&RBX::sHint>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x1008e8a0
	// RBX::FactoryProduct<RBX::Hint,RBX::Message,&RBX::sHint>::Creator::create
	// TEMPLATE: WEBSERVICE 0x1008e950
	// RBX::FactoryProduct<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue>::Creator::create
	// TEMPLATE: WEBSERVICE 0x100945c0
	// RBX::FactoryProduct<RBX::Sky,RBX::Instance,&RBX::sSky>::~FactoryProduct<RBX::Sky,RBX::Instance,&RBX::sSky>
	// TEMPLATE: WEBSERVICE 0x1009a8f0
	// RBX::FactoryProduct<RBX::PartInstance,RBX::PVInstance,&RBX::sPart>::~FactoryProduct<RBX::PartInstance,RBX::PVInstance,&RBX::sPart>
	// TEMPLATE: WEBSERVICE 0x1009ab10
	// RBX::FactoryProduct<RBX::PartInstance,RBX::PVInstance,&RBX::sPart>::FactoryProduct<RBX::PartInstance,RBX::PVInstance,&RBX::sPart><char const *>
	// TEMPLATE: WEBSERVICE 0x1009ccf0
	// RBX::FactoryProduct<RBX::PartInstance,RBX::PVInstance,&RBX::sPart>::getClassName
	// TEMPLATE: WEBSERVICE 0x100a09b0
	// RBX::FactoryProduct<RBX::Humanoid,RBX::Instance,&RBX::sHumanoid>::~FactoryProduct<RBX::Humanoid,RBX::Instance,&RBX::sHumanoid>
	// TEMPLATE: WEBSERVICE 0x10094610
	// RBX::FactoryProduct<RBX::Sky,RBX::Instance,&RBX::sSky>::FactoryProduct<RBX::Sky,RBX::Instance,&RBX::sSky>
	// TEMPLATE: WEBSERVICE 0x100a0a70
	// RBX::FactoryProduct<RBX::Humanoid,RBX::Instance,&RBX::sHumanoid>::FactoryProduct<RBX::Humanoid,RBX::Instance,&RBX::sHumanoid>
	// FUNCTION: WEBSERVICE 0x100a0b40
	// RBX::FactoryProduct<RBX::Humanoid,RBX::Instance,&RBX::sHumanoid>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x10094d60
	// RBX::FactoryProduct<RBX::Sky,RBX::Instance,&RBX::sSky>::getClassName
	// TEMPLATE: WEBSERVICE 0x100ad0d0
	// RBX::FactoryProduct<RBX::Texture,RBX::Decal,&RBX::sTexture>::Creator::create
	// FUNCTION: WEBSERVICE 0x100b3810
	// RBX::FactoryProduct<RBX::DebugSettings,RBX::GlobalSettings::Item,&RBX::sDebugSettings>::~FactoryProduct<RBX::DebugSettings,RBX::GlobalSettings::Item,&RBX::sDebugSettings>
	// FUNCTION: WEBSERVICE 0x100b38c0
	// RBX::FactoryProduct<RBX::DebugSettings,RBX::GlobalSettings::Item,&RBX::sDebugSettings>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100b38e0
	// RBX::FactoryProduct<RBX::DebugSettings,RBX::GlobalSettings::Item,&RBX::sDebugSettings>::FactoryProduct<RBX::DebugSettings,RBX::GlobalSettings::Item,&RBX::sDebugSettings>
	// FUNCTION: WEBSERVICE 0x100b3bc0
	// RBX::FactoryProduct<RBX::DebugSettings,RBX::GlobalSettings::Item,&RBX::sDebugSettings>::getClassName
	// TEMPLATE: WEBSERVICE 0x100bdef0
	// RBX::FactoryProduct<RBX::Team,RBX::Instance,&RBX::sTeam>::~FactoryProduct<RBX::Team,RBX::Instance,&RBX::sTeam>
	// TEMPLATE: WEBSERVICE 0x100bdf40
	// RBX::FactoryProduct<RBX::Team,RBX::Instance,&RBX::sTeam>::FactoryProduct<RBX::Team,RBX::Instance,&RBX::sTeam>
	// FUNCTION: WEBSERVICE 0x100bdfa0
	// RBX::FactoryProduct<RBX::Team,RBX::Instance,&RBX::sTeam>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x100694e0
	// RBX::FactoryProduct<RBX::Network::Player,RBX::Instance,&RBX::Network::sPlayer>::Creator::create
	// TEMPLATE: WEBSERVICE 0x100c7250
	// RBX::FactoryProduct<RBX::StarterPackService,RBX::Hopper,&RBX::sStarterPackService>::~FactoryProduct<RBX::StarterPackService,RBX::Hopper,&RBX::sStarterPackService>
	// TEMPLATE: WEBSERVICE 0x100c7910
	// RBX::FactoryProduct<RBX::StarterPackService,RBX::Hopper,&RBX::sStarterPackService>::FactoryProduct<RBX::StarterPackService,RBX::Hopper,&RBX::sStarterPackService>
	// TEMPLATE: WEBSERVICE 0x100f0a90
	// RBX::FactoryProduct<RBX::Value<int,&RBX::sIntValue>,RBX::Instance,&RBX::sIntValue>::~FactoryProduct<RBX::Value<int,&RBX::sIntValue>,RBX::Instance,&RBX::sIntValue>
	// TEMPLATE: WEBSERVICE 0x100f0ae0
	// RBX::FactoryProduct<RBX::Value<int,&RBX::sIntValue>,RBX::Instance,&RBX::sIntValue>::FactoryProduct<RBX::Value<int,&RBX::sIntValue>,RBX::Instance,&RBX::sIntValue>
	// TEMPLATE: WEBSERVICE 0x100f0d00
	// RBX::FactoryProduct<RBX::Value<bool,&RBX::sBoolValue>,RBX::Instance,&RBX::sBoolValue>::~FactoryProduct<RBX::Value<bool,&RBX::sBoolValue>,RBX::Instance,&RBX::sBoolValue>
	// TEMPLATE: WEBSERVICE 0x100f0d50
	// RBX::FactoryProduct<RBX::Value<bool,&RBX::sBoolValue>,RBX::Instance,&RBX::sBoolValue>::FactoryProduct<RBX::Value<bool,&RBX::sBoolValue>,RBX::Instance,&RBX::sBoolValue>
	// TEMPLATE: WEBSERVICE 0x100f0f50
	// RBX::FactoryProduct<RBX::Value<double,&RBX::sDoubleValue>,RBX::Instance,&RBX::sDoubleValue>::~FactoryProduct<RBX::Value<double,&RBX::sDoubleValue>,RBX::Instance,&RBX::sDoubleValue>
	// TEMPLATE: WEBSERVICE 0x100f0fa0
	// RBX::FactoryProduct<RBX::Value<double,&RBX::sDoubleValue>,RBX::Instance,&RBX::sDoubleValue>::FactoryProduct<RBX::Value<double,&RBX::sDoubleValue>,RBX::Instance,&RBX::sDoubleValue>
	// TEMPLATE: WEBSERVICE 0x100f1400
	// RBX::FactoryProduct<RBX::Value<G3D::Vector3,&RBX::sVector3Value>,RBX::Instance,&RBX::sVector3Value>::~FactoryProduct<RBX::Value<G3D::Vector3,&RBX::sVector3Value>,RBX::Instance,&RBX::sVector3Value>
	// TEMPLATE: WEBSERVICE 0x100f1450
	// RBX::FactoryProduct<RBX::Value<G3D::Vector3,&RBX::sVector3Value>,RBX::Instance,&RBX::sVector3Value>::FactoryProduct<RBX::Value<G3D::Vector3,&RBX::sVector3Value>,RBX::Instance,&RBX::sVector3Value>
	// TEMPLATE: WEBSERVICE 0x100f1670
	// RBX::FactoryProduct<RBX::Value<G3D::CoordinateFrame,&RBX::sCFrameValue>,RBX::Instance,&RBX::sCFrameValue>::~FactoryProduct<RBX::Value<G3D::CoordinateFrame,&RBX::sCFrameValue>,RBX::Instance,&RBX::sCFrameValue>
	// TEMPLATE: WEBSERVICE 0x100f16c0
	// RBX::FactoryProduct<RBX::Value<G3D::CoordinateFrame,&RBX::sCFrameValue>,RBX::Instance,&RBX::sCFrameValue>::FactoryProduct<RBX::Value<G3D::CoordinateFrame,&RBX::sCFrameValue>,RBX::Instance,&RBX::sCFrameValue>
	// TEMPLATE: WEBSERVICE 0x100f18c0
	// RBX::FactoryProduct<RBX::Value<G3D::Color3,&RBX::sColor3Value>,RBX::Instance,&RBX::sColor3Value>::~FactoryProduct<RBX::Value<G3D::Color3,&RBX::sColor3Value>,RBX::Instance,&RBX::sColor3Value>
	// TEMPLATE: WEBSERVICE 0x100f1910
	// RBX::FactoryProduct<RBX::Value<G3D::Color3,&RBX::sColor3Value>,RBX::Instance,&RBX::sColor3Value>::FactoryProduct<RBX::Value<G3D::Color3,&RBX::sColor3Value>,RBX::Instance,&RBX::sColor3Value>
	// TEMPLATE: WEBSERVICE 0x100f1b10
	// RBX::FactoryProduct<RBX::Value<RBX::BrickColor,&RBX::sBrickColorValue>,RBX::Instance,&RBX::sBrickColorValue>::~FactoryProduct<RBX::Value<RBX::BrickColor,&RBX::sBrickColorValue>,RBX::Instance,&RBX::sBrickColorValue>
	// TEMPLATE: WEBSERVICE 0x100f1b60
	// RBX::FactoryProduct<RBX::Value<RBX::BrickColor,&RBX::sBrickColorValue>,RBX::Instance,&RBX::sBrickColorValue>::FactoryProduct<RBX::Value<RBX::BrickColor,&RBX::sBrickColorValue>,RBX::Instance,&RBX::sBrickColorValue>
	// TEMPLATE: WEBSERVICE 0x100f6520
	// RBX::FactoryProduct<RBX::Value<int,&RBX::sIntValue>,RBX::Instance,&RBX::sIntValue>::Creator::create
	// TEMPLATE: WEBSERVICE 0x100f65d0
	// RBX::FactoryProduct<RBX::Value<bool,&RBX::sBoolValue>,RBX::Instance,&RBX::sBoolValue>::Creator::create
	// TEMPLATE: WEBSERVICE 0x100f6680
	// RBX::FactoryProduct<RBX::Value<double,&RBX::sDoubleValue>,RBX::Instance,&RBX::sDoubleValue>::Creator::create
	// TEMPLATE: WEBSERVICE 0x100f6730
	// RBX::FactoryProduct<RBX::Value<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,&RBX::sStringValue>,RBX::Instance,&RBX::sStringValue>::Creator::create
	// TEMPLATE: WEBSERVICE 0x100f67e0
	// RBX::FactoryProduct<RBX::Value<G3D::Vector3,&RBX::sVector3Value>,RBX::Instance,&RBX::sVector3Value>::Creator::create
	// TEMPLATE: WEBSERVICE 0x100f6890
	// RBX::FactoryProduct<RBX::Value<G3D::CoordinateFrame,&RBX::sCFrameValue>,RBX::Instance,&RBX::sCFrameValue>::Creator::create
	// TEMPLATE: WEBSERVICE 0x100f6940
	// RBX::FactoryProduct<RBX::Value<G3D::Color3,&RBX::sColor3Value>,RBX::Instance,&RBX::sColor3Value>::Creator::create
	// TEMPLATE: WEBSERVICE 0x100f69f0
	// RBX::FactoryProduct<RBX::Value<RBX::BrickColor,&RBX::sBrickColorValue>,RBX::Instance,&RBX::sBrickColorValue>::Creator::create
	// TEMPLATE: WEBSERVICE 0x1018c030
	// RBX::FactoryProduct<RBX::NetworkSettings,RBX::GlobalSettings::Item,&RBX::sNetworkSettings>::Creator::create
	// TEMPLATE: WEBSERVICE 0x100da470
	// RBX::FactoryProduct<RBX::Backpack,RBX::Hopper,&RBX::sBackpack>::FactoryProduct<RBX::Backpack,RBX::Hopper,&RBX::sBackpack>
	// TEMPLATE: WEBSERVICE 0x100da500
	// RBX::FactoryProduct<RBX::Backpack,RBX::Hopper,&RBX::sBackpack>::~FactoryProduct<RBX::Backpack,RBX::Hopper,&RBX::sBackpack>
	// TEMPLATE: WEBSERVICE 0x100c2710
	// RBX::FactoryProduct<RBX::Camera,RBX::Instance,&RBX::sCamera>::~FactoryProduct<RBX::Camera,RBX::Instance,&RBX::sCamera>
	// TEMPLATE: WEBSERVICE 0x100c2770
	// RBX::FactoryProduct<RBX::Camera,RBX::Instance,&RBX::sCamera>::FactoryProduct<RBX::Camera,RBX::Instance,&RBX::sCamera>
	// FUNCTION: WEBSERVICE 0x100c4100
	// RBX::FactoryProduct<RBX::Camera,RBX::Instance,&RBX::sCamera>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x100c59a0
	// RBX::FactoryProduct<RBX::ControllerService,RBX::Instance,&RBX::sControllerService>::~FactoryProduct<RBX::ControllerService,RBX::Instance,&RBX::sControllerService>
	// TEMPLATE: WEBSERVICE 0x100c59f0
	// RBX::FactoryProduct<RBX::ControllerService,RBX::Instance,&RBX::sControllerService>::FactoryProduct<RBX::ControllerService,RBX::Instance,&RBX::sControllerService>
	// FUNCTION: WEBSERVICE 0x100c5ab0
	// RBX::FactoryProduct<RBX::ControllerService,RBX::Instance,&RBX::sControllerService>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x100c5e20
	// RBX::FactoryProduct<RBX::ControllerService,RBX::Instance,&RBX::sControllerService>::getClassName
	// FUNCTION: WEBSERVICE 0x100d8300
	// RBX::FactoryProduct<RBX::Snap,RBX::AutoJoint,&RBX::sSnap>::~FactoryProduct<RBX::Snap,RBX::AutoJoint,&RBX::sSnap>
	// FUNCTION: WEBSERVICE 0x100d8390
	// RBX::FactoryProduct<RBX::Weld,RBX::AutoJoint,&RBX::sWeld>::~FactoryProduct<RBX::Weld,RBX::AutoJoint,&RBX::sWeld>
	// FUNCTION: WEBSERVICE 0x100d8430
	// RBX::FactoryProduct<RBX::Glue,RBX::AutoJoint,&RBX::sGlue>::~FactoryProduct<RBX::Glue,RBX::AutoJoint,&RBX::sGlue>
	// FUNCTION: WEBSERVICE 0x100d84e0
	// RBX::FactoryProduct<RBX::Rotate,RBX::AutoJoint,&RBX::sRotate>::~FactoryProduct<RBX::Rotate,RBX::AutoJoint,&RBX::sRotate>
	// FUNCTION: WEBSERVICE 0x100d8570
	// RBX::FactoryProduct<RBX::RotateP,RBX::AutoJoint,&RBX::sRotateP>::~FactoryProduct<RBX::RotateP,RBX::AutoJoint,&RBX::sRotateP>
	// FUNCTION: WEBSERVICE 0x100d8630
	// RBX::FactoryProduct<RBX::RotateV,RBX::AutoJoint,&RBX::sRotateV>::~FactoryProduct<RBX::RotateV,RBX::AutoJoint,&RBX::sRotateV>
	// FUNCTION: WEBSERVICE 0x100d86d0
	// RBX::FactoryProduct<RBX::Motor,RBX::AutoJoint,&RBX::sMotor>::~FactoryProduct<RBX::Motor,RBX::AutoJoint,&RBX::sMotor>
	// FUNCTION: WEBSERVICE 0x100d8750
	// RBX::FactoryProduct<RBX::Motor,RBX::AutoJoint,&RBX::sMotor>::getClassName
	// FUNCTION: WEBSERVICE 0x100d8770
	// RBX::FactoryProduct<RBX::Snap,RBX::AutoJoint,&RBX::sSnap>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100d8790
	// RBX::FactoryProduct<RBX::Weld,RBX::AutoJoint,&RBX::sWeld>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100d87b0
	// RBX::FactoryProduct<RBX::Glue,RBX::AutoJoint,&RBX::sGlue>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100d9100
	// RBX::FactoryProduct<RBX::Snap,RBX::AutoJoint,&RBX::sSnap>::FactoryProduct<RBX::Snap,RBX::AutoJoint,&RBX::sSnap><RBX::SnapJoint *>
	// FUNCTION: WEBSERVICE 0x100d9170
	// RBX::FactoryProduct<RBX::Weld,RBX::AutoJoint,&RBX::sWeld>::FactoryProduct<RBX::Weld,RBX::AutoJoint,&RBX::sWeld><RBX::WeldJoint *>
	// FUNCTION: WEBSERVICE 0x100d91e0
	// RBX::FactoryProduct<RBX::Glue,RBX::AutoJoint,&RBX::sGlue>::FactoryProduct<RBX::Glue,RBX::AutoJoint,&RBX::sGlue><RBX::GlueJoint *>
	// FUNCTION: WEBSERVICE 0x100d9250
	// RBX::FactoryProduct<RBX::Rotate,RBX::AutoJoint,&RBX::sRotate>::FactoryProduct<RBX::Rotate,RBX::AutoJoint,&RBX::sRotate><RBX::RotateJoint *>
	// FUNCTION: WEBSERVICE 0x100d92c0
	// RBX::FactoryProduct<RBX::RotateP,RBX::AutoJoint,&RBX::sRotateP>::FactoryProduct<RBX::RotateP,RBX::AutoJoint,&RBX::sRotateP><RBX::RotatePJoint *>
	// FUNCTION: WEBSERVICE 0x100d9330
	// RBX::FactoryProduct<RBX::RotateV,RBX::AutoJoint,&RBX::sRotateV>::FactoryProduct<RBX::RotateV,RBX::AutoJoint,&RBX::sRotateV><RBX::RotateVJoint *>
	// FUNCTION: WEBSERVICE 0x100d93a0
	// RBX::FactoryProduct<RBX::Motor,RBX::AutoJoint,&RBX::sMotor>::FactoryProduct<RBX::Motor,RBX::AutoJoint,&RBX::sMotor><RBX::MotorJoint *>
	// FUNCTION: WEBSERVICE 0x100d9950
	// RBX::FactoryProduct<RBX::Motor,RBX::AutoJoint,&RBX::sMotor>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100d9f70
	// RBX::FactoryProduct<RBX::Rotate,RBX::AutoJoint,&RBX::sRotate>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100da0e0
	// RBX::FactoryProduct<RBX::RotateP,RBX::AutoJoint,&RBX::sRotateP>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100da260
	// RBX::FactoryProduct<RBX::RotateV,RBX::AutoJoint,&RBX::sRotateV>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100da830
	// RBX::FactoryProduct<RBX::Teams,RBX::Instance,&RBX::sTeams>::~FactoryProduct<RBX::Teams,RBX::Instance,&RBX::sTeams>
	// FUNCTION: WEBSERVICE 0x100da880
	// RBX::FactoryProduct<RBX::Teams,RBX::Instance,&RBX::sTeams>::FactoryProduct<RBX::Teams,RBX::Instance,&RBX::sTeams>
	// FUNCTION: WEBSERVICE 0x100da8e0
	// RBX::FactoryProduct<RBX::Teams,RBX::Instance,&RBX::sTeams>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x100dabc0
	// RBX::FactoryProduct<RBX::Teams,RBX::Instance,&RBX::sTeams>::className
	// FUNCTION: WEBSERVICE 0x100db590
	// RBX::FactoryProduct<RBX::Accoutrement,RBX::Instance,&RBX::sAccoutrement>::~FactoryProduct<RBX::Accoutrement,RBX::Instance,&RBX::sAccoutrement>
	// FUNCTION: WEBSERVICE 0x100db5e0
	// RBX::FactoryProduct<RBX::Accoutrement,RBX::Instance,&RBX::sAccoutrement>::FactoryProduct<RBX::Accoutrement,RBX::Instance,&RBX::sAccoutrement>
	// STUB: WEBSERVICE 0x100dbdf0
	// RBX::FactoryProduct<RBX::Hat,RBX::Accoutrement,&RBX::sHat>::~FactoryProduct<RBX::Hat,RBX::Accoutrement,&RBX::sHat>
	// FUNCTION: WEBSERVICE 0x100dcce0
	// RBX::FactoryProduct<RBX::Accoutrement,RBX::Instance,&RBX::sAccoutrement>::`scalar deleting destructor'
	// STUB: WEBSERVICE 0x100dcec0
	// RBX::FactoryProduct<RBX::Hat,RBX::Accoutrement,&RBX::sHat>::FactoryProduct<RBX::Hat,RBX::Accoutrement,&RBX::sHat>
	// FUNCTION: WEBSERVICE 0x100dd950
	// RBX::FactoryProduct<RBX::Hat,RBX::Accoutrement,&RBX::sHat>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100de130
	// RBX::FactoryProduct<RBX::GameSettings,RBX::GlobalSettings::Item,&RBX::sGameSettings>::~FactoryProduct<RBX::GameSettings,RBX::GlobalSettings::Item,&RBX::sGameSettings>
	// FUNCTION: WEBSERVICE 0x100de1b0
	// RBX::FactoryProduct<RBX::GameSettings,RBX::GlobalSettings::Item,&RBX::sGameSettings>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x100de1d0
	// RBX::FactoryProduct<RBX::GameSettings,RBX::GlobalSettings::Item,&RBX::sGameSettings>::FactoryProduct<RBX::GameSettings,RBX::GlobalSettings::Item,&RBX::sGameSettings>
	// FUNCTION: WEBSERVICE 0x100e04a0
	// RBX::FactoryProduct<RBX::ShirtGraphic,RBX::CharacterAppearance,&RBX::sShirtGraphic>::~FactoryProduct<RBX::ShirtGraphic,RBX::CharacterAppearance,&RBX::sShirtGraphic>
	// FUNCTION: WEBSERVICE 0x100e0530
	// RBX::FactoryProduct<RBX::BodyColors,RBX::CharacterAppearance,&RBX::sBodyColors>::~FactoryProduct<RBX::BodyColors,RBX::CharacterAppearance,&RBX::sBodyColors>
	// FUNCTION: WEBSERVICE 0x100e05a0
	// RBX::FactoryProduct<RBX::Skin,RBX::CharacterAppearance,&RBX::sSkin>::~FactoryProduct<RBX::Skin,RBX::CharacterAppearance,&RBX::sSkin>
	// TEMPLATE: WEBSERVICE 0x100e0640
	// RBX::FactoryProduct<RBX::ShirtGraphic,RBX::CharacterAppearance,&RBX::sShirtGraphic>::FactoryProduct<RBX::ShirtGraphic,RBX::CharacterAppearance,&RBX::sShirtGraphic>
	// TEMPLATE: WEBSERVICE 0x100e06a0
	// RBX::FactoryProduct<RBX::BodyColors,RBX::CharacterAppearance,&RBX::sBodyColors>::FactoryProduct<RBX::BodyColors,RBX::CharacterAppearance,&RBX::sBodyColors>
	// TEMPLATE: WEBSERVICE 0x100e0700
	// RBX::FactoryProduct<RBX::Skin,RBX::CharacterAppearance,&RBX::sSkin>::FactoryProduct<RBX::Skin,RBX::CharacterAppearance,&RBX::sSkin>
	// FUNCTION: WEBSERVICE 0x100e0c40
	// RBX::FactoryProduct<RBX::Shirt,RBX::ShirtGraphic,&RBX::sShirt>::~FactoryProduct<RBX::Shirt,RBX::ShirtGraphic,&RBX::sShirt>
	// FUNCTION: WEBSERVICE 0x100e0cb0
	// RBX::FactoryProduct<RBX::Shirt,RBX::ShirtGraphic,&RBX::sShirt>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100e1290
	// RBX::FactoryProduct<RBX::ShirtGraphic,RBX::CharacterAppearance,&RBX::sShirtGraphic>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100e1380
	// RBX::FactoryProduct<RBX::BodyColors,RBX::CharacterAppearance,&RBX::sBodyColors>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100e16b0
	// RBX::FactoryProduct<RBX::Skin,RBX::CharacterAppearance,&RBX::sSkin>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100e17e0
	// RBX::FactoryProduct<RBX::Shirt,RBX::ShirtGraphic,&RBX::sShirt>::FactoryProduct<RBX::Shirt,RBX::ShirtGraphic,&RBX::sShirt>
	// FUNCTION: WEBSERVICE 0x100e1b80
	// RBX::FactoryProduct<RBX::Explosion,RBX::Instance,&RBX::sExplosion>::~FactoryProduct<RBX::Explosion,RBX::Instance,&RBX::sExplosion>
	// FUNCTION: WEBSERVICE 0x100e1bd0
	// RBX::FactoryProduct<RBX::Explosion,RBX::Instance,&RBX::sExplosion>::FactoryProduct<RBX::Explosion,RBX::Instance,&RBX::sExplosion>
	// FUNCTION: WEBSERVICE 0x100e2ce0
	// RBX::FactoryProduct<RBX::Explosion,RBX::Instance,&RBX::sExplosion>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100e3c40
	// RBX::FactoryProduct<RBX::VelocityMotor,RBX::JointInstance,&RBX::sVelocityMotor>::~FactoryProduct<RBX::VelocityMotor,RBX::JointInstance,&RBX::sVelocityMotor>
	// FUNCTION: WEBSERVICE 0x100e3d70
	// RBX::FactoryProduct<RBX::VelocityMotor,RBX::JointInstance,&RBX::sVelocityMotor>::FactoryProduct<RBX::VelocityMotor,RBX::JointInstance,&RBX::sVelocityMotor><RBX::MotorJoint *>
	// FUNCTION: WEBSERVICE 0x100e4860
	// RBX::FactoryProduct<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature>::~FactoryProduct<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature>
	// FUNCTION: WEBSERVICE 0x100e4900
	// RBX::FactoryProduct<RBX::Hole,RBX::Feature,&RBX::sHole>::~FactoryProduct<RBX::Hole,RBX::Feature,&RBX::sHole>
	// FUNCTION: WEBSERVICE 0x100e4f20
	// RBX::FactoryProduct<RBX::VelocityMotor,RBX::JointInstance,&RBX::sVelocityMotor>::getClassName
	// FUNCTION: WEBSERVICE 0x100e6d00
	// RBX::FactoryProduct<RBX::VelocityMotor,RBX::JointInstance,&RBX::sVelocityMotor>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100e7350
	// RBX::FactoryProduct<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature>::FactoryProduct<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature>
	// FUNCTION: WEBSERVICE 0x100e73c0
	// RBX::FactoryProduct<RBX::Hole,RBX::Feature,&RBX::sHole>::FactoryProduct<RBX::Hole,RBX::Feature,&RBX::sHole>
	// FUNCTION: WEBSERVICE 0x100e7890
	// RBX::FactoryProduct<RBX::Hole,RBX::Feature,&RBX::sHole>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100e7ab0
	// RBX::FactoryProduct<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100e7be0
	// RBX::FactoryProduct<RBX::FlagStandService,RBX::Instance,&RBX::sFlagStandService>::~FactoryProduct<RBX::FlagStandService,RBX::Instance,&RBX::sFlagStandService>
	// FUNCTION: WEBSERVICE 0x100e7c30
	// RBX::FactoryProduct<RBX::FlagStandService,RBX::Instance,&RBX::sFlagStandService>::FactoryProduct<RBX::FlagStandService,RBX::Instance,&RBX::sFlagStandService>
	// FUNCTION: WEBSERVICE 0x100e7d00
	// RBX::FactoryProduct<RBX::FlagStandService,RBX::Instance,&RBX::sFlagStandService>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100e92a0
	// RBX::FactoryProduct<RBX::ForceField,RBX::Instance,&RBX::sForceField>::~FactoryProduct<RBX::ForceField,RBX::Instance,&RBX::sForceField>
	// FUNCTION: WEBSERVICE 0x100e92f0
	// RBX::FactoryProduct<RBX::ForceField,RBX::Instance,&RBX::sForceField>::FactoryProduct<RBX::ForceField,RBX::Instance,&RBX::sForceField>
	// FUNCTION: WEBSERVICE 0x100e97d0
	// RBX::FactoryProduct<RBX::ForceField,RBX::Instance,&RBX::sForceField>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100e9a10
	// RBX::FactoryProduct<RBX::GeometryService,RBX::Instance,&RBX::sGeometryService>::~FactoryProduct<RBX::GeometryService,RBX::Instance,&RBX::sGeometryService>
	// FUNCTION: WEBSERVICE 0x100e9a60
	// RBX::FactoryProduct<RBX::GeometryService,RBX::Instance,&RBX::sGeometryService>::FactoryProduct<RBX::GeometryService,RBX::Instance,&RBX::sGeometryService><char *>
	// FUNCTION: WEBSERVICE 0x100e9ac0
	// RBX::FactoryProduct<RBX::GeometryService,RBX::Instance,&RBX::sGeometryService>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x100e9b70
	// RBX::FactoryProduct<RBX::GeometryService,RBX::Instance,&RBX::sGeometryService>::getClassName
	// FUNCTION: WEBSERVICE 0x100eb370
	// RBX::FactoryProduct<RBX::BodyGyro,RBX::BodyMover,&RBX::sBodyGyro>::~FactoryProduct<RBX::BodyGyro,RBX::BodyMover,&RBX::sBodyGyro>
	// FUNCTION: WEBSERVICE 0x100eb400
	// RBX::FactoryProduct<RBX::BodyForce,RBX::BodyMover,&RBX::sBodyForce>::~FactoryProduct<RBX::BodyForce,RBX::BodyMover,&RBX::sBodyForce>
	// FUNCTION: WEBSERVICE 0x100eb470
	// RBX::FactoryProduct<RBX::BodyThrust,RBX::BodyMover,&RBX::sBodyThrust>::~FactoryProduct<RBX::BodyThrust,RBX::BodyMover,&RBX::sBodyThrust>
	// FUNCTION: WEBSERVICE 0x100eb520
	// RBX::FactoryProduct<RBX::BodyPosition,RBX::BodyMover,&RBX::sBodyPosition>::~FactoryProduct<RBX::BodyPosition,RBX::BodyMover,&RBX::sBodyPosition>
	// FUNCTION: WEBSERVICE 0x100eb5d0
	// RBX::FactoryProduct<RBX::BodyVelocity,RBX::BodyMover,&RBX::sBodyVelocity>::~FactoryProduct<RBX::BodyVelocity,RBX::BodyMover,&RBX::sBodyVelocity>
	// FUNCTION: WEBSERVICE 0x100eb670
	// RBX::FactoryProduct<RBX::Rocket,RBX::BodyMover,&RBX::sRocket>::~FactoryProduct<RBX::Rocket,RBX::BodyMover,&RBX::sRocket>
	// FUNCTION: WEBSERVICE 0x100eb730
	// RBX::FactoryProduct<RBX::BodyGyro,RBX::BodyMover,&RBX::sBodyGyro>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100eb750
	// RBX::FactoryProduct<RBX::BodyThrust,RBX::BodyMover,&RBX::sBodyThrust>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100eb770
	// RBX::FactoryProduct<RBX::BodyPosition,RBX::BodyMover,&RBX::sBodyPosition>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100eb7f0
	// RBX::FactoryProduct<RBX::Rocket,RBX::BodyMover,&RBX::sRocket>::FactoryProduct<RBX::Rocket,RBX::BodyMover,&RBX::sRocket><char *>
	// FUNCTION: WEBSERVICE 0x100eb870
	// RBX::FactoryProduct<RBX::BodyGyro,RBX::BodyMover,&RBX::sBodyGyro>::FactoryProduct<RBX::BodyGyro,RBX::BodyMover,&RBX::sBodyGyro><char *>
	// FUNCTION: WEBSERVICE 0x100eb8f0
	// RBX::FactoryProduct<RBX::BodyPosition,RBX::BodyMover,&RBX::sBodyPosition>::FactoryProduct<RBX::BodyPosition,RBX::BodyMover,&RBX::sBodyPosition><char *>
	// FUNCTION: WEBSERVICE 0x100eb970
	// RBX::FactoryProduct<RBX::BodyVelocity,RBX::BodyMover,&RBX::sBodyVelocity>::FactoryProduct<RBX::BodyVelocity,RBX::BodyMover,&RBX::sBodyVelocity><char *>
	// FUNCTION: WEBSERVICE 0x100eb9f0
	// RBX::FactoryProduct<RBX::BodyForce,RBX::BodyMover,&RBX::sBodyForce>::FactoryProduct<RBX::BodyForce,RBX::BodyMover,&RBX::sBodyForce><char *>
	// FUNCTION: WEBSERVICE 0x100eba70
	// RBX::FactoryProduct<RBX::BodyThrust,RBX::BodyMover,&RBX::sBodyThrust>::FactoryProduct<RBX::BodyThrust,RBX::BodyMover,&RBX::sBodyThrust><char *>
	// FUNCTION: WEBSERVICE 0x100ed820
	// RBX::FactoryProduct<RBX::BodyVelocity,RBX::BodyMover,&RBX::sBodyVelocity>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100ed9c0
	// RBX::FactoryProduct<RBX::BodyForce,RBX::BodyMover,&RBX::sBodyForce>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100edeb0
	// RBX::FactoryProduct<RBX::Rocket,RBX::BodyMover,&RBX::sRocket>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100ee470
	// RBX::FactoryProduct<RBX::Message,RBX::Instance,&RBX::sMessage>::FactoryProduct<RBX::Message,RBX::Instance,&RBX::sMessage><char const *>
	// FUNCTION: WEBSERVICE 0x100f8b90
	// RBX::FactoryProduct<RBX::Visit,RBX::Instance,&RBX::sVisit>::~FactoryProduct<RBX::Visit,RBX::Instance,&RBX::sVisit>
	// FUNCTION: WEBSERVICE 0x100f8c40
	// RBX::FactoryProduct<RBX::Visit,RBX::Instance,&RBX::sVisit>::FactoryProduct<RBX::Visit,RBX::Instance,&RBX::sVisit>
	// FUNCTION: WEBSERVICE 0x100f95b0
	// RBX::FactoryProduct<RBX::Visit,RBX::Instance,&RBX::sVisit>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100f9df0
	// RBX::FactoryProduct<RBX::DebrisService,RBX::Instance,&RBX::sDebrisService>::~FactoryProduct<RBX::DebrisService,RBX::Instance,&RBX::sDebrisService>
	// FUNCTION: WEBSERVICE 0x100f9f20
	// RBX::FactoryProduct<RBX::DebrisService,RBX::Instance,&RBX::sDebrisService>::FactoryProduct<RBX::DebrisService,RBX::Instance,&RBX::sDebrisService><char *>
	// FUNCTION: WEBSERVICE 0x100fad10
	// RBX::FactoryProduct<RBX::DebrisService,RBX::Instance,&RBX::sDebrisService>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100fb580
	// RBX::FactoryProduct<RBX::TimerService,RBX::Instance,&RBX::sTimerService>::~FactoryProduct<RBX::TimerService,RBX::Instance,&RBX::sTimerService>
	// FUNCTION: WEBSERVICE 0x100fb5d0
	// RBX::FactoryProduct<RBX::TimerService,RBX::Instance,&RBX::sTimerService>::FactoryProduct<RBX::TimerService,RBX::Instance,&RBX::sTimerService>
	// FUNCTION: WEBSERVICE 0x100fb630
	// RBX::FactoryProduct<RBX::TimerService,RBX::Instance,&RBX::sTimerService>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100fc040
	// RBX::FactoryProduct<RBX::Seat,RBX::PartInstance,&RBX::sSeat>::~FactoryProduct<RBX::Seat,RBX::PartInstance,&RBX::sSeat>
	// FUNCTION: WEBSERVICE 0x100fc2a0
	// RBX::FactoryProduct<RBX::Seat,RBX::PartInstance,&RBX::sSeat>::`scalar deleting destructor'
	// FUNCTION: WEBSERVICE 0x100fc2c0
	// RBX::FactoryProduct<RBX::Seat,RBX::PartInstance,&RBX::sSeat>::FactoryProduct<RBX::Seat,RBX::PartInstance,&RBX::sSeat>
	// FUNCTION: WEBSERVICE 0x100fd610
	// RBX::FactoryProduct<RBX::SpawnerService,RBX::Instance,&RBX::sSpawnerService>::~FactoryProduct<RBX::SpawnerService,RBX::Instance,&RBX::sSpawnerService>
	// FUNCTION: WEBSERVICE 0x100fd660
	// RBX::FactoryProduct<RBX::SpawnerService,RBX::Instance,&RBX::sSpawnerService>::FactoryProduct<RBX::SpawnerService,RBX::Instance,&RBX::sSpawnerService>
	// FUNCTION: WEBSERVICE 0x100febb0
	// RBX::FactoryProduct<RBX::SpawnerService,RBX::Instance,&RBX::sSpawnerService>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x100ff720
	// RBX::FactoryProduct<RBX::Decal,RBX::FaceInstance,&RBX::sDecal>::~FactoryProduct<RBX::Decal,RBX::FaceInstance,&RBX::sDecal>
	// FUNCTION: WEBSERVICE 0x100ffaa0
	// RBX::FactoryProduct<RBX::Texture,RBX::Decal,&RBX::sTexture>::~FactoryProduct<RBX::Texture,RBX::Decal,&RBX::sTexture>
	// TEMPLATE: WEBSERVICE 0x100ff600
	// RBX::FactoryProduct<RBX::Decal,RBX::FaceInstance,&RBX::sDecal>::FactoryProduct<RBX::Decal,RBX::FaceInstance,&RBX::sDecal>
	// FUNCTION: WEBSERVICE 0x10100110
	// RBX::FactoryProduct<RBX::Texture,RBX::Decal,&RBX::sTexture>::FactoryProduct<RBX::Texture,RBX::Decal,&RBX::sTexture>
	// FUNCTION: WEBSERVICE 0x101004b0
	// RBX::FactoryProduct<RBX::Texture,RBX::Decal,&RBX::sTexture>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x10129f20
	// RBX::FactoryProduct<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape>::Creator::create
	// FUNCTION: WEBSERVICE 0x1012c970
	// RBX::FactoryProduct<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape>::~FactoryProduct<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape>
	// FUNCTION: WEBSERVICE 0x1012c9c0
	// RBX::FactoryProduct<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape>::FactoryProduct<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape>
	// FUNCTION: WEBSERVICE 0x1012ca90
	// RBX::FactoryProduct<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x1006a9d0
	// RBX::NonFactoryProduct<RBX::RootInstance,&RBX::sWorkspace>::NonFactoryProduct<RBX::RootInstance,&RBX::sWorkspace>
	// TEMPLATE: WEBSERVICE 0x10074490
	// RBX::NonFactoryProduct<RBX::Instance,&RBX::sServiceProvider>::getClassName
	// TEMPLATE: WEBSERVICE 0x1007e870
	// RBX::FactoryProduct<RBX::Soundscape::SoundChannel,RBX::Instance,&RBX::Soundscape::sSoundChannel>::getClassName
	// TEMPLATE: WEBSERVICE 0x100b7280
	// RBX::NonFactoryProduct<RBX::ServiceProvider,&RBX::sGlobalSettings>::getClassName
	// TEMPLATE: WEBSERVICE 0x100d7ab0
	// RBX::NonFactoryProduct<RBX::Instance,&RBX::sJointInstance>::NonFactoryProduct<RBX::Instance,&RBX::sJointInstance>
	// TEMPLATE: WEBSERVICE 0x100d8010
	// RBX::NonFactoryProduct<RBX::Instance,&RBX::sJointInstance>::getClassName
	// TEMPLATE: WEBSERVICE 0x100d82e0
	// RBX::NonFactoryProduct<RBX::JointInstance,&RBX::sAutoJoint>::getClassName
	// TEMPLATE: WEBSERVICE 0x100e3ca0
	// RBX::NonFactoryProduct<RBX::Instance,&RBX::sFeature>::NonFactoryProduct<RBX::Instance,&RBX::sFeature>
	// TEMPLATE: WEBSERVICE 0x1005f0b0
	// RBX::NonFactoryProduct<RBX::Instance,&RBX::Stats::sStatsItem>::NonFactoryProduct<RBX::Instance,&RBX::Stats::sStatsItem>
	// TEMPLATE: WEBSERVICE 0x1005f450
	// RBX::NonFactoryProduct<RBX::Instance,&RBX::Stats::sStatsItem>::NonFactoryProduct<RBX::Instance,&RBX::Stats::sStatsItem><char const *>
	// TEMPLATE: WEBSERVICE 0x1005f4b0
	// RBX::NonFactoryProduct<RBX::Instance,&RBX::Stats::sStats>::NonFactoryProduct<RBX::Instance,&RBX::Stats::sStats><char const *>
	// TEMPLATE: WEBSERVICE 0x100e4ec0
	// RBX::NonFactoryProduct<RBX::Instance,&RBX::sFeature>::getClassName
	// TEMPLATE: WEBSERVICE 0x10062f70
	// RBX::NonFactoryProduct<RBX::Instance,&RBX::Stats::sStatsItem>::getClassName
	// TEMPLATE: WEBSERVICE 0x100ffa80
	// RBX::FactoryProduct<RBX::Decal,RBX::FaceInstance,&RBX::sDecal>::getClassName
	// TEMPLATE: WEBSERVICE 0x1019b3a0
	// RBX::FactoryProduct<RBX::NetworkSettings,RBX::GlobalSettings::Item,&RBX::sNetworkSettings>::~FactoryProduct<RBX::NetworkSettings,RBX::GlobalSettings::Item,&RBX::sNetworkSettings>
	// TEMPLATE: WEBSERVICE 0x1018c410
	// RBX::FactoryProduct<RBX::Network::Player,RBX::Instance,&RBX::Network::sPlayer>::~FactoryProduct<RBX::Network::Player,RBX::Instance,&RBX::Network::sPlayer>
	// TEMPLATE: WEBSERVICE 0x1019b420
	// RBX::FactoryProduct<RBX::NetworkSettings,RBX::GlobalSettings::Item,&RBX::sNetworkSettings>::`scalar deleting destructor'
	// SYNTHETIC: WEBSERVICE 0x1018c4d0
	// RBX::FactoryProduct<RBX::Network::Player,RBX::Instance,&RBX::Network::sPlayer>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x1019b440
	// RBX::FactoryProduct<RBX::NetworkSettings,RBX::GlobalSettings::Item,&RBX::sNetworkSettings>::FactoryProduct<RBX::NetworkSettings,RBX::GlobalSettings::Item,&RBX::sNetworkSettings>
	// TEMPLATE: WEBSERVICE 0x1018c460
	// RBX::FactoryProduct<RBX::Network::Player,RBX::Instance,&RBX::Network::sPlayer>::FactoryProduct<RBX::Network::Player,RBX::Instance,&RBX::Network::sPlayer>
	// clang-format on
	const Creator& getCreator() const { return creator; }

	static const Name& className() { return Name::declare<sName>(); }

	virtual const Name& getClassName() const { return className(); }

protected:
	FactoryProduct() {}

	template <class U>
	FactoryProduct(U arg0) : Base(arg0)
	{
	}

	virtual ~FactoryProduct() {} // vtable+0x00

private:
	static const Creator creator;
};

template <class T, class Base, const char* sName>
const typename FactoryProduct<T, Base, sName>::Creator FactoryProduct<T, Base, sName>::creator;

// clang-format off
// SYNTHETIC: WEBSERVICE 0x10217e20
// `dynamic initializer for 'RBX::FactoryProduct<RBX::Lighting,RBX::Instance,&RBX::sLighting>::creator''
// SYNTHETIC: WEBSERVICE 0x10217e60
// `dynamic initializer for 'RBX::FactoryProduct<RBX::RunService,RBX::Instance,&RBX::sRunService>::creator''
// SYNTHETIC: WEBSERVICE 0x10217ea0
// `dynamic initializer for 'RBX::FactoryProduct<RBX::ModelInstance,RBX::PVInstance,&RBX::sModel>::creator''
// SYNTHETIC: WEBSERVICE 0x10217ee0
// `dynamic initializer for 'RBX::FactoryProduct<RBX::Selection,RBX::Instance,&RBX::sSelection>::creator''
// SYNTHETIC: WEBSERVICE 0x10217f20
// `dynamic initializer for 'RBX::FactoryProduct<RBX::ScriptContext,RBX::Instance,&RBX::sScriptContext>::creator''
// SYNTHETIC: WEBSERVICE 0x10217f60
// `dynamic initializer for 'RBX::FactoryProduct<RBX::Script,RBX::Instance,&RBX::sScript>::creator''
// SYNTHETIC: WEBSERVICE 0x10217fa0
// `dynamic initializer for 'RBX::FactoryProduct<RBX::LocalScript,RBX::Script,&RBX::sLocalScript>::creator''
// SYNTHETIC: WEBSERVICE 0x10218af0
// `dynamic initializer for 'RBX::FactoryProduct<RBX::Humanoid,RBX::Instance,&RBX::sHumanoid>::creator''
// SYNTHETIC: WEBSERVICE 0x10218cc0
// `dynamic initializer for 'RBX::FactoryProduct<RBX::Team,RBX::Instance,&RBX::sTeam>::creator''
// SYNTHETIC: WEBSERVICE 0x10218d00
// `dynamic initializer for 'RBX::FactoryProduct<RBX::Network::Player,RBX::Instance,&RBX::Network::sPlayer>::creator''
// SYNTHETIC: WEBSERVICE 0x10218ee0
// `dynamic initializer for 'RBX::FactoryProduct<RBX::ControllerService,RBX::Instance,&RBX::sControllerService>::creator''
// SYNTHETIC: WEBSERVICE 0x10218f20
// `dynamic initializer for 'RBX::FactoryProduct<RBX::Camera,RBX::Instance,&RBX::sCamera>::creator''
// SYNTHETIC: WEBSERVICE 0x10218fa0
// `dynamic initializer for 'RBX::FactoryProduct<RBX::StarterPackService,RBX::Hopper,&RBX::sStarterPackService>::creator''
// SYNTHETIC: WEBSERVICE 0x10219380
// `dynamic initializer for 'RBX::FactoryProduct<RBX::Snap,RBX::AutoJoint,&RBX::sSnap>::creator''
// SYNTHETIC: WEBSERVICE 0x102193c0
// `dynamic initializer for 'RBX::FactoryProduct<RBX::Weld,RBX::AutoJoint,&RBX::sWeld>::creator''
// SYNTHETIC: WEBSERVICE 0x10219400
// `dynamic initializer for 'RBX::FactoryProduct<RBX::Glue,RBX::AutoJoint,&RBX::sGlue>::creator''
// SYNTHETIC: WEBSERVICE 0x10219440
// `dynamic initializer for 'RBX::FactoryProduct<RBX::Rotate,RBX::AutoJoint,&RBX::sRotate>::creator''
// SYNTHETIC: WEBSERVICE 0x10219480
// `dynamic initializer for 'RBX::FactoryProduct<RBX::RotateP,RBX::AutoJoint,&RBX::sRotateP>::creator''
// SYNTHETIC: WEBSERVICE 0x102194c0
// `dynamic initializer for 'RBX::FactoryProduct<RBX::RotateV,RBX::AutoJoint,&RBX::sRotateV>::creator''
// SYNTHETIC: WEBSERVICE 0x10219540
// `dynamic initializer for 'RBX::FactoryProduct<RBX::Backpack,RBX::Hopper,&RBX::sBackpack>::creator''
// SYNTHETIC: WEBSERVICE 0x10219580
// `dynamic initializer for 'RBX::FactoryProduct<RBX::Teams,RBX::Instance,&RBX::sTeams>::creator''
// SYNTHETIC: WEBSERVICE 0x102195c0
// `dynamic initializer for 'RBX::FactoryProduct<RBX::Accoutrement,RBX::Instance,&RBX::sAccoutrement>::creator''
// SYNTHETIC: WEBSERVICE 0x10219600
// `dynamic initializer for 'RBX::FactoryProduct<RBX::Hat,RBX::Accoutrement,&RBX::sHat>::creator''
// SYNTHETIC: WEBSERVICE 0x10219930
// `dynamic initializer for 'RBX::FactoryProduct<RBX::GameSettings,RBX::GlobalSettings::Item,&RBX::sGameSettings>::creator''
// SYNTHETIC: WEBSERVICE 0x102199b0
// `dynamic initializer for 'RBX::FactoryProduct<RBX::ShirtGraphic,RBX::CharacterAppearance,&RBX::sShirtGraphic>::creator''
// SYNTHETIC: WEBSERVICE 0x102199f0
// `dynamic initializer for 'RBX::FactoryProduct<RBX::Shirt,RBX::ShirtGraphic,&RBX::sShirt>::creator''
// SYNTHETIC: WEBSERVICE 0x10219a30
// `dynamic initializer for 'RBX::FactoryProduct<RBX::BodyColors,RBX::CharacterAppearance,&RBX::sBodyColors>::creator''
// SYNTHETIC: WEBSERVICE 0x10219a70
// `dynamic initializer for 'RBX::FactoryProduct<RBX::Skin,RBX::CharacterAppearance,&RBX::sSkin>::creator''
// SYNTHETIC: WEBSERVICE 0x10219ab0
// `dynamic initializer for 'RBX::FactoryProduct<RBX::Explosion,RBX::Instance,&RBX::sExplosion>::creator''
// SYNTHETIC: WEBSERVICE 0x10219af0
// `dynamic initializer for 'RBX::FactoryProduct<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature>::creator''
// SYNTHETIC: WEBSERVICE 0x10219b30
// `dynamic initializer for 'RBX::FactoryProduct<RBX::Hole,RBX::Feature,&RBX::sHole>::creator''
// SYNTHETIC: WEBSERVICE 0x10219bf0
// `dynamic initializer for 'RBX::FactoryProduct<RBX::FlagStandService,RBX::Instance,&RBX::sFlagStandService>::creator''
// SYNTHETIC: WEBSERVICE 0x10219c30
// `dynamic initializer for 'RBX::FactoryProduct<RBX::ForceField,RBX::Instance,&RBX::sForceField>::creator''
// SYNTHETIC: WEBSERVICE 0x10219c70
// `dynamic initializer for 'RBX::FactoryProduct<RBX::GeometryService,RBX::Instance,&RBX::sGeometryService>::creator''
// SYNTHETIC: WEBSERVICE 0x10219cb0
// `dynamic initializer for 'RBX::FactoryProduct<RBX::BodyGyro,RBX::BodyMover,&RBX::sBodyGyro>::creator''
// SYNTHETIC: WEBSERVICE 0x10219cf0
// `dynamic initializer for 'RBX::FactoryProduct<RBX::BodyForce,RBX::BodyMover,&RBX::sBodyForce>::creator''
// SYNTHETIC: WEBSERVICE 0x10219d30
// `dynamic initializer for 'RBX::FactoryProduct<RBX::BodyThrust,RBX::BodyMover,&RBX::sBodyThrust>::creator''
// SYNTHETIC: WEBSERVICE 0x10219d70
// `dynamic initializer for 'RBX::FactoryProduct<RBX::BodyPosition,RBX::BodyMover,&RBX::sBodyPosition>::creator''
// SYNTHETIC: WEBSERVICE 0x10219db0
// `dynamic initializer for 'RBX::FactoryProduct<RBX::BodyVelocity,RBX::BodyMover,&RBX::sBodyVelocity>::creator''
// SYNTHETIC: WEBSERVICE 0x10219df0
// `dynamic initializer for 'RBX::FactoryProduct<RBX::Rocket,RBX::BodyMover,&RBX::sRocket>::creator''
// SYNTHETIC: WEBSERVICE 0x10219e30
// `dynamic initializer for 'RBX::FactoryProduct<RBX::Message,RBX::Instance,&RBX::sMessage>::creator''
// SYNTHETIC: WEBSERVICE 0x10219e70
// `dynamic initializer for 'RBX::FactoryProduct<RBX::Hint,RBX::Message,&RBX::sHint>::creator''
// SYNTHETIC: WEBSERVICE 0x10219eb0
// `dynamic initializer for 'RBX::FactoryProduct<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue>::creator''
// SYNTHETIC: WEBSERVICE 0x10219ef0
// `dynamic initializer for 'RBX::FactoryProduct<RBX::Visit,RBX::Instance,&RBX::sVisit>::creator''
// SYNTHETIC: WEBSERVICE 0x10219f30
// `dynamic initializer for 'RBX::FactoryProduct<RBX::DebrisService,RBX::Instance,&RBX::sDebrisService>::creator''
// SYNTHETIC: WEBSERVICE 0x10219f70
// `dynamic initializer for 'RBX::FactoryProduct<RBX::TimerService,RBX::Instance,&RBX::sTimerService>::creator''
// SYNTHETIC: WEBSERVICE 0x10219fb0
// `dynamic initializer for 'RBX::FactoryProduct<RBX::Seat,RBX::PartInstance,&RBX::sSeat>::creator''
// SYNTHETIC: WEBSERVICE 0x1021a030
// `dynamic initializer for 'RBX::FactoryProduct<RBX::SpawnerService,RBX::Instance,&RBX::sSpawnerService>::creator''
// SYNTHETIC: WEBSERVICE 0x1021b730
// `dynamic initializer for 'RBX::FactoryProduct<RBX::Texture,RBX::Decal,&RBX::sTexture>::creator''
// SYNTHETIC: WEBSERVICE 0x1021d8a0
// `dynamic initializer for 'RBX::FactoryProduct<RBX::Value<int,&RBX::sIntValue>,RBX::Instance,&RBX::sIntValue>::creator''
// SYNTHETIC: WEBSERVICE 0x1021d8e0
// `dynamic initializer for 'RBX::FactoryProduct<RBX::Value<bool,&RBX::sBoolValue>,RBX::Instance,&RBX::sBoolValue>::creator''
// SYNTHETIC: WEBSERVICE 0x1021d920
// `dynamic initializer for 'RBX::FactoryProduct<RBX::Value<double,&RBX::sDoubleValue>,RBX::Instance,&RBX::sDoubleValue>::creator''
// SYNTHETIC: WEBSERVICE 0x1021d960
// `dynamic initializer for 'RBX::FactoryProduct<RBX::Value<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,&RBX::sStringValue>,RBX::Instance,&RBX::sStringValue>::creator''
// SYNTHETIC: WEBSERVICE 0x1021d9a0
// `dynamic initializer for 'RBX::FactoryProduct<RBX::Value<G3D::Vector3,&RBX::sVector3Value>,RBX::Instance,&RBX::sVector3Value>::creator''
// SYNTHETIC: WEBSERVICE 0x1021d9e0
// `dynamic initializer for 'RBX::FactoryProduct<RBX::Value<G3D::CoordinateFrame,&RBX::sCFrameValue>,RBX::Instance,&RBX::sCFrameValue>::creator''
// SYNTHETIC: WEBSERVICE 0x1021da20
// `dynamic initializer for 'RBX::FactoryProduct<RBX::Value<G3D::Color3,&RBX::sColor3Value>,RBX::Instance,&RBX::sColor3Value>::creator''
// SYNTHETIC: WEBSERVICE 0x1021da60
// `dynamic initializer for 'RBX::FactoryProduct<RBX::Value<RBX::BrickColor,&RBX::sBrickColorValue>,RBX::Instance,&RBX::sBrickColorValue>::creator''
// SYNTHETIC: WEBSERVICE 0x1021e7d0
// `dynamic initializer for 'RBX::FactoryProduct<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape>::creator''
// SYNTHETIC: WEBSERVICE 0x1021f1e0
// `dynamic initializer for 'RBX::FactoryProduct<RBX::NetworkSettings,RBX::GlobalSettings::Item,&RBX::sNetworkSettings>::creator''
// SYNTHETIC: WEBSERVICE 0x10221020
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::LocalScript,RBX::Script,&RBX::sLocalScript>::creator''
// SYNTHETIC: WEBSERVICE 0x10221060
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::Script,RBX::Instance,&RBX::sScript>::creator''
// SYNTHETIC: WEBSERVICE 0x102210a0
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::ScriptContext,RBX::Instance,&RBX::sScriptContext>::creator''
// SYNTHETIC: WEBSERVICE 0x102210e0
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::Selection,RBX::Instance,&RBX::sSelection>::creator''
// SYNTHETIC: WEBSERVICE 0x10221120
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::ModelInstance,RBX::PVInstance,&RBX::sModel>::creator''
// SYNTHETIC: WEBSERVICE 0x10221160
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::RunService,RBX::Instance,&RBX::sRunService>::creator''
// SYNTHETIC: WEBSERVICE 0x102211a0
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::Lighting,RBX::Instance,&RBX::sLighting>::creator''
// SYNTHETIC: WEBSERVICE 0x10221980
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::Humanoid,RBX::Instance,&RBX::sHumanoid>::creator''
// SYNTHETIC: WEBSERVICE 0x10221bc0
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::Network::Player,RBX::Instance,&RBX::Network::sPlayer>::creator''
// SYNTHETIC: WEBSERVICE 0x10221c00
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::Team,RBX::Instance,&RBX::sTeam>::creator''
// SYNTHETIC: WEBSERVICE 0x10221d60
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::StarterPackService,RBX::Hopper,&RBX::sStarterPackService>::creator''
// SYNTHETIC: WEBSERVICE 0x10221de0
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::Camera,RBX::Instance,&RBX::sCamera>::creator''
// SYNTHETIC: WEBSERVICE 0x10221e20
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::ControllerService,RBX::Instance,&RBX::sControllerService>::creator''
// SYNTHETIC: WEBSERVICE 0x10221f00
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::Hat,RBX::Accoutrement,&RBX::sHat>::creator''
// SYNTHETIC: WEBSERVICE 0x10221f40
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::Accoutrement,RBX::Instance,&RBX::sAccoutrement>::creator''
// SYNTHETIC: WEBSERVICE 0x10221f80
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::Teams,RBX::Instance,&RBX::sTeams>::creator''
// SYNTHETIC: WEBSERVICE 0x10221fc0
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::Backpack,RBX::Hopper,&RBX::sBackpack>::creator''
// SYNTHETIC: WEBSERVICE 0x10222040
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::RotateV,RBX::AutoJoint,&RBX::sRotateV>::creator''
// SYNTHETIC: WEBSERVICE 0x10222080
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::RotateP,RBX::AutoJoint,&RBX::sRotateP>::creator''
// SYNTHETIC: WEBSERVICE 0x102220c0
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::Rotate,RBX::AutoJoint,&RBX::sRotate>::creator''
// SYNTHETIC: WEBSERVICE 0x10222100
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::Glue,RBX::AutoJoint,&RBX::sGlue>::creator''
// SYNTHETIC: WEBSERVICE 0x10222140
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::Weld,RBX::AutoJoint,&RBX::sWeld>::creator''
// SYNTHETIC: WEBSERVICE 0x10222180
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::Snap,RBX::AutoJoint,&RBX::sSnap>::creator''
// SYNTHETIC: WEBSERVICE 0x102224a0
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::GameSettings,RBX::GlobalSettings::Item,&RBX::sGameSettings>::creator''
// SYNTHETIC: WEBSERVICE 0x102227c0
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::SpawnerService,RBX::Instance,&RBX::sSpawnerService>::creator''
// SYNTHETIC: WEBSERVICE 0x10222840
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::Seat,RBX::PartInstance,&RBX::sSeat>::creator''
// SYNTHETIC: WEBSERVICE 0x10222880
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::TimerService,RBX::Instance,&RBX::sTimerService>::creator''
// SYNTHETIC: WEBSERVICE 0x102228c0
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::DebrisService,RBX::Instance,&RBX::sDebrisService>::creator''
// SYNTHETIC: WEBSERVICE 0x10222900
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::Visit,RBX::Instance,&RBX::sVisit>::creator''
// SYNTHETIC: WEBSERVICE 0x10222940
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue>::creator''
// SYNTHETIC: WEBSERVICE 0x10222980
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::Hint,RBX::Message,&RBX::sHint>::creator''
// SYNTHETIC: WEBSERVICE 0x102229c0
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::Message,RBX::Instance,&RBX::sMessage>::creator''
// SYNTHETIC: WEBSERVICE 0x10222a00
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::Rocket,RBX::BodyMover,&RBX::sRocket>::creator''
// SYNTHETIC: WEBSERVICE 0x10222a40
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::BodyVelocity,RBX::BodyMover,&RBX::sBodyVelocity>::creator''
// SYNTHETIC: WEBSERVICE 0x10222a80
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::BodyPosition,RBX::BodyMover,&RBX::sBodyPosition>::creator''
// SYNTHETIC: WEBSERVICE 0x10222ac0
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::BodyThrust,RBX::BodyMover,&RBX::sBodyThrust>::creator''
// SYNTHETIC: WEBSERVICE 0x10222b00
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::BodyForce,RBX::BodyMover,&RBX::sBodyForce>::creator''
// SYNTHETIC: WEBSERVICE 0x10222b40
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::BodyGyro,RBX::BodyMover,&RBX::sBodyGyro>::creator''
// SYNTHETIC: WEBSERVICE 0x10222b80
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::GeometryService,RBX::Instance,&RBX::sGeometryService>::creator''
// SYNTHETIC: WEBSERVICE 0x10222bc0
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::ForceField,RBX::Instance,&RBX::sForceField>::creator''
// SYNTHETIC: WEBSERVICE 0x10222c00
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::FlagStandService,RBX::Instance,&RBX::sFlagStandService>::creator''
// SYNTHETIC: WEBSERVICE 0x10222cc0
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::Hole,RBX::Feature,&RBX::sHole>::creator''
// SYNTHETIC: WEBSERVICE 0x10222d00
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature>::creator''
// SYNTHETIC: WEBSERVICE 0x10222d40
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::Explosion,RBX::Instance,&RBX::sExplosion>::creator''
// SYNTHETIC: WEBSERVICE 0x10222d80
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::Skin,RBX::CharacterAppearance,&RBX::sSkin>::creator''
// SYNTHETIC: WEBSERVICE 0x10222dc0
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::BodyColors,RBX::CharacterAppearance,&RBX::sBodyColors>::creator''
// SYNTHETIC: WEBSERVICE 0x10222e00
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::Shirt,RBX::ShirtGraphic,&RBX::sShirt>::creator''
// SYNTHETIC: WEBSERVICE 0x10222e40
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::ShirtGraphic,RBX::CharacterAppearance,&RBX::sShirtGraphic>::creator''
// SYNTHETIC: WEBSERVICE 0x102235b0
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::Texture,RBX::Decal,&RBX::sTexture>::creator''
// SYNTHETIC: WEBSERVICE 0x10224950
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::Value<RBX::BrickColor,&RBX::sBrickColorValue>,RBX::Instance,&RBX::sBrickColorValue>::creator''
// SYNTHETIC: WEBSERVICE 0x10224990
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::Value<G3D::Color3,&RBX::sColor3Value>,RBX::Instance,&RBX::sColor3Value>::creator''
// SYNTHETIC: WEBSERVICE 0x102249d0
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::Value<G3D::CoordinateFrame,&RBX::sCFrameValue>,RBX::Instance,&RBX::sCFrameValue>::creator''
// SYNTHETIC: WEBSERVICE 0x10224a10
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::Value<G3D::Vector3,&RBX::sVector3Value>,RBX::Instance,&RBX::sVector3Value>::creator''
// SYNTHETIC: WEBSERVICE 0x10224a50
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::Value<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,&RBX::sStringValue>,RBX::Instance,&RBX::sStringValue>::creator''
// SYNTHETIC: WEBSERVICE 0x10224a90
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::Value<double,&RBX::sDoubleValue>,RBX::Instance,&RBX::sDoubleValue>::creator''
// SYNTHETIC: WEBSERVICE 0x10224ad0
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::Value<bool,&RBX::sBoolValue>,RBX::Instance,&RBX::sBoolValue>::creator''
// SYNTHETIC: WEBSERVICE 0x10224b10
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::Value<int,&RBX::sIntValue>,RBX::Instance,&RBX::sIntValue>::creator''
// SYNTHETIC: WEBSERVICE 0x102255f0
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape>::creator''
// SYNTHETIC: WEBSERVICE 0x10225d80
// `dynamic atexit destructor for 'RBX::FactoryProduct<RBX::NetworkSettings,RBX::GlobalSettings::Item,&RBX::sNetworkSettings>::creator''
// clang-format on

template <class Base, const char* sName>
class NonFactoryProduct : public Base
{
public:
	NonFactoryProduct() {}

	template <class U>
	NonFactoryProduct(U arg0) : Base(arg0)
	{
	}

	static const Name& className() { return Name::declare<sName>(); }

	virtual const Name& getClassName() const { return className(); }
};

} // namespace RBX

#endif // UTIL_OBJECT_H
