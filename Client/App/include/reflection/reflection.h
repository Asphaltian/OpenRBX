#ifndef REFLECTION_REFLECTION_H
#define REFLECTION_REFLECTION_H

#include "decomp.h"
#include "reflection/object.h"
#include "reflection/property.h"
#include "reflection/signal.h"
#include "util/Handle.h"
#include "v8xml/Serializer.h"
#include "v8xml/XmlElement.h"

#include <boost/cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/type_traits/function_traits.hpp>

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
	// TEMPLATE: WEBSERVICE 0x1005bc50
	// RBX::Reflection::Described<RBX::ModelInstance,&RBX::sModel,RBX::FactoryProduct<RBX::ModelInstance,RBX::PVInstance,&RBX::sModel> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x1005d7f0
	// RBX::Reflection::Described<RBX::Selection,&RBX::sSelection,RBX::FactoryProduct<RBX::Selection,RBX::Instance,&RBX::sSelection> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x10064b80
	// RBX::Reflection::Described<RBX::ScriptContext,&RBX::sScriptContext,RBX::FactoryProduct<RBX::ScriptContext,RBX::Instance,&RBX::sScriptContext> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x10064bf0
	// RBX::Reflection::Described<RBX::Stats::Item,&RBX::Stats::sStatsItem,RBX::NonFactoryProduct<RBX::Instance,&RBX::Stats::sStatsItem> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x10069590
	// RBX::Reflection::Described<RBX::Script,&RBX::sScript,RBX::FactoryProduct<RBX::Script,RBX::Instance,&RBX::sScript> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x10069810
	// RBX::Reflection::Described<RBX::LocalScript,&RBX::sLocalScript,RBX::FactoryProduct<RBX::LocalScript,RBX::Script,&RBX::sLocalScript> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x1006f310
	// RBX::Reflection::Described<RBX::RootInstance,&RBX::sRootInstance,RBX::ModelInstance>::classDescriptor
	// FUNCTION: WEBSERVICE 0x1006f520
	// RBX::Reflection::Described<RBX::Workspace,&RBX::sWorkspace,RBX::NonFactoryProduct<RBX::RootInstance,&RBX::sWorkspace> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x10081b90
	// RBX::Reflection::Described<RBX::Soundscape::SoundChannel,&RBX::Soundscape::sSoundChannel,RBX::FactoryProduct<RBX::Soundscape::SoundChannel,RBX::Instance,&RBX::Soundscape::sSoundChannel> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x10088d00
	// RBX::Reflection::Described<RBX::Accoutrement,&RBX::sAccoutrement,RBX::FactoryProduct<RBX::Accoutrement,RBX::Instance,&RBX::sAccoutrement> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x10088d70
	// RBX::Reflection::Described<RBX::Hat,&RBX::sHat,RBX::FactoryProduct<RBX::Hat,RBX::Accoutrement,&RBX::sHat> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x10088de0
	// RBX::Reflection::Described<RBX::StarterPackService,&RBX::sStarterPackService,RBX::FactoryProduct<RBX::StarterPackService,RBX::Hopper,&RBX::sStarterPackService> >::classDescriptor
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
	// TEMPLATE: WEBSERVICE 0x10088ec0
	// RBX::Reflection::Described<RBX::Backpack,&RBX::sBackpack,RBX::FactoryProduct<RBX::Backpack,RBX::Hopper,&RBX::sBackpack> >::classDescriptor
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
	// TEMPLATE: WEBSERVICE 0x100f24f0
	// RBX::Reflection::Described<RBX::Value<int,&RBX::sIntValue>,&RBX::sIntValue,RBX::FactoryProduct<RBX::Value<int,&RBX::sIntValue>,RBX::Instance,&RBX::sIntValue> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x100f2630
	// RBX::Reflection::Described<RBX::Value<bool,&RBX::sBoolValue>,&RBX::sBoolValue,RBX::FactoryProduct<RBX::Value<bool,&RBX::sBoolValue>,RBX::Instance,&RBX::sBoolValue> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x100f2770
	// RBX::Reflection::Described<RBX::Value<double,&RBX::sDoubleValue>,&RBX::sDoubleValue,RBX::FactoryProduct<RBX::Value<double,&RBX::sDoubleValue>,RBX::Instance,&RBX::sDoubleValue> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x100f29f0
	// RBX::Reflection::Described<RBX::Value<G3D::Vector3,&RBX::sVector3Value>,&RBX::sVector3Value,RBX::FactoryProduct<RBX::Value<G3D::Vector3,&RBX::sVector3Value>,RBX::Instance,&RBX::sVector3Value> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x100f2b30
	// RBX::Reflection::Described<RBX::Value<G3D::CoordinateFrame,&RBX::sCFrameValue>,&RBX::sCFrameValue,RBX::FactoryProduct<RBX::Value<G3D::CoordinateFrame,&RBX::sCFrameValue>,RBX::Instance,&RBX::sCFrameValue> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x100f2c70
	// RBX::Reflection::Described<RBX::Value<G3D::Color3,&RBX::sColor3Value>,&RBX::sColor3Value,RBX::FactoryProduct<RBX::Value<G3D::Color3,&RBX::sColor3Value>,RBX::Instance,&RBX::sColor3Value> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x100f2db0
	// RBX::Reflection::Described<RBX::Value<RBX::BrickColor,&RBX::sBrickColorValue>,&RBX::sBrickColorValue,RBX::FactoryProduct<RBX::Value<RBX::BrickColor,&RBX::sBrickColorValue>,RBX::Instance,&RBX::sBrickColorValue> >::classDescriptor
	// FUNCTION: WEBSERVICE 0x100fe360
	// RBX::Reflection::Described<RBX::SpawnLocation,&RBX::sSpawnLocation,RBX::FactoryProduct<RBX::SpawnLocation,RBX::PartInstance,&RBX::sSpawnLocation> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x100ffb40
	// RBX::Reflection::Described<RBX::FaceInstance,&RBX::sFaceInstance,RBX::Instance>::classDescriptor
	// TEMPLATE: WEBSERVICE 0x100ffbb0
	// RBX::Reflection::Described<RBX::Decal,&RBX::sDecal,RBX::FactoryProduct<RBX::Decal,RBX::FaceInstance,&RBX::sDecal> >::classDescriptor
	// FUNCTION: WEBSERVICE 0x100ffc20
	// RBX::Reflection::Described<RBX::Texture,&RBX::sTexture,RBX::FactoryProduct<RBX::Texture,RBX::Decal,&RBX::sTexture> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x1012d2d0
	// RBX::Reflection::Described<RBX::SpecialShape,&RBX::sSpecialShape,RBX::FactoryProduct<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x1018c170
	// RBX::Reflection::Described<RBX::Network::Player,&RBX::Network::sPlayer,RBX::FactoryProduct<RBX::Network::Player,RBX::Instance,&RBX::Network::sPlayer> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x1018c270
	// RBX::Reflection::Described<RBX::Network::Players,&RBX::Network::sPlayers,RBX::NonFactoryProduct<RBX::Instance,&RBX::Network::sPlayers> >::classDescriptor
	// TEMPLATE: WEBSERVICE 0x1018c2f0
	// RBX::Reflection::Described<RBX::NetworkSettings,&RBX::sNetworkSettings,RBX::FactoryProduct<RBX::NetworkSettings,RBX::GlobalSettings::Item,&RBX::sNetworkSettings> >::classDescriptor
	// SYNTHETIC: WEBSERVICE 0x10221010
	// `RBX::Reflection::Described<RBX::Instance,&RBX::sInstance,RBX::Reflection::DescribedBase>::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x102215d0
	// `RBX::Reflection::Described<RBX::Lighting,&RBX::sLighting,RBX::FactoryProduct<RBX::Lighting,RBX::Instance,&RBX::sLighting> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10221810
	// `RBX::Reflection::Described<RBX::RunService,&RBX::sRunService,RBX::FactoryProduct<RBX::RunService,RBX::Instance,&RBX::sRunService> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10221950
	// `RBX::Reflection::Described<RBX::ModelInstance,&RBX::sModel,RBX::FactoryProduct<RBX::ModelInstance,RBX::PVInstance,&RBX::sModel> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10221960
	// `RBX::Reflection::Described<RBX::PVInstance,&RBX::sPVInstance,RBX::Instance>::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10221a30
	// `RBX::Reflection::Described<RBX::Selection,&RBX::sSelection,RBX::FactoryProduct<RBX::Selection,RBX::Instance,&RBX::sSelection> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10221a90
	// `RBX::Reflection::Described<RBX::Stats::Item,&RBX::Stats::sStatsItem,RBX::NonFactoryProduct<RBX::Instance,&RBX::Stats::sStatsItem> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10221aa0
	// `RBX::Reflection::Described<RBX::ScriptContext,&RBX::sScriptContext,RBX::FactoryProduct<RBX::ScriptContext,RBX::Instance,&RBX::sScriptContext> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10221ba0
	// `RBX::Reflection::Described<RBX::LocalScript,&RBX::sLocalScript,RBX::FactoryProduct<RBX::LocalScript,RBX::Script,&RBX::sLocalScript> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10221bb0
	// `RBX::Reflection::Described<RBX::Script,&RBX::sScript,RBX::FactoryProduct<RBX::Script,RBX::Instance,&RBX::sScript> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10221c80
	// `RBX::Reflection::Described<RBX::RootInstance,&RBX::sRootInstance,RBX::ModelInstance>::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
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
	// SYNTHETIC: WEBSERVICE 0x10222720
	// `RBX::Reflection::Described<RBX::StarterPackService,&RBX::sStarterPackService,RBX::FactoryProduct<RBX::StarterPackService,RBX::Hopper,&RBX::sStarterPackService> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
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
	// SYNTHETIC: WEBSERVICE 0x10222700
	// `RBX::Reflection::Described<RBX::Backpack,&RBX::sBackpack,RBX::FactoryProduct<RBX::Backpack,RBX::Hopper,&RBX::sBackpack> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
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
	// SYNTHETIC: WEBSERVICE 0x10221c70
	// `RBX::Reflection::Described<RBX::Workspace,&RBX::sWorkspace,RBX::NonFactoryProduct<RBX::RootInstance,&RBX::sWorkspace> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10224cc0
	// `RBX::Reflection::Described<RBX::SpawnLocation,&RBX::sSpawnLocation,RBX::FactoryProduct<RBX::SpawnLocation,RBX::PartInstance,&RBX::sSpawnLocation> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// TEMPLATE: WEBSERVICE 0x1007d7d0
	// RBX::Reflection::PropDescriptor<RBX::Decal,RBX::TextureId>::GetSetImpl<RBX::TextureId (__thiscall RBX::Decal::*)(void)const ,void (__thiscall RBX::Decal::*)(RBX::TextureId)>::setValue
	// TEMPLATE: WEBSERVICE 0x100d8960
	// RBX::Reflection::PropDescriptor<RBX::Motor,float>::PropDescriptor<RBX::Motor,float><float (__thiscall RBX::Motor::*)(void)const ,void (__thiscall RBX::Motor::*)(float)>
	// SYNTHETIC: WEBSERVICE 0x10225d20
	// `RBX::Reflection::Described<RBX::NetworkSettings,&RBX::sNetworkSettings,RBX::FactoryProduct<RBX::NetworkSettings,RBX::GlobalSettings::Item,&RBX::sNetworkSettings> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10225d30
	// `RBX::Reflection::Described<RBX::Network::Players,&RBX::Network::sPlayers,RBX::NonFactoryProduct<RBX::Instance,&RBX::Network::sPlayers> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
	// SYNTHETIC: WEBSERVICE 0x10225d50
	// `RBX::Reflection::Described<RBX::Network::Player,&RBX::Network::sPlayer,RBX::FactoryProduct<RBX::Network::Player,RBX::Instance,&RBX::Network::sPlayer> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
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
	// clang-format off
	// TEMPLATE: WEBSERVICE 0x1019b860
	// RBX::Reflection::Described<RBX::NetworkSettings,&RBX::sNetworkSettings,RBX::FactoryProduct<RBX::NetworkSettings,RBX::GlobalSettings::Item,&RBX::sNetworkSettings> >::Described<RBX::NetworkSettings,&RBX::sNetworkSettings,RBX::FactoryProduct<RBX::NetworkSettings,RBX::GlobalSettings::Item,&RBX::sNetworkSettings> >
	// clang-format on
	Described() { this->descriptor = &classDescriptor(); }

	template <class U>
	Described(U arg0) : Base(arg0)
	{
		this->descriptor = &classDescriptor();
	}

	static ClassDescriptor& classDescriptor()
	{
		static ClassDescriptor foo(Base::classDescriptor(), sName);

		return foo;
	}
};

