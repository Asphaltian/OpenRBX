#include "reflection/enumconverter.h"
#include "v8world/SurfaceData.h"

namespace RBX {
namespace Reflection {

// FUNCTION: WEBSERVICE 0x10121d40
template <>
EnumDesc<SurfaceType>::EnumDesc() : EnumDescriptor("SurfaceType", typeid(SurfaceType))
{
	addPair(NO_SURFACE, "Smooth");
	addPair(GLUE, "Glue");
	addPair(WELD, "Weld");
	addPair(STUDS, "Studs");
	addPair(INLET, "Inlet");
	addLegacyName("Spawn", NO_SURFACE);
	addPair(ROTATE, "Hinge");
	addPair(ROTATE_V, "Motor");
	addPair(ROTATE_P, "SteppingMotor");

	addLegacyName("Bumps", GLUE);
}

} // namespace Reflection
} // namespace RBX
