#ifndef V8DATAMODEL_VALUE_H
#define V8DATAMODEL_VALUE_H

#include "decomp.h"
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

// SIZE 0x100
template <class T, char* sName>
class Value : public DescribedCreatable<Value<T, sName>, Instance, sName>
{
private:
	T value; // 0xf8

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

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x1008e6a0
	// RBX::ObjectValue::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x100f44d0
	// RBX::Value<int,&RBX::sIntValue>::Value<int,&RBX::sIntValue>
	// TEMPLATE: WEBSERVICE 0x100f45f0
	// RBX::Value<bool,&RBX::sBoolValue>::Value<bool,&RBX::sBoolValue>
	// TEMPLATE: WEBSERVICE 0x100f46f0
	// RBX::Value<double,&RBX::sDoubleValue>::Value<double,&RBX::sDoubleValue>
	// TEMPLATE: WEBSERVICE 0x100f47c0
	// RBX::Value<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,&RBX::sStringValue>::Value<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,&RBX::sStringValue>
	// TEMPLATE: WEBSERVICE 0x100f4910
	// RBX::Value<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,&RBX::sStringValue>::~Value<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,&RBX::sStringValue>
	// TEMPLATE: WEBSERVICE 0x100f4960
	// RBX::Value<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,&RBX::sStringValue>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x100f4980
	// RBX::Value<G3D::Vector3,&RBX::sVector3Value>::Value<G3D::Vector3,&RBX::sVector3Value>
	// TEMPLATE: WEBSERVICE 0x100f4aa0
	// RBX::Value<G3D::CoordinateFrame,&RBX::sCFrameValue>::Value<G3D::CoordinateFrame,&RBX::sCFrameValue>
	// TEMPLATE: WEBSERVICE 0x100f4bf0
	// RBX::Value<G3D::Color3,&RBX::sColor3Value>::Value<G3D::Color3,&RBX::sColor3Value>
	// TEMPLATE: WEBSERVICE 0x100f4cf0
	// RBX::Value<RBX::BrickColor,&RBX::sBrickColorValue>::Value<RBX::BrickColor,&RBX::sBrickColorValue>
	// TEMPLATE: WEBSERVICE 0x100f72a0
	// RBX::Value<int,&RBX::sIntValue>::onValueChanged
	// TEMPLATE: WEBSERVICE 0x100f72e0
	// RBX::Value<bool,&RBX::sBoolValue>::onValueChanged
	// TEMPLATE: WEBSERVICE 0x100f7320
	// RBX::Value<double,&RBX::sDoubleValue>::onValueChanged
	// TEMPLATE: WEBSERVICE 0x100f7370
	// RBX::Value<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,&RBX::sStringValue>::onValueChanged
	// TEMPLATE: WEBSERVICE 0x100f73b0
	// RBX::Value<G3D::Vector3,&RBX::sVector3Value>::onValueChanged
	// TEMPLATE: WEBSERVICE 0x100f7420
	// RBX::Value<G3D::CoordinateFrame,&RBX::sCFrameValue>::onValueChanged
	// TEMPLATE: WEBSERVICE 0x100f74b0
	// RBX::Value<G3D::Color3,&RBX::sColor3Value>::onValueChanged
	// TEMPLATE: WEBSERVICE 0x100f7520
	// RBX::Value<RBX::BrickColor,&RBX::sBrickColorValue>::onValueChanged
	// clang-format on
};

// SIZE 0x100
class ObjectValue : public DescribedCreatable<ObjectValue, Instance, sObjectValue>
{
private:
	boost::shared_ptr<Instance> value; // 0xf8

public:
	static Reflection::RefPropDescriptor<ObjectValue, Instance> desc_Value;
	static Reflection::SignalDesc<ObjectValue, void(boost::shared_ptr<Instance>)> desc_ValueChanged;

	ObjectValue() { setName("Value"); }

	// FUNCTION: WEBSERVICE 0x10068b70 FOLDED
	Instance* getValue() const { return value.get(); }
	void setValue(Instance* value);

protected:
	virtual bool askSetParent(const Instance* instance) const { return true; }

private:
	void onValueChanged(const Reflection::PropertyDescriptor& descriptor);

public:
	// SYNTHETIC: WEBSERVICE 0x1008e620
	// RBX::ObjectValue::~ObjectValue
};