// SIZE 0x20
template <class Class, class T>
class EnumPropDescriptor : public EnumPropertyDescriptor
{
	std::auto_ptr<typename TypedPropertyDescriptor<T>::GetSet> getset; // 0x1c

public:
	template <class GetFunc, class SetFunc>
	EnumPropDescriptor(
		const char* name,
		const char* category,
		GetFunc get,
		SetFunc set,
		typename PropertyDescriptor::Functionality flags = PropertyDescriptor::STANDARD
	)
		: EnumPropertyDescriptor(Class::classDescriptor(), EnumDesc<T>::singleton(), name, category, flags),
		  getset(PropDescriptor<Class, T>::getset(get, set))
	{
	}

	virtual bool isReadOnly() const { return getset->isReadOnly(); }

	T getValue(const DescribedBase* instance) const { return getset->getValue(instance); }

	void setValue(DescribedBase* instance, T value) const { getset->setValue(instance, value); }

	virtual bool equalValues(const DescribedBase* a, const DescribedBase* b) const
	{
		return getValue(a) == getValue(b);
	}

	virtual int getEnumValue(const DescribedBase* instance) const { return getValue(instance); }

	virtual bool setEnumValue(DescribedBase* instance, int intValue) const
	{
		if (EnumDesc<T>::singleton().isValue(intValue)) {
			setValue(instance, static_cast<T>(intValue));
			return true;
		}
		else {
			return false;
		}
	}

