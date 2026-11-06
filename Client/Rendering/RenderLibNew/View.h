#ifndef RENDERLIBNEW_VIEW_H
#define RENDERLIBNEW_VIEW_H

#include "ViewBase.h"
#include "decomp.h"
#include "renderlibnew/RenderScene.h"

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

// SIZE 0x58
class View : public ViewBase
{
private:
	boost::shared_ptr<DataModel> dataModel;                               // 0x04
	std::map<Instance*, RenderNew::Model*> models;                        // 0x0c
	boost::signals::scoped_connection lightingChangedConnection;          // 0x18
	boost::signals::scoped_connection workspaceDescendentAddedConnection; // 0x2c
	bool lightingValid;                                                   // 0x40

public:
	std::auto_ptr<RenderNew::SceneManager> sceneManager;  // 0x44
	std::auto_ptr<RenderNew::RenderScene> renderLibScene; // 0x48
	std::auto_ptr<G3D::TextureManager> textureManager;    // 0x4c
	std::auto_ptr<MaterialFactory> materialFactory;       // 0x50

private:
	bool m_unk0x54; // 0x54

public:
	View(boost::shared_ptr<DataModel> dataModel);
	virtual ~View(); // vtable+0x00

	virtual void render(void* rd); // vtable+0x04

	virtual float getShadingQuality() const; // vtable+0x08
	virtual float getMeshDetail() const;     // vtable+0x0c
	virtual void updateSettings(
		float shadingQuality, float meshDetail, bool shadows, float cameraDistance
	);                                     // vtable+0x10
	virtual void suppressSkybox();         // vtable+0x14
	virtual Instance* getWorkspace();      // vtable+0x18
	virtual RenderStats& getRenderStats(); // vtable+0x1c

private:
	virtual void onWorkspaceDescendentAdded(boost::shared_ptr<Instance> descendent); // vtable+0x20
	virtual void updateLighting();                                                   // vtable+0x24
	virtual void invalidateLighting(bool updateSkybox);                              // vtable+0x28

	void FUN_0050d3d0(void* rd);

	// SYNTHETIC: ROBLOX 0x0050dca0
	// RBX::ViewNew::View::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(View, 0x58)

} // namespace ViewNew
} // namespace RBX

#endif // RENDERLIBNEW_VIEW_H
