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

// clang-format off
// TEMPLATE: WEBSERVICE 0x10006320
// RBX::Reflection::FuncDesc<ThumbnailGenerator>::~FuncDesc<ThumbnailGenerator>
// TEMPLATE: WEBSERVICE 0x10008b10
// RBX::Reflection::Described<ThumbnailGenerator,&sThumbnailGenerator,RBX::FactoryProduct<ThumbnailGenerator,RBX::Instance,&sThumbnailGenerator> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x10008d50
// TEMPLATE: ROBLOX 0x00419fb0
// RBX::Reflection::Described<RBX::Instance,&RBX::sInstance,RBX::Reflection::DescribedBase>::classDescriptor
// TEMPLATE: WEBSERVICE 0x10045560
// TEMPLATE: ROBLOX 0x005db300
// RBX::Reflection::Described<RBX::Lighting,&RBX::sLighting,RBX::FactoryProduct<RBX::Lighting,RBX::Instance,&RBX::sLighting> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x1004b760
// RBX::Reflection::PropDescriptor<RBX::Instance,std::basic_string<char,std::char_traits<char>,std::allocator<char> > >::PropDescriptor<RBX::Instance,std::basic_string<char,std::char_traits<char>,std::allocator<char> > ><std::basic_string<char,std::char_traits<char>,std::allocator<char> > (__thiscall RBX::Instance::*)(void)const ,int>
// TEMPLATE: WEBSERVICE 0x100561a0
// TEMPLATE: ROBLOX 0x0056a730
// RBX::Reflection::Described<RBX::RunService,&RBX::sRunService,RBX::FactoryProduct<RBX::RunService,RBX::Instance,&RBX::sRunService> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x100596e0
// STUB: ROBLOX 0x004b8450
// RBX::Reflection::Described<RBX::ServiceProvider,&RBX::sServiceProvider,RBX::NonFactoryProduct<RBX::Instance,&RBX::sServiceProvider> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x1005bbe0
// TEMPLATE: ROBLOX 0x00566490
// RBX::Reflection::Described<RBX::PVInstance,&RBX::sPVInstance,RBX::Instance>::classDescriptor
// TEMPLATE: WEBSERVICE 0x1005bc50
// TEMPLATE: ROBLOX 0x00566500
// RBX::Reflection::Described<RBX::ModelInstance,&RBX::sModel,RBX::FactoryProduct<RBX::ModelInstance,RBX::PVInstance,&RBX::sModel> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x1005d7f0
// TEMPLATE: ROBLOX 0x00567890
// RBX::Reflection::Described<RBX::Selection,&RBX::sSelection,RBX::FactoryProduct<RBX::Selection,RBX::Instance,&RBX::sSelection> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x10064820
// STUB: ROBLOX 0x0045c030
// RBX::Reflection::Described<RBX::Stats::StatsService,&RBX::Stats::sStats,RBX::NonFactoryProduct<RBX::Instance,&RBX::Stats::sStats> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x10064b80
// TEMPLATE: ROBLOX 0x00571ea0
// RBX::Reflection::Described<RBX::ScriptContext,&RBX::sScriptContext,RBX::FactoryProduct<RBX::ScriptContext,RBX::Instance,&RBX::sScriptContext> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x10064bf0
// STUB: ROBLOX 0x00461f40
// RBX::Reflection::Described<RBX::Stats::Item,&RBX::Stats::sStatsItem,RBX::NonFactoryProduct<RBX::Instance,&RBX::Stats::sStatsItem> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x10069590
// TEMPLATE: ROBLOX 0x005758b0
// RBX::Reflection::Described<RBX::Script,&RBX::sScript,RBX::FactoryProduct<RBX::Script,RBX::Instance,&RBX::sScript> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x10069810
// TEMPLATE: ROBLOX 0x00575b30
// RBX::Reflection::Described<RBX::LocalScript,&RBX::sLocalScript,RBX::FactoryProduct<RBX::LocalScript,RBX::Script,&RBX::sLocalScript> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x1006f310
// TEMPLATE: ROBLOX 0x005a1a30
// RBX::Reflection::Described<RBX::RootInstance,&RBX::sRootInstance,RBX::ModelInstance>::classDescriptor
// FUNCTION: WEBSERVICE 0x1006f520
// FUNCTION: ROBLOX 0x005b77f0
// RBX::Reflection::Described<RBX::Workspace,&RBX::sWorkspace,RBX::NonFactoryProduct<RBX::RootInstance,&RBX::sWorkspace> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x100794b0
// TEMPLATE: ROBLOX 0x00592780
// RBX::Reflection::Described<RBX::DataModel,&RBX::sDataModel,RBX::NonFactoryProduct<RBX::ServiceProvider,&RBX::sDataModel> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x1007d7d0
// TEMPLATE: ROBLOX 0x005c16f0
// RBX::Reflection::PropDescriptor<RBX::Decal,RBX::TextureId>::GetSetImpl<RBX::TextureId (__thiscall RBX::Decal::*)(void)const ,void (__thiscall RBX::Decal::*)(RBX::TextureId)>::setValue
// TEMPLATE: WEBSERVICE 0x10081b20
// TEMPLATE: ROBLOX 0x005c5670
// RBX::Reflection::Described<RBX::Soundscape::SoundService,&RBX::Soundscape::sSoundService,RBX::FactoryProduct<RBX::Soundscape::SoundService,RBX::Instance,&RBX::Soundscape::sSoundService> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x10081b90
// TEMPLATE: ROBLOX 0x005c56e0
// RBX::Reflection::Described<RBX::Soundscape::SoundChannel,&RBX::Soundscape::sSoundChannel,RBX::FactoryProduct<RBX::Soundscape::SoundChannel,RBX::Instance,&RBX::Soundscape::sSoundChannel> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x10081c00
// TEMPLATE: ROBLOX 0x005c5750
// RBX::Reflection::Described<StockSound,&sStockSound,RBX::FactoryProduct<StockSound,RBX::Soundscape::SoundChannel,&sStockSound> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x10088d00
// TEMPLATE: ROBLOX 0x005bb730
// RBX::Reflection::Described<RBX::Accoutrement,&RBX::sAccoutrement,RBX::FactoryProduct<RBX::Accoutrement,RBX::Instance,&RBX::sAccoutrement> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x10088d70
// TEMPLATE: ROBLOX 0x005bb7a0
// RBX::Reflection::Described<RBX::Hat,&RBX::sHat,RBX::FactoryProduct<RBX::Hat,RBX::Accoutrement,&RBX::sHat> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x10088de0
// TEMPLATE: ROBLOX 0x005ca290
// RBX::Reflection::Described<RBX::StarterPackService,&RBX::sStarterPackService,RBX::FactoryProduct<RBX::StarterPackService,RBX::Hopper,&RBX::sStarterPackService> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x10088e50
// TEMPLATE: ROBLOX 0x005ca300
// RBX::Reflection::Described<RBX::LegacyHopperService,&RBX::sLegacyHopperService,RBX::FactoryProduct<RBX::LegacyHopperService,RBX::Hopper,&RBX::sLegacyHopperService> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x10088ec0
// TEMPLATE: ROBLOX 0x005ca370
// RBX::Reflection::Described<RBX::Backpack,&RBX::sBackpack,RBX::FactoryProduct<RBX::Backpack,RBX::Hopper,&RBX::sBackpack> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x10088f30
// TEMPLATE: ROBLOX 0x005ca3e0
// RBX::Reflection::Described<RBX::ShirtGraphic,&RBX::sShirtGraphic,RBX::FactoryProduct<RBX::ShirtGraphic,RBX::CharacterAppearance,&RBX::sShirtGraphic> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x10088fa0
// TEMPLATE: ROBLOX 0x005ca450
// RBX::Reflection::Described<RBX::BodyColors,&RBX::sBodyColors,RBX::FactoryProduct<RBX::BodyColors,RBX::CharacterAppearance,&RBX::sBodyColors> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x10089010
// TEMPLATE: ROBLOX 0x005ca4c0
// RBX::Reflection::Described<RBX::Explosion,&RBX::sExplosion,RBX::FactoryProduct<RBX::Explosion,RBX::Instance,&RBX::sExplosion> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x10089080
// TEMPLATE: ROBLOX 0x005ca530
// RBX::Reflection::Described<RBX::FlagStandService,&RBX::sFlagStandService,RBX::FactoryProduct<RBX::FlagStandService,RBX::Instance,&RBX::sFlagStandService> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x100890f0
// TEMPLATE: ROBLOX 0x005ca5a0
// RBX::Reflection::Described<RBX::ForceField,&RBX::sForceField,RBX::FactoryProduct<RBX::ForceField,RBX::Instance,&RBX::sForceField> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x10089160
// TEMPLATE: ROBLOX 0x005c95a0
// RBX::Reflection::Described<RBX::GameSettings,&RBX::sGameSettings,RBX::FactoryProduct<RBX::GameSettings,RBX::GlobalSettings::Item,&RBX::sGameSettings> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x100891d0
// TEMPLATE: ROBLOX 0x005ca610
// RBX::Reflection::Described<RBX::GeometryService,&RBX::sGeometryService,RBX::FactoryProduct<RBX::GeometryService,RBX::Instance,&RBX::sGeometryService> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x10089240
// TEMPLATE: ROBLOX 0x005ca680
// RBX::Reflection::Described<RBX::LocalBackpackItem,&RBX::sLocalBackpackItem,RBX::FactoryProduct<RBX::LocalBackpackItem,RBX::Widget,&RBX::sLocalBackpackItem> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x100892b0
// TEMPLATE: ROBLOX 0x005ca6f0
// RBX::Reflection::Described<RBX::LocalBackpack,&RBX::sLocalBackpack,RBX::FactoryProduct<RBX::LocalBackpack,RBX::RelativePanel,&RBX::sLocalBackpack> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x10089320
// TEMPLATE: ROBLOX 0x005ca760
// RBX::Reflection::Described<RBX::Message,&RBX::sMessage,RBX::FactoryProduct<RBX::Message,RBX::Instance,&RBX::sMessage> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x10089390
// TEMPLATE: ROBLOX 0x005ca7d0
// RBX::Reflection::Described<RBX::Hint,&RBX::sHint,RBX::FactoryProduct<RBX::Hint,RBX::Message,&RBX::sHint> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x10089400
// TEMPLATE: ROBLOX 0x005ca840
// RBX::Reflection::Described<RBX::Visit,&RBX::sVisit,RBX::FactoryProduct<RBX::Visit,RBX::Instance,&RBX::sVisit> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x10089470
// TEMPLATE: ROBLOX 0x005ca8b0
// RBX::Reflection::Described<RBX::ControllerService,&RBX::sControllerService,RBX::FactoryProduct<RBX::ControllerService,RBX::Instance,&RBX::sControllerService> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x100894e0
// TEMPLATE: ROBLOX 0x005ca920
// RBX::Reflection::Described<RBX::DebrisService,&RBX::sDebrisService,RBX::FactoryProduct<RBX::DebrisService,RBX::Instance,&RBX::sDebrisService> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x10089550
// TEMPLATE: ROBLOX 0x005ca990
// RBX::Reflection::Described<RBX::TimerService,&RBX::sTimerService,RBX::FactoryProduct<RBX::TimerService,RBX::Instance,&RBX::sTimerService> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x100895c0
// TEMPLATE: ROBLOX 0x005caa00
// RBX::Reflection::Described<RBX::SpawnerService,&RBX::sSpawnerService,RBX::FactoryProduct<RBX::SpawnerService,RBX::Instance,&RBX::sSpawnerService> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x10089630
// TEMPLATE: ROBLOX 0x005caa70
// RBX::Reflection::Described<RBX::Humanoid,&RBX::sHumanoid,RBX::FactoryProduct<RBX::Humanoid,RBX::Instance,&RBX::sHumanoid> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x100896a0
// TEMPLATE: ROBLOX 0x005caae0
// RBX::Reflection::Described<RBX::Feature,&RBX::sFeature,RBX::NonFactoryProduct<RBX::Instance,&RBX::sFeature> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x10089710
// TEMPLATE: ROBLOX 0x005cab50
// RBX::Reflection::Described<RBX::BackpackItem,&RBX::sBackpackItem,RBX::NonFactoryProduct<RBX::Widget,&RBX::sBackpackItem> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x10089780
// TEMPLATE: ROBLOX 0x005cabc0
// RBX::Reflection::Described<RBX::JointInstance,&RBX::sJointInstance,RBX::NonFactoryProduct<RBX::Instance,&RBX::sJointInstance> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x1008cc60
// TEMPLATE: ROBLOX 0x005ccf90
// RBX::Reflection::Described<RBX::ObjectValue,&RBX::sObjectValue,RBX::FactoryProduct<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x1008db70
// STUB: ROBLOX 0x004b8710
// RBX::Reflection::Described<RBX::GlobalSettings,&RBX::sGlobalSettings,RBX::NonFactoryProduct<RBX::ServiceProvider,&RBX::sGlobalSettings> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x1008dbe0
// TEMPLATE: ROBLOX 0x005cd4e0
// RBX::Reflection::Described<RBX::MotorFeature,&RBX::sMotorFeature,RBX::FactoryProduct<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x1008dc50
// TEMPLATE: ROBLOX 0x005cd550
// RBX::Reflection::Described<RBX::Hole,&RBX::sHole,RBX::FactoryProduct<RBX::Hole,RBX::Feature,&RBX::sHole> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x1008dcc0
// TEMPLATE: ROBLOX 0x005cd5c0
// RBX::Reflection::Described<RBX::AutoJoint,&RBX::sAutoJoint,RBX::NonFactoryProduct<RBX::JointInstance,&RBX::sAutoJoint> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x1008dd30
// TEMPLATE: ROBLOX 0x005cd630
// RBX::Reflection::Described<RBX::Tool,&RBX::sTool,RBX::FactoryProduct<RBX::Tool,RBX::BackpackItem,&RBX::sTool> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x1008dda0
// TEMPLATE: ROBLOX 0x005b0950
// RBX::Reflection::Described<RBX::PartInstance,&RBX::sPart,RBX::FactoryProduct<RBX::PartInstance,RBX::PVInstance,&RBX::sPart> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x1008ded0
// TEMPLATE: ROBLOX 0x005cd760
// RBX::Reflection::Described<RBX::Snap,&RBX::sSnap,RBX::FactoryProduct<RBX::Snap,RBX::AutoJoint,&RBX::sSnap> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x1008df40
// TEMPLATE: ROBLOX 0x005cd7d0
// RBX::Reflection::Described<RBX::Weld,&RBX::sWeld,RBX::FactoryProduct<RBX::Weld,RBX::AutoJoint,&RBX::sWeld> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x1008dfb0
// TEMPLATE: ROBLOX 0x005cd840
// RBX::Reflection::Described<RBX::Glue,&RBX::sGlue,RBX::FactoryProduct<RBX::Glue,RBX::AutoJoint,&RBX::sGlue> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x1008e020
// TEMPLATE: ROBLOX 0x005cd8b0
// RBX::Reflection::Described<RBX::Rotate,&RBX::sRotate,RBX::FactoryProduct<RBX::Rotate,RBX::AutoJoint,&RBX::sRotate> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x1008e090
// TEMPLATE: ROBLOX 0x005cd920
// RBX::Reflection::Described<RBX::RotateP,&RBX::sRotateP,RBX::FactoryProduct<RBX::RotateP,RBX::AutoJoint,&RBX::sRotateP> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x1008e100
// TEMPLATE: ROBLOX 0x005cd990
// RBX::Reflection::Described<RBX::RotateV,&RBX::sRotateV,RBX::FactoryProduct<RBX::RotateV,RBX::AutoJoint,&RBX::sRotateV> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x1008e170
// TEMPLATE: ROBLOX 0x005cda00
// RBX::Reflection::Described<RBX::Motor,&RBX::sMotor,RBX::FactoryProduct<RBX::Motor,RBX::AutoJoint,&RBX::sMotor> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x1008e1e0
// TEMPLATE: ROBLOX 0x005cda70
// RBX::Reflection::Described<RBX::Flag,&RBX::sFlag,RBX::FactoryProduct<RBX::Flag,RBX::Tool,&RBX::sFlag> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x1008e250
// TEMPLATE: ROBLOX 0x005cdae0
// RBX::Reflection::Described<RBX::FlagStand,&RBX::sFlagStand,RBX::FactoryProduct<RBX::FlagStand,RBX::PartInstance,&RBX::sFlagStand> >::classDescriptor
// FUNCTION: WEBSERVICE 0x1008e390
// FUNCTION: ROBLOX 0x005cdc50
// RBX::Reflection::Described<RBX::Seat,&RBX::sSeat,RBX::FactoryProduct<RBX::Seat,RBX::PartInstance,&RBX::sSeat> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x10094d80
// TEMPLATE: ROBLOX 0x005dd590
// RBX::Reflection::Described<RBX::Sky,&RBX::sSky,RBX::FactoryProduct<RBX::Sky,RBX::Instance,&RBX::sSky> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x100b42f0
// TEMPLATE: ROBLOX 0x0057c2f0
// RBX::Reflection::Described<RBX::DebugSettings,&RBX::sDebugSettings,RBX::FactoryProduct<RBX::DebugSettings,RBX::GlobalSettings::Item,&RBX::sDebugSettings> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x100be240
// TEMPLATE: ROBLOX 0x0058b850
// RBX::Reflection::Described<RBX::Team,&RBX::sTeam,RBX::FactoryProduct<RBX::Team,RBX::Instance,&RBX::sTeam> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x100c3a60
// TEMPLATE: ROBLOX 0x005d7600
// RBX::Reflection::Described<RBX::Camera,&RBX::sCamera,RBX::FactoryProduct<RBX::Camera,RBX::Instance,&RBX::sCamera> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x100c8700
// TEMPLATE: ROBLOX 0x005e27e0
// RBX::Reflection::Described<RBX::HopperBin,&RBX::sHopperBin,RBX::FactoryProduct<RBX::HopperBin,RBX::BackpackItem,&RBX::sHopperBin> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x100cb390
// TEMPLATE: ROBLOX 0x0060ff60
// RBX::Reflection::Described<RBX::Mouse,&RBX::sMouse,RBX::NonFactoryProduct<RBX::Instance,&RBX::sMouse> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x100d8960
// RBX::Reflection::PropDescriptor<RBX::Motor,float>::PropDescriptor<RBX::Motor,float><float (__thiscall RBX::Motor::*)(void)const ,void (__thiscall RBX::Motor::*)(float)>
// TEMPLATE: WEBSERVICE 0x100dabe0
// TEMPLATE: ROBLOX 0x005e79d0
// RBX::Reflection::Described<RBX::Teams,&RBX::sTeams,RBX::FactoryProduct<RBX::Teams,RBX::Instance,&RBX::sTeams> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x100e0d60
// TEMPLATE: ROBLOX 0x005e6810
// RBX::Reflection::Described<RBX::Shirt,&RBX::sShirt,RBX::FactoryProduct<RBX::Shirt,RBX::ShirtGraphic,&RBX::sShirt> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x100e0dd0
// TEMPLATE: ROBLOX 0x005e6880
// RBX::Reflection::Described<RBX::Skin,&RBX::sSkin,RBX::FactoryProduct<RBX::Skin,RBX::CharacterAppearance,&RBX::sSkin> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x100e6160
// TEMPLATE: ROBLOX 0x00619050
// RBX::Reflection::Described<RBX::VelocityMotor,&RBX::sVelocityMotor,RBX::FactoryProduct<RBX::VelocityMotor,RBX::JointInstance,&RBX::sVelocityMotor> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x100ec1a0
// STUB: ROBLOX 0x0062b250
// RBX::Reflection::Described<RBX::BodyPosition,&RBX::sBodyPosition,RBX::FactoryProduct<RBX::BodyPosition,RBX::BodyMover,&RBX::sBodyPosition> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x100ec210
// TEMPLATE: ROBLOX 0x0062b2c0
// RBX::Reflection::Described<RBX::BodyVelocity,&RBX::sBodyVelocity,RBX::FactoryProduct<RBX::BodyVelocity,RBX::BodyMover,&RBX::sBodyVelocity> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x100ec5c0
// STUB: ROBLOX 0x0062b670
// RBX::Reflection::Described<RBX::BodyGyro,&RBX::sBodyGyro,RBX::FactoryProduct<RBX::BodyGyro,RBX::BodyMover,&RBX::sBodyGyro> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x100ec630
// TEMPLATE: ROBLOX 0x0062b6e0
// RBX::Reflection::Described<RBX::BodyForce,&RBX::sBodyForce,RBX::FactoryProduct<RBX::BodyForce,RBX::BodyMover,&RBX::sBodyForce> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x100ec6a0
// TEMPLATE: ROBLOX 0x0062b750
// RBX::Reflection::Described<RBX::BodyThrust,&RBX::sBodyThrust,RBX::FactoryProduct<RBX::BodyThrust,RBX::BodyMover,&RBX::sBodyThrust> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x100ec710
// TEMPLATE: ROBLOX 0x0062b7c0
// RBX::Reflection::Described<RBX::Rocket,&RBX::sRocket,RBX::FactoryProduct<RBX::Rocket,RBX::BodyMover,&RBX::sRocket> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x100f24f0
// TEMPLATE: ROBLOX 0x00631240
// RBX::Reflection::Described<RBX::Value<int,&RBX::sIntValue>,&RBX::sIntValue,RBX::FactoryProduct<RBX::Value<int,&RBX::sIntValue>,RBX::Instance,&RBX::sIntValue> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x100f2630
// TEMPLATE: ROBLOX 0x00631380
// RBX::Reflection::Described<RBX::Value<bool,&RBX::sBoolValue>,&RBX::sBoolValue,RBX::FactoryProduct<RBX::Value<bool,&RBX::sBoolValue>,RBX::Instance,&RBX::sBoolValue> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x100f2770
// TEMPLATE: ROBLOX 0x006314c0
// RBX::Reflection::Described<RBX::Value<double,&RBX::sDoubleValue>,&RBX::sDoubleValue,RBX::FactoryProduct<RBX::Value<double,&RBX::sDoubleValue>,RBX::Instance,&RBX::sDoubleValue> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x100f29f0
// TEMPLATE: ROBLOX 0x00631740
// RBX::Reflection::Described<RBX::Value<G3D::Vector3,&RBX::sVector3Value>,&RBX::sVector3Value,RBX::FactoryProduct<RBX::Value<G3D::Vector3,&RBX::sVector3Value>,RBX::Instance,&RBX::sVector3Value> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x100f2b30
// TEMPLATE: ROBLOX 0x00631880
// RBX::Reflection::Described<RBX::Value<G3D::CoordinateFrame,&RBX::sCFrameValue>,&RBX::sCFrameValue,RBX::FactoryProduct<RBX::Value<G3D::CoordinateFrame,&RBX::sCFrameValue>,RBX::Instance,&RBX::sCFrameValue> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x100f2c70
// TEMPLATE: ROBLOX 0x006319c0
// RBX::Reflection::Described<RBX::Value<G3D::Color3,&RBX::sColor3Value>,&RBX::sColor3Value,RBX::FactoryProduct<RBX::Value<G3D::Color3,&RBX::sColor3Value>,RBX::Instance,&RBX::sColor3Value> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x100f2db0
// TEMPLATE: ROBLOX 0x00631b00
// RBX::Reflection::Described<RBX::Value<RBX::BrickColor,&RBX::sBrickColorValue>,&RBX::sBrickColorValue,RBX::FactoryProduct<RBX::Value<RBX::BrickColor,&RBX::sBrickColorValue>,RBX::Instance,&RBX::sBrickColorValue> >::classDescriptor
// FUNCTION: WEBSERVICE 0x100fe360
// FUNCTION: ROBLOX 0x005e4d80
// RBX::Reflection::Described<RBX::SpawnLocation,&RBX::sSpawnLocation,RBX::FactoryProduct<RBX::SpawnLocation,RBX::PartInstance,&RBX::sSpawnLocation> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x100ffb40
// TEMPLATE: ROBLOX 0x005ac6a0
// RBX::Reflection::Described<RBX::FaceInstance,&RBX::sFaceInstance,RBX::Instance>::classDescriptor
// TEMPLATE: WEBSERVICE 0x100ffbb0
// TEMPLATE: ROBLOX 0x005ac710
// RBX::Reflection::Described<RBX::Decal,&RBX::sDecal,RBX::FactoryProduct<RBX::Decal,RBX::FaceInstance,&RBX::sDecal> >::classDescriptor
// FUNCTION: WEBSERVICE 0x100ffc20
// FUNCTION: ROBLOX 0x005ac780
// RBX::Reflection::Described<RBX::Texture,&RBX::sTexture,RBX::FactoryProduct<RBX::Texture,RBX::Decal,&RBX::sTexture> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x1012d2d0
// TEMPLATE: ROBLOX 0x005b3b90
// RBX::Reflection::Described<RBX::SpecialShape,&RBX::sSpecialShape,RBX::FactoryProduct<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x1018c0f0
// TEMPLATE: ROBLOX 0x004b8610
// RBX::Reflection::Described<RBX::Network::Client,&RBX::Network::sClient,RBX::FactoryProduct<RBX::Network::Client,RBX::Network::Peer,&RBX::Network::sClient> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x1018c170
// TEMPLATE: ROBLOX 0x004ab470
// RBX::Reflection::Described<RBX::Network::Player,&RBX::Network::sPlayer,RBX::FactoryProduct<RBX::Network::Player,RBX::Instance,&RBX::Network::sPlayer> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x1018c1f0
// TEMPLATE: ROBLOX 0x004b8690
// RBX::Reflection::Described<RBX::Network::Server,&RBX::Network::sServer,RBX::FactoryProduct<RBX::Network::Server,RBX::Network::Peer,&RBX::Network::sServer> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x1018c270
// TEMPLATE: ROBLOX 0x004b3af0
// RBX::Reflection::Described<RBX::Network::Players,&RBX::Network::sPlayers,RBX::NonFactoryProduct<RBX::Instance,&RBX::Network::sPlayers> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x1018c2f0
// TEMPLATE: ROBLOX 0x004b8790
// RBX::Reflection::Described<RBX::NetworkSettings,&RBX::sNetworkSettings,RBX::FactoryProduct<RBX::NetworkSettings,RBX::GlobalSettings::Item,&RBX::sNetworkSettings> >::classDescriptor
// TEMPLATE: WEBSERVICE 0x101a3140
// TEMPLATE: ROBLOX 0x004c9a70
// RBX::Reflection::Described<RBX::Network::Marker,&RBX::Network::sMarker,RBX::NonFactoryProduct<RBX::Reflection::DescribedBase,&RBX::Network::sMarker> >::classDescriptor
// SYNTHETIC: WEBSERVICE 0x10221000
// `RBX::Reflection::Described<ThumbnailGenerator,&sThumbnailGenerator,RBX::FactoryProduct<ThumbnailGenerator,RBX::Instance,&sThumbnailGenerator> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10221010
// SYNTHETIC: ROBLOX 0x007ef5b0
// `RBX::Reflection::Described<RBX::Instance,&RBX::sInstance,RBX::Reflection::DescribedBase>::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x102215d0
// SYNTHETIC: ROBLOX 0x007f3a00
// `RBX::Reflection::Described<RBX::Lighting,&RBX::sLighting,RBX::FactoryProduct<RBX::Lighting,RBX::Instance,&RBX::sLighting> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10221810
// SYNTHETIC: ROBLOX 0x007f1ce0
// `RBX::Reflection::Described<RBX::RunService,&RBX::sRunService,RBX::FactoryProduct<RBX::RunService,RBX::Instance,&RBX::sRunService> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10221950
// SYNTHETIC: ROBLOX 0x007f1c50
// `RBX::Reflection::Described<RBX::ModelInstance,&RBX::sModel,RBX::FactoryProduct<RBX::ModelInstance,RBX::PVInstance,&RBX::sModel> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10221960
// SYNTHETIC: ROBLOX 0x007f1c60
// `RBX::Reflection::Described<RBX::PVInstance,&RBX::sPVInstance,RBX::Instance>::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10221a30
// SYNTHETIC: ROBLOX 0x007f1cb0
// `RBX::Reflection::Described<RBX::Selection,&RBX::sSelection,RBX::FactoryProduct<RBX::Selection,RBX::Instance,&RBX::sSelection> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10221a90
// `RBX::Reflection::Described<RBX::Stats::Item,&RBX::Stats::sStatsItem,RBX::NonFactoryProduct<RBX::Instance,&RBX::Stats::sStatsItem> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10221aa0
// SYNTHETIC: ROBLOX 0x007f1d60
// `RBX::Reflection::Described<RBX::ScriptContext,&RBX::sScriptContext,RBX::FactoryProduct<RBX::ScriptContext,RBX::Instance,&RBX::sScriptContext> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10221ab0
// `RBX::Reflection::Described<RBX::Stats::StatsService,&RBX::Stats::sStats,RBX::NonFactoryProduct<RBX::Instance,&RBX::Stats::sStats> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10221ba0
// SYNTHETIC: ROBLOX 0x007f1e10
// `RBX::Reflection::Described<RBX::LocalScript,&RBX::sLocalScript,RBX::FactoryProduct<RBX::LocalScript,RBX::Script,&RBX::sLocalScript> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10221bb0
// SYNTHETIC: ROBLOX 0x007f1e20
// `RBX::Reflection::Described<RBX::Script,&RBX::sScript,RBX::FactoryProduct<RBX::Script,RBX::Instance,&RBX::sScript> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10221c70
// SYNTHETIC: ROBLOX 0x007f2cd0
// `RBX::Reflection::Described<RBX::Workspace,&RBX::sWorkspace,RBX::NonFactoryProduct<RBX::RootInstance,&RBX::sWorkspace> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10221c80
// SYNTHETIC: ROBLOX 0x007f2660
// `RBX::Reflection::Described<RBX::RootInstance,&RBX::sRootInstance,RBX::ModelInstance>::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10221eb0
// SYNTHETIC: ROBLOX 0x007f2410
// `RBX::Reflection::Described<RBX::DataModel,&RBX::sDataModel,RBX::NonFactoryProduct<RBX::ServiceProvider,&RBX::sDataModel> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10222420
// SYNTHETIC: ROBLOX 0x007f3050
// `RBX::Reflection::Described<StockSound,&sStockSound,RBX::FactoryProduct<StockSound,RBX::Soundscape::SoundChannel,&sStockSound> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10222430
// SYNTHETIC: ROBLOX 0x007f3060
// `RBX::Reflection::Described<RBX::Soundscape::SoundChannel,&RBX::Soundscape::sSoundChannel,RBX::FactoryProduct<RBX::Soundscape::SoundChannel,RBX::Instance,&RBX::Soundscape::sSoundChannel> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10222440
// SYNTHETIC: ROBLOX 0x007f3070
// `RBX::Reflection::Described<RBX::Soundscape::SoundService,&RBX::Soundscape::sSoundService,RBX::FactoryProduct<RBX::Soundscape::SoundService,RBX::Instance,&RBX::Soundscape::sSoundService> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10222520
// SYNTHETIC: ROBLOX 0x007f31c0
// `RBX::Reflection::Described<RBX::Humanoid,&RBX::sHumanoid,RBX::FactoryProduct<RBX::Humanoid,RBX::Instance,&RBX::sHumanoid> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10222530
// SYNTHETIC: ROBLOX 0x007f31d0
// `RBX::Reflection::Described<RBX::SpawnerService,&RBX::sSpawnerService,RBX::FactoryProduct<RBX::SpawnerService,RBX::Instance,&RBX::sSpawnerService> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10222540
// SYNTHETIC: ROBLOX 0x007f31e0
// `RBX::Reflection::Described<RBX::Seat,&RBX::sSeat,RBX::FactoryProduct<RBX::Seat,RBX::PartInstance,&RBX::sSeat> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10222550
// SYNTHETIC: ROBLOX 0x007f31f0
// `RBX::Reflection::Described<RBX::TimerService,&RBX::sTimerService,RBX::FactoryProduct<RBX::TimerService,RBX::Instance,&RBX::sTimerService> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10222560
// SYNTHETIC: ROBLOX 0x007f3200
// `RBX::Reflection::Described<RBX::DebrisService,&RBX::sDebrisService,RBX::FactoryProduct<RBX::DebrisService,RBX::Instance,&RBX::sDebrisService> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10222570
// SYNTHETIC: ROBLOX 0x007f3210
// `RBX::Reflection::Described<RBX::ControllerService,&RBX::sControllerService,RBX::FactoryProduct<RBX::ControllerService,RBX::Instance,&RBX::sControllerService> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10222580
// SYNTHETIC: ROBLOX 0x007f3220
// `RBX::Reflection::Described<RBX::Visit,&RBX::sVisit,RBX::FactoryProduct<RBX::Visit,RBX::Instance,&RBX::sVisit> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10222590
// SYNTHETIC: ROBLOX 0x007f3230
// `RBX::Reflection::Described<RBX::Hint,&RBX::sHint,RBX::FactoryProduct<RBX::Hint,RBX::Message,&RBX::sHint> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x102225a0
// SYNTHETIC: ROBLOX 0x007f3240
// `RBX::Reflection::Described<RBX::Message,&RBX::sMessage,RBX::FactoryProduct<RBX::Message,RBX::Instance,&RBX::sMessage> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x102225b0
// SYNTHETIC: ROBLOX 0x007f3250
// `RBX::Reflection::Described<RBX::LocalBackpack,&RBX::sLocalBackpack,RBX::FactoryProduct<RBX::LocalBackpack,RBX::RelativePanel,&RBX::sLocalBackpack> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x102225c0
// SYNTHETIC: ROBLOX 0x007f3260
// `RBX::Reflection::Described<RBX::LocalBackpackItem,&RBX::sLocalBackpackItem,RBX::FactoryProduct<RBX::LocalBackpackItem,RBX::Widget,&RBX::sLocalBackpackItem> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x102225d0
// SYNTHETIC: ROBLOX 0x007f3270
// `RBX::Reflection::Described<RBX::GeometryService,&RBX::sGeometryService,RBX::FactoryProduct<RBX::GeometryService,RBX::Instance,&RBX::sGeometryService> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x102225e0
// SYNTHETIC: ROBLOX 0x007f31b0
// `RBX::Reflection::Described<RBX::GameSettings,&RBX::sGameSettings,RBX::FactoryProduct<RBX::GameSettings,RBX::GlobalSettings::Item,&RBX::sGameSettings> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x102225f0
// SYNTHETIC: ROBLOX 0x007f3280
// `RBX::Reflection::Described<RBX::ForceField,&RBX::sForceField,RBX::FactoryProduct<RBX::ForceField,RBX::Instance,&RBX::sForceField> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10222600
// SYNTHETIC: ROBLOX 0x007f3290
// `RBX::Reflection::Described<RBX::FlagStandService,&RBX::sFlagStandService,RBX::FactoryProduct<RBX::FlagStandService,RBX::Instance,&RBX::sFlagStandService> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10222610
// SYNTHETIC: ROBLOX 0x007f32a0
// `RBX::Reflection::Described<RBX::FlagStand,&RBX::sFlagStand,RBX::FactoryProduct<RBX::FlagStand,RBX::PartInstance,&RBX::sFlagStand> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10222620
// SYNTHETIC: ROBLOX 0x007f32b0
// `RBX::Reflection::Described<RBX::Flag,&RBX::sFlag,RBX::FactoryProduct<RBX::Flag,RBX::Tool,&RBX::sFlag> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10222630
// SYNTHETIC: ROBLOX 0x007f32c0
// `RBX::Reflection::Described<RBX::Hole,&RBX::sHole,RBX::FactoryProduct<RBX::Hole,RBX::Feature,&RBX::sHole> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10222640
// SYNTHETIC: ROBLOX 0x007f32d0
// `RBX::Reflection::Described<RBX::MotorFeature,&RBX::sMotorFeature,RBX::FactoryProduct<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10222650
// SYNTHETIC: ROBLOX 0x007f32e0
// `RBX::Reflection::Described<RBX::Motor,&RBX::sMotor,RBX::FactoryProduct<RBX::Motor,RBX::AutoJoint,&RBX::sMotor> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10222660
// SYNTHETIC: ROBLOX 0x007f32f0
// `RBX::Reflection::Described<RBX::RotateV,&RBX::sRotateV,RBX::FactoryProduct<RBX::RotateV,RBX::AutoJoint,&RBX::sRotateV> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10222670
// SYNTHETIC: ROBLOX 0x007f3300
// `RBX::Reflection::Described<RBX::RotateP,&RBX::sRotateP,RBX::FactoryProduct<RBX::RotateP,RBX::AutoJoint,&RBX::sRotateP> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10222680
// SYNTHETIC: ROBLOX 0x007f3310
// `RBX::Reflection::Described<RBX::Rotate,&RBX::sRotate,RBX::FactoryProduct<RBX::Rotate,RBX::AutoJoint,&RBX::sRotate> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10222690
// SYNTHETIC: ROBLOX 0x007f3320
// `RBX::Reflection::Described<RBX::Glue,&RBX::sGlue,RBX::FactoryProduct<RBX::Glue,RBX::AutoJoint,&RBX::sGlue> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x102226a0
// SYNTHETIC: ROBLOX 0x007f3330
// `RBX::Reflection::Described<RBX::Weld,&RBX::sWeld,RBX::FactoryProduct<RBX::Weld,RBX::AutoJoint,&RBX::sWeld> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x102226b0
// SYNTHETIC: ROBLOX 0x007f3340
// `RBX::Reflection::Described<RBX::Snap,&RBX::sSnap,RBX::FactoryProduct<RBX::Snap,RBX::AutoJoint,&RBX::sSnap> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x102226c0
// SYNTHETIC: ROBLOX 0x007f3350
// `RBX::Reflection::Described<RBX::Explosion,&RBX::sExplosion,RBX::FactoryProduct<RBX::Explosion,RBX::Instance,&RBX::sExplosion> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x102226d0
// SYNTHETIC: ROBLOX 0x007f3360
// `RBX::Reflection::Described<RBX::BodyColors,&RBX::sBodyColors,RBX::FactoryProduct<RBX::BodyColors,RBX::CharacterAppearance,&RBX::sBodyColors> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x102226e0
// SYNTHETIC: ROBLOX 0x007f3370
// `RBX::Reflection::Described<RBX::ShirtGraphic,&RBX::sShirtGraphic,RBX::FactoryProduct<RBX::ShirtGraphic,RBX::CharacterAppearance,&RBX::sShirtGraphic> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10222700
// SYNTHETIC: ROBLOX 0x007f3380
// `RBX::Reflection::Described<RBX::Backpack,&RBX::sBackpack,RBX::FactoryProduct<RBX::Backpack,RBX::Hopper,&RBX::sBackpack> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10222710
// SYNTHETIC: ROBLOX 0x007f3390
// `RBX::Reflection::Described<RBX::LegacyHopperService,&RBX::sLegacyHopperService,RBX::FactoryProduct<RBX::LegacyHopperService,RBX::Hopper,&RBX::sLegacyHopperService> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10222720
// SYNTHETIC: ROBLOX 0x007f33a0
// `RBX::Reflection::Described<RBX::StarterPackService,&RBX::sStarterPackService,RBX::FactoryProduct<RBX::StarterPackService,RBX::Hopper,&RBX::sStarterPackService> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10222730
// SYNTHETIC: ROBLOX 0x007f2e30
// `RBX::Reflection::Described<RBX::Hat,&RBX::sHat,RBX::FactoryProduct<RBX::Hat,RBX::Accoutrement,&RBX::sHat> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10222740
// SYNTHETIC: ROBLOX 0x007f2e40
// `RBX::Reflection::Described<RBX::Accoutrement,&RBX::sAccoutrement,RBX::FactoryProduct<RBX::Accoutrement,RBX::Instance,&RBX::sAccoutrement> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10222750
// SYNTHETIC: ROBLOX 0x007f2b70
// `RBX::Reflection::Described<RBX::PartInstance,&RBX::sPart,RBX::FactoryProduct<RBX::PartInstance,RBX::PVInstance,&RBX::sPart> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10222760
// SYNTHETIC: ROBLOX 0x007f33b0
// `RBX::Reflection::Described<RBX::Tool,&RBX::sTool,RBX::FactoryProduct<RBX::Tool,RBX::BackpackItem,&RBX::sTool> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10222770
// SYNTHETIC: ROBLOX 0x007f33c0
// `RBX::Reflection::Described<RBX::Feature,&RBX::sFeature,RBX::NonFactoryProduct<RBX::Instance,&RBX::sFeature> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10222780
// SYNTHETIC: ROBLOX 0x007f33d0
// `RBX::Reflection::Described<RBX::AutoJoint,&RBX::sAutoJoint,RBX::NonFactoryProduct<RBX::JointInstance,&RBX::sAutoJoint> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10222790
// SYNTHETIC: ROBLOX 0x007f33e0
// `RBX::Reflection::Described<RBX::ObjectValue,&RBX::sObjectValue,RBX::FactoryProduct<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x102227a0
// SYNTHETIC: ROBLOX 0x007f33f0
// `RBX::Reflection::Described<RBX::JointInstance,&RBX::sJointInstance,RBX::NonFactoryProduct<RBX::Instance,&RBX::sJointInstance> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x102227b0
// SYNTHETIC: ROBLOX 0x007f3400
// `RBX::Reflection::Described<RBX::BackpackItem,&RBX::sBackpackItem,RBX::NonFactoryProduct<RBX::Widget,&RBX::sBackpackItem> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10222fe0
// SYNTHETIC: ROBLOX 0x007f3b70
// `RBX::Reflection::Described<RBX::Sky,&RBX::sSky,RBX::FactoryProduct<RBX::Sky,RBX::Instance,&RBX::sSky> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10223960
// SYNTHETIC: ROBLOX 0x007f2250
// `RBX::Reflection::Described<RBX::DebugSettings,&RBX::sDebugSettings,RBX::FactoryProduct<RBX::DebugSettings,RBX::GlobalSettings::Item,&RBX::sDebugSettings> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10223aa0
// SYNTHETIC: ROBLOX 0x007f23d0
// `RBX::Reflection::Described<RBX::Team,&RBX::sTeam,RBX::FactoryProduct<RBX::Team,RBX::Instance,&RBX::sTeam> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10223b30
// SYNTHETIC: ROBLOX 0x007f3920
// `RBX::Reflection::Described<RBX::Camera,&RBX::sCamera,RBX::FactoryProduct<RBX::Camera,RBX::Instance,&RBX::sCamera> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10223bd0
// SYNTHETIC: ROBLOX 0x007f3fc0
// `RBX::Reflection::Described<RBX::HopperBin,&RBX::sHopperBin,RBX::FactoryProduct<RBX::HopperBin,RBX::BackpackItem,&RBX::sHopperBin> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10223d10
// SYNTHETIC: ROBLOX 0x007f46c0
// `RBX::Reflection::Described<RBX::Mouse,&RBX::sMouse,RBX::NonFactoryProduct<RBX::Instance,&RBX::sMouse> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10223e70
// SYNTHETIC: ROBLOX 0x007f4210
// `RBX::Reflection::Described<RBX::Teams,&RBX::sTeams,RBX::FactoryProduct<RBX::Teams,RBX::Instance,&RBX::sTeams> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10224160
// SYNTHETIC: ROBLOX 0x007f41f0
// `RBX::Reflection::Described<RBX::Skin,&RBX::sSkin,RBX::FactoryProduct<RBX::Skin,RBX::CharacterAppearance,&RBX::sSkin> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10224170
// SYNTHETIC: ROBLOX 0x007f4200
// `RBX::Reflection::Described<RBX::Shirt,&RBX::sShirt,RBX::FactoryProduct<RBX::Shirt,RBX::ShirtGraphic,&RBX::sShirt> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10224300
// SYNTHETIC: ROBLOX 0x007f4820
// `RBX::Reflection::Described<RBX::VelocityMotor,&RBX::sVelocityMotor,RBX::FactoryProduct<RBX::VelocityMotor,RBX::JointInstance,&RBX::sVelocityMotor> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x102246a0
// SYNTHETIC: ROBLOX 0x007f4c50
// `RBX::Reflection::Described<RBX::Rocket,&RBX::sRocket,RBX::FactoryProduct<RBX::Rocket,RBX::BodyMover,&RBX::sRocket> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x102246b0
// SYNTHETIC: ROBLOX 0x007f4c60
// `RBX::Reflection::Described<RBX::BodyVelocity,&RBX::sBodyVelocity,RBX::FactoryProduct<RBX::BodyVelocity,RBX::BodyMover,&RBX::sBodyVelocity> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x102246c0
// STUB: ROBLOX 0x007f4c70
// `RBX::Reflection::Described<RBX::BodyPosition,&RBX::sBodyPosition,RBX::FactoryProduct<RBX::BodyPosition,RBX::BodyMover,&RBX::sBodyPosition> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x102246d0
// SYNTHETIC: ROBLOX 0x007f4c80
// `RBX::Reflection::Described<RBX::BodyThrust,&RBX::sBodyThrust,RBX::FactoryProduct<RBX::BodyThrust,RBX::BodyMover,&RBX::sBodyThrust> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x102246e0
// SYNTHETIC: ROBLOX 0x007f4c90
// `RBX::Reflection::Described<RBX::BodyForce,&RBX::sBodyForce,RBX::FactoryProduct<RBX::BodyForce,RBX::BodyMover,&RBX::sBodyForce> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x102246f0
// STUB: ROBLOX 0x007f4ca0
// `RBX::Reflection::Described<RBX::BodyGyro,&RBX::sBodyGyro,RBX::FactoryProduct<RBX::BodyGyro,RBX::BodyMover,&RBX::sBodyGyro> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10224cc0
// SYNTHETIC: ROBLOX 0x007f4080
// `RBX::Reflection::Described<RBX::SpawnLocation,&RBX::sSpawnLocation,RBX::FactoryProduct<RBX::SpawnLocation,RBX::PartInstance,&RBX::sSpawnLocation> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10224d70
// SYNTHETIC: ROBLOX 0x007f2880
// `RBX::Reflection::Described<RBX::Texture,&RBX::sTexture,RBX::FactoryProduct<RBX::Texture,RBX::Decal,&RBX::sTexture> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10224d80
// SYNTHETIC: ROBLOX 0x007f2890
// `RBX::Reflection::Described<RBX::Decal,&RBX::sDecal,RBX::FactoryProduct<RBX::Decal,RBX::FaceInstance,&RBX::sDecal> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10224d90
// SYNTHETIC: ROBLOX 0x007f28a0
// `RBX::Reflection::Described<RBX::FaceInstance,&RBX::sFaceInstance,RBX::Instance>::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x102256d0
// SYNTHETIC: ROBLOX 0x007f2c80
// `RBX::Reflection::Described<RBX::SpecialShape,&RBX::sSpecialShape,RBX::FactoryProduct<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10225d20
// SYNTHETIC: ROBLOX 0x007f0e20
// `RBX::Reflection::Described<RBX::NetworkSettings,&RBX::sNetworkSettings,RBX::FactoryProduct<RBX::NetworkSettings,RBX::GlobalSettings::Item,&RBX::sNetworkSettings> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10225d30
// SYNTHETIC: ROBLOX 0x007f0cd0
// `RBX::Reflection::Described<RBX::Network::Players,&RBX::Network::sPlayers,RBX::NonFactoryProduct<RBX::Instance,&RBX::Network::sPlayers> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10225d40
// SYNTHETIC: ROBLOX 0x007f0e40
// `RBX::Reflection::Described<RBX::Network::Server,&RBX::Network::sServer,RBX::FactoryProduct<RBX::Network::Server,RBX::Network::Peer,&RBX::Network::sServer> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10225d50
// SYNTHETIC: ROBLOX 0x007f08b0
// `RBX::Reflection::Described<RBX::Network::Player,&RBX::Network::sPlayer,RBX::FactoryProduct<RBX::Network::Player,RBX::Instance,&RBX::Network::sPlayer> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10225d60
// SYNTHETIC: ROBLOX 0x007f0e50
// `RBX::Reflection::Described<RBX::Network::Client,&RBX::Network::sClient,RBX::FactoryProduct<RBX::Network::Client,RBX::Network::Peer,&RBX::Network::sClient> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x102262b0
// SYNTHETIC: ROBLOX 0x007f10c0
// `RBX::Reflection::Described<RBX::Network::Marker,&RBX::Network::sMarker,RBX::NonFactoryProduct<RBX::Reflection::DescribedBase,&RBX::Network::sMarker> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// clang-format on