	virtual unsigned int getIndexValue(const DescribedBase* instance) const
	{
		return EnumDesc<T>::singleton().convertToIndex(getValue(instance));
	}

	virtual bool setIndexValue(DescribedBase* instance, unsigned int index) const
	{
		T value;

		if (EnumDesc<T>::singleton().convertToValue(index, value)) {
			setValue(instance, value);
			return true;
		}
		else {
			return false;
		}
	}

	virtual bool hasStringValue() const { return true; }

	virtual std::string getStringValue(const DescribedBase* instance) const
	{
		return EnumDesc<T>::singleton().convertToString(getValue(instance));
	}

	virtual bool setStringValue(DescribedBase* instance, const std::string& text) const
	{
		T value;

		if (EnumDesc<T>::singleton().convertToValue(text, value)) {
			setValue(instance, value);
			return true;
		}
		else {
			return false;
		}
	}

	virtual bool setStringValue(DescribedBase* instance, const Name& name) const // vtable+0x34
	{
		T value;

		if (EnumDesc<T>::singleton().convertToValue(name, value)) {
			setValue(instance, value);
			return true;
		}
		else {
			return false;
		}
	}

	virtual void readValue(DescribedBase* instance, const XmlElement* element, IReferenceBinder& binder) const
	{
		if (!element->isXsiNil()) {
			if (element->isValueType<std::string>()) {
				std::string sValue;

				if (element->getValue(sValue)) {
					T e;

					if (EnumDesc<T>::singleton().convertToValue(sValue, e)) {
						setValue(instance, e);
						return;
					}
					else if (sValue.size() == 0) {
						if (setIndexValue(instance, 0)) {
							return;
						}
					}
				}
			}

			int value;

			if (element->getValue(value)) {
				setValue(instance, static_cast<T>(value));
			}
		}
	}

