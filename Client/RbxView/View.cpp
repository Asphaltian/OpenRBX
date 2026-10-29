#include "rbxview/View.h"

#include "BrickMesh.h"
#include "CylinderMesh.h"
#include "MaterialFactory.h"
#include "PBBMesh.h"
#include "Part.h"
#include "SphereMesh.h"
#include "renderlib/RenderScene.h"
#include "util/standardout.h"
#include "v8datamodel/DataModel.h"
#include "v8datamodel/Decal.h"
#include "v8datamodel/Lighting.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/Sky.h"
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

// FUNCTION: WEBSERVICE 0x1016aa20
void View::onWorkspaceDescendentAdded(boost::shared_ptr<Instance> descendent)
{
	PartInstance* part = dynamic_cast<PartInstance*>(descendent.get());
	if (part) {
		new Part(shared_from(part), this);
	}
	else if (PartInstance* parentPart = dynamic_cast<PartInstance*>(descendent->getParent())) {
		RBX::Texture* texture = dynamic_cast<RBX::Texture*>(descendent.get());
		if (texture) {
			new Texture(*texture, *parentPart, this);
			return;
		}

		RBX::Decal* decal = dynamic_cast<RBX::Decal*>(descendent.get());
		if (decal) {
			new Decal(*decal, *parentPart, this);
			return;
		}
	}
}

// FUNCTION: WEBSERVICE 0x1016b800
void View::suppressSkybox()
{
	Lighting* lighting = dataModel ? dataModel->create<Lighting>() : NULL;

	renderLibScene->presetLighting(
		NULL, lighting->getSkyParameters(), lighting->getAmbientTop(), lighting->getAmbientBottom()
	);

	sky = NULL;

	G3D::Color4 clearColor = lighting->getClearColor();
	renderLibScene->colorClearValue = clearColor;
}

// STUB: WEBSERVICE 0x1016b950
void View::updateLighting()
{
	if (!lightingValid) {
		Lighting* lighting = dataModel ? dataModel->create<Lighting>() : NULL;

		if (lighting->isSkySuppressed()) {
			sky = NULL;
		}
		else if (sky.isNull()) {
			try {
				sky = Sky::getG3DSky(lighting->sky.get());
			}
			catch (const G3D::GImage::Error& e) {
				StandardOut::singleton()->print(MESSAGE_ERROR, "Failed to load sky box. %s", e.reason.c_str());
			}
			catch (...) {
				StandardOut::singleton()->print(MESSAGE_ERROR, "Failed to load sky box");
			}
		}

		renderLibScene->presetLighting(
			sky, lighting->getSkyParameters(), lighting->getAmbientTop(), lighting->getAmbientBottom()
		);
		renderLibScene->colorClearValue = lighting->getClearColor();

		lightingValid = true;
	}
}

// FUNCTION: WEBSERVICE 0x1016bd60
View::View(boost::shared_ptr<DataModel> dataModel)
	: dataModel(dataModel), lightingValid(false), renderLibScene(new Render::RenderScene()),
	  textureManager(new G3D::TextureManager())
{
	sceneManager.reset(new Render::AggregatingSceneManager(renderLibScene.get()));

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