template <class T, const char* sName, class Base>
class __declspec(novtable) Described : public Base
{
public:
	// clang-format off
	// SYNTHETIC: WEBSERVICE 0x10047b10
	// RBX::Reflection::Described<RBX::Instance,&RBX::sInstance,RBX::Reflection::DescribedBase>::~Described<RBX::Instance,&RBX::sInstance,RBX::Reflection::DescribedBase>
	// TEMPLATE: WEBSERVICE 0x1004e950
	// TEMPLATE: ROBLOX 0x0057a670
	// RBX::Reflection::Described<RBX::Instance,&RBX::sInstance,RBX::Reflection::DescribedBase>::Described<RBX::Instance,&RBX::sInstance,RBX::Reflection::DescribedBase>
	// TEMPLATE: WEBSERVICE 0x10197b40
	// TEMPLATE: ROBLOX 0x004b9de0
	// RBX::Reflection::Described<RBX::Network::Client,&RBX::Network::sClient,RBX::FactoryProduct<RBX::Network::Client,RBX::Network::Peer,&RBX::Network::sClient> >::Described<RBX::Network::Client,&RBX::Network::sClient,RBX::FactoryProduct<RBX::Network::Client,RBX::Network::Peer,&RBX::Network::sClient> >
	// TEMPLATE: WEBSERVICE 0x1019a7a0
	// TEMPLATE: ROBLOX 0x004bc990
	// RBX::Reflection::Described<RBX::Network::Server,&RBX::Network::sServer,RBX::FactoryProduct<RBX::Network::Server,RBX::Network::Peer,&RBX::Network::sServer> >::Described<RBX::Network::Server,&RBX::Network::sServer,RBX::FactoryProduct<RBX::Network::Server,RBX::Network::Peer,&RBX::Network::sServer> >
	// TEMPLATE: WEBSERVICE 0x1019b860
	// TEMPLATE: ROBLOX 0x004c20b0
	// RBX::Reflection::Described<RBX::NetworkSettings,&RBX::sNetworkSettings,RBX::FactoryProduct<RBX::NetworkSettings,RBX::GlobalSettings::Item,&RBX::sNetworkSettings> >::Described<RBX::NetworkSettings,&RBX::sNetworkSettings,RBX::FactoryProduct<RBX::NetworkSettings,RBX::GlobalSettings::Item,&RBX::sNetworkSettings> >
	// TEMPLATE: WEBSERVICE 0x101a96a0
	// TEMPLATE: ROBLOX 0x004d27d0
	// RBX::Reflection::Described<RBX::Network::Marker,&RBX::Network::sMarker,RBX::NonFactoryProduct<RBX::Reflection::DescribedBase,&RBX::Network::sMarker> >::Described<RBX::Network::Marker,&RBX::Network::sMarker,RBX::NonFactoryProduct<RBX::Reflection::DescribedBase,&RBX::Network::sMarker> >
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
	// TEMPLATE: WEBSERVICE 0x100823f0
	// TEMPLATE: ROBLOX 0x005c5ea0
	// RBX::Reflection::EnumPropDescriptor<RBX::Soundscape::SoundService,enum RBX::Soundscape::ReverbType>::setEnumValue
	// TEMPLATE: WEBSERVICE 0x10082430
	// TEMPLATE: ROBLOX 0x005c5ee0
	// RBX::Reflection::EnumPropDescriptor<RBX::Soundscape::SoundService,enum RBX::Soundscape::ReverbType>::getIndexValue
	// TEMPLATE: WEBSERVICE 0x1009a0e0
	// TEMPLATE: ROBLOX 0x005f83b0
	// RBX::Reflection::EnumPropDescriptor<RBX::PVInstance,enum RBX::Controller::ControllerType>::setEnumValue
	// TEMPLATE: WEBSERVICE 0x1009a120
	// TEMPLATE: ROBLOX 0x005f83f0
	// RBX::Reflection::EnumPropDescriptor<RBX::PVInstance,enum RBX::Controller::ControllerType>::getIndexValue
	// TEMPLATE: WEBSERVICE 0x1009e720
	// TEMPLATE: ROBLOX 0x005b1190
	// RBX::Reflection::EnumPropDescriptor<RBX::PartInstance,enum RBX::PartInstance::FormFactor>::setEnumValue
	// TEMPLATE: WEBSERVICE 0x1009e760
	// TEMPLATE: ROBLOX 0x005b11d0
	// RBX::Reflection::EnumPropDescriptor<RBX::PartInstance,enum RBX::PartInstance::FormFactor>::getIndexValue
	// TEMPLATE: WEBSERVICE 0x1009ef50
	// TEMPLATE: ROBLOX 0x005b19e0
	// RBX::Reflection::EnumPropDescriptor<RBX::PartInstance,enum RBX::Part::PartType>::setEnumValue
	// TEMPLATE: WEBSERVICE 0x1009ef90
	// TEMPLATE: ROBLOX 0x005b1a20
	// RBX::Reflection::EnumPropDescriptor<RBX::PartInstance,enum RBX::Part::PartType>::getIndexValue
	// TEMPLATE: WEBSERVICE 0x100b4980
	// TEMPLATE: ROBLOX 0x0057c920
	// RBX::Reflection::EnumPropDescriptor<RBX::DebugSettings,enum RBX::Debugable::AssertAction>::setEnumValue
	// TEMPLATE: WEBSERVICE 0x100b49c0
	// TEMPLATE: ROBLOX 0x0057c960
	// RBX::Reflection::EnumPropDescriptor<RBX::DebugSettings,enum RBX::Debugable::AssertAction>::getIndexValue
	// TEMPLATE: WEBSERVICE 0x100b4c50
	// TEMPLATE: ROBLOX 0x0057cbf0
	// RBX::Reflection::EnumPropDescriptor<RBX::DebugSettings,enum RBX::DebugSettings::ErrorReporting>::setEnumValue
	// TEMPLATE: WEBSERVICE 0x100b4c90
	// TEMPLATE: ROBLOX 0x0057cc30
	// RBX::Reflection::EnumPropDescriptor<RBX::DebugSettings,enum RBX::DebugSettings::ErrorReporting>::getIndexValue
	// TEMPLATE: WEBSERVICE 0x100c3ad0
	// TEMPLATE: ROBLOX 0x005d7670
	// RBX::Reflection::EnumPropDescriptor<RBX::Camera,enum RBX::Camera::CameraType>::EnumPropDescriptor<RBX::Camera,enum RBX::Camera::CameraType><enum RBX::Camera::CameraType (__thiscall RBX::Camera::*)(void)const ,void (__thiscall RBX::Camera::*)(enum RBX::Camera::CameraType)>
	// TEMPLATE: WEBSERVICE 0x100c3d60
	// TEMPLATE: ROBLOX 0x005d7900
	// RBX::Reflection::EnumPropDescriptor<RBX::Camera,enum RBX::Camera::CameraType>::setEnumValue
	// TEMPLATE: WEBSERVICE 0x100c3da0
	// TEMPLATE: ROBLOX 0x005d7940
	// RBX::Reflection::EnumPropDescriptor<RBX::Camera,enum RBX::Camera::CameraType>::getIndexValue
	// TEMPLATE: WEBSERVICE 0x100c8830
	// TEMPLATE: ROBLOX 0x005e2900
	// RBX::Reflection::EnumPropDescriptor<RBX::HopperBin,enum RBX::HopperBin::BinType>::EnumPropDescriptor<RBX::HopperBin,enum RBX::HopperBin::BinType><enum RBX::HopperBin::BinType (__thiscall RBX::HopperBin::*)(void)const ,void (__thiscall RBX::HopperBin::*)(enum RBX::HopperBin::BinType)>
	// TEMPLATE: WEBSERVICE 0x100c88d0
	// TEMPLATE: ROBLOX 0x005e29a0
	// RBX::Reflection::EnumPropDescriptor<RBX::HopperBin,enum RBX::HopperBin::BinType>::setEnumValue
	// TEMPLATE: WEBSERVICE 0x100c8910
	// TEMPLATE: ROBLOX 0x005e29e0
	// RBX::Reflection::EnumPropDescriptor<RBX::HopperBin,enum RBX::HopperBin::BinType>::getIndexValue
	// TEMPLATE: WEBSERVICE 0x100e61d0
	// TEMPLATE: ROBLOX 0x006190c0
	// RBX::Reflection::EnumPropDescriptor<RBX::Feature,enum RBX::Feature::TopBottom>::setEnumValue
	// TEMPLATE: WEBSERVICE 0x100e6210
	// TEMPLATE: ROBLOX 0x00619100
	// RBX::Reflection::EnumPropDescriptor<RBX::Feature,enum RBX::Feature::TopBottom>::getIndexValue
	// TEMPLATE: WEBSERVICE 0x100e64a0
	// TEMPLATE: ROBLOX 0x00619390
	// RBX::Reflection::EnumPropDescriptor<RBX::Feature,enum RBX::Feature::LeftRight>::setEnumValue
	// TEMPLATE: WEBSERVICE 0x100e64e0
	// TEMPLATE: ROBLOX 0x006193d0
	// RBX::Reflection::EnumPropDescriptor<RBX::Feature,enum RBX::Feature::LeftRight>::getIndexValue
	// TEMPLATE: WEBSERVICE 0x100e6770
	// TEMPLATE: ROBLOX 0x00619660
	// RBX::Reflection::EnumPropDescriptor<RBX::Feature,enum RBX::Feature::InOut>::setEnumValue
	// TEMPLATE: WEBSERVICE 0x100e67b0
	// TEMPLATE: ROBLOX 0x006196a0
	// RBX::Reflection::EnumPropDescriptor<RBX::Feature,enum RBX::Feature::InOut>::getIndexValue
	// TEMPLATE: WEBSERVICE 0x100e7260
	// TEMPLATE: ROBLOX 0x005e0020
	// RBX::Reflection::EnumPropDescriptor<RBX::FaceInstance,enum RBX::NormalId>::setEnumValue
	// TEMPLATE: WEBSERVICE 0x100e72a0
	// TEMPLATE: ROBLOX 0x005e0060
	// RBX::Reflection::EnumPropDescriptor<RBX::FaceInstance,enum RBX::NormalId>::getIndexValue
	// TEMPLATE: WEBSERVICE 0x1012d5c0
	// TEMPLATE: ROBLOX 0x005b3eb0
	// RBX::Reflection::EnumPropDescriptor<RBX::SpecialShape,enum RBX::SpecialShape::MeshType>::setEnumValue
	// TEMPLATE: WEBSERVICE 0x1012d600
	// TEMPLATE: ROBLOX 0x005b3ef0
	// RBX::Reflection::EnumPropDescriptor<RBX::SpecialShape,enum RBX::SpecialShape::MeshType>::getIndexValue
	// clang-format on

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x10081db0
	// STUB: ROBLOX 0x005c5900
	// RBX::Reflection::EnumPropDescriptor<RBX::Soundscape::SoundService,enum RBX::Soundscape::ReverbType>::EnumPropDescriptor<RBX::Soundscape::SoundService,enum RBX::Soundscape::ReverbType><enum RBX::Soundscape::ReverbType (__thiscall RBX::Soundscape::SoundService::*)(void)const ,void (__thiscall RBX::Soundscape::SoundService::*)(enum RBX::Soundscape::ReverbType const &)>
	// TEMPLATE: WEBSERVICE 0x10082450
	// TEMPLATE: ROBLOX 0x005c5f00
	// RBX::Reflection::EnumPropDescriptor<RBX::Soundscape::SoundService,enum RBX::Soundscape::ReverbType>::setIndexValue
	// TEMPLATE: WEBSERVICE 0x10082590
	// TEMPLATE: ROBLOX 0x005c6040
	// RBX::Reflection::EnumPropDescriptor<RBX::Soundscape::SoundService,enum RBX::Soundscape::ReverbType>::readValue
	// TEMPLATE: WEBSERVICE 0x10099de0
	// TEMPLATE: ROBLOX 0x005f7fc0
	// RBX::Reflection::EnumPropDescriptor<RBX::PVInstance,enum RBX::Controller::ControllerType>::EnumPropDescriptor<RBX::PVInstance,enum RBX::Controller::ControllerType><enum RBX::Controller::ControllerType (__thiscall RBX::PVInstance::*)(void)const ,void (__thiscall RBX::PVInstance::*)(enum RBX::Controller::ControllerType)>
	// TEMPLATE: WEBSERVICE 0x1009a140
	// TEMPLATE: ROBLOX 0x005f8410
	// RBX::Reflection::EnumPropDescriptor<RBX::PVInstance,enum RBX::Controller::ControllerType>::setIndexValue
	// TEMPLATE: WEBSERVICE 0x1009a280
	// TEMPLATE: ROBLOX 0x005f8550
	// RBX::Reflection::EnumPropDescriptor<RBX::PVInstance,enum RBX::Controller::ControllerType>::readValue
	// TEMPLATE: WEBSERVICE 0x1009e780
	// TEMPLATE: ROBLOX 0x005b11f0
	// RBX::Reflection::EnumPropDescriptor<RBX::PartInstance,enum RBX::PartInstance::FormFactor>::setIndexValue
	// TEMPLATE: WEBSERVICE 0x1009a1a0
	// TEMPLATE: ROBLOX 0x005f8470
	// RBX::Reflection::EnumPropDescriptor<RBX::PVInstance,enum RBX::Controller::ControllerType>::getStringValue
	// TEMPLATE: WEBSERVICE 0x1009e7e0
	// TEMPLATE: ROBLOX 0x005b1250
	// RBX::Reflection::EnumPropDescriptor<RBX::PartInstance,enum RBX::PartInstance::FormFactor>::getStringValue
	// TEMPLATE: WEBSERVICE 0x1009f010
	// TEMPLATE: ROBLOX 0x005b1aa0
	// RBX::Reflection::EnumPropDescriptor<RBX::PartInstance,enum RBX::Part::PartType>::getStringValue
	// TEMPLATE: WEBSERVICE 0x100b4a40
	// TEMPLATE: ROBLOX 0x0057c9e0
	// RBX::Reflection::EnumPropDescriptor<RBX::DebugSettings,enum RBX::Debugable::AssertAction>::getStringValue
	// TEMPLATE: WEBSERVICE 0x100b4d10
	// TEMPLATE: ROBLOX 0x0057ccb0
	// RBX::Reflection::EnumPropDescriptor<RBX::DebugSettings,enum RBX::DebugSettings::ErrorReporting>::getStringValue
	// TEMPLATE: WEBSERVICE 0x100c3dc0
	// TEMPLATE: ROBLOX 0x005d7960
	// RBX::Reflection::EnumPropDescriptor<RBX::Camera,enum RBX::Camera::CameraType>::setIndexValue
	// TEMPLATE: WEBSERVICE 0x100c3e20
	// TEMPLATE: ROBLOX 0x005d79c0
	// RBX::Reflection::EnumPropDescriptor<RBX::Camera,enum RBX::Camera::CameraType>::getStringValue
	// TEMPLATE: WEBSERVICE 0x100c3f00
	// TEMPLATE: ROBLOX 0x005d7aa0
	// RBX::Reflection::EnumPropDescriptor<RBX::Camera,enum RBX::Camera::CameraType>::readValue
	// TEMPLATE: WEBSERVICE 0x100c8930
	// TEMPLATE: ROBLOX 0x005e2a00
	// RBX::Reflection::EnumPropDescriptor<RBX::HopperBin,enum RBX::HopperBin::BinType>::setIndexValue
	// TEMPLATE: WEBSERVICE 0x100c8990
	// TEMPLATE: ROBLOX 0x005e2a60
	// RBX::Reflection::EnumPropDescriptor<RBX::HopperBin,enum RBX::HopperBin::BinType>::getStringValue
	// TEMPLATE: WEBSERVICE 0x100c8a70
	// TEMPLATE: ROBLOX 0x005e2b40
	// RBX::Reflection::EnumPropDescriptor<RBX::HopperBin,enum RBX::HopperBin::BinType>::readValue
	// TEMPLATE: WEBSERVICE 0x100e6560
	// TEMPLATE: ROBLOX 0x00619450
	// RBX::Reflection::EnumPropDescriptor<RBX::Feature,enum RBX::Feature::LeftRight>::getStringValue
	// TEMPLATE: WEBSERVICE 0x100e6830
	// TEMPLATE: ROBLOX 0x00619720
	// RBX::Reflection::EnumPropDescriptor<RBX::Feature,enum RBX::Feature::InOut>::getStringValue
	// TEMPLATE: WEBSERVICE 0x1012d680
	// TEMPLATE: ROBLOX 0x005b3f70
	// RBX::Reflection::EnumPropDescriptor<RBX::SpecialShape,enum RBX::SpecialShape::MeshType>::getStringValue
	// TEMPLATE: WEBSERVICE 0x1009e8c0
	// TEMPLATE: ROBLOX 0x005b1330
	// RBX::Reflection::EnumPropDescriptor<RBX::PartInstance,enum RBX::PartInstance::FormFactor>::readValue
	// TEMPLATE: WEBSERVICE 0x1009efb0
	// TEMPLATE: ROBLOX 0x005b1a40
	// RBX::Reflection::EnumPropDescriptor<RBX::PartInstance,enum RBX::Part::PartType>::setIndexValue
	// TEMPLATE: WEBSERVICE 0x1009f0f0
	// TEMPLATE: ROBLOX 0x005b1b80
	// RBX::Reflection::EnumPropDescriptor<RBX::PartInstance,enum RBX::Part::PartType>::readValue
	// TEMPLATE: WEBSERVICE 0x100b46b0
	// TEMPLATE: ROBLOX 0x0057c6b0
	// RBX::Reflection::EnumPropDescriptor<RBX::DebugSettings,enum RBX::Debugable::AssertAction>::EnumPropDescriptor<RBX::DebugSettings,enum RBX::Debugable::AssertAction><enum RBX::Debugable::AssertAction (__thiscall RBX::DebugSettings::*)(void)const ,void (__thiscall RBX::DebugSettings::*)(enum RBX::Debugable::AssertAction)>
	// TEMPLATE: WEBSERVICE 0x100b48e0
	// STUB: ROBLOX 0x0057c750
	// RBX::Reflection::EnumPropDescriptor<RBX::DebugSettings,enum RBX::DebugSettings::ErrorReporting>::EnumPropDescriptor<RBX::DebugSettings,enum RBX::DebugSettings::ErrorReporting><enum RBX::DebugSettings::ErrorReporting (__thiscall RBX::DebugSettings::*)(void)const ,void (__thiscall RBX::DebugSettings::*)(enum RBX::DebugSettings::ErrorReporting)>
	// TEMPLATE: WEBSERVICE 0x100b49e0
	// TEMPLATE: ROBLOX 0x0057c980
	// RBX::Reflection::EnumPropDescriptor<RBX::DebugSettings,enum RBX::Debugable::AssertAction>::setIndexValue
	// TEMPLATE: WEBSERVICE 0x100b4b20
	// TEMPLATE: ROBLOX 0x0057cac0
	// RBX::Reflection::EnumPropDescriptor<RBX::DebugSettings,enum RBX::Debugable::AssertAction>::readValue
	// TEMPLATE: WEBSERVICE 0x100b4cb0
	// TEMPLATE: ROBLOX 0x0057cc50
	// RBX::Reflection::EnumPropDescriptor<RBX::DebugSettings,enum RBX::DebugSettings::ErrorReporting>::setIndexValue
	// TEMPLATE: WEBSERVICE 0x100b4df0
	// TEMPLATE: ROBLOX 0x0057cd90
	// RBX::Reflection::EnumPropDescriptor<RBX::DebugSettings,enum RBX::DebugSettings::ErrorReporting>::readValue
	// TEMPLATE: WEBSERVICE 0x100e6230
	// TEMPLATE: ROBLOX 0x00619120
	// RBX::Reflection::EnumPropDescriptor<RBX::Feature,enum RBX::Feature::TopBottom>::setIndexValue
	// TEMPLATE: WEBSERVICE 0x100e6290
	// TEMPLATE: ROBLOX 0x00619180
	// RBX::Reflection::EnumPropDescriptor<RBX::Feature,enum RBX::Feature::TopBottom>::getStringValue
	// TEMPLATE: WEBSERVICE 0x100e6370
	// TEMPLATE: ROBLOX 0x00619260
	// RBX::Reflection::EnumPropDescriptor<RBX::Feature,enum RBX::Feature::TopBottom>::readValue
	// TEMPLATE: WEBSERVICE 0x100e6500
	// TEMPLATE: ROBLOX 0x006193f0
	// RBX::Reflection::EnumPropDescriptor<RBX::Feature,enum RBX::Feature::LeftRight>::setIndexValue
	// TEMPLATE: WEBSERVICE 0x100e6640
	// TEMPLATE: ROBLOX 0x00619530
	// RBX::Reflection::EnumPropDescriptor<RBX::Feature,enum RBX::Feature::LeftRight>::readValue
	// TEMPLATE: WEBSERVICE 0x100e67d0
	// TEMPLATE: ROBLOX 0x006196c0
	// RBX::Reflection::EnumPropDescriptor<RBX::Feature,enum RBX::Feature::InOut>::setIndexValue
	// TEMPLATE: WEBSERVICE 0x100e6910
	// TEMPLATE: ROBLOX 0x00619800
	// RBX::Reflection::EnumPropDescriptor<RBX::Feature,enum RBX::Feature::InOut>::readValue
	// TEMPLATE: WEBSERVICE 0x10102130
	// TEMPLATE: ROBLOX 0x005b3ca0
	// RBX::Reflection::EnumPropDescriptor<RBX::HopperBin,enum RBX::HopperBin::BinType>::writeValue
	// TEMPLATE: WEBSERVICE 0x10121b30
	// TEMPLATE: ROBLOX 0x005e0080
	// RBX::Reflection::EnumPropDescriptor<RBX::FaceInstance,enum RBX::NormalId>::setIndexValue
	// TEMPLATE: WEBSERVICE 0x10121be0
	// TEMPLATE: ROBLOX 0x005e0130
	// RBX::Reflection::EnumPropDescriptor<RBX::FaceInstance,enum RBX::NormalId>::readValue
	// TEMPLATE: WEBSERVICE 0x1012d340
	// TEMPLATE: ROBLOX 0x005b3c00
	// RBX::Reflection::EnumPropDescriptor<RBX::SpecialShape,enum RBX::SpecialShape::MeshType>::EnumPropDescriptor<RBX::SpecialShape,enum RBX::SpecialShape::MeshType><enum RBX::SpecialShape::MeshType const (__thiscall RBX::SpecialShape::*)(void)const ,void (__thiscall RBX::SpecialShape::*)(enum RBX::SpecialShape::MeshType)>
	// TEMPLATE: WEBSERVICE 0x1012d620
	// TEMPLATE: ROBLOX 0x005b3f10
	// RBX::Reflection::EnumPropDescriptor<RBX::SpecialShape,enum RBX::SpecialShape::MeshType>::setIndexValue
	// TEMPLATE: WEBSERVICE 0x1012d760
	// TEMPLATE: ROBLOX 0x005b4050
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
		typename PropertyDescriptor::Functionality flags = PropertyDescriptor::STANDARD
	)
		: RefPropertyDescriptor(Class::classDescriptor(), RefType::singleton<T*>(), name, category, flags),
		  getset(PropDescriptor<Class, T*>::template getset<Getter, Setter>(get, set))
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
		element->setValue(InstanceHandle(getValue(instance)));
	}

	virtual void assignIDREF(DescribedBase* propertyOwner, const InstanceHandle& handle) const
	{
		boost::shared_ptr<Instance> t = handle.getTarget();

		setValue(propertyOwner, static_cast<T*>(t.get()));
	}

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x1004b8a0
	// TEMPLATE: ROBLOX 0x005787f0
	// RBX::Reflection::RefPropDescriptor<RBX::Instance,RBX::Instance>::RefPropDescriptor<RBX::Instance,RBX::Instance><RBX::Instance * (__thiscall RBX::Instance::*)(void)const ,void (__thiscall RBX::Instance::*)(RBX::Instance *)>
	// TEMPLATE: WEBSERVICE 0x1004ba50
	// TEMPLATE: ROBLOX 0x00632700
	// RBX::Reflection::RefPropDescriptor<RBX::Instance,RBX::Instance>::setRefValue
	// TEMPLATE: WEBSERVICE 0x1004ba90
	// TEMPLATE: ROBLOX 0x00656ff0
	// RBX::Reflection::RefPropDescriptor<RBX::Humanoid,RBX::PartInstance>::assignIDREF
	// TEMPLATE: WEBSERVICE 0x1005bd80
	// TEMPLATE: ROBLOX 0x00566630
	// RBX::Reflection::RefPropDescriptor<RBX::ModelInstance,RBX::PartInstance>::RefPropDescriptor<RBX::ModelInstance,RBX::PartInstance><RBX::PartInstance * (__thiscall RBX::ModelInstance::*)(void)const ,void (__thiscall RBX::ModelInstance::*)(RBX::PartInstance *)>
	// TEMPLATE: WEBSERVICE 0x1005be40
	// TEMPLATE: ROBLOX 0x005ebcc0
	// RBX::Reflection::RefPropDescriptor<RBX::Humanoid,RBX::PartInstance>::readValue
	// TEMPLATE: WEBSERVICE 0x1006f590
	// TEMPLATE: ROBLOX 0x005b7860
	// RBX::Reflection::RefPropDescriptor<RBX::Workspace,RBX::Camera>::RefPropDescriptor<RBX::Workspace,RBX::Camera><RBX::Camera * (__thiscall RBX::Workspace::*)(void)const ,void (__thiscall RBX::Workspace::*)(RBX::Camera *)>
	// TEMPLATE: WEBSERVICE 0x1006f6a0
	// TEMPLATE: ROBLOX 0x005b7970
	// RBX::Reflection::RefPropDescriptor<RBX::Workspace,RBX::Camera>::setRefValue
	// TEMPLATE: WEBSERVICE 0x100a35c0
	// TEMPLATE: ROBLOX 0x005ebc00
	// RBX::Reflection::RefPropDescriptor<RBX::Humanoid,RBX::PartInstance>::RefPropDescriptor<RBX::Humanoid,RBX::PartInstance><RBX::PartInstance * (__thiscall RBX::Humanoid::*)(void)const ,void (__thiscall RBX::Humanoid::*)(RBX::PartInstance *)>
	// TEMPLATE: WEBSERVICE 0x100a3680
	// TEMPLATE: ROBLOX 0x005ebd40
	// RBX::Reflection::RefPropDescriptor<RBX::Humanoid,RBX::PartInstance>::writeValue
	// TEMPLATE: WEBSERVICE 0x100b4750
	// TEMPLATE: ROBLOX 0x005df170
	// RBX::Reflection::EnumPropDescriptor<RBX::HopperBin,enum RBX::HopperBin::BinType>::equalValues
	// TEMPLATE: WEBSERVICE 0x100c3cb0
	// TEMPLATE: ROBLOX 0x005d7850
	// RBX::Reflection::RefPropDescriptor<RBX::Camera,RBX::Instance>::RefPropDescriptor<RBX::Camera,RBX::Instance><RBX::Instance * (__thiscall RBX::Camera::*)(void)const ,void (__thiscall RBX::Camera::*)(RBX::Instance *)>
	// TEMPLATE: WEBSERVICE 0x100d8810
	// TEMPLATE: ROBLOX 0x005f26d0
	// RBX::Reflection::RefPropDescriptor<RBX::AutoJoint,RBX::PartInstance>::RefPropDescriptor<RBX::AutoJoint,RBX::PartInstance><RBX::PartInstance * (__thiscall RBX::AutoJoint::*)(void)const ,void (__thiscall RBX::AutoJoint::*)(RBX::PartInstance *)>
	// SYNTHETIC: WEBSERVICE 0x100e5fe0
	// SYNTHETIC: ROBLOX 0x005f80d0
	// RBX::Reflection::EnumPropDescriptor<RBX::HopperBin,enum RBX::HopperBin::BinType>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x100e6a40
	// TEMPLATE: ROBLOX 0x00619930
	// RBX::Reflection::RefPropDescriptor<RBX::VelocityMotor,RBX::Hole>::RefPropDescriptor<RBX::VelocityMotor,RBX::Hole><RBX::Hole * (__thiscall RBX::VelocityMotor::*)(void)const ,void (__thiscall RBX::VelocityMotor::*)(RBX::Hole *)>
	// TEMPLATE: WEBSERVICE 0x100e6b40
	// TEMPLATE: ROBLOX 0x00619a30
	// RBX::Reflection::RefPropDescriptor<RBX::VelocityMotor,RBX::Hole>::setRefValue
	// TEMPLATE: WEBSERVICE 0x100ec830
	// TEMPLATE: ROBLOX 0x004ab840
	// RBX::Reflection::RefPropDescriptor<RBX::Humanoid,RBX::PartInstance>::getRefValue
	// TEMPLATE: WEBSERVICE 0x100ec850
	// TEMPLATE: ROBLOX 0x005ebd00
	// RBX::Reflection::RefPropDescriptor<RBX::Humanoid,RBX::PartInstance>::setRefValue
	// TEMPLATE: WEBSERVICE 0x100f3970
	// TEMPLATE: ROBLOX 0x00632650
	// RBX::Reflection::RefPropDescriptor<RBX::ObjectValue,RBX::Instance>::RefPropDescriptor<RBX::ObjectValue,RBX::Instance><RBX::Instance * (__thiscall RBX::ObjectValue::*)(void)const ,void (__thiscall RBX::ObjectValue::*)(RBX::Instance *)>
	// TEMPLATE: WEBSERVICE 0x10102200
	// TEMPLATE: ROBLOX 0x005df580
	// RBX::Reflection::EnumPropDescriptor<RBX::HopperBin,enum RBX::HopperBin::BinType>::isReadOnly
	// TEMPLATE: WEBSERVICE 0x10117770
	// TEMPLATE: ROBLOX 0x00656f50
	// RBX::Reflection::RefPropDescriptor<RBX::Mouse,RBX::PartInstance>::RefPropDescriptor<RBX::Mouse,RBX::PartInstance><RBX::PartInstance * (__thiscall RBX::Mouse::*)(void)const ,int>
	// TEMPLATE: WEBSERVICE 0x1018e3e0
	// TEMPLATE: ROBLOX 0x004ab640
	// RBX::Reflection::RefPropDescriptor<RBX::Network::Player,RBX::ModelInstance>::RefPropDescriptor<RBX::Network::Player,RBX::ModelInstance><RBX::ModelInstance * (__thiscall RBX::Network::Player::*)(void)const ,void (__thiscall RBX::Network::Player::*)(RBX::ModelInstance *)>
	// TEMPLATE: WEBSERVICE 0x1018e490
	// TEMPLATE: ROBLOX 0x004ab860
	// RBX::Reflection::RefPropDescriptor<RBX::Network::Player,RBX::ModelInstance>::setRefValue
	// TEMPLATE: WEBSERVICE 0x101933c0
	// TEMPLATE: ROBLOX 0x004b3e20
	// RBX::Reflection::RefPropDescriptor<RBX::Network::Players,RBX::Network::Player>::RefPropDescriptor<RBX::Network::Players,RBX::Network::Player><RBX::Network::Player * (__thiscall RBX::Network::Players::*)(void)const ,int>
	// TEMPLATE: WEBSERVICE 0x10193470
	// TEMPLATE: ROBLOX 0x004b3ed0
	// RBX::Reflection::RefPropDescriptor<RBX::Network::Players,RBX::Network::Player>::setRefValue
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
	// TEMPLATE: WEBSERVICE 0x10045be0
	// TEMPLATE: ROBLOX 0x005db7f0
	// RBX::Reflection::BoundFuncDesc<RBX::Lighting,float __cdecl(void),0>::BoundFuncDesc<RBX::Lighting,float __cdecl(void),0>
	// TEMPLATE: WEBSERVICE 0x10045c50
	// TEMPLATE: ROBLOX 0x005db860
	// RBX::Reflection::BoundFuncDesc<RBX::Lighting,float __cdecl(void),0>::call<float>
	// TEMPLATE: WEBSERVICE 0x10045cb0
	// TEMPLATE: ROBLOX 0x005db8c0
	// RBX::Reflection::BoundFuncDesc<RBX::Lighting,float __cdecl(void),0>::execute
	// TEMPLATE: WEBSERVICE 0x10045d10
	// TEMPLATE: ROBLOX 0x005db920
	// RBX::Reflection::BoundFuncDesc<RBX::Lighting,G3D::Vector3 __cdecl(void),0>::BoundFuncDesc<RBX::Lighting,G3D::Vector3 __cdecl(void),0>
	// TEMPLATE: WEBSERVICE 0x10045dd0
	// TEMPLATE: ROBLOX 0x005db990
	// RBX::Reflection::BoundFuncDesc<RBX::Lighting,G3D::Vector3 __cdecl(void),0>::execute
	// TEMPLATE: WEBSERVICE 0x10045e40
	// TEMPLATE: ROBLOX 0x005dba00
	// RBX::Reflection::BoundFuncDesc<RBX::Lighting,double __cdecl(void),0>::BoundFuncDesc<RBX::Lighting,double __cdecl(void),0>
	// TEMPLATE: WEBSERVICE 0x10045f00 FOLDED
	// TEMPLATE: ROBLOX 0x005dba70 FOLDED
	// RBX::Reflection::BoundFuncDesc<RBX::Stats::Item,double __cdecl(void),0>::call<double>
	// TEMPLATE: WEBSERVICE 0x10045f00 FOLDED
	// TEMPLATE: ROBLOX 0x005dba70 FOLDED
	// RBX::Reflection::BoundFuncDesc<RBX::Lighting,double __cdecl(void),0>::call<double>
	// TEMPLATE: WEBSERVICE 0x10045f60
	// TEMPLATE: ROBLOX 0x005dbad0
	// RBX::Reflection::BoundFuncDesc<RBX::Lighting,double __cdecl(void),0>::execute
	// TEMPLATE: WEBSERVICE 0x1004c510
	// TEMPLATE: ROBLOX 0x00578e90
	// RBX::Reflection::BoundFuncDesc<RBX::Instance,boost::shared_ptr<RBX::Instance> __cdecl(void),0>::BoundFuncDesc<RBX::Instance,boost::shared_ptr<RBX::Instance> __cdecl(void),0>
	// TEMPLATE: WEBSERVICE 0x1004c580
	// TEMPLATE: ROBLOX 0x00578f00
	// RBX::Reflection::BoundFuncDesc<RBX::Instance,boost::shared_ptr<RBX::Instance> __cdecl(void),0>::call<boost::shared_ptr<RBX::Instance> >
	// TEMPLATE: WEBSERVICE 0x1004c620
	// TEMPLATE: ROBLOX 0x00578fa0
	// RBX::Reflection::BoundFuncDesc<RBX::Instance,boost::shared_ptr<RBX::Instance> __cdecl(void),0>::execute
	// TEMPLATE: WEBSERVICE 0x1004c680
	// RBX::Reflection::BoundFuncDesc<RBX::Instance,void __cdecl(void),0>::BoundFuncDesc<RBX::Instance,void __cdecl(void),0>
	// TEMPLATE: WEBSERVICE 0x1004c6f0
	// TEMPLATE: ROBLOX 0x00579070
	// RBX::Reflection::BoundFuncDesc<RBX::Instance,void __cdecl(void),0>::execute
	// TEMPLATE: WEBSERVICE 0x1004c900
	// TEMPLATE: ROBLOX 0x005791e0
	// RBX::Reflection::BoundFuncDesc<RBX::Instance,boost::shared_ptr<std::vector<boost::shared_ptr<RBX::Instance>,std::allocator<boost::shared_ptr<RBX::Instance> > > const > __cdecl(void),0>::execute
	// TEMPLATE: WEBSERVICE 0x100567d0
	// TEMPLATE: ROBLOX 0x0056ac50
	// RBX::Reflection::BoundFuncDesc<RBX::RunService,void __cdecl(void),0>::BoundFuncDesc<RBX::RunService,void __cdecl(void),0>
	// TEMPLATE: WEBSERVICE 0x10056840
	// TEMPLATE: ROBLOX 0x0056acc0
	// RBX::Reflection::BoundFuncDesc<RBX::RunService,void __cdecl(void),0>::execute
	// TEMPLATE: WEBSERVICE 0x1005c230
	// TEMPLATE: ROBLOX 0x00566ab0
	// RBX::Reflection::BoundFuncDesc<RBX::ModelInstance,void __cdecl(void),0>::BoundFuncDesc<RBX::ModelInstance,void __cdecl(void),0>
	// TEMPLATE: WEBSERVICE 0x1005c300
	// TEMPLATE: ROBLOX 0x00566b30
	// RBX::Reflection::BoundFuncDesc<RBX::ModelInstance,void __cdecl(void),0>::execute
	// TEMPLATE: WEBSERVICE 0x1005e520
	// TEMPLATE: ROBLOX 0x00568550
	// RBX::Reflection::BoundFuncDesc<RBX::Selection,boost::shared_ptr<std::vector<boost::shared_ptr<RBX::Instance>,std::allocator<boost::shared_ptr<RBX::Instance> > > const > __cdecl(void),0>::execute
	// TEMPLATE: WEBSERVICE 0x10070210
	// TEMPLATE: ROBLOX 0x005b8600
	// RBX::Reflection::BoundFuncDesc<RBX::Workspace,void __cdecl(void),0>::BoundFuncDesc<RBX::Workspace,void __cdecl(void),0>
	// TEMPLATE: WEBSERVICE 0x10070290
	// TEMPLATE: ROBLOX 0x005b8680
	// RBX::Reflection::BoundFuncDesc<RBX::Workspace,void __cdecl(void),0>::execute
	// TEMPLATE: WEBSERVICE 0x10082940
	// TEMPLATE: ROBLOX 0x005c6400
	// RBX::Reflection::BoundFuncDesc<RBX::Soundscape::SoundChannel,void __cdecl(void),0>::BoundFuncDesc<RBX::Soundscape::SoundChannel,void __cdecl(void),0>
	// TEMPLATE: WEBSERVICE 0x100829b0
	// TEMPLATE: ROBLOX 0x005c6470
	// RBX::Reflection::BoundFuncDesc<RBX::Soundscape::SoundChannel,void __cdecl(void),0>::execute
	// TEMPLATE: WEBSERVICE 0x1009eac0
	// TEMPLATE: ROBLOX 0x005b1530
	// RBX::Reflection::BoundFuncDesc<RBX::PartInstance,void __cdecl(void),0>::BoundFuncDesc<RBX::PartInstance,void __cdecl(void),0>
	// TEMPLATE: WEBSERVICE 0x1009eb40
	// TEMPLATE: ROBLOX 0x005b15b0
	// RBX::Reflection::BoundFuncDesc<RBX::PartInstance,void __cdecl(void),0>::execute
	// TEMPLATE: WEBSERVICE 0x1009ebb0
	// TEMPLATE: ROBLOX 0x005b1620
	// RBX::Reflection::BoundFuncDesc<RBX::PartInstance,float __cdecl(void),0>::BoundFuncDesc<RBX::PartInstance,float __cdecl(void),0>
	// TEMPLATE: WEBSERVICE 0x1009ec30
	// TEMPLATE: ROBLOX 0x005b16a0
	// RBX::Reflection::BoundFuncDesc<RBX::PartInstance,float __cdecl(void),0>::call<float>
	// TEMPLATE: WEBSERVICE 0x1009eca0
	// TEMPLATE: ROBLOX 0x005b1710
	// RBX::Reflection::BoundFuncDesc<RBX::PartInstance,float __cdecl(void),0>::execute
	// TEMPLATE: WEBSERVICE 0x100b23b0
	// TEMPLATE: ROBLOX 0x005d3c90
	// RBX::Reflection::BoundFuncDesc<RBX::Stats::Item,std::basic_string<char,std::char_traits<char>,std::allocator<char> > __cdecl(void),0>::BoundFuncDesc<RBX::Stats::Item,std::basic_string<char,std::char_traits<char>,std::allocator<char> > __cdecl(void),0>
	// TEMPLATE: WEBSERVICE 0x100b2420
	// TEMPLATE: ROBLOX 0x005cf470
	// RBX::Reflection::BoundFuncDesc<RBX::Stats::Item,std::basic_string<char,std::char_traits<char>,std::allocator<char> > __cdecl(void),0>::call<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >
	// TEMPLATE: WEBSERVICE 0x100b24b0
	// TEMPLATE: ROBLOX 0x005d3d00
	// RBX::Reflection::BoundFuncDesc<RBX::Stats::Item,std::basic_string<char,std::char_traits<char>,std::allocator<char> > __cdecl(void),0>::execute
	// TEMPLATE: WEBSERVICE 0x100b2510
	// TEMPLATE: ROBLOX 0x005d3d60
	// RBX::Reflection::BoundFuncDesc<RBX::Stats::Item,double __cdecl(void),0>::BoundFuncDesc<RBX::Stats::Item,double __cdecl(void),0>
	// TEMPLATE: WEBSERVICE 0x100b2580
	// TEMPLATE: ROBLOX 0x005d3dd0
	// RBX::Reflection::BoundFuncDesc<RBX::Stats::Item,double __cdecl(void),0>::execute
	// TEMPLATE: WEBSERVICE 0x100db180
	// TEMPLATE: ROBLOX 0x005e7fc0
	// RBX::Reflection::BoundFuncDesc<RBX::Teams,void __cdecl(void),0>::BoundFuncDesc<RBX::Teams,void __cdecl(void),0>
	// TEMPLATE: WEBSERVICE 0x100db1f0
	// TEMPLATE: ROBLOX 0x005e8030
	// RBX::Reflection::BoundFuncDesc<RBX::Teams,void __cdecl(void),0>::execute
	// TEMPLATE: WEBSERVICE 0x100db2c0
	// TEMPLATE: ROBLOX 0x005e8100
	// RBX::Reflection::BoundFuncDesc<RBX::Teams,boost::shared_ptr<std::vector<boost::shared_ptr<RBX::Instance>,std::allocator<boost::shared_ptr<RBX::Instance> > > const > __cdecl(void),0>::execute
	// TEMPLATE: WEBSERVICE 0x100edb20
	// TEMPLATE: ROBLOX 0x0062cba0
	// RBX::Reflection::BoundFuncDesc<RBX::Rocket,void __cdecl(void),0>::BoundFuncDesc<RBX::Rocket,void __cdecl(void),0>
	// TEMPLATE: WEBSERVICE 0x100edb90
	// TEMPLATE: ROBLOX 0x0062cc10
	// RBX::Reflection::BoundFuncDesc<RBX::Rocket,void __cdecl(void),0>::execute
	// TEMPLATE: WEBSERVICE 0x100edbf0
	// STUB: ROBLOX 0x00579000
	// RBX::Reflection::BoundFuncDesc<RBX::BodyPosition,G3D::Vector3 __cdecl(void),0>::BoundFuncDesc<RBX::BodyPosition,G3D::Vector3 __cdecl(void),0>
	// TEMPLATE: WEBSERVICE 0x100edc60
	// TEMPLATE: ROBLOX 0x0062cce0
	// RBX::Reflection::BoundFuncDesc<RBX::BodyPosition,G3D::Vector3 __cdecl(void),0>::execute
	// TEMPLATE: WEBSERVICE 0x100edcd0
	// TEMPLATE: ROBLOX 0x0062cd50
	// RBX::Reflection::BoundFuncDesc<RBX::BodyVelocity,G3D::Vector3 __cdecl(void),0>::BoundFuncDesc<RBX::BodyVelocity,G3D::Vector3 __cdecl(void),0>
	// TEMPLATE: WEBSERVICE 0x100edd40
	// TEMPLATE: ROBLOX 0x0062cdc0
	// RBX::Reflection::BoundFuncDesc<RBX::BodyVelocity,G3D::Vector3 __cdecl(void),0>::execute
	// TEMPLATE: WEBSERVICE 0x100f9750
	// TEMPLATE: ROBLOX 0x005cf400
	// RBX::Reflection::BoundFuncDesc<RBX::Visit,std::basic_string<char,std::char_traits<char>,std::allocator<char> > __cdecl(void),0>::BoundFuncDesc<RBX::Visit,std::basic_string<char,std::char_traits<char>,std::allocator<char> > __cdecl(void),0>
	// TEMPLATE: WEBSERVICE 0x100f97c0
	// TEMPLATE: ROBLOX 0x005cf500
	// RBX::Reflection::BoundFuncDesc<RBX::Visit,std::basic_string<char,std::char_traits<char>,std::allocator<char> > __cdecl(void),0>::execute
	// TEMPLATE: WEBSERVICE 0x1018f020
	// TEMPLATE: ROBLOX 0x004ac8f0
	// RBX::Reflection::BoundFuncDesc<RBX::Network::Player,void __cdecl(void),0>::BoundFuncDesc<RBX::Network::Player,void __cdecl(void),0>
	// TEMPLATE: WEBSERVICE 0x1018f0f0
	// TEMPLATE: ROBLOX 0x004ac9c0
	// RBX::Reflection::BoundFuncDesc<RBX::Network::Player,void __cdecl(void),0>::execute
	// TEMPLATE: WEBSERVICE 0x10193ee0
	// STUB: ROBLOX 0x005b86f0
	// RBX::Reflection::FuncDesc<RBX::Humanoid>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x1019af60
	// TEMPLATE: ROBLOX 0x004bd180
	// RBX::Reflection::BoundFuncDesc<RBX::Network::Server,int __cdecl(void),0>::BoundFuncDesc<RBX::Network::Server,int __cdecl(void),0>
	// TEMPLATE: WEBSERVICE 0x1019afe0
	// TEMPLATE: ROBLOX 0x004bd200
	// RBX::Reflection::BoundFuncDesc<RBX::Network::Server,int __cdecl(void),0>::execute
	// TEMPLATE: WEBSERVICE 0x101a4420
	// TEMPLATE: ROBLOX 0x004ccc10
	// RBX::Reflection::BoundFuncDesc<RBX::Network::Replicator,boost::shared_ptr<RBX::Reflection::DescribedBase> __cdecl(void),0>::BoundFuncDesc<RBX::Network::Replicator,boost::shared_ptr<RBX::Reflection::DescribedBase> __cdecl(void),0>
	// TEMPLATE: WEBSERVICE 0x101a44a0
	// TEMPLATE: ROBLOX 0x004ccd60
	// RBX::Reflection::BoundFuncDesc<RBX::Network::Replicator,boost::shared_ptr<RBX::Reflection::DescribedBase> __cdecl(void),0>::call<boost::shared_ptr<RBX::Reflection::DescribedBase> >
	// TEMPLATE: WEBSERVICE 0x101a4550
	// TEMPLATE: ROBLOX 0x004cce10
	// RBX::Reflection::BoundFuncDesc<RBX::Network::Replicator,boost::shared_ptr<RBX::Reflection::DescribedBase> __cdecl(void),0>::execute
	// TEMPLATE: WEBSERVICE 0x101a45b0
	// TEMPLATE: ROBLOX 0x004cce70
	// RBX::Reflection::BoundFuncDesc<RBX::Network::Replicator,void __cdecl(void),0>::BoundFuncDesc<RBX::Network::Replicator,void __cdecl(void),0>
	// TEMPLATE: WEBSERVICE 0x101a4630
	// TEMPLATE: ROBLOX 0x004ccef0
	// RBX::Reflection::BoundFuncDesc<RBX::Network::Replicator,void __cdecl(void),0>::execute
	// TEMPLATE: WEBSERVICE 0x101a4690
	// TEMPLATE: ROBLOX 0x004ccf50
	// RBX::Reflection::BoundFuncDesc<RBX::Network::Replicator,boost::shared_ptr<RBX::Instance> __cdecl(void),0>::BoundFuncDesc<RBX::Network::Replicator,boost::shared_ptr<RBX::Instance> __cdecl(void),0>
	// TEMPLATE: WEBSERVICE 0x101a4710
	// TEMPLATE: ROBLOX 0x004ccfd0
	// RBX::Reflection::BoundFuncDesc<RBX::Network::Replicator,boost::shared_ptr<RBX::Instance> __cdecl(void),0>::call<boost::shared_ptr<RBX::Instance> >
	// TEMPLATE: WEBSERVICE 0x101a47c0
	// TEMPLATE: ROBLOX 0x004cd080
	// RBX::Reflection::BoundFuncDesc<RBX::Network::Replicator,boost::shared_ptr<RBX::Instance> __cdecl(void),0>::execute
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
	void call(Class* o, Value& returnValue, Value& arg1) const
	{
		returnValue = (o->*function)(arg1.convert<Arg1>());
	}

	template <>
	void call<void>(Class* o, Value& returnValue, Value& arg1) const
	{
		(o->*function)(arg1.convert<Arg1>());
	}

