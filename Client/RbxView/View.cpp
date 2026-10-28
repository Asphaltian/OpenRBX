#include "View.h"

#include "BrickMesh.h"
#include "CylinderMesh.h"
#include "MaterialFactory.h"
#include "PBBMesh.h"
#include "SphereMesh.h"
#include "renderlib/RenderScene.h"
#include "v8datamodel/DataModel.h"
#include "v8datamodel/Lighting.h"
#include "v8datamodel/Workspace.h"

#include <GLG3D/Sky.h>

namespace RBX {
namespace View {

// FUNCTION: WEBSERVICE 0x1016a4b0
Instance* View::getWorkspace()
{
	return dataModel->getWorkspace();
}

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

// FUNCTION: WEBSERVICE 0x1016a630
void View::render(void* rd)
{
	updateLighting();
	sceneManager->prerender(0.1f);
	renderLibScene->render(static_cast<G3D::RenderDevice*>(rd), dataModel->getWorkspace()->getGCamera());
}

// FUNCTION: WEBSERVICE 0x1016a680
RenderStats& View::getRenderStats()
{
	return renderLibScene->renderStats;
}

// STUB: WEBSERVICE 0x1016aa20
void View::onWorkspaceDescendentAdded(boost::shared_ptr<Instance> descendent)
{
	STUB(0x1016aa20);
}

// STUB: WEBSERVICE 0x1016b800
void View::suppressSkybox()
{
	STUB(0x1016b800);
}

// STUB: WEBSERVICE 0x1016b950
void View::updateLighting()
{
	STUB(0x1016b950);
}

// FUNCTION: WEBSERVICE 0x1016c080
View::~View()
{
	sceneManager->clear();

	if (dataModel) {
		dataModel->find<Lighting>();
	}

	MeshFactory<CylinderAlongXMesh, 1>::flushCache();
	MeshFactory<SphereMesh, 1>::flushCache();
	MeshFactory<PBBMesh, 4>::flushCache();
	BrickMesh::flushCache();
}

} // namespace View
} // namespace RBX
