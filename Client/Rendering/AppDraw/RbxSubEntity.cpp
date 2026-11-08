#include "appdraw/RbxSubEntity.h"

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreHardwareBufferManager.h>
#include <OgreLogManager.h>
#include <OgreSubMesh.h>
#include <exception>

namespace Ogre {

// FUNCTION: ROBLOX 0x0067b3e0
void RbxSubEntity::getRenderOperation(RenderOperation& op)
{
	if (m_unk0x120 == 0) {
		mSubMesh->_getRenderOperation(op);
	}
	else {
		m_unk0x11c[m_unk0x120 - 1]->_getRenderOperation(op);
	}
}

// FUNCTION: ROBLOX 0x0067b420
Real RbxSubEntity::getSquaredViewDepth(const Camera* cam) const
{
	if (mCachedCamera == cam) {
		return mCachedCameraDist;
	}
	dynamic_cast<RbxSceneNode*>(mParentEntity->getParentNode());
	Vector3 diff = m_unk0xdc.getTrans() - cam->getDerivedPosition();
	return diff.squaredLength();
}

// FUNCTION: ROBLOX 0x0067b4b0
void RbxSubEntity::FUN_0067b4b0(const MaterialPtr& material)
{
	mpMaterial = material;
}

// FUNCTION: ROBLOX 0x0067b4c0
void RbxSubEntity::FUN_0067b4c0(int lodIndex, SubMesh* subMesh)
{
	if (lodIndex == 0) {
		mSubMesh = subMesh;
	}
	else {
		m_unk0x11c[lodIndex - 1] = subMesh;
	}
}

// FUNCTION: ROBLOX 0x0067b4e0
SubMesh* RbxSubEntity::FUN_0067b4e0(int lodIndex)
{
	if (lodIndex == 0) {
		return mSubMesh;
	}
	return m_unk0x11c[lodIndex - 1];
}

void RbxSubEntity::FUN_0067b500(int lodIndex)
{
	m_unk0x120 = lodIndex;
}

// FUNCTION: ROBLOX 0x0067b510
const AxisAlignedBox& RbxSubEntity::getLightCapBounds(void) const
{
	return getWorldBoundingBox();
}

// FUNCTION: ROBLOX 0x0067b520
Real RbxSubEntity::getPointExtrusionDistance(const Light* l) const
{
	return getExtrusionDistance(m_unk0xdc.getTrans(), l);
}

// FUNCTION: ROBLOX 0x0067b550
EdgeData* RbxSubEntity::getEdgeList(void)
{
	throw std::exception("RBX RbxSubEntity::getEdgeList(): should not be called");
}

// FUNCTION: ROBLOX 0x0067b580
bool RbxSubEntity::hasEdgeList(void)
{
	throw std::exception("RBX RbxSubEntity::hasEdgeList(): should not be called");
}

// FUNCTION: ROBLOX 0x0067b5b0
RbxSubEntity::RbxSubEntityShadowRenderable::RbxSubEntityShadowRenderable(
	Entity* parent,
	HardwareIndexBufferSharedPtr* indexBuffer,
	const VertexData* vertexData,
	bool createSeparateLightCap,
	RbxSubEntity* subent,
	bool isLightCap
)
	: mSubEntity(subent)
{
	mCurrentVertexData = vertexData;

	mRenderOp.indexData = new IndexData();
	mRenderOp.indexData->indexBuffer = *indexBuffer;
	mRenderOp.indexData->indexStart = 0;

	mRenderOp.vertexData = new VertexData();
	mRenderOp.vertexData->vertexDeclaration->addElement(0, 0, VET_FLOAT3, VES_POSITION);
	mOriginalPosBufferBinding = vertexData->vertexDeclaration->findElementBySemantic(VES_POSITION)->getSource();
	mPositionBuffer = vertexData->vertexBufferBinding->getBuffer(mOriginalPosBufferBinding);
	mRenderOp.vertexData->vertexBufferBinding->setBinding(0, mPositionBuffer);
	if (!vertexData->hardwareShadowVolWBuffer.isNull()) {
		mRenderOp.vertexData->vertexDeclaration->addElement(1, 0, VET_FLOAT1, VES_TEXTURE_COORDINATES, 0);
		mWBuffer = vertexData->hardwareShadowVolWBuffer;
		mRenderOp.vertexData->vertexBufferBinding->setBinding(1, mWBuffer);
	}
	mRenderOp.vertexData->vertexStart = vertexData->vertexStart;

	if (isLightCap) {
		mRenderOp.vertexData->vertexCount = vertexData->vertexCount;
	}
	else {
		mRenderOp.vertexData->vertexCount = vertexData->vertexCount * 2;
		if (createSeparateLightCap) {
			mLightCap = new RbxSubEntityShadowRenderable(parent, indexBuffer, vertexData, false, subent, true);
		}
	}
}

// FUNCTION: ROBLOX 0x0067b790
RbxSubEntity::RbxSubEntityShadowRenderable::~RbxSubEntityShadowRenderable()
{
	delete mRenderOp.indexData;
	delete mRenderOp.vertexData;
	mRenderOp.indexData = NULL;
	mRenderOp.vertexData = NULL;
}

// FUNCTION: ROBLOX 0x0067b850
void RbxSubEntity::RbxSubEntityShadowRenderable::getWorldTransforms(Matrix4* xform) const
{
	mSubEntity->getWorldTransforms(xform);
}

// FUNCTION: ROBLOX 0x0067b860
const Quaternion& RbxSubEntity::RbxSubEntityShadowRenderable::getWorldOrientation(void) const
{
	return mSubEntity->getWorldOrientation();
}

// FUNCTION: ROBLOX 0x0067b870
const Vector3& RbxSubEntity::RbxSubEntityShadowRenderable::getWorldPosition(void) const
{
	return mSubEntity->getWorldPosition();
}

// FUNCTION: ROBLOX 0x0067b880
bool RbxSubEntity::RbxSubEntityShadowRenderable::isVisible(void) const
{
	return mSubEntity->isVisible();
}

// STUB: ROBLOX 0x0067b890
const AxisAlignedBox& RbxSubEntity::getWorldBoundingBox(bool derive) const
{
	STUB(0x67b890);
	static AxisAlignedBox box;
	return box;
}

// STUB: ROBLOX 0x0067ba00
const AxisAlignedBox& RbxSubEntity::getDarkCapBounds(const Light& light, Real dirLightExtrusionDist) const
{
	STUB(0x67ba00);
	return getWorldBoundingBox();
}

// FUNCTION: ROBLOX 0x0067bae0
RbxSubEntity::RbxSubEntity(Entity* parent) : SubEntity(parent, NULL), m_unk0x120(0), m_unk0x128(true)
{
	m_unk0x12c = NULL;
	m_unk0x11c[0] = NULL;
}

// FUNCTION: ROBLOX 0x0067bb70
bool RbxSubEntity::getCastShadows(void) const
{
	return m_unk0x128;
}

// FUNCTION: ROBLOX 0x0067bb80
void RbxSubEntity::getWorldTransforms(Matrix4* xform) const
{
	*xform = m_unk0xdc;
}

// FUNCTION: ROBLOX 0x0067bba0
const Quaternion& RbxSubEntity::getWorldOrientation(void) const
{
	LogManager::getSingleton().logMessage("RBX perf warning!", LML_NORMAL, false);
	static Quaternion orientation = m_unk0xdc.extractQuaternion();
	return orientation;
}

// FUNCTION: ROBLOX 0x0067bc60
const Vector3& RbxSubEntity::getWorldPosition(void) const
{
	static Vector3 position(m_unk0xdc[0][3], m_unk0xdc[1][3], m_unk0xdc[2][3]);
	return position;
}

// FUNCTION: ROBLOX 0x0067bcc0
void RbxSubEntity::FUN_0067bcc0(Real value)
{
	m_unk0x124 = value;
}

// FUNCTION: ROBLOX 0x0067bcd0
Real RbxSubEntity::FUN_0067bcd0() const
{
	return m_unk0x124;
}

// FUNCTION: ROBLOX 0x0067bcf0
RbxSubEntity::RbxSubEntity(Entity* parent, SubMesh* subMesh, int lodIndex)
	: SubEntity(parent, NULL), m_unk0x120(lodIndex), m_unk0x128(true)
{
	m_unk0x11c[0] = NULL;
	if (lodIndex == 0) {
		mSubMesh = subMesh;
	}
	else {
		m_unk0x11c[m_unk0x120 - 1] = subMesh;
	}
	m_unk0x12c = NULL;
}

// STUB: ROBLOX 0x0067bda0
RbxSubEntity::~RbxSubEntity()
{
	STUB(0x67bda0);
}

// STUB: ROBLOX 0x0067c230
ShadowCaster::ShadowRenderableListIterator RbxSubEntity::getShadowVolumeRenderableIterator(
	ShadowTechnique shadowTechnique,
	const Light* light,
	HardwareIndexBufferSharedPtr* indexBuffer,
	bool extrudeVertices,
	Real extrusionDistance,
	unsigned long flags
)
{
	STUB(0x67c230);
	return ShadowRenderableListIterator(m_unk0x130.begin(), m_unk0x130.end());
}

} // namespace Ogre