public:
	BoundFuncDesc(
		Function function, const char* name, const char* arg1Name, typename FunctionDescriptor::Security security
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
		call<result_type>(boost::polymorphic_cast<Class*>(instance), arguments.returnValue, arg1);
	}

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x10045fc0
	// TEMPLATE: ROBLOX 0x005dbb30
	// RBX::Reflection::BoundFuncDesc<RBX::Lighting,void __cdecl(double),1>::BoundFuncDesc<RBX::Lighting,void __cdecl(double),1>
	// TEMPLATE: WEBSERVICE 0x10046060
	// TEMPLATE: ROBLOX 0x005dbbd0
	// RBX::Reflection::BoundFuncDesc<RBX::Lighting,void __cdecl(double),1>::execute
	// TEMPLATE: WEBSERVICE 0x1004ca50
	// TEMPLATE: ROBLOX 0x005792e0
	// RBX::Reflection::BoundFuncDesc<RBX::Instance,bool __cdecl(boost::shared_ptr<RBX::Instance>),1>::call<bool>
	// TEMPLATE: WEBSERVICE 0x1004cad0
	// TEMPLATE: ROBLOX 0x00579360
	// RBX::Reflection::BoundFuncDesc<RBX::Instance,bool __cdecl(boost::shared_ptr<RBX::Instance>),1>::execute
	// TEMPLATE: WEBSERVICE 0x100598b0
	// TEMPLATE: ROBLOX 0x0058c8b0
	// RBX::Reflection::BoundFuncDesc<RBX::ServiceProvider,boost::shared_ptr<RBX::Instance> __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >),1>::call<boost::shared_ptr<RBX::Instance> >
	// TEMPLATE: WEBSERVICE 0x10059970
	// TEMPLATE: ROBLOX 0x0058c970
	// RBX::Reflection::BoundFuncDesc<RBX::ServiceProvider,boost::shared_ptr<RBX::Instance> __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >),1>::execute
	// TEMPLATE: WEBSERVICE 0x1005c370
	// TEMPLATE: ROBLOX 0x00566ba0
	// RBX::Reflection::BoundFuncDesc<RBX::ModelInstance,void __cdecl(G3D::Vector3),1>::BoundFuncDesc<RBX::ModelInstance,void __cdecl(G3D::Vector3),1>
	// TEMPLATE: WEBSERVICE 0x1005c420
	// TEMPLATE: ROBLOX 0x00566c50
	// RBX::Reflection::BoundFuncDesc<RBX::ModelInstance,void __cdecl(G3D::Vector3),1>::execute
	// TEMPLATE: WEBSERVICE 0x1005e620
	// TEMPLATE: ROBLOX 0x00568650
	// RBX::Reflection::BoundFuncDesc<RBX::Selection,void __cdecl(boost::shared_ptr<std::vector<boost::shared_ptr<RBX::Instance>,std::allocator<boost::shared_ptr<RBX::Instance> > > const >),1>::execute
	// SYNTHETIC: WEBSERVICE 0x1006ea20 FOLDED
	// SYNTHETIC: ROBLOX 0x005b6e40 FOLDED
	// RBX::Reflection::BoundFuncDesc<RBX::ModelInstance,void __cdecl(G3D::Vector3),1>::~BoundFuncDesc<RBX::ModelInstance,void __cdecl(G3D::Vector3),1>
	// SYNTHETIC: WEBSERVICE 0x1006ea20 FOLDED
	// SYNTHETIC: ROBLOX 0x005b6e40 FOLDED
	// RBX::Reflection::BoundFuncDesc<RBX::Humanoid,void __cdecl(float),1>::~BoundFuncDesc<RBX::Humanoid,void __cdecl(float),1>
	// TEMPLATE: WEBSERVICE 0x1006fdc0
	// TEMPLATE: ROBLOX 0x005b81b0
	// RBX::Reflection::BoundFuncDesc<RBX::Workspace,boost::shared_ptr<std::vector<boost::shared_ptr<RBX::Instance>,std::allocator<boost::shared_ptr<RBX::Instance> > > const > __cdecl(RBX::ContentId),1>::execute
	// TEMPLATE: WEBSERVICE 0x1006ff40
	// TEMPLATE: ROBLOX 0x005b8330
	// RBX::Reflection::BoundFuncDesc<RBX::Workspace,void __cdecl(boost::shared_ptr<std::vector<boost::shared_ptr<RBX::Instance>,std::allocator<boost::shared_ptr<RBX::Instance> > > const >),1>::execute
	// SYNTHETIC: WEBSERVICE 0x10070050
	// SYNTHETIC: ROBLOX 0x005b8440
	// RBX::Reflection::BoundFuncDesc<RBX::Humanoid,void __cdecl(float),1>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x10070070
	// RBX::Reflection::BoundFuncDesc<RBX::Workspace,void __cdecl(bool),1>::BoundFuncDesc<RBX::Workspace,void __cdecl(bool),1>
	// TEMPLATE: WEBSERVICE 0x10070120
	// TEMPLATE: ROBLOX 0x005b8510
	// RBX::Reflection::BoundFuncDesc<RBX::Workspace,void __cdecl(bool),1>::execute
	// TEMPLATE: WEBSERVICE 0x100a3dc0
	// TEMPLATE: ROBLOX 0x005ec4b0
	// RBX::Reflection::BoundFuncDesc<RBX::Humanoid,void __cdecl(float),1>::BoundFuncDesc<RBX::Humanoid,void __cdecl(float),1>
	// TEMPLATE: WEBSERVICE 0x100a3e70
	// TEMPLATE: ROBLOX 0x005ec560
	// RBX::Reflection::BoundFuncDesc<RBX::Humanoid,void __cdecl(float),1>::execute
	// TEMPLATE: WEBSERVICE 0x100f95d0
	// RBX::Reflection::BoundFuncDesc<RBX::Visit,void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >),1>::BoundFuncDesc<RBX::Visit,void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >),1>
	// TEMPLATE: WEBSERVICE 0x100f9670
	// TEMPLATE: ROBLOX 0x005cf320
	// RBX::Reflection::BoundFuncDesc<RBX::Visit,void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >),1>::execute
	// TEMPLATE: WEBSERVICE 0x1018f150
	// TEMPLATE: ROBLOX 0x004aca20
	// RBX::Reflection::BoundFuncDesc<RBX::Network::Player,void __cdecl(bool),1>::BoundFuncDesc<RBX::Network::Player,void __cdecl(bool),1>
	// TEMPLATE: WEBSERVICE 0x1018f200
	// TEMPLATE: ROBLOX 0x004acad0
	// RBX::Reflection::BoundFuncDesc<RBX::Network::Player,void __cdecl(bool),1>::execute
	// SYNTHETIC: WEBSERVICE 0x101975b0
	// SYNTHETIC: ROBLOX 0x004b3a30
	// RBX::Reflection::BoundFuncDesc<RBX::Network::Player,void __cdecl(bool),1>::~BoundFuncDesc<RBX::Network::Player,void __cdecl(bool),1>
	// TEMPLATE: WEBSERVICE 0x101983b0
	// TEMPLATE: ROBLOX 0x004ba650
	// RBX::Reflection::BoundFuncDesc<RBX::Network::Client,void __cdecl(int),1>::BoundFuncDesc<RBX::Network::Client,void __cdecl(int),1>
	// TEMPLATE: WEBSERVICE 0x10198480
	// TEMPLATE: ROBLOX 0x004ba720
	// RBX::Reflection::BoundFuncDesc<RBX::Network::Client,void __cdecl(int),1>::execute
	// SYNTHETIC: WEBSERVICE 0x10198560
	// SYNTHETIC: ROBLOX 0x004b4700
	// RBX::Reflection::BoundFuncDesc<RBX::Network::Player,void __cdecl(bool),1>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x1019adb0
	// TEMPLATE: ROBLOX 0x004bcfd0
	// RBX::Reflection::BoundFuncDesc<RBX::Network::Server,void __cdecl(int),1>::BoundFuncDesc<RBX::Network::Server,void __cdecl(int),1>
	// TEMPLATE: WEBSERVICE 0x1019ae80
	// TEMPLATE: ROBLOX 0x004bd0a0
	// RBX::Reflection::BoundFuncDesc<RBX::Network::Server,void __cdecl(int),1>::execute
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
	void call(Class* o, Value& returnValue, Value& arg1, Value& arg2) const
	{
		returnValue = (o->*function)(arg1.convert<Arg1>(), arg2.convert<Arg2>());
	}

	template <>
	void call<void>(Class* o, Value& returnValue, Value& arg1, Value& arg2) const
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
		call<result_type>(boost::polymorphic_cast<Class*>(instance), arguments.returnValue, arg1, arg2);
	}

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x10047b70
	// RBX::Reflection::BoundFuncDesc<RBX::Instance,boost::shared_ptr<RBX::Instance> __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >,bool),2>::declareSignature
	// TEMPLATE: WEBSERVICE 0x1004c330
	// TEMPLATE: ROBLOX 0x00578cb0
	// RBX::Reflection::BoundFuncDesc<RBX::Instance,boost::shared_ptr<RBX::Instance> __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >,bool),2>::call<boost::shared_ptr<RBX::Instance> >
	// TEMPLATE: WEBSERVICE 0x1004c3f0
	// TEMPLATE: ROBLOX 0x00578d70
	// RBX::Reflection::BoundFuncDesc<RBX::Instance,boost::shared_ptr<RBX::Instance> __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >,bool),2>::execute
	// TEMPLATE: WEBSERVICE 0x100f8be0
	// TEMPLATE: ROBLOX 0x005ce890
	// RBX::Reflection::BoundFuncDesc<RBX::Visit,void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >,int),2>::declareSignature
	// TEMPLATE: WEBSERVICE 0x100f9820
	// TEMPLATE: ROBLOX 0x005cf560
	// RBX::Reflection::BoundFuncDesc<RBX::Visit,void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >,int),2>::BoundFuncDesc<RBX::Visit,void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >,int),2>
	// TEMPLATE: WEBSERVICE 0x100f98b0
	// TEMPLATE: ROBLOX 0x005cf5f0
	// RBX::Reflection::BoundFuncDesc<RBX::Visit,void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >,int),2>::execute
	// TEMPLATE: WEBSERVICE 0x100f9e40
	// TEMPLATE: ROBLOX 0x00636f50
	// RBX::Reflection::BoundFuncDesc<RBX::DebrisService,void __cdecl(boost::shared_ptr<RBX::Instance>,double),2>::declareSignature
	// TEMPLATE: WEBSERVICE 0x100fafb0
	// TEMPLATE: ROBLOX 0x00637ee0
	// RBX::Reflection::BoundFuncDesc<RBX::DebrisService,void __cdecl(boost::shared_ptr<RBX::Instance>,double),2>::BoundFuncDesc<RBX::DebrisService,void __cdecl(boost::shared_ptr<RBX::Instance>,double),2>
	// TEMPLATE: WEBSERVICE 0x100fb070
	// TEMPLATE: ROBLOX 0x00637fa0
	// RBX::Reflection::BoundFuncDesc<RBX::DebrisService,void __cdecl(boost::shared_ptr<RBX::Instance>,double),2>::execute
	// TEMPLATE: WEBSERVICE 0x100a0a10
	// TEMPLATE: ROBLOX 0x005e8f80
	// RBX::Reflection::BoundFuncDesc<RBX::Humanoid,void __cdecl(G3D::Vector3,boost::shared_ptr<RBX::Instance>),2>::declareSignature
	// SYNTHETIC: WEBSERVICE 0x100a33a0
	// SYNTHETIC: ROBLOX 0x005eb9e0
	// RBX::Reflection::BoundFuncDesc<RBX::Humanoid,void __cdecl(G3D::Vector3,boost::shared_ptr<RBX::Instance>),2>::~BoundFuncDesc<RBX::Humanoid,void __cdecl(G3D::Vector3,boost::shared_ptr<RBX::Instance>),2>
	// TEMPLATE: WEBSERVICE 0x100a3f60
	// TEMPLATE: ROBLOX 0x005ec650
	// RBX::Reflection::BoundFuncDesc<RBX::Humanoid,void __cdecl(G3D::Vector3,boost::shared_ptr<RBX::Instance>),2>::BoundFuncDesc<RBX::Humanoid,void __cdecl(G3D::Vector3,boost::shared_ptr<RBX::Instance>),2>
	// TEMPLATE: WEBSERVICE 0x100a4000
	// TEMPLATE: ROBLOX 0x005ec6f0
	// RBX::Reflection::BoundFuncDesc<RBX::Humanoid,void __cdecl(G3D::Vector3,boost::shared_ptr<RBX::Instance>),2>::call<void>
	// TEMPLATE: WEBSERVICE 0x100a40b0
	// TEMPLATE: ROBLOX 0x005ec7a0
	// RBX::Reflection::BoundFuncDesc<RBX::Humanoid,void __cdecl(G3D::Vector3,boost::shared_ptr<RBX::Instance>),2>::execute
	// SYNTHETIC: WEBSERVICE 0x100a41d0
	// SYNTHETIC: ROBLOX 0x005ec8c0
	// RBX::Reflection::BoundFuncDesc<RBX::Humanoid,void __cdecl(G3D::Vector3,boost::shared_ptr<RBX::Instance>),2>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x10193010
	// TEMPLATE: ROBLOX 0x00418c90
	// RBX::Reflection::BoundFuncDesc<RBX::Network::Server,void __cdecl(int,int),2>::~BoundFuncDesc<RBX::Network::Server,void __cdecl(int,int),2>
	// TEMPLATE: WEBSERVICE 0x10193d30
	// TEMPLATE: ROBLOX 0x0041b7c0
	// RBX::Reflection::BoundFuncDesc<RBX::Network::Players,void __cdecl(boost::shared_ptr<RBX::Instance>,std::basic_string<char,std::char_traits<char>,std::allocator<char> >),2>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x10196790
	// TEMPLATE: ROBLOX 0x004b8970
	// RBX::Reflection::BoundFuncDesc<RBX::Network::Client,void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >,int,int,int),4>::declareSignature
	// TEMPLATE: WEBSERVICE 0x101981a0
	// TEMPLATE: ROBLOX 0x004ba440
	// RBX::Reflection::BoundFuncDesc<RBX::Network::Client,void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >,int,int,int),4>::execute
	// SYNTHETIC: WEBSERVICE 0x10198390
	// SYNTHETIC: ROBLOX 0x004ba630
	// RBX::Reflection::BoundFuncDesc<RBX::Network::Client,void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >,int,int,int),4>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x10199300
	// TEMPLATE: ROBLOX 0x004bb580
	// RBX::Reflection::BoundFuncDesc<RBX::Network::Server,void __cdecl(int,int),2>::declareSignature
	// TEMPLATE: WEBSERVICE 0x1019abb0
	// TEMPLATE: ROBLOX 0x004bcdd0
	// RBX::Reflection::BoundFuncDesc<RBX::Network::Server,void __cdecl(int,int),2>::BoundFuncDesc<RBX::Network::Server,void __cdecl(int,int),2>
	// TEMPLATE: WEBSERVICE 0x1019ac70
	// TEMPLATE: ROBLOX 0x004bce90
	// RBX::Reflection::BoundFuncDesc<RBX::Network::Server,void __cdecl(int,int),2>::execute
	// clang-format on
};

