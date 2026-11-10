#ifndef APPDRAW_RBXSUBENTITY_H
#define APPDRAW_RBXSUBENTITY_H

#include "appdraw/RbxSceneNode.h"
#include "decomp.h"

#include <OgreMatrix4.h>
#include <OgreSceneNode.h>
#include <OgreShadowCaster.h>
#include <OgreSubEntity.h>
#include <vector>

namespace RBX {
class AdornRbxGfx;
namespace RenderNew {
class RenderScene;
} // namespace RenderNew
} // namespace RBX

namespace Ogre {

class Unknown00671c70;

// VTABLE: ROBLOX 0x0083df0c Ogre::SubEntity
// VTABLE: ROBLOX 0x0083ded4 Ogre::ShadowCaster
// SIZE 0x140
class RbxSubEntity : public SubEntity, public ShadowCaster
{
public:
	// VTABLE: ROBLOX 0x0083de84
	// SIZE 0x5c
	class RbxSubEntityShadowRenderable : public ShadowRenderable
	{
	protected:
		HardwareVertexBufferSharedPtr mPositionBuffer; // 0x38
		HardwareVertexBufferSharedPtr mWBuffer;        // 0x44
		const VertexData* mCurrentVertexData;          // 0x50
		unsigned short mOriginalPosBufferBinding;      // 0x54
		RbxSubEntity* mSubEntity;                      // 0x58

	public:
		RbxSubEntityShadowRenderable(
			Entity* parent,
			HardwareIndexBufferSharedPtr* indexBuffer,
			const VertexData* vertexData,
			bool createSeparateLightCap,
			RbxSubEntity* subent,
			bool isLightCap = false
		);
		~RbxSubEntityShadowRenderable();

		void getWorldTransforms(Matrix4* xform) const;
		const Quaternion& getWorldOrientation(void) const;
		const Vector3& getWorldPosition(void) const;
		bool isVisible(void) const;

		// SYNTHETIC: ROBLOX 0x0067bac0
		// Ogre::RbxSubEntity::RbxSubEntityShadowRenderable::`scalar deleting destructor'
	};

	RbxSubEntity(Entity* parent);
	RbxSubEntity(Entity* parent, SubMesh* subMesh, int lodIndex);
	virtual ~RbxSubEntity();

	void FUN_0067b3d0(SubMesh* subMesh);
	void FUN_0067b4b0(const MaterialPtr& material);
	void FUN_0067b4c0(int lodIndex, SubMesh* subMesh);
	SubMesh* FUN_0067b4e0(int lodIndex);
	void FUN_0067b500(int lodIndex);

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
	friend class RBX::RenderNew::RenderScene;
	friend class RbxEntity;
	friend class RBX::AdornRbxGfx;

	Matrix4 m_unk0xdc;                         // 0xdc
	SubMesh* m_unk0x11c[1];                    // 0x11c
	int m_unk0x120;                            // 0x120
	Real m_unk0x124;                           // 0x124
	bool m_unk0x128;                           // 0x128
	Unknown00671c70* m_unk0x12c;               // 0x12c
	std::vector<ShadowRenderable*> m_unk0x130; // 0x130
};

DECOMP_SIZE_ASSERT(RbxSubEntity::RbxSubEntityShadowRenderable, 0x5c)
DECOMP_SIZE_ASSERT(SubEntity, 0xd8)
DECOMP_SIZE_ASSERT(RbxSubEntity, 0x140)

} // namespace Ogre

#endif // APPDRAW_RBXSUBENTITY_H
