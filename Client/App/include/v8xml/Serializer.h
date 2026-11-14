#ifndef V8XML_SERIALIZER_H
#define V8XML_SERIALIZER_H

#include "placeholder.h"

class ArchiveBinder;
class XmlNameValuePair;

namespace RBX {

class Instance;
class InstanceHandle;

namespace Reflection {
class DescribedBase;
}

class __declspec(novtable) IIDREF
{
	friend class ::ArchiveBinder;
	friend class MergeBinder;

private:
	virtual void assignIDREF(Reflection::DescribedBase* propertyOwner, const InstanceHandle& value) const = 0;
	// vtable+0x00
};

class __declspec(novtable) IReferenceBinder
{
public:
	virtual void announceID(const XmlNameValuePair* valueID, Instance* instance) = 0;

	virtual void announceIDREF(
		const XmlNameValuePair* valueIDREF, Reflection::DescribedBase* propertyOwner, const IIDREF* idref
	) = 0;
};

} // namespace RBX

#endif // V8XML_SERIALIZER_H