template <class Class, class Signature>
class BoundFuncDesc<Class, Signature, 3> : public FuncDesc<Class>
{
public:
	typedef typename boost::function_traits<Signature>::result_type result_type;
	typedef typename boost::function_traits<Signature>::arg1_type Arg1;
	typedef typename boost::function_traits<Signature>::arg2_type Arg2;
	typedef typename boost::function_traits<Signature>::arg3_type Arg3;
	typedef result_type (Class::*Function)(Arg1, Arg2, Arg3);

private:
	Function function; // 0x28
	Value default1;    // 0x30
	Value default2;    // 0x38
	Value default3;    // 0x40

	void declareSignature(const char* arg1Name, const char* arg2Name, const char* arg3Name)
	{
		this->signature.resultType = &Type::singleton<result_type>();
		this->signature.addArgument(Name::declare(arg1Name, -1), Type::singleton<Arg1>(), default1);
		this->signature.addArgument(Name::declare(arg2Name, -1), Type::singleton<Arg2>(), default2);
		this->signature.addArgument(Name::declare(arg3Name, -1), Type::singleton<Arg3>(), default3);
	}

	template <class R>
	void call(Class* o, Value& returnValue, Value& arg1, Value& arg2, Value& arg3) const
	{
		returnValue = (o->*function)(arg1.convert<Arg1>(), arg2.convert<Arg2>(), arg3.convert<Arg3>());
	}

