#include "reflection/enumconverter.h"
#include "reflection/property.h"
#include "util/NormalId.h"
#include "v8datamodel/BrickColor.h"

#include <G3D/Color3.h>
#include <G3D/Vector3.h>
#include <string>
#include <vector>

namespace RBX {
namespace Reflection {

// FUNCTION: WEBSERVICE 0x1003f0b0
template <>
const Type& Type::singleton<int>()
{
	static Type type("int", typeid(int));

	return type;
}

// FUNCTION: WEBSERVICE 0x1003f120
template <>
const Type& Type::singleton<bool>()
{
	static Type type("bool", typeid(bool));

	return type;
}

// FUNCTION: WEBSERVICE 0x1003f190
template <>
const Type& Type::singleton<float>()
{
	static Type type("float", typeid(float));

	return type;
}

// FUNCTION: WEBSERVICE 0x1003f200
template <>
const Type& Type::singleton<double>()
{
	static Type type("double", typeid(double));

	return type;
}

// FUNCTION: WEBSERVICE 0x1003f2e0
template <>
const Type& Type::singleton<std::string>()
{
	static Type type("string", typeid(std::string));

	return type;
}

// FUNCTION: WEBSERVICE 0x1003f350
template <>
const Type& Type::singleton<G3D::Vector3>()
{
	static Type type("Vector3", typeid(G3D::Vector3));

	return type;
}

// FUNCTION: WEBSERVICE 0x1003f3c0
template <>
const Type& Type::singleton<G3D::Color3>()
{
	static Type type("Color3", typeid(G3D::Color3));

	return type;
}

// FUNCTION: WEBSERVICE 0x1003f5c0
template <>
const Type& Type::singleton<BrickColor>()
{
	static Type type("BrickColor", typeid(BrickColor), "int");

	return type;
}

// FUNCTION: WEBSERVICE 0x100421f0
template <>
EnumDesc<NormalId>::EnumDesc() : EnumDescriptor("NormalId", typeid(NormalId))
{
	addPair(NORM_Y, "Top");
	addPair(NORM_Y_NEG, "Bottom");
	addPair(NORM_Z, "Back");
	addPair(NORM_Z_NEG, "Front");
	addPair(NORM_X, "Right");
	addPair(NORM_X_NEG, "Left");
}

} // namespace Reflection
} // namespace RBX
