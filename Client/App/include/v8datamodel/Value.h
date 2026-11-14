#ifndef V8DATAMODEL_VALUE_H
#define V8DATAMODEL_VALUE_H

#include "placeholder.h"
#include "reflection/property.h"
#include "reflection/signal.h"
#include "v8tree/Instance.h"

#include <boost/shared_ptr.hpp>

namespace RBX {

extern char sBrickColorValue[];

extern char sColor3Value[];

extern char sCFrameValue[];

extern char sVector3Value[];

extern char sStringValue[];

extern char sDoubleValue[];

extern char sBoolValue[];

extern char sIntValue[];

extern char sObjectValue[];

template <class T, char* sName>
class Value : public DescribedCreatable<Value<T, sName>, Instance, sName>
{
private:
	T value;

	static const T defaultValue;

public:
	static Reflection::BoundProp<T, 1> desc_Value;
	static Reflection::SignalDesc<Value, void(T)> desc_ValueChanged;

	Value() : value(defaultValue) { this->setName("Value"); }

	const T& getValue() const { return value; }
	void setValue(const T& value);

protected:
	virtual bool askSetParent(const Instance* instance) const { return true; }

private:
	void onValueChanged(const Reflection::PropertyDescriptor&) { desc_ValueChanged.fire(this, value); }
};

class ObjectValue : public DescribedCreatable<ObjectValue, Instance, sObjectValue>
{
private:
	boost::shared_ptr<Instance> value;

public:
	static Reflection::RefPropDescriptor<ObjectValue, Instance> desc_Value;
	static Reflection::SignalDesc<ObjectValue, void(boost::shared_ptr<Instance>)> desc_ValueChanged;

	ObjectValue() { setName("Value"); }

	Instance* getValue() const { return value.get(); }
	void setValue(Instance* value);

protected:
	virtual bool askSetParent(const Instance* instance) const { return true; }

private:
	void onValueChanged(const Reflection::PropertyDescriptor& descriptor);

public:
};

void registerValueClasses();

} // namespace RBX

#endif // V8DATAMODEL_VALUE_H