	template <>
	void call<void>(Class* o, Value& returnValue, Value& arg1, Value& arg2, Value& arg3) const
	{
		(o->*function)(arg1.convert<Arg1>(), arg2.convert<Arg2>(), arg3.convert<Arg3>());
	}

public:
	BoundFuncDesc(
		Function function,
		const char* name,
		const char* arg1Name,
		const char* arg2Name,
		const char* arg3Name,
		typename FunctionDescriptor::Security security
	)
		: FuncDesc<Class>(name, security), function(function)
	{
		declareSignature(arg1Name, arg2Name, arg3Name);
	}

	BoundFuncDesc(
		Function function,
		const char* name,
		const char* arg1Name,
		const char* arg2Name,
		const char* arg3Name,
		Arg3 default3,
		typename FunctionDescriptor::Security security
	)
		: FuncDesc<Class>(name, security), function(function), default3(default3)
	{
		declareSignature(arg1Name, arg2Name, arg3Name);
	}

	virtual void execute(DescribedBase* instance, typename FunctionDescriptor::Arguments& arguments) const
	{
		Value arg1(default1);
		arguments.get(1, arg1);
		Value arg2(default2);
		arguments.get(2, arg2);
		Value arg3(default3);
		arguments.get(3, arg3);
		call<result_type>(boost::polymorphic_cast<Class*>(instance), arguments.returnValue, arg1, arg2, arg3);
	}
};

