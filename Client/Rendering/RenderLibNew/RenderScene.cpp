#include "renderlibnew/RenderScene.h"

#include <G3D/g3dmath.h>

namespace RBX {
namespace RenderNew {

// STUB: ROBLOX 0x0052d400
void RenderScene::setThrottle(float t, float m, bool s, float c)
{
	STUB(0x52d400);
}

// STUB: ROBLOX 0x0052d7e0
RenderScene::~RenderScene()
{
	STUB(0x52d7e0);
}

// FUNCTION: ROBLOX 0x0052dd00
RenderScene::RenderScene()
	: m_unk0x04(NULL), shadingQuality(-1.0f), meshDetail(-1.0f), shadows(false), cameraDistance((float) G3D::inf()),
	  debugShadowVolumes(false), colorClearValue(0.5f, 0.5f, 1.0f, 1.0f)
{
	desiredLighting = lighting = G3D::Lighting::create();
	m_unk0x00 = NULL;
}

// STUB: ROBLOX 0x0052df80
void RenderScene::render(G3D::RenderDevice* rd, const G3D::GCamera& camera)
{
	STUB(0x52df80);
}

} // namespace RenderNew
} // namespace RBX