DECOMP_SIZE_ASSERT(ObjectValue, 0x100)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x1021d570
// `dynamic initializer for 'RBX::Value<int,&RBX::sIntValue>::desc_Value''
// SYNTHETIC: WEBSERVICE 0x1021d5a0
// `dynamic initializer for 'RBX::Value<int,&RBX::sIntValue>::desc_ValueChanged''
// SYNTHETIC: WEBSERVICE 0x1021d5c0
// `dynamic initializer for 'RBX::Value<bool,&RBX::sBoolValue>::desc_Value''
// SYNTHETIC: WEBSERVICE 0x1021d5f0
// `dynamic initializer for 'RBX::Value<bool,&RBX::sBoolValue>::desc_ValueChanged''
// SYNTHETIC: WEBSERVICE 0x1021d610
// `dynamic initializer for 'RBX::Value<double,&RBX::sDoubleValue>::desc_Value''
// SYNTHETIC: WEBSERVICE 0x1021d640
// `dynamic initializer for 'RBX::Value<double,&RBX::sDoubleValue>::desc_ValueChanged''
// SYNTHETIC: WEBSERVICE 0x1021d660
// `dynamic initializer for 'RBX::Value<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,&RBX::sStringValue>::defaultValue''
// SYNTHETIC: WEBSERVICE 0x1021d680
// `dynamic initializer for 'RBX::Value<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,&RBX::sStringValue>::desc_Value''
// SYNTHETIC: WEBSERVICE 0x1021d6b0
// `dynamic initializer for 'RBX::Value<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,&RBX::sStringValue>::desc_ValueChanged''
// SYNTHETIC: WEBSERVICE 0x1021d6d0
// `dynamic initializer for 'RBX::Value<G3D::Vector3,&RBX::sVector3Value>::desc_Value''
// SYNTHETIC: WEBSERVICE 0x1021d700
// `dynamic initializer for 'RBX::Value<G3D::Vector3,&RBX::sVector3Value>::desc_ValueChanged''
// SYNTHETIC: WEBSERVICE 0x1021d720
// `dynamic initializer for 'RBX::Value<G3D::CoordinateFrame,&RBX::sCFrameValue>::defaultValue''
// SYNTHETIC: WEBSERVICE 0x1021d740
// `dynamic initializer for 'RBX::Value<G3D::CoordinateFrame,&RBX::sCFrameValue>::desc_Value''
// SYNTHETIC: WEBSERVICE 0x1021d770
// `dynamic initializer for 'RBX::Value<G3D::CoordinateFrame,&RBX::sCFrameValue>::desc_ValueChanged''
// SYNTHETIC: WEBSERVICE 0x1021d790
// `dynamic initializer for 'RBX::Value<G3D::Color3,&RBX::sColor3Value>::desc_Value''
// SYNTHETIC: WEBSERVICE 0x1021d7c0
// `dynamic initializer for 'RBX::Value<G3D::Color3,&RBX::sColor3Value>::desc_ValueChanged''
// SYNTHETIC: WEBSERVICE 0x1021d7e0
// `dynamic initializer for 'RBX::Value<RBX::BrickColor,&RBX::sBrickColorValue>::defaultValue''
// SYNTHETIC: WEBSERVICE 0x1021d7f0
// `dynamic initializer for 'RBX::Value<RBX::BrickColor,&RBX::sBrickColorValue>::desc_Value''
// SYNTHETIC: WEBSERVICE 0x1021d820
// `dynamic initializer for 'RBX::Value<RBX::BrickColor,&RBX::sBrickColorValue>::desc_ValueChanged''
// SYNTHETIC: WEBSERVICE 0x1021d840
// `dynamic initializer for 'RBX::ObjectValue::desc_Value''
// SYNTHETIC: WEBSERVICE 0x1021d880
// `dynamic initializer for 'RBX::ObjectValue::desc_ValueChanged''
// SYNTHETIC: WEBSERVICE 0x10224790
// `dynamic atexit destructor for 'RBX::Value<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,&RBX::sStringValue>::defaultValue''
// SYNTHETIC: WEBSERVICE 0x102247a0
// `dynamic atexit destructor for 'RBX::Value<G3D::CoordinateFrame,&RBX::sCFrameValue>::defaultValue''
// SYNTHETIC: WEBSERVICE 0x102247b0
// `dynamic atexit destructor for 'RBX::ObjectValue::desc_Value''
// SYNTHETIC: WEBSERVICE 0x102247d0
// `dynamic atexit destructor for 'RBX::Value<int,&RBX::sIntValue>::desc_Value''
// SYNTHETIC: WEBSERVICE 0x102247f0
// `dynamic atexit destructor for 'RBX::Value<bool,&RBX::sBoolValue>::desc_Value''
// SYNTHETIC: WEBSERVICE 0x10224810
// `dynamic atexit destructor for 'RBX::Value<double,&RBX::sDoubleValue>::desc_Value''
// SYNTHETIC: WEBSERVICE 0x10224830
// `dynamic atexit destructor for 'RBX::Value<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,&RBX::sStringValue>::desc_Value''
// SYNTHETIC: WEBSERVICE 0x10224850
// `dynamic atexit destructor for 'RBX::Value<G3D::Vector3,&RBX::sVector3Value>::desc_Value''
// SYNTHETIC: WEBSERVICE 0x10224870
// `dynamic atexit destructor for 'RBX::Value<G3D::CoordinateFrame,&RBX::sCFrameValue>::desc_Value''
// SYNTHETIC: WEBSERVICE 0x10224890
// `dynamic atexit destructor for 'RBX::Value<G3D::Color3,&RBX::sColor3Value>::desc_Value''
// SYNTHETIC: WEBSERVICE 0x102248b0
// `dynamic atexit destructor for 'RBX::Value<RBX::BrickColor,&RBX::sBrickColorValue>::desc_Value''
// SYNTHETIC: WEBSERVICE 0x102248d0
// `RBX::Reflection::Described<RBX::Value<RBX::BrickColor,&RBX::sBrickColorValue>,&RBX::sBrickColorValue,RBX::FactoryProduct<RBX::Value<RBX::BrickColor,&RBX::sBrickColorValue>,RBX::Instance,&RBX::sBrickColorValue> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x102248e0
// `RBX::Reflection::Described<RBX::Value<G3D::Color3,&RBX::sColor3Value>,&RBX::sColor3Value,RBX::FactoryProduct<RBX::Value<G3D::Color3,&RBX::sColor3Value>,RBX::Instance,&RBX::sColor3Value> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x102248f0
// `RBX::Reflection::Described<RBX::Value<G3D::CoordinateFrame,&RBX::sCFrameValue>,&RBX::sCFrameValue,RBX::FactoryProduct<RBX::Value<G3D::CoordinateFrame,&RBX::sCFrameValue>,RBX::Instance,&RBX::sCFrameValue> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10224900
// `RBX::Reflection::Described<RBX::Value<G3D::Vector3,&RBX::sVector3Value>,&RBX::sVector3Value,RBX::FactoryProduct<RBX::Value<G3D::Vector3,&RBX::sVector3Value>,RBX::Instance,&RBX::sVector3Value> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10224910
// `RBX::Reflection::Described<RBX::Value<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,&RBX::sStringValue>,&RBX::sStringValue,RBX::FactoryProduct<RBX::Value<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,&RBX::sStringValue>,RBX::Instance,&RBX::sStringValue> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10224920
// `RBX::Reflection::Described<RBX::Value<double,&RBX::sDoubleValue>,&RBX::sDoubleValue,RBX::FactoryProduct<RBX::Value<double,&RBX::sDoubleValue>,RBX::Instance,&RBX::sDoubleValue> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10224930
// `RBX::Reflection::Described<RBX::Value<bool,&RBX::sBoolValue>,&RBX::sBoolValue,RBX::FactoryProduct<RBX::Value<bool,&RBX::sBoolValue>,RBX::Instance,&RBX::sBoolValue> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10224940
// `RBX::Reflection::Described<RBX::Value<int,&RBX::sIntValue>,&RBX::sIntValue,RBX::FactoryProduct<RBX::Value<int,&RBX::sIntValue>,RBX::Instance,&RBX::sIntValue> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x10224b50
// `dynamic atexit destructor for 'RBX::Value<bool,&RBX::sBoolValue>::desc_ValueChanged''
// SYNTHETIC: WEBSERVICE 0x10224b60
// `dynamic atexit destructor for 'RBX::Value<double,&RBX::sDoubleValue>::desc_ValueChanged''
// SYNTHETIC: WEBSERVICE 0x10224b70
// `dynamic atexit destructor for 'RBX::Value<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,&RBX::sStringValue>::desc_ValueChanged''
// SYNTHETIC: WEBSERVICE 0x10224b80
// `dynamic atexit destructor for 'RBX::Value<G3D::Vector3,&RBX::sVector3Value>::desc_ValueChanged''
// SYNTHETIC: WEBSERVICE 0x10224b90
// `dynamic atexit destructor for 'RBX::Value<G3D::CoordinateFrame,&RBX::sCFrameValue>::desc_ValueChanged''
// SYNTHETIC: WEBSERVICE 0x10224ba0
// `dynamic atexit destructor for 'RBX::Value<G3D::Color3,&RBX::sColor3Value>::desc_ValueChanged''
// SYNTHETIC: WEBSERVICE 0x10224bb0
// `dynamic atexit destructor for 'RBX::Value<RBX::BrickColor,&RBX::sBrickColorValue>::desc_ValueChanged''
// SYNTHETIC: WEBSERVICE 0x10224bc0
// `dynamic atexit destructor for 'RBX::ObjectValue::desc_ValueChanged''
// SYNTHETIC: WEBSERVICE 0x10224bd0
// `dynamic atexit destructor for 'RBX::Value<int,&RBX::sIntValue>::desc_ValueChanged''
// clang-format on

void registerValueClasses();

} // namespace RBX

#endif // V8DATAMODEL_VALUE_H
