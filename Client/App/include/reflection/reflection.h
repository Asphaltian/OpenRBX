#ifndef REFLECTION_REFLECTION_H
#define REFLECTION_REFLECTION_H

#include "decomp.h"
#include "reflection/object.h"
#include "reflection/signal.h"

namespace RBX {
namespace Reflection {

// SIZE 0x0c
class DescribedBase : public SignalSource
{
public:
	// clang-format off
	// STUB: WEBSERVICE 0x10008d50
	// RBX::Reflection::Described<RBX::Instance,&RBX::sInstance,RBX::Reflection::DescribedBase>::classDescriptor
	// TEMPLATE: WEBSERVICE 0x10045560
	// RBX::Reflection::Described<RBX::Lighting,&RBX::sLighting,RBX::FactoryProduct<RBX::Lighting,RBX::Instance,&RBX::sLighting> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x100561a0
	// RBX::Reflection::Described<RBX::RunService,&RBX::sRunService,RBX::FactoryProduct<RBX::RunService,RBX::Instance,&RBX::sRunService> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x100596e0
	// RBX::Reflection::Described<RBX::ServiceProvider,&RBX::sServiceProvider,RBX::NonFactoryProduct<RBX::Instance,&RBX::sServiceProvider> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x1005bbe0
	// RBX::Reflection::Described<RBX::PVInstance,&RBX::sPVInstance,RBX::Instance>::classDescriptor
	// TEMPLATE: WEBSERVICE 0x1005d7f0
	// RBX::Reflection::Described<RBX::Selection,&RBX::sSelection,RBX::FactoryProduct<RBX::Selection,RBX::Instance,&RBX::sSelection> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x10064b80
	// RBX::Reflection::Described<RBX::ScriptContext,&RBX::sScriptContext,RBX::FactoryProduct<RBX::ScriptContext,RBX::Instance,&RBX::sScriptContext> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x10069590
	// RBX::Reflection::Described<RBX::Script,&RBX::sScript,RBX::FactoryProduct<RBX::Script,RBX::Instance,&RBX::sScript> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x10069810
	// RBX::Reflection::Described<RBX::LocalScript,&RBX::sLocalScript,RBX::FactoryProduct<RBX::LocalScript,RBX::Script,&RBX::sLocalScript> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x10081b90
	// RBX::Reflection::Described<RBX::Soundscape::SoundChannel,&RBX::Soundscape::sSoundChannel,RBX::FactoryProduct<RBX::Soundscape::SoundChannel,RBX::Instance,&RBX::Soundscape::sSoundChannel> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x10088d00
	// RBX::Reflection::Described<RBX::Accoutrement,&RBX::sAccoutrement,RBX::FactoryProduct<RBX::Accoutrement,RBX::Instance,&RBX::sAccoutrement> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x10088d70
	// RBX::Reflection::Described<RBX::Hat,&RBX::sHat,RBX::FactoryProduct<RBX::Hat,RBX::Accoutrement,&RBX::sHat> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x10088f30
	// RBX::Reflection::Described<RBX::ShirtGraphic,&RBX::sShirtGraphic,RBX::FactoryProduct<RBX::ShirtGraphic,RBX::CharacterAppearance,&RBX::sShirtGraphic> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x10088fa0
	// RBX::Reflection::Described<RBX::BodyColors,&RBX::sBodyColors,RBX::FactoryProduct<RBX::BodyColors,RBX::CharacterAppearance,&RBX::sBodyColors> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x10089010
	// RBX::Reflection::Described<RBX::Explosion,&RBX::sExplosion,RBX::FactoryProduct<RBX::Explosion,RBX::Instance,&RBX::sExplosion> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x10089080
	// RBX::Reflection::Described<RBX::FlagStandService,&RBX::sFlagStandService,RBX::FactoryProduct<RBX::FlagStandService,RBX::Instance,&RBX::sFlagStandService> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x100890f0
	// RBX::Reflection::Described<RBX::ForceField,&RBX::sForceField,RBX::FactoryProduct<RBX::ForceField,RBX::Instance,&RBX::sForceField> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x10089160
	// RBX::Reflection::Described<RBX::GameSettings,&RBX::sGameSettings,RBX::FactoryProduct<RBX::GameSettings,RBX::GlobalSettings::Item,&RBX::sGameSettings> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x100891d0
	// RBX::Reflection::Described<RBX::GeometryService,&RBX::sGeometryService,RBX::FactoryProduct<RBX::GeometryService,RBX::Instance,&RBX::sGeometryService> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x10089320
	// RBX::Reflection::Described<RBX::Message,&RBX::sMessage,RBX::FactoryProduct<RBX::Message,RBX::Instance,&RBX::sMessage> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x10089390
	// RBX::Reflection::Described<RBX::Hint,&RBX::sHint,RBX::FactoryProduct<RBX::Hint,RBX::Message,&RBX::sHint> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x10089400
	// RBX::Reflection::Described<RBX::Visit,&RBX::sVisit,RBX::FactoryProduct<RBX::Visit,RBX::Instance,&RBX::sVisit> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x10089470
	// RBX::Reflection::Described<RBX::ControllerService,&RBX::sControllerService,RBX::FactoryProduct<RBX::ControllerService,RBX::Instance,&RBX::sControllerService> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x100894e0
	// RBX::Reflection::Described<RBX::DebrisService,&RBX::sDebrisService,RBX::FactoryProduct<RBX::DebrisService,RBX::Instance,&RBX::sDebrisService> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x10089550
	// RBX::Reflection::Described<RBX::TimerService,&RBX::sTimerService,RBX::FactoryProduct<RBX::TimerService,RBX::Instance,&RBX::sTimerService> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x100895c0
	// RBX::Reflection::Described<RBX::SpawnerService,&RBX::sSpawnerService,RBX::FactoryProduct<RBX::SpawnerService,RBX::Instance,&RBX::sSpawnerService> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x10089630
	// RBX::Reflection::Described<RBX::Humanoid,&RBX::sHumanoid,RBX::FactoryProduct<RBX::Humanoid,RBX::Instance,&RBX::sHumanoid> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x100896a0
	// RBX::Reflection::Described<RBX::Feature,&RBX::sFeature,RBX::NonFactoryProduct<RBX::Instance,&RBX::sFeature> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x10089710
	// RBX::Reflection::Described<RBX::BackpackItem,&RBX::sBackpackItem,RBX::NonFactoryProduct<RBX::Widget,&RBX::sBackpackItem> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x10089780
	// RBX::Reflection::Described<RBX::JointInstance,&RBX::sJointInstance,RBX::NonFactoryProduct<RBX::Instance,&RBX::sJointInstance> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x1008cc60
	// RBX::Reflection::Described<RBX::ObjectValue,&RBX::sObjectValue,RBX::FactoryProduct<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x1008db70
	// RBX::Reflection::Described<RBX::GlobalSettings,&RBX::sGlobalSettings,RBX::NonFactoryProduct<RBX::ServiceProvider,&RBX::sGlobalSettings> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x1008dbe0
	// RBX::Reflection::Described<RBX::MotorFeature,&RBX::sMotorFeature,RBX::FactoryProduct<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x1008dc50
	// RBX::Reflection::Described<RBX::Hole,&RBX::sHole,RBX::FactoryProduct<RBX::Hole,RBX::Feature,&RBX::sHole> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x1008dcc0
	// RBX::Reflection::Described<RBX::AutoJoint,&RBX::sAutoJoint,RBX::NonFactoryProduct<RBX::JointInstance,&RBX::sAutoJoint> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x1008dd30
	// RBX::Reflection::Described<RBX::Tool,&RBX::sTool,RBX::FactoryProduct<RBX::Tool,RBX::BackpackItem,&RBX::sTool> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x1008dda0
	// RBX::Reflection::Described<RBX::PartInstance,&RBX::sPart,RBX::FactoryProduct<RBX::PartInstance,RBX::PVInstance,&RBX::sPart> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x1008ded0
	// RBX::Reflection::Described<RBX::Snap,&RBX::sSnap,RBX::FactoryProduct<RBX::Snap,RBX::AutoJoint,&RBX::sSnap> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x1008df40
	// RBX::Reflection::Described<RBX::Weld,&RBX::sWeld,RBX::FactoryProduct<RBX::Weld,RBX::AutoJoint,&RBX::sWeld> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x1008dfb0
	// RBX::Reflection::Described<RBX::Glue,&RBX::sGlue,RBX::FactoryProduct<RBX::Glue,RBX::AutoJoint,&RBX::sGlue> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x1008e020
	// RBX::Reflection::Described<RBX::Rotate,&RBX::sRotate,RBX::FactoryProduct<RBX::Rotate,RBX::AutoJoint,&RBX::sRotate> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x1008e090
	// RBX::Reflection::Described<RBX::RotateP,&RBX::sRotateP,RBX::FactoryProduct<RBX::RotateP,RBX::AutoJoint,&RBX::sRotateP> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x1008e100
	// RBX::Reflection::Described<RBX::RotateV,&RBX::sRotateV,RBX::FactoryProduct<RBX::RotateV,RBX::AutoJoint,&RBX::sRotateV> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x1008e170
	// RBX::Reflection::Described<RBX::Motor,&RBX::sMotor,RBX::FactoryProduct<RBX::Motor,RBX::AutoJoint,&RBX::sMotor> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x1008e1e0
	// RBX::Reflection::Described<RBX::Flag,&RBX::sFlag,RBX::FactoryProduct<RBX::Flag,RBX::Tool,&RBX::sFlag> >::classDescriptor
	// FUNCTION: WEBSERVICE 0x1008e390
	// RBX::Reflection::Described<RBX::Seat,&RBX::sSeat,RBX::FactoryProduct<RBX::Seat,RBX::PartInstance,&RBX::sSeat> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x10094d80
	// RBX::Reflection::Described<RBX::Sky,&RBX::sSky,RBX::FactoryProduct<RBX::Sky,RBX::Instance,&RBX::sSky> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x100b42f0
	// RBX::Reflection::Described<RBX::DebugSettings,&RBX::sDebugSettings,RBX::FactoryProduct<RBX::DebugSettings,RBX::GlobalSettings::Item,&RBX::sDebugSettings> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x100be240
	// RBX::Reflection::Described<RBX::Team,&RBX::sTeam,RBX::FactoryProduct<RBX::Team,RBX::Instance,&RBX::sTeam> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x100c3a60
	// RBX::Reflection::Described<RBX::Camera,&RBX::sCamera,RBX::FactoryProduct<RBX::Camera,RBX::Instance,&RBX::sCamera> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x100dabe0
	// RBX::Reflection::Described<RBX::Teams,&RBX::sTeams,RBX::FactoryProduct<RBX::Teams,RBX::Instance,&RBX::sTeams> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x100e0d60
	// RBX::Reflection::Described<RBX::Shirt,&RBX::sShirt,RBX::FactoryProduct<RBX::Shirt,RBX::ShirtGraphic,&RBX::sShirt> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x100e0dd0
	// RBX::Reflection::Described<RBX::Skin,&RBX::sSkin,RBX::FactoryProduct<RBX::Skin,RBX::CharacterAppearance,&RBX::sSkin> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x100e6160
	// RBX::Reflection::Described<RBX::VelocityMotor,&RBX::sVelocityMotor,RBX::FactoryProduct<RBX::VelocityMotor,RBX::JointInstance,&RBX::sVelocityMotor> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x100ec1a0
	// RBX::Reflection::Described<RBX::BodyPosition,&RBX::sBodyPosition,RBX::FactoryProduct<RBX::BodyPosition,RBX::BodyMover,&RBX::sBodyPosition> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x100ec210
	// RBX::Reflection::Described<RBX::BodyVelocity,&RBX::sBodyVelocity,RBX::FactoryProduct<RBX::BodyVelocity,RBX::BodyMover,&RBX::sBodyVelocity> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x100ec5c0
	// RBX::Reflection::Described<RBX::BodyGyro,&RBX::sBodyGyro,RBX::FactoryProduct<RBX::BodyGyro,RBX::BodyMover,&RBX::sBodyGyro> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x100ec630
	// RBX::Reflection::Described<RBX::BodyForce,&RBX::sBodyForce,RBX::FactoryProduct<RBX::BodyForce,RBX::BodyMover,&RBX::sBodyForce> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x100ec6a0
	// RBX::Reflection::Described<RBX::BodyThrust,&RBX::sBodyThrust,RBX::FactoryProduct<RBX::BodyThrust,RBX::BodyMover,&RBX::sBodyThrust> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x100ec710
	// RBX::Reflection::Described<RBX::Rocket,&RBX::sRocket,RBX::FactoryProduct<RBX::Rocket,RBX::BodyMover,&RBX::sRocket> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x100ffb40
	// RBX::Reflection::Described<RBX::FaceInstance,&RBX::sFaceInstance,RBX::Instance>::classDescriptor
	// TEMPLATE: WEBSERVICE 0x100ffbb0
	// RBX::Reflection::Described<RBX::Decal,&RBX::sDecal,RBX::FactoryProduct<RBX::Decal,RBX::FaceInstance,&RBX::sDecal> >::classDescriptor
	// FUNCTION: WEBSERVICE 0x100ffc20
	// RBX::Reflection::Described<RBX::Texture,&RBX::sTexture,RBX::FactoryProduct<RBX::Texture,RBX::Decal,&RBX::sTexture> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x1012d2d0
	// RBX::Reflection::Described<RBX::SpecialShape,&RBX::sSpecialShape,RBX::FactoryProduct<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape> >::classDescriptor
	// SYNTHETIC: WEBSERVICE 0x10221010
	// `RBX::Reflection::Described<RBX::Instance,&RBX::sInstance,RBX::Reflection::DescribedBase>::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x102215d0
	// `RBX::Reflection::Described<RBX::Lighting,&RBX::sLighting,RBX::FactoryProduct<RBX::Lighting,RBX::Instance,&RBX::sLighting> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10221810
	// `RBX::Reflection::Described<RBX::RunService,&RBX::sRunService,RBX::FactoryProduct<RBX::RunService,RBX::Instance,&RBX::sRunService> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10221960
	// `RBX::Reflection::Described<RBX::PVInstance,&RBX::sPVInstance,RBX::Instance>::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10221a30
	// `RBX::Reflection::Described<RBX::Selection,&RBX::sSelection,RBX::FactoryProduct<RBX::Selection,RBX::Instance,&RBX::sSelection> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10221aa0
	// `RBX::Reflection::Described<RBX::ScriptContext,&RBX::sScriptContext,RBX::FactoryProduct<RBX::ScriptContext,RBX::Instance,&RBX::sScriptContext> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10221ba0
	// `RBX::Reflection::Described<RBX::LocalScript,&RBX::sLocalScript,RBX::FactoryProduct<RBX::LocalScript,RBX::Script,&RBX::sLocalScript> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10221bb0
	// `RBX::Reflection::Described<RBX::Script,&RBX::sScript,RBX::FactoryProduct<RBX::Script,RBX::Instance,&RBX::sScript> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10222430
	// `RBX::Reflection::Described<RBX::Soundscape::SoundChannel,&RBX::Soundscape::sSoundChannel,RBX::FactoryProduct<RBX::Soundscape::SoundChannel,RBX::Instance,&RBX::Soundscape::sSoundChannel> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10222520
	// `RBX::Reflection::Described<RBX::Humanoid,&RBX::sHumanoid,RBX::FactoryProduct<RBX::Humanoid,RBX::Instance,&RBX::sHumanoid> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10222530
	// `RBX::Reflection::Described<RBX::SpawnerService,&RBX::sSpawnerService,RBX::FactoryProduct<RBX::SpawnerService,RBX::Instance,&RBX::sSpawnerService> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10222540
	// `RBX::Reflection::Described<RBX::Seat,&RBX::sSeat,RBX::FactoryProduct<RBX::Seat,RBX::PartInstance,&RBX::sSeat> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10222550
	// `RBX::Reflection::Described<RBX::TimerService,&RBX::sTimerService,RBX::FactoryProduct<RBX::TimerService,RBX::Instance,&RBX::sTimerService> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10222560
	// `RBX::Reflection::Described<RBX::DebrisService,&RBX::sDebrisService,RBX::FactoryProduct<RBX::DebrisService,RBX::Instance,&RBX::sDebrisService> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10222570
	// `RBX::Reflection::Described<RBX::ControllerService,&RBX::sControllerService,RBX::FactoryProduct<RBX::ControllerService,RBX::Instance,&RBX::sControllerService> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10222580
	// `RBX::Reflection::Described<RBX::Visit,&RBX::sVisit,RBX::FactoryProduct<RBX::Visit,RBX::Instance,&RBX::sVisit> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10222590
	// `RBX::Reflection::Described<RBX::Hint,&RBX::sHint,RBX::FactoryProduct<RBX::Hint,RBX::Message,&RBX::sHint> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x102225a0
	// `RBX::Reflection::Described<RBX::Message,&RBX::sMessage,RBX::FactoryProduct<RBX::Message,RBX::Instance,&RBX::sMessage> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x102225d0
	// `RBX::Reflection::Described<RBX::GeometryService,&RBX::sGeometryService,RBX::FactoryProduct<RBX::GeometryService,RBX::Instance,&RBX::sGeometryService> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x102225e0
	// `RBX::Reflection::Described<RBX::GameSettings,&RBX::sGameSettings,RBX::FactoryProduct<RBX::GameSettings,RBX::GlobalSettings::Item,&RBX::sGameSettings> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x102225f0
	// `RBX::Reflection::Described<RBX::ForceField,&RBX::sForceField,RBX::FactoryProduct<RBX::ForceField,RBX::Instance,&RBX::sForceField> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10222600
	// `RBX::Reflection::Described<RBX::FlagStandService,&RBX::sFlagStandService,RBX::FactoryProduct<RBX::FlagStandService,RBX::Instance,&RBX::sFlagStandService> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10222620
	// `RBX::Reflection::Described<RBX::Flag,&RBX::sFlag,RBX::FactoryProduct<RBX::Flag,RBX::Tool,&RBX::sFlag> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10222630
	// `RBX::Reflection::Described<RBX::Hole,&RBX::sHole,RBX::FactoryProduct<RBX::Hole,RBX::Feature,&RBX::sHole> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10222640
	// `RBX::Reflection::Described<RBX::MotorFeature,&RBX::sMotorFeature,RBX::FactoryProduct<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10222650
	// `RBX::Reflection::Described<RBX::Motor,&RBX::sMotor,RBX::FactoryProduct<RBX::Motor,RBX::AutoJoint,&RBX::sMotor> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10222660
	// `RBX::Reflection::Described<RBX::RotateV,&RBX::sRotateV,RBX::FactoryProduct<RBX::RotateV,RBX::AutoJoint,&RBX::sRotateV> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10222670
	// `RBX::Reflection::Described<RBX::RotateP,&RBX::sRotateP,RBX::FactoryProduct<RBX::RotateP,RBX::AutoJoint,&RBX::sRotateP> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10222680
	// `RBX::Reflection::Described<RBX::Rotate,&RBX::sRotate,RBX::FactoryProduct<RBX::Rotate,RBX::AutoJoint,&RBX::sRotate> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10222690
	// `RBX::Reflection::Described<RBX::Glue,&RBX::sGlue,RBX::FactoryProduct<RBX::Glue,RBX::AutoJoint,&RBX::sGlue> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x102226a0
	// `RBX::Reflection::Described<RBX::Weld,&RBX::sWeld,RBX::FactoryProduct<RBX::Weld,RBX::AutoJoint,&RBX::sWeld> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x102226b0
	// `RBX::Reflection::Described<RBX::Snap,&RBX::sSnap,RBX::FactoryProduct<RBX::Snap,RBX::AutoJoint,&RBX::sSnap> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x102226c0
	// `RBX::Reflection::Described<RBX::Explosion,&RBX::sExplosion,RBX::FactoryProduct<RBX::Explosion,RBX::Instance,&RBX::sExplosion> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x102226d0
	// `RBX::Reflection::Described<RBX::BodyColors,&RBX::sBodyColors,RBX::FactoryProduct<RBX::BodyColors,RBX::CharacterAppearance,&RBX::sBodyColors> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x102226e0
	// `RBX::Reflection::Described<RBX::ShirtGraphic,&RBX::sShirtGraphic,RBX::FactoryProduct<RBX::ShirtGraphic,RBX::CharacterAppearance,&RBX::sShirtGraphic> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10222730
	// `RBX::Reflection::Described<RBX::Hat,&RBX::sHat,RBX::FactoryProduct<RBX::Hat,RBX::Accoutrement,&RBX::sHat> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10222740
	// `RBX::Reflection::Described<RBX::Accoutrement,&RBX::sAccoutrement,RBX::FactoryProduct<RBX::Accoutrement,RBX::Instance,&RBX::sAccoutrement> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10222750
	// `RBX::Reflection::Described<RBX::PartInstance,&RBX::sPart,RBX::FactoryProduct<RBX::PartInstance,RBX::PVInstance,&RBX::sPart> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10222780
	// `RBX::Reflection::Described<RBX::AutoJoint,&RBX::sAutoJoint,RBX::NonFactoryProduct<RBX::JointInstance,&RBX::sAutoJoint> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10222770
	// `RBX::Reflection::Described<RBX::Feature,&RBX::sFeature,RBX::NonFactoryProduct<RBX::Instance,&RBX::sFeature> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10222760
	// `RBX::Reflection::Described<RBX::Tool,&RBX::sTool,RBX::FactoryProduct<RBX::Tool,RBX::BackpackItem,&RBX::sTool> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10222790
	// `RBX::Reflection::Described<RBX::ObjectValue,&RBX::sObjectValue,RBX::FactoryProduct<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x102227b0
	// `RBX::Reflection::Described<RBX::BackpackItem,&RBX::sBackpackItem,RBX::NonFactoryProduct<RBX::Widget,&RBX::sBackpackItem> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x102227a0
	// `RBX::Reflection::Described<RBX::JointInstance,&RBX::sJointInstance,RBX::NonFactoryProduct<RBX::Instance,&RBX::sJointInstance> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10222fe0
	// `RBX::Reflection::Described<RBX::Sky,&RBX::sSky,RBX::FactoryProduct<RBX::Sky,RBX::Instance,&RBX::sSky> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10223960
	// `RBX::Reflection::Described<RBX::DebugSettings,&RBX::sDebugSettings,RBX::FactoryProduct<RBX::DebugSettings,RBX::GlobalSettings::Item,&RBX::sDebugSettings> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10223aa0
	// `RBX::Reflection::Described<RBX::Team,&RBX::sTeam,RBX::FactoryProduct<RBX::Team,RBX::Instance,&RBX::sTeam> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10223b30
	// `RBX::Reflection::Described<RBX::Camera,&RBX::sCamera,RBX::FactoryProduct<RBX::Camera,RBX::Instance,&RBX::sCamera> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10223e70
	// `RBX::Reflection::Described<RBX::Teams,&RBX::sTeams,RBX::FactoryProduct<RBX::Teams,RBX::Instance,&RBX::sTeams> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10224160
	// `RBX::Reflection::Described<RBX::Skin,&RBX::sSkin,RBX::FactoryProduct<RBX::Skin,RBX::CharacterAppearance,&RBX::sSkin> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10224170
	// `RBX::Reflection::Described<RBX::Shirt,&RBX::sShirt,RBX::FactoryProduct<RBX::Shirt,RBX::ShirtGraphic,&RBX::sShirt> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10224300
	// `RBX::Reflection::Described<RBX::VelocityMotor,&RBX::sVelocityMotor,RBX::FactoryProduct<RBX::VelocityMotor,RBX::JointInstance,&RBX::sVelocityMotor> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x102246a0
	// `RBX::Reflection::Described<RBX::Rocket,&RBX::sRocket,RBX::FactoryProduct<RBX::Rocket,RBX::BodyMover,&RBX::sRocket> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x102246b0
	// `RBX::Reflection::Described<RBX::BodyVelocity,&RBX::sBodyVelocity,RBX::FactoryProduct<RBX::BodyVelocity,RBX::BodyMover,&RBX::sBodyVelocity> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x102246c0
	// `RBX::Reflection::Described<RBX::BodyPosition,&RBX::sBodyPosition,RBX::FactoryProduct<RBX::BodyPosition,RBX::BodyMover,&RBX::sBodyPosition> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x102246d0
	// `RBX::Reflection::Described<RBX::BodyThrust,&RBX::sBodyThrust,RBX::FactoryProduct<RBX::BodyThrust,RBX::BodyMover,&RBX::sBodyThrust> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x102246e0
	// `RBX::Reflection::Described<RBX::BodyForce,&RBX::sBodyForce,RBX::FactoryProduct<RBX::BodyForce,RBX::BodyMover,&RBX::sBodyForce> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x102246f0
	// `RBX::Reflection::Described<RBX::BodyGyro,&RBX::sBodyGyro,RBX::FactoryProduct<RBX::BodyGyro,RBX::BodyMover,&RBX::sBodyGyro> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10224d70
	// `RBX::Reflection::Described<RBX::Texture,&RBX::sTexture,RBX::FactoryProduct<RBX::Texture,RBX::Decal,&RBX::sTexture> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10224d80
	// `RBX::Reflection::Described<RBX::Decal,&RBX::sDecal,RBX::FactoryProduct<RBX::Decal,RBX::FaceInstance,&RBX::sDecal> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10224d90
	// `RBX::Reflection::Described<RBX::FaceInstance,&RBX::sFaceInstance,RBX::Instance>::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x102256d0
	// `RBX::Reflection::Described<RBX::SpecialShape,&RBX::sSpecialShape,RBX::FactoryProduct<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// clang-format on
	static ClassDescriptor& classDescriptor() { return ClassDescriptor::rootDescriptor(); }

	virtual ~DescribedBase() {} // vtable+0x00

	const ClassDescriptor& getDescriptor() const { return *descriptor; }

protected:
	const ClassDescriptor* descriptor; // 0x08
};

DECOMP_SIZE_ASSERT(DescribedBase, 0x0c)

template <class T, const char* sName, class Base>
class __declspec(novtable) Described : public Base
{
public:
	Described() { this->descriptor = &classDescriptor(); }

	template <class U>
	Described(U* joint) : Base(joint)
	{
		this->descriptor = &classDescriptor();
	}

	static ClassDescriptor& classDescriptor()
	{
		static ClassDescriptor foo(Base::classDescriptor(), sName);

		return foo;
	}
};

} // namespace Reflection
} // namespace RBX

#endif // REFLECTION_REFLECTION_H
