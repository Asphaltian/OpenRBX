#include "View.h"

#include "MaterialFactory.h"
#include "renderlibnew/AggregatingSceneManager.h"
#include "v8datamodel/DataModel.h"
#include "v8datamodel/Lighting.h"
#include "v8datamodel/Workspace.h"

#include <GLG3D/TextureManager.h>
#include <boost/bind.hpp>

namespace RBX {
namespace ViewNew {

// FUNCTION: ROBLOX 0x0050b280
void View::invalidateLighting(bool updateSkybox)
{
	lightingValid = false;
	if (updateSkybox) {
		m_unk0x54 = false;
	}
}

// FUNCTION: ROBLOX 0x0050b2e0
Instance* View::getWorkspace()
{
	return dataModel->getWorkspace();
}

// FUNCTION: ROBLOX 0x0050b430
float View::getShadingQuality() const
{
	return renderLibScene->getShadingQuality();
}

// FUNCTION: ROBLOX 0x0050b440
float View::getMeshDetail() const
{
	return renderLibScene->getMeshDetail();
}

// FUNCTION: ROBLOX 0x0050b450
void View::updateSettings(float shadingQuality, float meshDetail, bool shadows, float cameraDistance)
{
	renderLibScene->setThrottle(shadingQuality, meshDetail, shadows, cameraDistance);
}

// FUNCTION: ROBLOX 0x0050b480
RenderStats& View::getRenderStats()
{
	return renderLibScene->renderStats;
}

// STUB: ROBLOX 0x0050b7e0
void View::onWorkspaceDescendentAdded(boost::shared_ptr<Instance> descendent)
{
	STUB(0x50b7e0);
}

// STUB: ROBLOX 0x0050c4a0
void View::suppressSkybox()
{
	STUB(0x50c4a0);
}

// STUB: ROBLOX 0x0050d3d0
void View::FUN_0050d3d0(void* rd)
{
	STUB(0x50d3d0);
}

// STUB: ROBLOX 0x0050d610
void View::render(void* rd)
{
	FUN_0050d3d0(rd);
	renderLibScene->render(static_cast<G3D::RenderDevice*>(rd), dataModel->getWorkspace()->getGCamera());
}

// STUB: ROBLOX 0x0050d810
View::View(boost::shared_ptr<DataModel> dataModel)
	: dataModel(dataModel), lightingValid(false), renderLibScene(new RenderNew::RenderScene()),
	  textureManager(new G3D::TextureManager()), m_unk0x54(false)
{
	sceneManager.reset(new RenderNew::AggregatingSceneManager(renderLibScene.get()));

	materialFactory.reset(new MaterialFactory(textureManager.get()));

	renderLibScene->setThrottle(0, 0, false, 0);

	Lighting* lighting = dataModel ? dataModel->create<Lighting>() : NULL;
	lightingChangedConnection =
		Lighting::event_LightingChanged.connect(lighting, boost::bind(&View::invalidateLighting, this, _1), 10);

	dataModel->getWorkspace()->visitDescendents(boost::bind(&View::onWorkspaceDescendentAdded, this, _1));
	workspaceDescendentAddedConnection = Instance::event_descendentAdded.connect(
		dataModel->getWorkspace(), boost::bind(&View::onWorkspaceDescendentAdded, this, _1), 10
	);
}

// STUB: ROBLOX 0x0050db30
View::~View()
{
	sceneManager->clear();

	if (dataModel) {
		dataModel->find<Lighting>();
	}
}

void View::updateLighting()
{
}

} // namespace ViewNew
} // namespace RBX
