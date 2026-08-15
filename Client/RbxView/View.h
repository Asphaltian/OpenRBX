#ifndef RBXVIEW_VIEW_H
#define RBXVIEW_VIEW_H

#include "ViewBase.h"
#include "decomp.h"
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
class Model;
class RenderScene;
} // namespace Render

namespace View {

class MaterialFactory;

// SIZE 0x58
class View : public ViewBase
{
private:
	G3D::ReferenceCountedPointer<G3D::Sky> sky;                           // 0x04
	boost::shared_ptr<DataModel> dataModel;                               // 0x08
	std::map<Instance*, Render::Model*> models;                           // 0x10
	boost::signals::scoped_connection lightingChangedConnection;          // 0x1c
	boost::signals::scoped_connection workspaceDescendentAddedConnection; // 0x30
	bool lightingValid;                                                   // 0x44

public:
	std::auto_ptr<Render::SceneManager> sceneManager;  // 0x48
	std::auto_ptr<Render::RenderScene> renderLibScene; // 0x4c
	std::auto_ptr<G3D::TextureManager> textureManager; // 0x50
	std::auto_ptr<MaterialFactory> materialFactory;    // 0x54
};

DECOMP_SIZE_ASSERT(View, 0x58)

} // namespace View
} // namespace RBX

#endif // RBXVIEW_VIEW_H
