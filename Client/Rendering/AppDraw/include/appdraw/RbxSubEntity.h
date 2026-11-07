#ifndef APPDRAW_RBXSUBENTITY_H
#define APPDRAW_RBXSUBENTITY_H

#include "decomp.h"

#include <OgreMatrix4.h>
#include <OgreSceneNode.h>
#include <OgreShadowCaster.h>
#include <OgreSubEntity.h>
#include <vector>

namespace Ogre {

class RbxSceneNode : public SceneNode
{
};

// VTABLE: ROBLOX 0x0083df0c Ogre::SubEntity
// VTABLE: ROBLOX 0x0083ded4 Ogre::ShadowCaster
// SIZE 0x140
class RbxSubEntity : public SubEntity, public ShadowCaster
{
public:
	RbxSubEntity(Entity* parent);
	RbxSubEntity(Entity* parent, SubMesh* subMesh, int lodIndex);
	virtual ~RbxSubEntity();

	virtual void getRenderOperation(RenderOperation& op);
	virtual void getWorldTransforms(Matrix4* xform) const;
	virtual const Quaternion& getWorldOrientation(void) const;
	virtual const Vector3& getWorldPosition(void) const;
	virtual Real getSquaredViewDepth(const Camera* cam) const;

	virtual void FUN_0067bcc0(Real value);
	virtual Real FUN_0067bcd0() const;

	virtual bool getCastShadows(void) const;
	virtual EdgeData* getEdgeList(void);
	virtual bool hasEdgeList(void);
	virtual const AxisAlignedBox& getWorldBoundingBox(bool derive = false) const;
	virtual const AxisAlignedBox& getLightCapBounds(void) const;
	virtual const AxisAlignedBox& getDarkCapBounds(const Light& light, Real dirLightExtrusionDist) const;
	virtual ShadowRenderableListIterator getShadowVolumeRenderableIterator(
		ShadowTechnique shadowTechnique,
		const Light* light,
		HardwareIndexBufferSharedPtr* indexBuffer,
		bool extrudeVertices,
		Real extrusionDistance,
		unsigned long flags = 0
	);
	virtual Real getPointExtrusionDistance(const Light* l) const;

	// SYNTHETIC: ROBLOX 0x0067bf90
	// Ogre::RbxSubEntity::`scalar deleting destructor'

private:
	Matrix4 m_unk0xdc;                         // 0xdc
	SubMesh* m_unk0x11c[1];                    // 0x11c
	int m_unk0x120;                            // 0x120
	Real m_unk0x124;                           // 0x124
	bool m_unk0x128;                           // 0x128
	undefined4 m_unk0x12c;                     // 0x12c
	std::vector<ShadowRenderable*> m_unk0x130; // 0x130
};

DECOMP_SIZE_ASSERT(SubEntity, 0xd8)
DECOMP_SIZE_ASSERT(RbxSubEntity, 0x140)

} // namespace Ogre

#endif // APPDRAW_RBXSUBENTITY_H
