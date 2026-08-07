#ifndef V8XML_SERIALIZERV2_H
#define V8XML_SERIALIZERV2_H

#include "decomp.h"
#include "util/Handle.h"
#include "util/object.h"
#include "v8tree/Instance.h"
#include "v8xml/Serializer.h"
#include "v8xml/XmlElement.h"

#include <vector>

class XmlElement;

namespace RBX {

class DataModel;

// SIZE 0x14
class __declspec(novtable) MergeBinder : public IReferenceBinder
{
public:
	// SIZE 0x10
	struct IDREFItem
	{
		const IIDREF* idref;                      // 0x00
		Reflection::DescribedBase* propertyOwner; // 0x04
		InstanceHandle value;                     // 0x08
	};

	// STUB: WEBSERVICE 0x10096f20
	virtual bool resolveRefs() // vtable+0x08
	{
		for (std::vector<IDREFItem>::iterator it = deferredIDREFItems.begin(); it != deferredIDREFItems.end(); ++it) {
			it->idref->assignIDREF(it->propertyOwner, it->value);
		}

		deferredIDREFItems.clear();

		return true;
	}

	// FUNCTION: WEBSERVICE 0x10097030
	virtual void announceID(const XmlNameValuePair* valueID, Instance* target) // vtable+0x00
	{
		processID(valueID, target);
	}

	// FUNCTION: WEBSERVICE 0x10097040
	virtual void announceIDREF(
		const XmlNameValuePair* valueIDREF,
		Reflection::DescribedBase* propertyOwner,
		const IIDREF* idref
	) // vtable+0x04
	{
		processIDREF(valueIDREF, propertyOwner, idref);
	}

protected:
	// FUNCTION: WEBSERVICE 0x10097200
	virtual bool processID(const XmlNameValuePair* valueID, Instance* source) // vtable+0x0c
	{
		InstanceHandle h;

		if (valueID->getValue(h)) {
			h.linkTo(shared_from(source));
			return true;
		}
		else if (valueID->isValueEqual(&value_IDREF_nil)) {
			return true;
		}

		return false;
	}

	// FUNCTION: WEBSERVICE 0x100979e0
	virtual bool processIDREF(
		const XmlNameValuePair* valueIDREF,
		Reflection::DescribedBase* propertyOwner,
		const IIDREF* idref
	) // vtable+0x10
	{
		InstanceHandle value;

		if (valueIDREF->getValue(value)) {
			if (!value.empty()) {
				idref->assignIDREF(propertyOwner, value);
			}
			else {
				IDREFItem item = {idref, propertyOwner, value};

				deferredIDREFItems.push_back(item);
			}

			return true;
		}

		if (valueIDREF->isValueEqual(&value_IDREF_nil)) {
			return true;
		}

		return false;
	}

protected:
	std::vector<IDREFItem> deferredIDREFItems; // 0x04
};

DECOMP_SIZE_ASSERT(MergeBinder, 0x14)
DECOMP_SIZE_ASSERT(MergeBinder::IDREFItem, 0x10)

} // namespace RBX

// SIZE 0x04
class SerializerV2
{
public:
	static XmlElement* newRootElement();

	static void load(XmlElement* root, RBX::DataModel* dataModel);

	static void isolateHandles(XmlElement* root);

private:
	int schemaVersionLoading; // 0x00
};

DECOMP_SIZE_ASSERT(SerializerV2, 0x04)

#endif // V8XML_SERIALIZERV2_H
