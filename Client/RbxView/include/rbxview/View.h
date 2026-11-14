#ifndef RBXVIEW_VIEW_H
#define RBXVIEW_VIEW_H

#include "ViewBase.h"
#include "placeholder.h"
#include "renderlib/AggregatingSceneManager.h"

#include <G3D/ReferenceCount.h>
#include <boost/shared_ptr.hpp>
#include <boost/signals/connection.hpp>
#include <map>
#include <memory>

namespace G3D {
class Sky;
class TextureManager;
} // namespace G3D

namespace RBX {

class DataModel;
class Instance;

namespace Render {
class Material;
class Model;
class RenderScene;
} // namespace Render

namespace View {

class MaterialFactory;

class View : public ViewBase
{
private:
	G3D::ReferenceCountedPointer<G3D::Sky> sky;
	boost::shared_ptr<DataModel> dataModel;
	std::map<Instance*, Render::Model*> models;
	boost::signals::scoped_connection lightingChangedConnection;
	boost::signals::scoped_connection workspaceDescendentAddedConnection;
	bool lightingValid;

public:
	std::auto_ptr<Render::SceneManager> sceneManager;
	std::auto_ptr<Render::RenderScene> renderLibScene;
	std::auto_ptr<G3D::TextureManager> textureManager;
	std::auto_ptr<MaterialFactory> materialFactory;

	View(boost::shared_ptr<DataModel> dataModel);
	virtual ~View();

	virtual void render(void* rd);

	G3D::ReferenceCountedPointer<Render::Material> getMaterial(G3D::ReferenceCountedPointer<Render::Material>);

	virtual float getShadingQuality() const;
	virtual float getMeshDetail() const;
	virtual void updateSettings(float shadingQuality, float meshDetail, bool shadows, float cameraDistance);
	virtual void suppressSkybox();
	virtual Instance* getWorkspace();
	virtual RenderStats& getRenderStats();

private:
	virtual void onWorkspaceDescendentAdded(boost::shared_ptr<Instance> descendent);
	virtual void updateLighting();
	virtual void invalidateLighting(bool updateSkybox);
};

} // namespace View
} // namespace RBX

#endif // RBXVIEW_VIEW_H
