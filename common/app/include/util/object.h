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

	// TEMPLATE: WEBSERVICE 0x100bdef0
	// RBX::FactoryProduct<RBX::Team,RBX::Instance,&RBX::sTeam>::~FactoryProduct<RBX::Team,RBX::Instance,&RBX::sTeam>
	// STUB: WEBSERVICE 0x100bdf40
	// RBX::FactoryProduct<RBX::Team,RBX::Instance,&RBX::sTeam>::FactoryProduct<RBX::Team,RBX::Instance,&RBX::sTeam>
	const Creator& getCreator() const { return creator; }

	static const Name& className() { return Name::declare<sName>(); }

	// TEMPLATE: WEBSERVICE 0x100c3690
	// RBX::FactoryProduct<RBX::Camera,RBX::Instance,&RBX::sCamera>::getClassName
	// TEMPLATE: WEBSERVICE 0x1008cc40
	// RBX::FactoryProduct<RBX::ObjectValue,RBX::Instance,&RBX::sObjectValue>::getClassName
	// TEMPLATE: WEBSERVICE 0x10069070
	// RBX::FactoryProduct<RBX::Script,RBX::Instance,&RBX::sScript>::getClassName
	// TEMPLATE: WEBSERVICE 0x1012cef0
	// RBX::FactoryProduct<RBX::SpecialShape,RBX::Instance,&RBX::sSpecialShape>::getClassName
	// TEMPLATE: WEBSERVICE 0x100e4f00
	// RBX::FactoryProduct<RBX::Hole,RBX::Feature,&RBX::sHole>::getClassName
	// TEMPLATE: WEBSERVICE 0x100e4ee0
	// RBX::FactoryProduct<RBX::MotorFeature,RBX::Feature,&RBX::sMotorFeature>::getClassName
	// TEMPLATE: WEBSERVICE 0x100e4f20
	// RBX::FactoryProduct<RBX::VelocityMotor,RBX::JointInstance,&RBX::sVelocityMotor>::getClassName
	// TEMPLATE: WEBSERVICE 0x10069090
	// RBX::FactoryProduct<RBX::LocalScript,RBX::Script,&RBX::sLocalScript>::getClassName
	// TEMPLATE: WEBSERVICE 0x1008c9c0
	// RBX::FactoryProduct<RBX::Hint,RBX::Message,&RBX::sHint>::getClassName
	// TEMPLATE: WEBSERVICE 0x100be220
	// RBX::FactoryProduct<RBX::Team,RBX::Instance,&RBX::sTeam>::getClassName
	virtual const Name& getClassName() const { return className(); }

private:
	static Creator creator;
};

template <class T, class Base, const char* sName>
typename FactoryProduct<T, Base, sName>::Creator FactoryProduct<T, Base, sName>::creator;

} // namespace RBX

#endif // UTIL_OBJECT_H
