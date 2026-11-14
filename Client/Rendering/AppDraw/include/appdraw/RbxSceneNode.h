#ifndef APPDRAW_RBXSCENENODE_H
#define APPDRAW_RBXSCENENODE_H

#include "placeholder.h"

#include <OgreMatrix4.h>
#include <OgreSceneNode.h>
#include <OgreSphere.h>
#include <map>

namespace Ogre {

class RbxSceneNode : public SceneNode
{
public:
	RbxSceneNode(SceneManager* creator, const String& name) : SceneNode(creator, name) {}

	virtual ~RbxSceneNode();

	virtual void _update(bool updateChildren, bool parentHasChanged);
	virtual void _updateBounds(void);
	virtual void _findVisibleObjects(
		Camera* cam,
		RenderQueue* queue,
		VisibleObjectsBoundsInfo* visibleBounds,
		bool includeChildren = true,
		bool displayNodes = false,
		bool onlyShadowCasters = false
	);

	virtual void FUN_0067aeb0(SubMesh* subMesh, const Matrix4& transform);
	virtual const Matrix4& FUN_0067ac90(SubMesh* subMesh);
	virtual bool FUN_0067ada0(SubMesh* subMesh) const;
	virtual void FUN_0067b2b0(SubMesh* subMesh, const Sphere& sphere);
	virtual const Sphere& FUN_0067af60(SubMesh* subMesh);
	virtual bool FUN_0067b020(SubMesh* subMesh) const;

	static bool FUN_0067b070(SubMesh* subMesh);
	static void FUN_0067b220();

private:
	std::map<const SubMesh*, Matrix4*> m_unk0x1c0;
};

extern std::map<const SubMesh*, Sphere>* g_unk0x009613c4;

} // namespace Ogre

#endif // APPDRAW_RBXSCENENODE_H
