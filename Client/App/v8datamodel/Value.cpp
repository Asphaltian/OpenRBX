#include "v8datamodel/Value.h"

#include "util/Name.h"
#include "v8datamodel/BrickColor.h"

#include <G3D/Color3.h>
#include <G3D/CoordinateFrame.h>
#include <G3D/Vector3.h>
#include <string>

namespace RBX {

char sBrickColorValue[] = "BrickColorValue";

char sColor3Value[] = "Color3Value";

char sCFrameValue[] = "CFrameValue";

char sVector3Value[] = "Vector3Value";

char sStringValue[] = "StringValue";

char sDoubleValue[] = "NumberValue";

char sBoolValue[] = "BoolValue";

char sIntValue[] = "IntValue";

char sObjectValue[] = "ObjectValue";

template <>
const int Value<int, sIntValue>::defaultValue = 0;
template <>
Reflection::BoundProp<int, 1> Value<int, sIntValue>::desc_Value(
	"Value", "Data", &Value<int, sIntValue>::value, &Value<int, sIntValue>::onValueChanged
);

template <>
Reflection::SignalDesc<Value<int, sIntValue>, void(int)> Value<int, sIntValue>::desc_ValueChanged("Changed", "value");

template <>
const bool Value<bool, sBoolValue>::defaultValue = false;
template <>
Reflection::BoundProp<bool, 1> Value<bool, sBoolValue>::desc_Value(
	"Value", "Data", &Value<bool, sBoolValue>::value, &Value<bool, sBoolValue>::onValueChanged
);

template <>
Reflection::SignalDesc<Value<bool, sBoolValue>, void(bool)> Value<bool, sBoolValue>::desc_ValueChanged(
	"Changed", "value"
);

template <>
const double Value<double, sDoubleValue>::defaultValue = 0.0;
template <>
Reflection::BoundProp<double, 1> Value<double, sDoubleValue>::desc_Value(
	"Value", "Data", &Value<double, sDoubleValue>::value, &Value<double, sDoubleValue>::onValueChanged
);

template <>
Reflection::SignalDesc<Value<double, sDoubleValue>, void(double)> Value<double, sDoubleValue>::desc_ValueChanged(
	"Changed", "value"
);

template <>
const std::string Value<std::string, sStringValue>::defaultValue = std::string();
template <>
Reflection::BoundProp<std::string, 1> Value<std::string, sStringValue>::desc_Value(
	"Value", "Data", &Value<std::string, sStringValue>::value, &Value<std::string, sStringValue>::onValueChanged
);

template <>
Reflection::SignalDesc<Value<std::string, sStringValue>, void(std::string)> Value<std::string, sStringValue>::
	desc_ValueChanged("Changed", "value");

template <>
const G3D::Vector3 Value<G3D::Vector3, sVector3Value>::defaultValue = G3D::Vector3(0.0f, 0.0f, 0.0f);
template <>
Reflection::BoundProp<G3D::Vector3, 1> Value<G3D::Vector3, sVector3Value>::desc_Value(
	"Value", "Data", &Value<G3D::Vector3, sVector3Value>::value, &Value<G3D::Vector3, sVector3Value>::onValueChanged
);

template <>
Reflection::SignalDesc<Value<G3D::Vector3, sVector3Value>, void(G3D::Vector3)> Value<G3D::Vector3, sVector3Value>::
	desc_ValueChanged("Changed", "value");

template <>
const G3D::CoordinateFrame Value<G3D::CoordinateFrame, sCFrameValue>::defaultValue = G3D::CoordinateFrame();
template <>
Reflection::BoundProp<G3D::CoordinateFrame, 1> Value<G3D::CoordinateFrame, sCFrameValue>::desc_Value(
	"Value",
	"Data",
	&Value<G3D::CoordinateFrame, sCFrameValue>::value,
	&Value<G3D::CoordinateFrame, sCFrameValue>::onValueChanged
);

template <>
Reflection::SignalDesc<Value<G3D::CoordinateFrame, sCFrameValue>, void(G3D::CoordinateFrame)> Value<
	G3D::CoordinateFrame,
	sCFrameValue>::desc_ValueChanged("Changed", "value");

template <>
const G3D::Color3 Value<G3D::Color3, sColor3Value>::defaultValue = G3D::Color3();
template <>
Reflection::BoundProp<G3D::Color3, 1> Value<G3D::Color3, sColor3Value>::desc_Value(
	"Value", "Data", &Value<G3D::Color3, sColor3Value>::value, &Value<G3D::Color3, sColor3Value>::onValueChanged
);

template <>
Reflection::SignalDesc<Value<G3D::Color3, sColor3Value>, void(G3D::Color3)> Value<G3D::Color3, sColor3Value>::
	desc_ValueChanged("Changed", "value");

template <>
const BrickColor Value<BrickColor, sBrickColorValue>::defaultValue = BrickColor::random();
template <>
Reflection::BoundProp<BrickColor, 1> Value<BrickColor, sBrickColorValue>::desc_Value(
	"Value", "Data", &Value<BrickColor, sBrickColorValue>::value, &Value<BrickColor, sBrickColorValue>::onValueChanged
);

template <>
Reflection::SignalDesc<Value<BrickColor, sBrickColorValue>, void(BrickColor)> Value<BrickColor, sBrickColorValue>::
	desc_ValueChanged("Changed", "value");

Reflection::RefPropDescriptor<ObjectValue, Instance> ObjectValue::desc_Value(
	"Value", "Data", &ObjectValue::getValue, &ObjectValue::setValue
);

Reflection::SignalDesc<ObjectValue, void(boost::shared_ptr<Instance>)> ObjectValue::desc_ValueChanged(
	"Changed", "value"
);

// FUNCTION: WEBSERVICE 0x100f3b60
// FUNCTION: ROBLOX 0x00632820
void registerValueClasses()
{
	Value<int, sIntValue>::classDescriptor();
	Value<double, sDoubleValue>::classDescriptor();
	Value<bool, sBoolValue>::classDescriptor();
	Value<std::string, sStringValue>::classDescriptor();
	Value<G3D::Vector3, sVector3Value>::classDescriptor();
	Value<G3D::CoordinateFrame, sCFrameValue>::classDescriptor();
	Value<G3D::Color3, sColor3Value>::classDescriptor();
	Value<BrickColor, sBrickColorValue>::classDescriptor();
	ObjectValue::classDescriptor();
}

// FUNCTION: WEBSERVICE 0x100f71b0
// STUB: ROBLOX 0x006359a0
void ObjectValue::setValue(Instance* value)
{
	if (this->value.get() != value) {
		this->value = shared_from(value);
		raisePropertyChanged(desc_Value);
		desc_ValueChanged.fire(this, this->value);
	}
}

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sIntValue>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sBoolValue>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sDoubleValue>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sStringValue>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sVector3Value>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sCFrameValue>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sColor3Value>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sBrickColorValue>();
template void RBX::Name::callDoDeclare<RBX::sIntValue>();
template void RBX::Name::callDoDeclare<RBX::sBoolValue>();
template void RBX::Name::callDoDeclare<RBX::sDoubleValue>();
template void RBX::Name::callDoDeclare<RBX::sStringValue>();
template void RBX::Name::callDoDeclare<RBX::sVector3Value>();
template void RBX::Name::callDoDeclare<RBX::sCFrameValue>();
template void RBX::Name::callDoDeclare<RBX::sColor3Value>();
template void RBX::Name::callDoDeclare<RBX::sBrickColorValue>();

template class RBX::FactoryProduct<RBX::Value<int, RBX::sIntValue>, RBX::Instance, RBX::sIntValue>;
template class RBX::Reflection::Described<
	RBX::Value<int, RBX::sIntValue>,
	RBX::sIntValue,
	RBX::FactoryProduct<RBX::Value<int, RBX::sIntValue>, RBX::Instance, RBX::sIntValue> >;
template class RBX::DescribedCreatable<RBX::Value<int, RBX::sIntValue>, RBX::Instance, RBX::sIntValue>;

template class RBX::FactoryProduct<RBX::Value<bool, RBX::sBoolValue>, RBX::Instance, RBX::sBoolValue>;
template class RBX::Reflection::Described<
	RBX::Value<bool, RBX::sBoolValue>,
	RBX::sBoolValue,
	RBX::FactoryProduct<RBX::Value<bool, RBX::sBoolValue>, RBX::Instance, RBX::sBoolValue> >;
template class RBX::DescribedCreatable<RBX::Value<bool, RBX::sBoolValue>, RBX::Instance, RBX::sBoolValue>;

template class RBX::FactoryProduct<RBX::Value<double, RBX::sDoubleValue>, RBX::Instance, RBX::sDoubleValue>;
template class RBX::Reflection::Described<
	RBX::Value<double, RBX::sDoubleValue>,
	RBX::sDoubleValue,
	RBX::FactoryProduct<RBX::Value<double, RBX::sDoubleValue>, RBX::Instance, RBX::sDoubleValue> >;
template class RBX::DescribedCreatable<RBX::Value<double, RBX::sDoubleValue>, RBX::Instance, RBX::sDoubleValue>;

template class RBX::FactoryProduct<RBX::Value<std::string, RBX::sStringValue>, RBX::Instance, RBX::sStringValue>;
template class RBX::Reflection::Described<
	RBX::Value<std::string, RBX::sStringValue>,
	RBX::sStringValue,
	RBX::FactoryProduct<RBX::Value<std::string, RBX::sStringValue>, RBX::Instance, RBX::sStringValue> >;
template class RBX::DescribedCreatable<RBX::Value<std::string, RBX::sStringValue>, RBX::Instance, RBX::sStringValue>;

template class RBX::FactoryProduct<RBX::Value<G3D::Vector3, RBX::sVector3Value>, RBX::Instance, RBX::sVector3Value>;
template class RBX::Reflection::Described<
	RBX::Value<G3D::Vector3, RBX::sVector3Value>,
	RBX::sVector3Value,
	RBX::FactoryProduct<RBX::Value<G3D::Vector3, RBX::sVector3Value>, RBX::Instance, RBX::sVector3Value> >;
template class RBX::DescribedCreatable<RBX::Value<G3D::Vector3, RBX::sVector3Value>, RBX::Instance, RBX::sVector3Value>;

template class RBX::
	FactoryProduct<RBX::Value<G3D::CoordinateFrame, RBX::sCFrameValue>, RBX::Instance, RBX::sCFrameValue>;
template class RBX::Reflection::Described<
	RBX::Value<G3D::CoordinateFrame, RBX::sCFrameValue>,
	RBX::sCFrameValue,
	RBX::FactoryProduct<RBX::Value<G3D::CoordinateFrame, RBX::sCFrameValue>, RBX::Instance, RBX::sCFrameValue> >;
template class RBX::
	DescribedCreatable<RBX::Value<G3D::CoordinateFrame, RBX::sCFrameValue>, RBX::Instance, RBX::sCFrameValue>;

template class RBX::FactoryProduct<RBX::Value<G3D::Color3, RBX::sColor3Value>, RBX::Instance, RBX::sColor3Value>;
template class RBX::Reflection::Described<
	RBX::Value<G3D::Color3, RBX::sColor3Value>,
	RBX::sColor3Value,
	RBX::FactoryProduct<RBX::Value<G3D::Color3, RBX::sColor3Value>, RBX::Instance, RBX::sColor3Value> >;
template class RBX::DescribedCreatable<RBX::Value<G3D::Color3, RBX::sColor3Value>, RBX::Instance, RBX::sColor3Value>;

template class RBX::
	FactoryProduct<RBX::Value<RBX::BrickColor, RBX::sBrickColorValue>, RBX::Instance, RBX::sBrickColorValue>;
template class RBX::Reflection::Described<
	RBX::Value<RBX::BrickColor, RBX::sBrickColorValue>,
	RBX::sBrickColorValue,
	RBX::FactoryProduct<RBX::Value<RBX::BrickColor, RBX::sBrickColorValue>, RBX::Instance, RBX::sBrickColorValue> >;
template class RBX::
	DescribedCreatable<RBX::Value<RBX::BrickColor, RBX::sBrickColorValue>, RBX::Instance, RBX::sBrickColorValue>;
template class RBX::FactoryProduct<RBX::ObjectValue, RBX::Instance, RBX::sObjectValue>;
template class RBX::Reflection::Described<
	RBX::ObjectValue,
	RBX::sObjectValue,
	RBX::FactoryProduct<RBX::ObjectValue, RBX::Instance, RBX::sObjectValue> >;
template class RBX::DescribedCreatable<RBX::ObjectValue, RBX::Instance, RBX::sObjectValue>;