template <class Class, class Signature>
class BoundFuncDesc<Class, Signature, 4> : public FuncDesc<Class>
{
public:
	typedef typename boost::function_traits<Signature>::result_type result_type;
	typedef typename boost::function_traits<Signature>::arg1_type Arg1;
	typedef typename boost::function_traits<Signature>::arg2_type Arg2;
	typedef typename boost::function_traits<Signature>::arg3_type Arg3;
	typedef typename boost::function_traits<Signature>::arg4_type Arg4;
	typedef result_type (Class::*Function)(Arg1, Arg2, Arg3, Arg4);

private:
	Function function; // 0x28
	Value default1;    // 0x30
	Value default2;    // 0x38
	Value default3;    // 0x40
	Value default4;    // 0x48

	void declareSignature(const char* arg1Name, const char* arg2Name, const char* arg3Name, const char* arg4Name)
	{
		this->signature.resultType = &Type::singleton<result_type>();
		this->signature.addArgument(Name::declare(arg1Name, -1), Type::singleton<Arg1>(), default1);
		this->signature.addArgument(Name::declare(arg2Name, -1), Type::singleton<Arg2>(), default2);
		this->signature.addArgument(Name::declare(arg3Name, -1), Type::singleton<Arg3>(), default3);
		this->signature.addArgument(Name::declare(arg4Name, -1), Type::singleton<Arg4>(), default4);
	}