	virtual void writeValue(const DescribedBase* instance, XmlElement* element) const
	{
		element->setValue(static_cast<int>(getValue(instance)));
	}

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x1009a0e0
	// RBX::Reflection::EnumPropDescriptor<RBX::PVInstance,enum RBX::Controller::ControllerType>::setEnumValue
	// TEMPLATE: WEBSERVICE 0x1009a120
	// RBX::Reflection::EnumPropDescriptor<RBX::PVInstance,enum RBX::Controller::ControllerType>::getIndexValue
	// TEMPLATE: WEBSERVICE 0x1009e720
	// RBX::Reflection::EnumPropDescriptor<RBX::PartInstance,enum RBX::PartInstance::FormFactor>::setEnumValue
	// TEMPLATE: WEBSERVICE 0x1009e760
	// RBX::Reflection::EnumPropDescriptor<RBX::PartInstance,enum RBX::PartInstance::FormFactor>::getIndexValue
	// TEMPLATE: WEBSERVICE 0x1009ef50
	// RBX::Reflection::EnumPropDescriptor<RBX::PartInstance,enum RBX::Part::PartType>::setEnumValue
	// TEMPLATE: WEBSERVICE 0x1009ef90
	// RBX::Reflection::EnumPropDescriptor<RBX::PartInstance,enum RBX::Part::PartType>::getIndexValue
	// TEMPLATE: WEBSERVICE 0x100b4980
	// RBX::Reflection::EnumPropDescriptor<RBX::DebugSettings,enum RBX::Debugable::AssertAction>::setEnumValue
	// TEMPLATE: WEBSERVICE 0x100b49c0
	// RBX::Reflection::EnumPropDescriptor<RBX::DebugSettings,enum RBX::Debugable::AssertAction>::getIndexValue
	// TEMPLATE: WEBSERVICE 0x100b4c50
	// RBX::Reflection::EnumPropDescriptor<RBX::DebugSettings,enum RBX::DebugSettings::ErrorReporting>::setEnumValue
	// TEMPLATE: WEBSERVICE 0x100b4c90
	// RBX::Reflection::EnumPropDescriptor<RBX::DebugSettings,enum RBX::DebugSettings::ErrorReporting>::getIndexValue
	// TEMPLATE: WEBSERVICE 0x100e61d0
	// RBX::Reflection::EnumPropDescriptor<RBX::Feature,enum RBX::Feature::TopBottom>::setEnumValue
	// TEMPLATE: WEBSERVICE 0x100e6210
	// RBX::Reflection::EnumPropDescriptor<RBX::Feature,enum RBX::Feature::TopBottom>::getIndexValue
	// TEMPLATE: WEBSERVICE 0x100e64a0
	// RBX::Reflection::EnumPropDescriptor<RBX::Feature,enum RBX::Feature::LeftRight>::setEnumValue
	// TEMPLATE: WEBSERVICE 0x100e64e0
	// RBX::Reflection::EnumPropDescriptor<RBX::Feature,enum RBX::Feature::LeftRight>::getIndexValue
	// TEMPLATE: WEBSERVICE 0x100e6770
	// RBX::Reflection::EnumPropDescriptor<RBX::Feature,enum RBX::Feature::InOut>::setEnumValue
	// TEMPLATE: WEBSERVICE 0x100e67b0
	// RBX::Reflection::EnumPropDescriptor<RBX::Feature,enum RBX::Feature::InOut>::getIndexValue
	// TEMPLATE: WEBSERVICE 0x100e7260
	// RBX::Reflection::EnumPropDescriptor<RBX::FaceInstance,enum RBX::NormalId>::setEnumValue
	// TEMPLATE: WEBSERVICE 0x100e72a0
	// RBX::Reflection::EnumPropDescriptor<RBX::FaceInstance,enum RBX::NormalId>::getIndexValue
	// TEMPLATE: WEBSERVICE 0x1012d5c0
	// RBX::Reflection::EnumPropDescriptor<RBX::SpecialShape,enum RBX::SpecialShape::MeshType>::setEnumValue
	// TEMPLATE: WEBSERVICE 0x1012d600
	// RBX::Reflection::EnumPropDescriptor<RBX::SpecialShape,enum RBX::SpecialShape::MeshType>::getIndexValue
	// clang-format on

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x10099de0
	// RBX::Reflection::EnumPropDescriptor<RBX::PVInstance,enum RBX::Controller::ControllerType>::EnumPropDescriptor<RBX::PVInstance,enum RBX::Controller::ControllerType><enum RBX::Controller::ControllerType (__thiscall RBX::PVInstance::*)(void)const ,void (__thiscall RBX::PVInstance::*)(enum RBX::Controller::ControllerType)>
	// TEMPLATE: WEBSERVICE 0x1009a140
	// RBX::Reflection::EnumPropDescriptor<RBX::PVInstance,enum RBX::Controller::ControllerType>::setIndexValue
	// TEMPLATE: WEBSERVICE 0x1009a280
	// RBX::Reflection::EnumPropDescriptor<RBX::PVInstance,enum RBX::Controller::ControllerType>::readValue
	// TEMPLATE: WEBSERVICE 0x1009e780
	// RBX::Reflection::EnumPropDescriptor<RBX::PartInstance,enum RBX::PartInstance::FormFactor>::setIndexValue
	// TEMPLATE: WEBSERVICE 0x1009a1a0
	// RBX::Reflection::EnumPropDescriptor<RBX::PVInstance,enum RBX::Controller::ControllerType>::getStringValue
	// TEMPLATE: WEBSERVICE 0x1009e7e0
	// RBX::Reflection::EnumPropDescriptor<RBX::PartInstance,enum RBX::PartInstance::FormFactor>::getStringValue
	// TEMPLATE: WEBSERVICE 0x1009f010
	// RBX::Reflection::EnumPropDescriptor<RBX::PartInstance,enum RBX::Part::PartType>::getStringValue
	// TEMPLATE: WEBSERVICE 0x100b4a40
	// RBX::Reflection::EnumPropDescriptor<RBX::DebugSettings,enum RBX::Debugable::AssertAction>::getStringValue
	// TEMPLATE: WEBSERVICE 0x100b4d10
	// RBX::Reflection::EnumPropDescriptor<RBX::DebugSettings,enum RBX::DebugSettings::ErrorReporting>::getStringValue
	// TEMPLATE: WEBSERVICE 0x100e6560
	// RBX::Reflection::EnumPropDescriptor<RBX::Feature,enum RBX::Feature::LeftRight>::getStringValue
	// TEMPLATE: WEBSERVICE 0x100e6830
	// RBX::Reflection::EnumPropDescriptor<RBX::Feature,enum RBX::Feature::InOut>::getStringValue
	// TEMPLATE: WEBSERVICE 0x1012d680
	// RBX::Reflection::EnumPropDescriptor<RBX::SpecialShape,enum RBX::SpecialShape::MeshType>::getStringValue
	// TEMPLATE: WEBSERVICE 0x1009e8c0
	// RBX::Reflection::EnumPropDescriptor<RBX::PartInstance,enum RBX::PartInstance::FormFactor>::readValue
	// TEMPLATE: WEBSERVICE 0x1009efb0
	// RBX::Reflection::EnumPropDescriptor<RBX::PartInstance,enum RBX::Part::PartType>::setIndexValue
	// TEMPLATE: WEBSERVICE 0x1009f0f0
	// RBX::Reflection::EnumPropDescriptor<RBX::PartInstance,enum RBX::Part::PartType>::readValue
	// TEMPLATE: WEBSERVICE 0x100b46b0
	// RBX::Reflection::EnumPropDescriptor<RBX::DebugSettings,enum RBX::Debugable::AssertAction>::EnumPropDescriptor<RBX::DebugSettings,enum RBX::Debugable::AssertAction><enum RBX::Debugable::AssertAction (__thiscall RBX::DebugSettings::*)(void)const ,void (__thiscall RBX::DebugSettings::*)(enum RBX::Debugable::AssertAction)>
	// TEMPLATE: WEBSERVICE 0x100b48e0
	// RBX::Reflection::EnumPropDescriptor<RBX::DebugSettings,enum RBX::DebugSettings::ErrorReporting>::EnumPropDescriptor<RBX::DebugSettings,enum RBX::DebugSettings::ErrorReporting><enum RBX::DebugSettings::ErrorReporting (__thiscall RBX::DebugSettings::*)(void)const ,void (__thiscall RBX::DebugSettings::*)(enum RBX::DebugSettings::ErrorReporting)>
	// TEMPLATE: WEBSERVICE 0x100b49e0
	// RBX::Reflection::EnumPropDescriptor<RBX::DebugSettings,enum RBX::Debugable::AssertAction>::setIndexValue
	// TEMPLATE: WEBSERVICE 0x100b4b20
	// RBX::Reflection::EnumPropDescriptor<RBX::DebugSettings,enum RBX::Debugable::AssertAction>::readValue
	// TEMPLATE: WEBSERVICE 0x100b4cb0
	// RBX::Reflection::EnumPropDescriptor<RBX::DebugSettings,enum RBX::DebugSettings::ErrorReporting>::setIndexValue
	// TEMPLATE: WEBSERVICE 0x100b4df0
	// RBX::Reflection::EnumPropDescriptor<RBX::DebugSettings,enum RBX::DebugSettings::ErrorReporting>::readValue
	// TEMPLATE: WEBSERVICE 0x100e6230
	// RBX::Reflection::EnumPropDescriptor<RBX::Feature,enum RBX::Feature::TopBottom>::setIndexValue
	// TEMPLATE: WEBSERVICE 0x100e6290
	// RBX::Reflection::EnumPropDescriptor<RBX::Feature,enum RBX::Feature::TopBottom>::getStringValue
	// TEMPLATE: WEBSERVICE 0x100e6370
	// RBX::Reflection::EnumPropDescriptor<RBX::Feature,enum RBX::Feature::TopBottom>::readValue
	// TEMPLATE: WEBSERVICE 0x100e6500
	// RBX::Reflection::EnumPropDescriptor<RBX::Feature,enum RBX::Feature::LeftRight>::setIndexValue
	// TEMPLATE: WEBSERVICE 0x100e6640
	// RBX::Reflection::EnumPropDescriptor<RBX::Feature,enum RBX::Feature::LeftRight>::readValue
	// TEMPLATE: WEBSERVICE 0x100e67d0
	// RBX::Reflection::EnumPropDescriptor<RBX::Feature,enum RBX::Feature::InOut>::setIndexValue
	// TEMPLATE: WEBSERVICE 0x100e6910
	// RBX::Reflection::EnumPropDescriptor<RBX::Feature,enum RBX::Feature::InOut>::readValue
	// TEMPLATE: WEBSERVICE 0x10102130
	// RBX::Reflection::EnumPropDescriptor<RBX::SpecialShape,enum RBX::SpecialShape::MeshType>::writeValue
	// TEMPLATE: WEBSERVICE 0x10121b30
	// RBX::Reflection::EnumPropDescriptor<RBX::FaceInstance,enum RBX::NormalId>::setIndexValue
	// TEMPLATE: WEBSERVICE 0x10121be0
	// RBX::Reflection::EnumPropDescriptor<RBX::FaceInstance,enum RBX::NormalId>::readValue
	// TEMPLATE: WEBSERVICE 0x1012d340
	// RBX::Reflection::EnumPropDescriptor<RBX::SpecialShape,enum RBX::SpecialShape::MeshType>::EnumPropDescriptor<RBX::SpecialShape,enum RBX::SpecialShape::MeshType><enum RBX::SpecialShape::MeshType const (__thiscall RBX::SpecialShape::*)(void)const ,void (__thiscall RBX::SpecialShape::*)(enum RBX::SpecialShape::MeshType)>
	// TEMPLATE: WEBSERVICE 0x1012d620
	// RBX::Reflection::EnumPropDescriptor<RBX::SpecialShape,enum RBX::SpecialShape::MeshType>::setIndexValue
	// TEMPLATE: WEBSERVICE 0x1012d760
	// RBX::Reflection::EnumPropDescriptor<RBX::SpecialShape,enum RBX::SpecialShape::MeshType>::readValue
	// clang-format on
};

