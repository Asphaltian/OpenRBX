#ifndef V8XML_SERIALIZERV2_H
#define V8XML_SERIALIZERV2_H

#include "placeholder.h"
#include "util/Handle.h"
#include "util/object.h"
#include "v8tree/Instance.h"
#include "v8xml/Serializer.h"
#include "v8xml/XmlElement.h"

#include <iosfwd>
#include <vector>

class XmlElement;

namespace RBX {

class DataModel;

class MergeBinder : public IReferenceBinder
{
public:
	struct IDREFItem
	{
		const IIDREF* idref;
		Reflection::DescribedBase* propertyOwner;
		InstanceHandle value;
	};

	virtual bool resolveRefs()
	{
		for (std::vector<IDREFItem>::iterator it = deferredIDREFItems.begin(); it != deferredIDREFItems.end(); ++it) {
			it->idref->assignIDREF(it->propertyOwner, it->value);
		}

		deferredIDREFItems.clear();

		return true;
	}

	virtual void announceID(const XmlNameValuePair* valueID, Instance* target) { processID(valueID, target); }

	virtual void announceIDREF(
		const XmlNameValuePair* valueIDREF, Reflection::DescribedBase* propertyOwner, const IIDREF* idref
	)
	{
		processIDREF(valueIDREF, propertyOwner, idref);
	}

protected:
	virtual bool processID(const XmlNameValuePair* valueID, Instance* source)
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

	virtual bool processIDREF(
		const XmlNameValuePair* valueIDREF, Reflection::DescribedBase* propertyOwner, const IIDREF* idref
	)
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

private:
	std::vector<IDREFItem> deferredIDREFItems;
};

} // namespace RBX

class SerializerV2
{
protected:
	int schemaVersionLoading;

public:
	static XmlElement* newRootElement();

	static void isolateHandles(XmlElement* root);

	void merge(const XmlElement* root, RBX::DataModel* dataModel);

	void loadInstances(XmlElement* root, std::vector<boost::shared_ptr<RBX::Instance> >& instances);

	static void load(XmlElement* root, RBX::DataModel* dataModel);
	void load(std::istream& stream, RBX::DataModel* dataModel);
	void loadXML(std::istream& stream, RBX::DataModel* dataModel);
};

class Serializer : public SerializerV2
{
};

#endif // V8XML_SERIALIZERV2_H