	template <class R>
	void call(Class* o, Value& returnValue, Value& arg1, Value& arg2, Value& arg3, Value& arg4) const
	{
		returnValue =
			(o->*function)(arg1.convert<Arg1>(), arg2.convert<Arg2>(), arg3.convert<Arg3>(), arg4.convert<Arg4>());
	}

	template <>
	void call<void>(Class* o, Value& returnValue, Value& arg1, Value& arg2, Value& arg3, Value& arg4) const
	{
		(o->*function)(arg1.convert<Arg1>(), arg2.convert<Arg2>(), arg3.convert<Arg3>(), arg4.convert<Arg4>());
	}

public:
	BoundFuncDesc(
		Function function,
		const char* name,
		const char* arg1Name,
		Arg1 default1,
		const char* arg2Name,
		Arg2 default2,
		const char* arg3Name,
		Arg3 default3,
		const char* arg4Name,
		Arg4 default4,
		typename FunctionDescriptor::Security security
	)
		: FuncDesc<Class>(name, security), function(function), default1(default1), default2(default2),
		  default3(default3), default4(default4)
	{
		declareSignature(arg1Name, arg2Name, arg3Name, arg4Name);
	}

	BoundFuncDesc(
		Function function,
		const char* name,
		const char* arg1Name,
		const char* arg2Name,
		Arg2 default2,
		const char* arg3Name,
		Arg3 default3,
		const char* arg4Name,
		Arg4 default4,
		typename FunctionDescriptor::Security security
	)
		: FuncDesc<Class>(name, security), function(function), default2(default2), default3(default3),
		  default4(default4)
	{
		declareSignature(arg1Name, arg2Name, arg3Name, arg4Name);
	}

