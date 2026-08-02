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
	// STUB: WEBSERVICE 0x10069590
	// RBX::Reflection::Described<RBX::Script,&RBX::sScript,RBX::FactoryProduct<RBX::Script,RBX::Instance,&RBX::sScript> >::classDescriptor
	// STUB: WEBSERVICE 0x10069810
	// RBX::Reflection::Described<RBX::LocalScript,&RBX::sLocalScript,RBX::FactoryProduct<RBX::LocalScript,RBX::Script,&RBX::sLocalScript> >::classDescriptor
	// STUB: WEBSERVICE 0x10088f30
	// RBX::Reflection::Described<RBX::ShirtGraphic,&RBX::sShirtGraphic,RBX::FactoryProduct<RBX::ShirtGraphic,RBX::CharacterAppearance,&RBX::sShirtGraphic> >::classDescriptor
	// STUB: WEBSERVICE 0x10088fa0
	// RBX::Reflection::Described<RBX::BodyColors,&RBX::sBodyColors,RBX::FactoryProduct<RBX::BodyColors,RBX::CharacterAppearance,&RBX::sBodyColors> >::classDescriptor
	// STUB: WEBSERVICE 0x10089160
	// RBX::Reflection::Described<RBX::GameSettings,&RBX::sGameSettings,RBX::FactoryProduct<RBX::GameSettings,RBX::GlobalSettings::Item,&RBX::sGameSettings> >::classDescriptor
	// STUB: WEBSERVICE 0x10089390
	// RBX::Reflection::Described<RBX::Hint,&RBX::sHint,RBX::FactoryProduct<RBX::Hint,RBX::Message,&RBX::sHint> >::classDescriptor
	// STUB: WEBSERVICE 0x1008cc60
	// RBX::Reflection::Described<RBX::ObjectValue,&RBX::sObjectValue,RBX::FactoryProduct<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue> >::classDescriptor
	// STUB: WEBSERVICE 0x1008dbe0
	// RBX::Reflection::Described<RBX::MotorFeature,&RBX::sMotorFeature,RBX::FactoryProduct<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature> >::classDescriptor
	// STUB: WEBSERVICE 0x1008dc50
	// RBX::Reflection::Described<RBX::Hole,&RBX::sHole,RBX::FactoryProduct<RBX::Hole,RBX::Feature,&RBX::sHole> >::classDescriptor
	// STUB: WEBSERVICE 0x1008ded0
	// RBX::Reflection::Described<RBX::Snap,&RBX::sSnap,RBX::FactoryProduct<RBX::Snap,RBX::AutoJoint,&RBX::sSnap> >::classDescriptor
	// STUB: WEBSERVICE 0x1008df40
	// RBX::Reflection::Described<RBX::Weld,&RBX::sWeld,RBX::FactoryProduct<RBX::Weld,RBX::AutoJoint,&RBX::sWeld> >::classDescriptor
	// STUB: WEBSERVICE 0x1008dfb0
	// RBX::Reflection::Described<RBX::Glue,&RBX::sGlue,RBX::FactoryProduct<RBX::Glue,RBX::AutoJoint,&RBX::sGlue> >::classDescriptor
	// STUB: WEBSERVICE 0x1008e020
	// RBX::Reflection::Described<RBX::Rotate,&RBX::sRotate,RBX::FactoryProduct<RBX::Rotate,RBX::AutoJoint,&RBX::sRotate> >::classDescriptor
	// STUB: WEBSERVICE 0x1008e090
	// RBX::Reflection::Described<RBX::RotateP,&RBX::sRotateP,RBX::FactoryProduct<RBX::RotateP,RBX::AutoJoint,&RBX::sRotateP> >::classDescriptor
	// STUB: WEBSERVICE 0x1008e100
	// RBX::Reflection::Described<RBX::RotateV,&RBX::sRotateV,RBX::FactoryProduct<RBX::RotateV,RBX::AutoJoint,&RBX::sRotateV> >::classDescriptor
	// STUB: WEBSERVICE 0x100b42f0
	// RBX::Reflection::Described<RBX::DebugSettings,&RBX::sDebugSettings,RBX::FactoryProduct<RBX::DebugSettings,RBX::GlobalSettings::Item,&RBX::sDebugSettings> >::classDescriptor
	// STUB: WEBSERVICE 0x100be240
	// RBX::Reflection::Described<RBX::Team,&RBX::sTeam,RBX::FactoryProduct<RBX::Team,RBX::Instance,&RBX::sTeam> >::classDescriptor
	// STUB: WEBSERVICE 0x100c3a60
	// RBX::Reflection::Described<RBX::Camera,&RBX::sCamera,RBX::FactoryProduct<RBX::Camera,RBX::Instance,&RBX::sCamera> >::classDescriptor
	// STUB: WEBSERVICE 0x100e0d60
	// RBX::Reflection::Described<RBX::Shirt,&RBX::sShirt,RBX::FactoryProduct<RBX::Shirt,RBX::ShirtGraphic,&RBX::sShirt> >::classDescriptor
	// STUB: WEBSERVICE 0x100e0dd0
	// RBX::Reflection::Described<RBX::Skin,&RBX::sSkin,RBX::FactoryProduct<RBX::Skin,RBX::CharacterAppearance,&RBX::sSkin> >::classDescriptor
	// STUB: WEBSERVICE 0x100ec1a0
	// RBX::Reflection::Described<RBX::BodyPosition,&RBX::sBodyPosition,RBX::FactoryProduct<RBX::BodyPosition,RBX::BodyMover,&RBX::sBodyPosition> >::classDescriptor
	// STUB: WEBSERVICE 0x100ec210
	// RBX::Reflection::Described<RBX::BodyVelocity,&RBX::sBodyVelocity,RBX::FactoryProduct<RBX::BodyVelocity,RBX::BodyMover,&RBX::sBodyVelocity> >::classDescriptor
	// STUB: WEBSERVICE 0x100ec5c0
	// RBX::Reflection::Described<RBX::BodyGyro,&RBX::sBodyGyro,RBX::FactoryProduct<RBX::BodyGyro,RBX::BodyMover,&RBX::sBodyGyro> >::classDescriptor
	// STUB: WEBSERVICE 0x100ec630
	// RBX::Reflection::Described<RBX::BodyForce,&RBX::sBodyForce,RBX::FactoryProduct<RBX::BodyForce,RBX::BodyMover,&RBX::sBodyForce> >::classDescriptor
	// STUB: WEBSERVICE 0x100ec6a0
	// RBX::Reflection::Described<RBX::BodyThrust,&RBX::sBodyThrust,RBX::FactoryProduct<RBX::BodyThrust,RBX::BodyMover,&RBX::sBodyThrust> >::classDescriptor
	// STUB: WEBSERVICE 0x100ec710
	// RBX::Reflection::Described<RBX::Rocket,&RBX::sRocket,RBX::FactoryProduct<RBX::Rocket,RBX::BodyMover,&RBX::sRocket> >::classDescriptor
	// STUB: WEBSERVICE 0x100ffc20
	// RBX::Reflection::Described<RBX::Texture,&RBX::sTexture,RBX::FactoryProduct<RBX::Texture,RBX::Decal,&RBX::sTexture> >::classDescriptor
	// STUB: WEBSERVICE 0x1012d2d0
	// RBX::Reflection::Described<RBX::SpecialShape,&RBX::sSpecialShape,RBX::FactoryProduct<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape> >::classDescriptor
	// clang-format on
	static ClassDescriptor& classDescriptor() { return ClassDescriptor::rootDescriptor(); }

	virtual ~DescribedBase() {} // vtable+0x00

protected:
	// const ClassDescriptor& descriptor
	undefined m_unk0x08[0x0c - 0x08]; // 0x08
};

DECOMP_SIZE_ASSERT(DescribedBase, 0x0c)

template <class T, const char* sName, class Base>
class Described : public Base
{
public:
	static ClassDescriptor& classDescriptor()
	{
		static ClassDescriptor d(Base::classDescriptor(), sName);

		return d;
	}
};

} // namespace Reflection
} // namespace RBX

#endif // REFLECTION_REFLECTION_H
