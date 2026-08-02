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
		// TEMPLATE: WEBSERVICE 0x10069270
		// RBX::FactoryProduct<RBX::Team,RBX::Instance,&RBX::sTeam>::Creator::create
		T* create() const { return new T(); }
	};

	// STUB: WEBSERVICE 0x100bdf40
	// RBX::FactoryProduct<RBX::Team,RBX::Instance,&RBX::sTeam>::FactoryProduct<RBX::Team,RBX::Instance,&RBX::sTeam>
	const Creator& getCreator() const { return creator; }

	static const Name& className() { return Name::declare<sName>(); }

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x10069070
	// RBX::FactoryProduct<RBX::Script,RBX::Instance,&RBX::sScript>::getClassName
	// TEMPLATE: WEBSERVICE 0x10069090
	// RBX::FactoryProduct<RBX::LocalScript,RBX::Script,&RBX::sLocalScript>::getClassName
	// TEMPLATE: WEBSERVICE 0x1008c9c0
	// RBX::FactoryProduct<RBX::Hint,RBX::Message,&RBX::sHint>::getClassName
	// TEMPLATE: WEBSERVICE 0x1008cc40
	// RBX::FactoryProduct<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue>::getClassName
	// TEMPLATE: WEBSERVICE 0x100b3bc0
	// RBX::FactoryProduct<RBX::DebugSettings,RBX::GlobalSettings::Item,&RBX::sDebugSettings>::getClassName
	// TEMPLATE: WEBSERVICE 0x100be220
	// RBX::FactoryProduct<RBX::Team,RBX::Instance,&RBX::sTeam>::getClassName
	// TEMPLATE: WEBSERVICE 0x100c3690
	// RBX::FactoryProduct<RBX::Camera,RBX::Instance,&RBX::sCamera>::getClassName
	// TEMPLATE: WEBSERVICE 0x100d8370
	// RBX::FactoryProduct<RBX::Snap,RBX::AutoJoint,&RBX::sSnap>::getClassName
	// TEMPLATE: WEBSERVICE 0x100d8410
	// RBX::FactoryProduct<RBX::Weld,RBX::AutoJoint,&RBX::sWeld>::getClassName
	// TEMPLATE: WEBSERVICE 0x100d84c0
	// RBX::FactoryProduct<RBX::Glue,RBX::AutoJoint,&RBX::sGlue>::getClassName
	// TEMPLATE: WEBSERVICE 0x100d8550
	// RBX::FactoryProduct<RBX::Rotate,RBX::AutoJoint,&RBX::sRotate>::getClassName
	// TEMPLATE: WEBSERVICE 0x100d8610
	// RBX::FactoryProduct<RBX::RotateP,RBX::AutoJoint,&RBX::sRotateP>::getClassName
	// TEMPLATE: WEBSERVICE 0x100d86b0
	// RBX::FactoryProduct<RBX::RotateV,RBX::AutoJoint,&RBX::sRotateV>::getClassName
	// TEMPLATE: WEBSERVICE 0x100de2e0
	// RBX::FactoryProduct<RBX::GameSettings,RBX::GlobalSettings::Item,&RBX::sGameSettings>::getClassName
	// TEMPLATE: WEBSERVICE 0x100e0cd0
	// RBX::FactoryProduct<RBX::ShirtGraphic,RBX::CharacterAppearance,&RBX::sShirtGraphic>::getClassName
	// TEMPLATE: WEBSERVICE 0x100e0cf0
	// RBX::FactoryProduct<RBX::Shirt,RBX::ShirtGraphic,&RBX::sShirt>::getClassName
	// TEMPLATE: WEBSERVICE 0x100e0d10
	// RBX::FactoryProduct<RBX::BodyColors,RBX::CharacterAppearance,&RBX::sBodyColors>::getClassName
	// TEMPLATE: WEBSERVICE 0x100e0d30
	// RBX::FactoryProduct<RBX::Skin,RBX::CharacterAppearance,&RBX::sSkin>::getClassName
	// TEMPLATE: WEBSERVICE 0x100e4ee0
	// RBX::FactoryProduct<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature>::getClassName
	// TEMPLATE: WEBSERVICE 0x100e4f00
	// RBX::FactoryProduct<RBX::Hole,RBX::Feature,&RBX::sHole>::getClassName
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
	// TEMPLATE: WEBSERVICE 0x100ffb10
	// RBX::FactoryProduct<RBX::Texture,RBX::Decal,&RBX::sTexture>::getClassName
	// TEMPLATE: WEBSERVICE 0x1012cef0
	// RBX::FactoryProduct<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape>::getClassName
	// clang-format on
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