// SIZE 0x20
template <class Class, class T>
class RefPropDescriptor : public RefPropertyDescriptor, public IIDREF
{
public:
	template <class Getter, class Setter>
	RefPropDescriptor(
		const char* name,
		const char* category,
		Getter get,
		Setter set,
		typename PropertyDescriptor::Functionality functionality = PropertyDescriptor::STANDARD
	)
		: RefPropertyDescriptor(Class::classDescriptor(), RefType::singleton<T*>(), name, category, functionality),
		  getset(PropDescriptor<Class, T*>::getset(get, set))
	{
	}

	virtual bool isReadOnly() const { return getset->isReadOnly(); }

	T* getValue(const DescribedBase* instance) const { return getset->getValue(instance); }

	void setValue(DescribedBase* instance, T* value) const { getset->setValue(instance, value); }

	virtual bool equalValues(const DescribedBase* a, const DescribedBase* b) const
	{
		return getValue(a) == getValue(b);
	}

	virtual DescribedBase* getRefValue(const DescribedBase* instance) const { return getValue(instance); }

	virtual void setRefValue(DescribedBase* instance, DescribedBase* value) const
	{
		T* t = value != NULL ? boost::polymorphic_cast<T*>(value) : NULL;

		setValue(instance, t);
	}

	virtual void readValue(DescribedBase* instance, const XmlElement* element, IReferenceBinder& binder) const
	{
		binder.announceIDREF(element, instance, this);
	}

