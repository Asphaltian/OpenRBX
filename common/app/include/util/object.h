#ifndef UTIL_OBJECT_H
#define UTIL_OBJECT_H

#include "decomp.h"
#include "util/Name.h"

namespace RBX {

template <class T, class Base, const char* sName>
class FactoryProduct : public Base
{
public:
	class Creator
	{
	public:
		T* create() const { return new T(); }
	};

	// clang-format off
	// STUB: WEBSERVICE 0x10007ee0
	// RBX::FactoryProduct<RBX::Script,RBX::Instance,&RBX::sScript>::Creator::create
	// STUB: WEBSERVICE 0x10007fa0
	// RBX::FactoryProduct<RBX::LocalScript,RBX::Script,&RBX::sLocalScript>::Creator::create
	// STUB: WEBSERVICE 0x10064700
	// RBX::FactoryProduct<RBX::DebugSettings,RBX::GlobalSettings::Item,&RBX::sDebugSettings>::Creator::create
	// STUB: WEBSERVICE 0x10068970
	// RBX::FactoryProduct<RBX::Script,RBX::Instance,&RBX::sScript>::~FactoryProduct<RBX::Script,RBX::Instance,&RBX::sScript>
	// STUB: WEBSERVICE 0x10068aa0
	// RBX::FactoryProduct<RBX::Script,RBX::Instance,&RBX::sScript>::FactoryProduct<RBX::Script,RBX::Instance,&RBX::sScript><char const *>
	// STUB: WEBSERVICE 0x10068b10
	// RBX::FactoryProduct<RBX::Script,RBX::Instance,&RBX::sScript>::`scalar deleting destructor'
	// STUB: WEBSERVICE 0x10068e10
	// RBX::FactoryProduct<RBX::LocalScript,RBX::Script,&RBX::sLocalScript>::~FactoryProduct<RBX::LocalScript,RBX::Script,&RBX::sLocalScript>
	// STUB: WEBSERVICE 0x10068e80
	// RBX::FactoryProduct<RBX::LocalScript,RBX::Script,&RBX::sLocalScript>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x10069070
	// RBX::FactoryProduct<RBX::Script,RBX::Instance,&RBX::sScript>::getClassName
	// TEMPLATE: WEBSERVICE 0x10069090
	// RBX::FactoryProduct<RBX::LocalScript,RBX::Script,&RBX::sLocalScript>::getClassName
	// STUB: WEBSERVICE 0x10069270
	// RBX::FactoryProduct<RBX::Team,RBX::Instance,&RBX::sTeam>::Creator::create
	// TEMPLATE: WEBSERVICE 0x10069b80
	// RBX::FactoryProduct<RBX::LocalScript,RBX::Script,&RBX::sLocalScript>::FactoryProduct<RBX::LocalScript,RBX::Script,&RBX::sLocalScript>
	// STUB: WEBSERVICE 0x1006d980
	// RBX::FactoryProduct<RBX::Camera,RBX::Instance,&RBX::sCamera>::Creator::create
	// STUB: WEBSERVICE 0x100766a0
	// RBX::FactoryProduct<RBX::Snap,RBX::AutoJoint,&RBX::sSnap>::Creator::create
	// STUB: WEBSERVICE 0x10076910
	// RBX::FactoryProduct<RBX::Weld,RBX::AutoJoint,&RBX::sWeld>::Creator::create
	// STUB: WEBSERVICE 0x10076b80
	// RBX::FactoryProduct<RBX::Glue,RBX::AutoJoint,&RBX::sGlue>::Creator::create
	// STUB: WEBSERVICE 0x10076df0
	// RBX::FactoryProduct<RBX::Rotate,RBX::AutoJoint,&RBX::sRotate>::Creator::create
	// STUB: WEBSERVICE 0x10077060
	// RBX::FactoryProduct<RBX::RotateP,RBX::AutoJoint,&RBX::sRotateP>::Creator::create
	// STUB: WEBSERVICE 0x100772d0
	// RBX::FactoryProduct<RBX::RotateV,RBX::AutoJoint,&RBX::sRotateV>::Creator::create
	// STUB: WEBSERVICE 0x100815c0
	// RBX::FactoryProduct<RBX::GameSettings,RBX::GlobalSettings::Item,&RBX::sGameSettings>::Creator::create
	// STUB: WEBSERVICE 0x100899b0
	// RBX::FactoryProduct<RBX::ShirtGraphic,RBX::CharacterAppearance,&RBX::sShirtGraphic>::Creator::create
	// STUB: WEBSERVICE 0x10089c20
	// RBX::FactoryProduct<RBX::Shirt,RBX::ShirtGraphic,&RBX::sShirt>::Creator::create
	// STUB: WEBSERVICE 0x10089e90
	// RBX::FactoryProduct<RBX::BodyColors,RBX::CharacterAppearance,&RBX::sBodyColors>::Creator::create
	// STUB: WEBSERVICE 0x1008a100
	// RBX::FactoryProduct<RBX::Skin,RBX::CharacterAppearance,&RBX::sSkin>::Creator::create
	// STUB: WEBSERVICE 0x1008a5e0
	// RBX::FactoryProduct<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature>::Creator::create
	// STUB: WEBSERVICE 0x1008a850
	// RBX::FactoryProduct<RBX::Hole,RBX::Feature,&RBX::sHole>::Creator::create
	// STUB: WEBSERVICE 0x1008b6f0
	// RBX::FactoryProduct<RBX::BodyGyro,RBX::BodyMover,&RBX::sBodyGyro>::Creator::create
	// STUB: WEBSERVICE 0x1008b960
	// RBX::FactoryProduct<RBX::BodyForce,RBX::BodyMover,&RBX::sBodyForce>::Creator::create
	// STUB: WEBSERVICE 0x1008bbd0
	// RBX::FactoryProduct<RBX::BodyThrust,RBX::BodyMover,&RBX::sBodyThrust>::Creator::create
	// STUB: WEBSERVICE 0x1008be40
	// RBX::FactoryProduct<RBX::BodyPosition,RBX::BodyMover,&RBX::sBodyPosition>::Creator::create
	// STUB: WEBSERVICE 0x1008c0b0
	// RBX::FactoryProduct<RBX::BodyVelocity,RBX::BodyMover,&RBX::sBodyVelocity>::Creator::create
	// STUB: WEBSERVICE 0x1008c320
	// RBX::FactoryProduct<RBX::Rocket,RBX::BodyMover,&RBX::sRocket>::Creator::create
	// STUB: WEBSERVICE 0x1008c800
	// RBX::FactoryProduct<RBX::Hint,RBX::Message,&RBX::sHint>::FactoryProduct<RBX::Hint,RBX::Message,&RBX::sHint>
	// STUB: WEBSERVICE 0x1008c940
	// RBX::FactoryProduct<RBX::Hint,RBX::Message,&RBX::sHint>::~FactoryProduct<RBX::Hint,RBX::Message,&RBX::sHint>
	// TEMPLATE: WEBSERVICE 0x1008c9c0
	// RBX::FactoryProduct<RBX::Hint,RBX::Message,&RBX::sHint>::getClassName
	// STUB: WEBSERVICE 0x1008c9e0
	// RBX::FactoryProduct<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue>::~FactoryProduct<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue>
	// TEMPLATE: WEBSERVICE 0x1008ca30
	// RBX::FactoryProduct<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue>::FactoryProduct<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue>
	// STUB: WEBSERVICE 0x1008caa0
	// RBX::FactoryProduct<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x1008cc40
	// RBX::FactoryProduct<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue>::getClassName
	// STUB: WEBSERVICE 0x1008e4b0
	// RBX::FactoryProduct<RBX::Hint,RBX::Message,&RBX::sHint>::`scalar deleting destructor'
	// STUB: WEBSERVICE 0x1008e8a0
	// RBX::FactoryProduct<RBX::Hint,RBX::Message,&RBX::sHint>::Creator::create
	// STUB: WEBSERVICE 0x1008e950
	// RBX::FactoryProduct<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue>::Creator::create
	// STUB: WEBSERVICE 0x100ad0d0
	// RBX::FactoryProduct<RBX::Texture,RBX::Decal,&RBX::sTexture>::Creator::create
	// STUB: WEBSERVICE 0x100b3810
	// RBX::FactoryProduct<RBX::DebugSettings,RBX::GlobalSettings::Item,&RBX::sDebugSettings>::~FactoryProduct<RBX::DebugSettings,RBX::GlobalSettings::Item,&RBX::sDebugSettings>
	// STUB: WEBSERVICE 0x100b38c0
	// RBX::FactoryProduct<RBX::DebugSettings,RBX::GlobalSettings::Item,&RBX::sDebugSettings>::`scalar deleting destructor'
	// STUB: WEBSERVICE 0x100b38e0
	// RBX::FactoryProduct<RBX::DebugSettings,RBX::GlobalSettings::Item,&RBX::sDebugSettings>::FactoryProduct<RBX::DebugSettings,RBX::GlobalSettings::Item,&RBX::sDebugSettings>
	// STUB: WEBSERVICE 0x100b3bc0
	// RBX::FactoryProduct<RBX::DebugSettings,RBX::GlobalSettings::Item,&RBX::sDebugSettings>::getClassName
	// STUB: WEBSERVICE 0x100bdef0
	// RBX::FactoryProduct<RBX::Team,RBX::Instance,&RBX::sTeam>::~FactoryProduct<RBX::Team,RBX::Instance,&RBX::sTeam>
	// STUB: WEBSERVICE 0x100bdfa0
	// RBX::FactoryProduct<RBX::Team,RBX::Instance,&RBX::sTeam>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x100be220
	// RBX::FactoryProduct<RBX::Team,RBX::Instance,&RBX::sTeam>::getClassName
	// STUB: WEBSERVICE 0x100c2710
	// RBX::FactoryProduct<RBX::Camera,RBX::Instance,&RBX::sCamera>::~FactoryProduct<RBX::Camera,RBX::Instance,&RBX::sCamera>
	// TEMPLATE: WEBSERVICE 0x100c2770
	// RBX::FactoryProduct<RBX::Camera,RBX::Instance,&RBX::sCamera>::FactoryProduct<RBX::Camera,RBX::Instance,&RBX::sCamera>
	// TEMPLATE: WEBSERVICE 0x100c3690
	// RBX::FactoryProduct<RBX::Camera,RBX::Instance,&RBX::sCamera>::getClassName
	// STUB: WEBSERVICE 0x100c4100
	// RBX::FactoryProduct<RBX::Camera,RBX::Instance,&RBX::sCamera>::`scalar deleting destructor'
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
	// STUB: WEBSERVICE 0x100d9f70
	// RBX::FactoryProduct<RBX::Rotate,RBX::AutoJoint,&RBX::sRotate>::`scalar deleting destructor'
	// STUB: WEBSERVICE 0x100da0e0
	// RBX::FactoryProduct<RBX::RotateP,RBX::AutoJoint,&RBX::sRotateP>::`scalar deleting destructor'
	// STUB: WEBSERVICE 0x100da260
	// RBX::FactoryProduct<RBX::RotateV,RBX::AutoJoint,&RBX::sRotateV>::`scalar deleting destructor'
	// STUB: WEBSERVICE 0x100de130
	// RBX::FactoryProduct<RBX::GameSettings,RBX::GlobalSettings::Item,&RBX::sGameSettings>::~FactoryProduct<RBX::GameSettings,RBX::GlobalSettings::Item,&RBX::sGameSettings>
	// STUB: WEBSERVICE 0x100de1b0
	// RBX::FactoryProduct<RBX::GameSettings,RBX::GlobalSettings::Item,&RBX::sGameSettings>::`scalar deleting destructor'
	// STUB: WEBSERVICE 0x100de1d0
	// RBX::FactoryProduct<RBX::GameSettings,RBX::GlobalSettings::Item,&RBX::sGameSettings>::FactoryProduct<RBX::GameSettings,RBX::GlobalSettings::Item,&RBX::sGameSettings>
	// TEMPLATE: WEBSERVICE 0x100de2e0
	// RBX::FactoryProduct<RBX::GameSettings,RBX::GlobalSettings::Item,&RBX::sGameSettings>::getClassName
	// STUB: WEBSERVICE 0x100e04a0
	// RBX::FactoryProduct<RBX::ShirtGraphic,RBX::CharacterAppearance,&RBX::sShirtGraphic>::~FactoryProduct<RBX::ShirtGraphic,RBX::CharacterAppearance,&RBX::sShirtGraphic>
	// STUB: WEBSERVICE 0x100e0530
	// RBX::FactoryProduct<RBX::BodyColors,RBX::CharacterAppearance,&RBX::sBodyColors>::~FactoryProduct<RBX::BodyColors,RBX::CharacterAppearance,&RBX::sBodyColors>
	// STUB: WEBSERVICE 0x100e05a0
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
	// TEMPLATE: WEBSERVICE 0x100e17e0
	// RBX::FactoryProduct<RBX::Shirt,RBX::ShirtGraphic,&RBX::sShirt>::FactoryProduct<RBX::Shirt,RBX::ShirtGraphic,&RBX::sShirt>
	// STUB: WEBSERVICE 0x100e4860
	// RBX::FactoryProduct<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature>::~FactoryProduct<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature>
	// STUB: WEBSERVICE 0x100e4900
	// RBX::FactoryProduct<RBX::Hole,RBX::Feature,&RBX::sHole>::~FactoryProduct<RBX::Hole,RBX::Feature,&RBX::sHole>
	// TEMPLATE: WEBSERVICE 0x100e4ee0
	// RBX::FactoryProduct<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature>::getClassName
	// TEMPLATE: WEBSERVICE 0x100e4f00
	// RBX::FactoryProduct<RBX::Hole,RBX::Feature,&RBX::sHole>::getClassName
	// STUB: WEBSERVICE 0x100e7350
	// RBX::FactoryProduct<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature>::FactoryProduct<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature>
	// STUB: WEBSERVICE 0x100e73c0
	// RBX::FactoryProduct<RBX::Hole,RBX::Feature,&RBX::sHole>::FactoryProduct<RBX::Hole,RBX::Feature,&RBX::sHole>
	// STUB: WEBSERVICE 0x100e7890
	// RBX::FactoryProduct<RBX::Hole,RBX::Feature,&RBX::sHole>::`scalar deleting destructor'
	// STUB: WEBSERVICE 0x100e7ab0
	// RBX::FactoryProduct<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature>::`scalar deleting destructor'
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
	// STUB: WEBSERVICE 0x100ffaa0
	// RBX::FactoryProduct<RBX::Texture,RBX::Decal,&RBX::sTexture>::~FactoryProduct<RBX::Texture,RBX::Decal,&RBX::sTexture>
	// TEMPLATE: WEBSERVICE 0x100ffb10
	// RBX::FactoryProduct<RBX::Texture,RBX::Decal,&RBX::sTexture>::getClassName
	// TEMPLATE: WEBSERVICE 0x10100110
	// RBX::FactoryProduct<RBX::Texture,RBX::Decal,&RBX::sTexture>::FactoryProduct<RBX::Texture,RBX::Decal,&RBX::sTexture>
	// STUB: WEBSERVICE 0x101004b0
	// RBX::FactoryProduct<RBX::Texture,RBX::Decal,&RBX::sTexture>::`scalar deleting destructor'
	// STUB: WEBSERVICE 0x10129f20
	// RBX::FactoryProduct<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape>::Creator::create
	// STUB: WEBSERVICE 0x1012c970
	// RBX::FactoryProduct<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape>::~FactoryProduct<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape>
	// TEMPLATE: WEBSERVICE 0x1012c9c0
	// RBX::FactoryProduct<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape>::FactoryProduct<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape>
	// STUB: WEBSERVICE 0x1012ca90
	// RBX::FactoryProduct<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x1012cef0
	// RBX::FactoryProduct<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape>::getClassName
	// clang-format on
	const Creator& getCreator() const { return creator; }

	static const Name& className() { return Name::declare<sName>(); }

	virtual const Name& getClassName() const { return className(); }

private:
	static Creator creator;
};

template <class T, class Base, const char* sName>
typename FactoryProduct<T, Base, sName>::Creator FactoryProduct<T, Base, sName>::creator;

template <class Base, const char* sName>
class NonFactoryProduct : public Base
{
public:
	static const Name& className() { return Name::declare<sName>(); }

	virtual const Name& getClassName() const { return className(); }
};

} // namespace RBX

#endif // UTIL_OBJECT_H
