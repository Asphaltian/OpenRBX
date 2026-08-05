#include "appdraw/Part.h"
#include "reflection/enumconverter.h"

namespace RBX {
namespace Reflection {

// FUNCTION: WEBSERVICE 0x10105f50
template <>
EnumDesc<Part::PartType>::EnumDesc() : EnumDescriptor("PartType", typeid(Part::PartType))
{
	addPair(Part::BALL_PART, "Ball");
	addPair(Part::BLOCK_PART, "Block");
	addPair(Part::CYLINDER_PART, "Cylinder");
}

} // namespace Reflection
} // namespace RBX