	virtual void writeValue(const DescribedBase* instance, XmlElement* element) const
	{
		element->setValue(getValue(instance));
	}

	virtual void assignIDREF(DescribedBase* propertyOwner, const InstanceHandle& handle) const
	{
		boost::shared_ptr<Instance> t = handle.getTarget();

		setValue(propertyOwner, static_cast<T*>(t.get()));
	}

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x1004b8a0
	// RBX::Reflection::RefPropDescriptor<RBX::Instance,RBX::Instance>::RefPropDescriptor<RBX::Instance,RBX::Instance><RBX::Instance * (__thiscall RBX::Instance::*)(void)const ,void (__thiscall RBX::Instance::*)(RBX::Instance *)>
	// TEMPLATE: WEBSERVICE 0x1004ba50
	// RBX::Reflection::RefPropDescriptor<RBX::Instance,RBX::Instance>::setRefValue
	// TEMPLATE: WEBSERVICE 0x1004ba90
	// RBX::Reflection::RefPropDescriptor<RBX::Instance,RBX::Instance>::assignIDREF
	// TEMPLATE: WEBSERVICE 0x1005be40
	// RBX::Reflection::RefPropDescriptor<RBX::Instance,RBX::Instance>::readValue
	// TEMPLATE: WEBSERVICE 0x100a3680
	// RBX::Reflection::RefPropDescriptor<RBX::Instance,RBX::Instance>::writeValue
	// TEMPLATE: WEBSERVICE 0x100b4750
	// RBX::Reflection::RefPropDescriptor<RBX::Instance,RBX::Instance>::equalValues
	// TEMPLATE: WEBSERVICE 0x100d8810
	// RBX::Reflection::RefPropDescriptor<RBX::AutoJoint,RBX::PartInstance>::RefPropDescriptor<RBX::AutoJoint,RBX::PartInstance><RBX::PartInstance * (__thiscall RBX::AutoJoint::*)(void)const ,void (__thiscall RBX::AutoJoint::*)(RBX::PartInstance *)>
	// SYNTHETIC: WEBSERVICE 0x100e5fe0
	// RBX::Reflection::RefPropDescriptor<RBX::Instance,RBX::Instance>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x100ec830
	// RBX::Reflection::RefPropDescriptor<RBX::Instance,RBX::Instance>::getRefValue
	// TEMPLATE: WEBSERVICE 0x100ec850
	// RBX::Reflection::RefPropDescriptor<RBX::AutoJoint,RBX::PartInstance>::setRefValue
	// TEMPLATE: WEBSERVICE 0x100f3970
	// RBX::Reflection::RefPropDescriptor<RBX::ObjectValue,RBX::Instance>::RefPropDescriptor<RBX::ObjectValue,RBX::Instance><RBX::Instance * (__thiscall RBX::ObjectValue::*)(void)const ,void (__thiscall RBX::ObjectValue::*)(RBX::Instance *)>
	// TEMPLATE: WEBSERVICE 0x10102200
	// RBX::Reflection::RefPropDescriptor<RBX::Instance,RBX::Instance>::isReadOnly
	// clang-format on

private:
	std::auto_ptr<typename TypedPropertyDescriptor<T*>::GetSet> getset; // 0x1c
};

