#include "View.h"

#include "renderlib/RenderScene.h"

#include <GLG3D/Sky.h>

namespace RBX {
namespace View {

// FUNCTION: WEBSERVICE 0x1016a590
void View::invalidateLighting(bool updateSkybox)
{
	lightingValid = false;
	if (updateSkybox) {
		sky = NULL;
	}
}

// FUNCTION: WEBSERVICE 0x1016a5e0
float View::getShadingQuality() const
{
	return renderLibScene->getShadingQuality();
}

// FUNCTION: WEBSERVICE 0x1016a5f0
float View::getMeshDetail() const
{
	return renderLibScene->getMeshDetail();
}

// FUNCTION: WEBSERVICE 0x1016a600
void View::updateSettings(float shadingQuality, float meshDetail, bool shadows, float cameraDistance)
{
	renderLibScene->setThrottle(shadingQuality, meshDetail, shadows, cameraDistance);
}

// FUNCTION: WEBSERVICE 0x1016a680
RenderStats& View::getRenderStats()
{
	return renderLibScene->renderStats;
}

} // namespace View
} // namespace RBX
