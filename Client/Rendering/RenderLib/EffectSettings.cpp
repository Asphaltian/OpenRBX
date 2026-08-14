#include "renderlib/EffectSettings.h"

#include <memory>

namespace RBX {
namespace Render {

// FUNCTION: WEBSERVICE 0x101f8950
G3D::ToneMap* EffectSettings::getToneMap()
{
	static std::auto_ptr<G3D::ToneMap> toneMap(new G3D::ToneMap());
	return toneMap.get();
}

// FUNCTION: WEBSERVICE 0x101f89e0
DepthBlur* EffectSettings::getDepthBlur()
{
	static std::auto_ptr<DepthBlur> depthBlur(new DepthBlur());
	return depthBlur.get();
}

} // namespace Render
} // namespace RBX
