#include "util/MeshId.h"

namespace RBX {
namespace Reflection {

// FUNCTION: WEBSERVICE 0x1012dd60 FOLDED
template <>
const Type& Type::singleton<MeshId>()
{
	return singleton<ContentId>();
}

} // namespace Reflection
} // namespace RBX
