#ifndef V8XML_SERIALIZER_H
#define V8XML_SERIALIZER_H

#include "decomp.h"

class XmlNameValuePair;

namespace RBX {

class Instance;
class InstanceHandle;

namespace Reflection {
class DescribedBase;
}

// SIZE 0x04
class __declspec(novtable) IIDREF
{
public:
	virtual void assignIDREF(Reflection::DescribedBase* propertyOwner, const InstanceHandle& value) const = 0;
	// vtable+0x00
};

// SIZE 0x04
class __declspec(novtable) IReferenceBinder
{
public:
	virtual void announceID(const XmlNameValuePair* valueID, Instance* instance) = 0; // vtable+0x00

	virtual void announceIDREF(
		const XmlNameValuePair* valueIDREF,
		Reflection::DescribedBase* propertyOwner,
		const IIDREF* idref
	) = 0; // vtable+0x04
};

} // namespace RBX

#endif // V8XML_SERIALIZER_H