// SIZE 0x24
template <class Class>
class FuncDesc : public FunctionDescriptor
{
protected:
	FuncDesc(const char* name, Security security) : FunctionDescriptor(Class::classDescriptor(), name, security) {}
};

template <class Class, class Signature, int arity>
class BoundFuncDesc;

// SIZE 0x30
template <class Class, class Signature>
class BoundFuncDesc<Class, Signature, 0> : public FuncDesc<Class>
{
public:
	typedef typename boost::function_traits<Signature>::result_type result_type;
	typedef result_type (Class::*Function)();

private:
	Function function; // 0x28

	template <class R>
	void call(Class* o, Value& returnValue) const
	{
		returnValue = (o->*function)();
	}

	template <>
	void call<void>(Class* o, Value& returnValue) const
	{
		(o->*function)();
	}

	void declareSignature() { this->signature.resultType = &Type::singleton<result_type>(); }

public:
	BoundFuncDesc(Function function, const char* name, typename FunctionDescriptor::Security security)
		: FuncDesc<Class>(name, security), function(function)
	{
		declareSignature();
	}

	virtual void execute(DescribedBase* instance, typename FunctionDescriptor::Arguments& arguments) const
	{
		call<result_type>(boost::polymorphic_cast<Class*>(instance), arguments.returnValue);
	}

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x10045f00
	// RBX::Reflection::BoundFuncDesc<RBX::Stats::Item,double __cdecl(void),0>::call<double>
	// TEMPLATE: WEBSERVICE 0x100567d0
	// RBX::Reflection::BoundFuncDesc<RBX::RunService,void __cdecl(void),0>::BoundFuncDesc<RBX::RunService,void __cdecl(void),0>
	// TEMPLATE: WEBSERVICE 0x10056840
	// RBX::Reflection::BoundFuncDesc<RBX::RunService,void __cdecl(void),0>::execute
	// TEMPLATE: WEBSERVICE 0x100b23b0
	// RBX::Reflection::BoundFuncDesc<RBX::Stats::Item,std::basic_string<char,std::char_traits<char>,std::allocator<char> > __cdecl(void),0>::BoundFuncDesc<RBX::Stats::Item,std::basic_string<char,std::char_traits<char>,std::allocator<char> > __cdecl(void),0>
	// TEMPLATE: WEBSERVICE 0x100b2420
	// RBX::Reflection::BoundFuncDesc<RBX::Stats::Item,std::basic_string<char,std::char_traits<char>,std::allocator<char> > __cdecl(void),0>::call<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >
	// TEMPLATE: WEBSERVICE 0x100b24b0
	// RBX::Reflection::BoundFuncDesc<RBX::Stats::Item,std::basic_string<char,std::char_traits<char>,std::allocator<char> > __cdecl(void),0>::execute
	// TEMPLATE: WEBSERVICE 0x100b2510
	// RBX::Reflection::BoundFuncDesc<RBX::Stats::Item,double __cdecl(void),0>::BoundFuncDesc<RBX::Stats::Item,double __cdecl(void),0>
	// TEMPLATE: WEBSERVICE 0x100b2580
	// RBX::Reflection::BoundFuncDesc<RBX::Stats::Item,double __cdecl(void),0>::execute
	// TEMPLATE: WEBSERVICE 0x100db180
	// RBX::Reflection::BoundFuncDesc<RBX::Teams,void __cdecl(void),0>::BoundFuncDesc<RBX::Teams,void __cdecl(void),0>
	// TEMPLATE: WEBSERVICE 0x100db1f0
	// RBX::Reflection::BoundFuncDesc<RBX::Teams,void __cdecl(void),0>::execute
	// TEMPLATE: WEBSERVICE 0x100db2c0
	// RBX::Reflection::BoundFuncDesc<RBX::Teams,boost::shared_ptr<std::vector<boost::shared_ptr<RBX::Instance>,std::allocator<boost::shared_ptr<RBX::Instance> > > const > __cdecl(void),0>::execute
	// TEMPLATE: WEBSERVICE 0x100f9750
	// RBX::Reflection::BoundFuncDesc<RBX::Visit,std::basic_string<char,std::char_traits<char>,std::allocator<char> > __cdecl(void),0>::BoundFuncDesc<RBX::Visit,std::basic_string<char,std::char_traits<char>,std::allocator<char> > __cdecl(void),0>
	// TEMPLATE: WEBSERVICE 0x100f97c0
	// RBX::Reflection::BoundFuncDesc<RBX::Visit,std::basic_string<char,std::char_traits<char>,std::allocator<char> > __cdecl(void),0>::execute
	// clang-format on
};

// SIZE 0x40
template <class Class, class Signature>
class BoundFuncDesc<Class, Signature, 1> : public FuncDesc<Class>
{
public:
	typedef typename boost::function_traits<Signature>::result_type result_type;
	typedef typename boost::function_traits<Signature>::arg1_type Arg1;
	typedef result_type (Class::*Function)(Arg1);

private:
	Function function; // 0x28
	Value default1;    // 0x30

	void declareSignature(const char* arg1Name)
	{
		this->signature.resultType = &Type::singleton<result_type>();
		this->signature.addArgument(Name::declare(arg1Name, -1), Type::singleton<Arg1>(), default1);
	}

	template <class R>
	void call(Class* o, Value& arg1, Value& returnValue) const
	{
		returnValue = (o->*function)(arg1.convert<Arg1>());
	}

