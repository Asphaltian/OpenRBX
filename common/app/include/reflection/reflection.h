#ifndef REFLECTION_REFLECTION_H
#define REFLECTION_REFLECTION_H

#include "decomp.h"
#include "reflection/object.h"

namespace RBX {
namespace Reflection {

// SIZE 0x0c
class DescribedBase
{
public:
	static ClassDescriptor& classDescriptor() { return ClassDescriptor::rootDescriptor(); }

	virtual ~DescribedBase() {} // vtable+0x00
};

template <class T, const char* sName, class Base>
class Described : public Base
{
public:
	// STUB: WEBSERVICE 0x100be240
	// RBX::Reflection::Described<RBX::Team,&RBX::sTeam,RBX::FactoryProduct<RBX::Team,RBX::Instance,&RBX::sTeam>
	// >::classDescriptor
	static ClassDescriptor& classDescriptor()
	{
		static ClassDescriptor d(Base::classDescriptor(), sName);

		return d;
	}
};

} // namespace Reflection
} // namespace RBX

#endif // REFLECTION_REFLECTION_H