	BoundFuncDesc(
		Function function,
		const char* name,
		const char* arg1Name,
		const char* arg2Name,
		const char* arg3Name,
		Arg3 default3,
		const char* arg4Name,
		Arg4 default4,
		typename FunctionDescriptor::Security security
	)
		: FuncDesc<Class>(name, security), function(function), default3(default3), default4(default4)
	{
		declareSignature(arg1Name, arg2Name, arg3Name, arg4Name);
	}

	BoundFuncDesc(
		Function function,
		const char* name,
		const char* arg1Name,
		const char* arg2Name,
		const char* arg3Name,
		const char* arg4Name,
		Arg4 default4,
		typename FunctionDescriptor::Security security
	)
		: FuncDesc<Class>(name, security), function(function), default4(default4)
	{
		declareSignature(arg1Name, arg2Name, arg3Name, arg4Name);
	}

	BoundFuncDesc(
		Function function,
		const char* name,
		const char* arg1Name,
		const char* arg2Name,
		const char* arg3Name,
		const char* arg4Name,
		typename FunctionDescriptor::Security security
	)
		: FuncDesc<Class>(name, security), function(function)
	{
		declareSignature(arg1Name, arg2Name, arg3Name, arg4Name);
	}

	virtual void execute(DescribedBase* instance, typename FunctionDescriptor::Arguments& arguments) const
	{
		Value arg1(default1);
		arguments.get(1, arg1);
		Value arg2(default2);
		arguments.get(2, arg2);
		Value arg3(default3);
		arguments.get(3, arg3);
		Value arg4(default4);
		arguments.get(4, arg4);
		call<result_type>(boost::polymorphic_cast<Class*>(instance), arguments.returnValue, arg1, arg2, arg3, arg4);
	}
};

} // namespace Reflection
} // namespace RBX

#endif // REFLECTION_REFLECTION_H