	template <>
	void call<void>(Class* o, Value& arg1, Value& returnValue) const
	{
		(o->*function)(arg1.convert<Arg1>());
	}

public:
	BoundFuncDesc(
		Function function,
		const char* name,
		const char* arg1Name,
		typename FunctionDescriptor::Security security
	)
		: FuncDesc<Class>(name, security), function(function)
	{
		declareSignature(arg1Name);
	}

	BoundFuncDesc(
		Function function,
		const char* name,
		const char* arg1Name,
		Arg1 default1,
		typename FunctionDescriptor::Security security
	)
		: FuncDesc<Class>(name, security), function(function), default1(default1)
	{
		declareSignature(arg1Name);
	}

	virtual void execute(DescribedBase* instance, typename FunctionDescriptor::Arguments& arguments) const
	{
		Value arg1(default1);
		arguments.get(1, arg1);
		call<result_type>(boost::polymorphic_cast<Class*>(instance), arg1, arguments.returnValue);
	}

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x100f95d0
	// RBX::Reflection::BoundFuncDesc<RBX::Visit,void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >),1>::BoundFuncDesc<RBX::Visit,void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >),1>
	// TEMPLATE: WEBSERVICE 0x100f9670
	// RBX::Reflection::BoundFuncDesc<RBX::Visit,void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >),1>::execute
	// clang-format on
};

template <class Class, class Signature>
class BoundFuncDesc<Class, Signature, 2> : public FuncDesc<Class>
{
public:
	typedef typename boost::function_traits<Signature>::result_type result_type;
	typedef typename boost::function_traits<Signature>::arg1_type Arg1;
	typedef typename boost::function_traits<Signature>::arg2_type Arg2;
	typedef result_type (Class::*Function)(Arg1, Arg2);

private:
	Function function; // 0x28
	Value default1;    // 0x30
	Value default2;    // 0x38

	void declareSignature(const char* arg1Name, const char* arg2Name)
	{
		this->signature.resultType = &Type::singleton<result_type>();
		this->signature.addArgument(Name::declare(arg1Name, -1), Type::singleton<Arg1>(), default1);
		this->signature.addArgument(Name::declare(arg2Name, -1), Type::singleton<Arg2>(), default2);
	}

	template <class R>
	void call(Class* o, Value& arg1, Value& arg2, Value& returnValue) const
	{
		returnValue = (o->*function)(arg1.convert<Arg1>(), arg2.convert<Arg2>());
	}

	template <>
	void call<void>(Class* o, Value& arg1, Value& arg2, Value& returnValue) const
	{
		(o->*function)(arg1.convert<Arg1>(), arg2.convert<Arg2>());
	}

public:
	BoundFuncDesc(
		Function function,
		const char* name,
		const char* arg1Name,
		const char* arg2Name,
		typename FunctionDescriptor::Security security
	)
		: FuncDesc<Class>(name, security), function(function)
	{
		declareSignature(arg1Name, arg2Name);
	}

	BoundFuncDesc(
		Function function,
		const char* name,
		const char* arg1Name,
		const char* arg2Name,
		Arg2 default2,
		typename FunctionDescriptor::Security security
	)
		: FuncDesc<Class>(name, security), function(function), default2(default2)
	{
		declareSignature(arg1Name, arg2Name);
	}

	BoundFuncDesc(
		Function function,
		const char* name,
		const char* arg1Name,
		Arg1 default1,
		const char* arg2Name,
		Arg2 default2,
		typename FunctionDescriptor::Security security
	)
		: FuncDesc<Class>(name, security), function(function), default1(default1), default2(default2)
	{
		declareSignature(arg1Name, arg2Name);
	}

	virtual void execute(DescribedBase* instance, typename FunctionDescriptor::Arguments& arguments) const
	{
		Value arg1(default1);
		arguments.get(1, arg1);
		Value arg2(default2);
		arguments.get(2, arg2);
		call<result_type>(boost::polymorphic_cast<Class*>(instance), arg1, arg2, arguments.returnValue);
	}

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x100f8be0
	// RBX::Reflection::BoundFuncDesc<RBX::Visit,void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >,int),2>::declareSignature
	// TEMPLATE: WEBSERVICE 0x100f9820
	// RBX::Reflection::BoundFuncDesc<RBX::Visit,void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >,int),2>::BoundFuncDesc<RBX::Visit,void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >,int),2>
	// TEMPLATE: WEBSERVICE 0x100f98b0
	// RBX::Reflection::BoundFuncDesc<RBX::Visit,void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >,int),2>::execute
	// TEMPLATE: WEBSERVICE 0x100f9e40
	// RBX::Reflection::BoundFuncDesc<RBX::DebrisService,void __cdecl(boost::shared_ptr<RBX::Instance>,double),2>::declareSignature
	// TEMPLATE: WEBSERVICE 0x100fafb0
	// RBX::Reflection::BoundFuncDesc<RBX::DebrisService,void __cdecl(boost::shared_ptr<RBX::Instance>,double),2>::BoundFuncDesc<RBX::DebrisService,void __cdecl(boost::shared_ptr<RBX::Instance>,double),2>
	// TEMPLATE: WEBSERVICE 0x100fb070
	// RBX::Reflection::BoundFuncDesc<RBX::DebrisService,void __cdecl(boost::shared_ptr<RBX::Instance>,double),2>::execute
	// clang-format on
};

} // namespace Reflection
} // namespace RBX

#endif // REFLECTION_REFLECTION_H
