#ifndef APPDRAW_RBXSUBENTITY_H
#define APPDRAW_RBXSUBENTITY_H

#include "appdraw/RbxSceneNode.h"
#include "placeholder.h"

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

class RbxSubEntity : public SubEntity, public ShadowCaster
{
public:
	class RbxSubEntityShadowRenderable : public ShadowRenderable
	{
	protected:
		HardwareVertexBufferSharedPtr mPositionBuffer;
		HardwareVertexBufferSharedPtr mWBuffer;
		const VertexData* mCurrentVertexData;
		unsigned short mOriginalPosBufferBinding;
		RbxSubEntity* mSubEntity;

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

		HardwareVertexBufferSharedPtr getPositionBuffer(void) { return mPositionBuffer; }

		void getWorldTransforms(Matrix4* xform) const;
		const Quaternion& getWorldOrientation(void) const;
		const Vector3& getWorldPosition(void) const;
		bool isVisible(void) const;
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

private:
	friend class RBX::RenderNew::RenderScene;
	friend class RbxEntity;
	friend class RBX::AdornRbxGfx;

	Matrix4 m_unk0xdc;
	SubMesh* m_unk0x11c[1];
	int m_unk0x120;
	Real m_unk0x124;
	bool m_unk0x128;
	Unknown00671c70* m_unk0x12c;
	std::vector<ShadowRenderable*> m_unk0x130;
};

} // namespace Ogre

#endif // APPDRAW_RBXSUBENTITY_H
