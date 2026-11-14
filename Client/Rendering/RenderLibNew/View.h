#ifndef RENDERLIBNEW_VIEW_H
#define RENDERLIBNEW_VIEW_H

#include "ViewBase.h"
#include "renderlibnew/RenderScene.h"
#include "placeholder.h"

#include <boost/shared_ptr.hpp>
#include <boost/signals/connection.hpp>
#include <map>
#include <memory>

namespace G3D {
class TextureManager;
} // namespace G3D

namespace RBX {

class DataModel;
class Instance;

namespace RenderNew {
class Model;
} // namespace RenderNew

namespace ViewNew {

class MaterialFactory;

class View : public ViewBase
{
private:
	boost::shared_ptr<DataModel> dataModel;
	std::map<Instance*, RenderNew::Model*> models;
	boost::signals::scoped_connection lightingChangedConnection;
	boost::signals::scoped_connection workspaceDescendentAddedConnection;
	bool lightingValid;

public:
	std::auto_ptr<RenderNew::SceneManager> sceneManager;
	std::auto_ptr<RenderNew::RenderScene> renderLibScene;
	std::auto_ptr<G3D::TextureManager> textureManager;
	std::auto_ptr<MaterialFactory> materialFactory;

private:
	bool m_unk0x54;

public:
	View(boost::shared_ptr<DataModel> dataModel);
	virtual ~View();

	virtual void render(void* rd);

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

	void FUN_0050c5c0(void* rd);
	void FUN_0050d3d0(void* rd);
};

extern bool g_unk0x0092a24c;

void FUN_0050b2a0(bool value);
bool FUN_0050b2b0();

} // namespace ViewNew
} // namespace RBX

#endif // RENDERLIBNEW_VIEW_H
