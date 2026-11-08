#include "appdraw/RbxSky.h"

#include "appdraw/VisualEngine.h"

#include <GLG3D/LightingParameters.h>
#include <OgreViewport.h>

namespace Ogre {

// STUB: ROBLOX 0x00677eb0
void RbxSky::FUN_00677eb0(VisualEngine* engine, const G3D::LightingParameters& parameters)
{
	STUB(0x677eb0);
}

// STUB: ROBLOX 0x00678210
void RbxSky::FUN_00678210(VisualEngine* engine, const G3D::LightingParameters& parameters)
{
	STUB(0x678210);
}

// STUB: ROBLOX 0x00678540
void RbxSky::FUN_00678540(VisualEngine* engine, const G3D::LightingParameters& parameters)
{
	engine->getViewport()->setBackgroundColour(
		ColourValue(parameters.skyAmbient.r, parameters.skyAmbient.g, parameters.skyAmbient.b, 1.0f)
	);
	if (m_unk0xa8) {
		FUN_00677eb0(engine, parameters);
		FUN_00678210(engine, parameters);
	}
}

// FUNCTION: ROBLOX 0x00678b20
RbxSky::~RbxSky()
{
}

// STUB: ROBLOX 0x00678d30
RbxSky::RbxSky(SceneManager* sceneManager, const String& folder, bool p_unk0x0c)
{
	STUB(0x678d30);
}

} // namespace Ogre
