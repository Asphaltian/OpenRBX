#include "appdraw/RbxSubEntity.h"

#include "appdraw/VisualEngine.h"

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreHardwareBufferManager.h>
#include <OgreLight.h>
#include <OgreLogManager.h>
#include <OgreSubMesh.h>
#include <exception>

namespace Ogre {

void RbxSubEntity::FUN_0067b3d0(SubMesh* subMesh)
{
	mSubMesh = subMesh;
}

void RbxSubEntity::getRenderOperation(RenderOperation& op)
{
	if (m_unk0x120 == 0) {
		mSubMesh->_getRenderOperation(op);
	}
	else {
		m_unk0x11c[m_unk0x120 - 1]->_getRenderOperation(op);
	}
}

Real RbxSubEntity::getSquaredViewDepth(const Camera* cam) const
{
	if (mCachedCamera == cam) {
		return mCachedCameraDist;
	}
	dynamic_cast<RbxSceneNode*>(mParentEntity->getParentNode());
	Vector3 diff = m_unk0xdc.getTrans() - cam->getDerivedPosition();
	return diff.squaredLength();
}

void RbxSubEntity::FUN_0067b4b0(const MaterialPtr& material)
{
	mpMaterial = material;
}

void RbxSubEntity::FUN_0067b4c0(int lodIndex, SubMesh* subMesh)
{
	if (lodIndex == 0) {
		mSubMesh = subMesh;
	}
	else {
		m_unk0x11c[lodIndex - 1] = subMesh;
	}
}

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

const AxisAlignedBox& RbxSubEntity::getLightCapBounds(void) const
{
	return getWorldBoundingBox();
}

Real RbxSubEntity::getPointExtrusionDistance(const Light* l) const
{
	return getExtrusionDistance(m_unk0xdc.getTrans(), l);
}

EdgeData* RbxSubEntity::getEdgeList(void)
{
	throw std::exception("RBX RbxSubEntity::getEdgeList(): should not be called");
}

bool RbxSubEntity::hasEdgeList(void)
{
	throw std::exception("RBX RbxSubEntity::hasEdgeList(): should not be called");
}

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

RbxSubEntity::RbxSubEntityShadowRenderable::~RbxSubEntityShadowRenderable()
{
	delete mRenderOp.indexData;
	delete mRenderOp.vertexData;
	mRenderOp.indexData = NULL;
	mRenderOp.vertexData = NULL;
}

void RbxSubEntity::RbxSubEntityShadowRenderable::getWorldTransforms(Matrix4* xform) const
{
	mSubEntity->getWorldTransforms(xform);
}

const Quaternion& RbxSubEntity::RbxSubEntityShadowRenderable::getWorldOrientation(void) const
{
	return mSubEntity->getWorldOrientation();
}

const Vector3& RbxSubEntity::RbxSubEntityShadowRenderable::getWorldPosition(void) const
{
	return mSubEntity->getWorldPosition();
}

bool RbxSubEntity::RbxSubEntityShadowRenderable::isVisible(void) const
{
	return mSubEntity->isVisible();
}

const AxisAlignedBox& RbxSubEntity::getWorldBoundingBox(bool derive) const
{
	static AxisAlignedBox box;
	Unknown00671c70* subMesh = m_unk0x12c;
	if (!subMesh) {
		for (int i = 0; i >= 0; i--) {
			if (m_unk0x11c[i]) {
				subMesh = static_cast<Unknown00671c70*>(m_unk0x11c[i]);
				break;
			}
		}
		if (!subMesh) {
			subMesh = static_cast<Unknown00671c70*>(mSubMesh);
		}
	}

	Vector3 center = m_unk0xdc.getTrans();
	Real radius = subMesh->m_unk0xac * 1.7f;
	box.setExtents(center + Vector3(-radius), center + radius);
	return box;
}

const AxisAlignedBox& RbxSubEntity::getDarkCapBounds(const Light& light, Real dirLightExtrusionDist) const
{
	static AxisAlignedBox box;
	box = getLightCapBounds();
	extrudeBounds(box, light.getAs4DVector(), dirLightExtrusionDist);
	return box;
}

RbxSubEntity::RbxSubEntity(Entity* parent) : SubEntity(parent, NULL), m_unk0x120(0), m_unk0x128(true)
{
	m_unk0x12c = NULL;
	m_unk0x11c[0] = NULL;
}

bool RbxSubEntity::getCastShadows(void) const
{
	return m_unk0x128;
}

void RbxSubEntity::getWorldTransforms(Matrix4* xform) const
{
	*xform = m_unk0xdc;
}

const Quaternion& RbxSubEntity::getWorldOrientation(void) const
{
	LogManager::getSingleton().logMessage("RBX perf warning!", LML_NORMAL, false);
	static Quaternion orientation = m_unk0xdc.extractQuaternion();
	return orientation;
}

const Vector3& RbxSubEntity::getWorldPosition(void) const
{
	static Vector3 position(m_unk0xdc[0][3], m_unk0xdc[1][3], m_unk0xdc[2][3]);
	return position;
}

void RbxSubEntity::FUN_0067bcc0(Real value)
{
	m_unk0x124 = value;
}

Real RbxSubEntity::FUN_0067bcd0() const
{
	return m_unk0x124;
}

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

RbxSubEntity::~RbxSubEntity()
{
	for (int i = 0; i < (int) m_unk0x130.size(); i++) {
		delete m_unk0x130[i];
	}
	m_unk0x130.clear();

	if (mSubMesh) {
		if (mSubMesh->useSharedVertices) {
			throw std::exception("RBX RbxSubEntity dtor(1)");
		}
		delete static_cast<Unknown00671c70*>(mSubMesh);
		mSubMesh = NULL;
	}
	mSubMesh = NULL;

	for (int i = 0; i < 1; i++) {
		if (m_unk0x11c[i]) {
			if (m_unk0x11c[i]->useSharedVertices) {
				throw std::exception("RBX RbxSubEntity dtor (2)");
			}
			delete static_cast<Unknown00671c70*>(m_unk0x11c[i]);
			m_unk0x11c[i] = NULL;
		}
	}
}

ShadowCaster::ShadowRenderableListIterator RbxSubEntity::getShadowVolumeRenderableIterator(
	ShadowTechnique shadowTechnique,
	const Light* light,
	HardwareIndexBufferSharedPtr* indexBuffer,
	bool extrude,
	Real extrusionDistance,
	unsigned long flags
)
{
	Unknown00671c70* subMesh = m_unk0x12c;
	if (!subMesh) {
		for (int i = 0; i >= 0; i--) {
			if (m_unk0x11c[i]) {
				subMesh = static_cast<Unknown00671c70*>(m_unk0x11c[i]);
				break;
			}
		}
		if (!subMesh) {
			subMesh = static_cast<Unknown00671c70*>(mSubMesh);
		}
	}

	if (m_unk0x130.empty()) {
		subMesh->FUN_0067c590();
	}

	Vector4 lightPos = light->getAs4DVector();
	Matrix4 world2Obj = m_unk0xdc.inverseAffine();
	lightPos = world2Obj.transformAffine(lightPos);

	EdgeData* edgeList = subMesh->m_unk0x88;
	if (!edgeList) {
		return ShadowRenderableListIterator(m_unk0x130.begin(), m_unk0x130.end());
	}

	bool init = m_unk0x130.empty();
	EdgeData::EdgeGroupList::iterator egi;
	ShadowRenderableList::iterator si, siend;
	RbxSubEntityShadowRenderable* esr = NULL;
	if (init) {
		m_unk0x130.resize(edgeList->edgeGroups.size());
	}

	siend = m_unk0x130.end();
	egi = edgeList->edgeGroups.begin();
	for (si = m_unk0x130.begin(); si != siend; ++si, ++egi) {
		const VertexData* pVertData = egi->vertexData;
		if (init) {
			*si = new RbxSubEntityShadowRenderable(NULL, indexBuffer, pVertData, !extrude, this);
		}

		esr = static_cast<RbxSubEntityShadowRenderable*>(*si);
		HardwareVertexBufferSharedPtr esrPositionBuffer = esr->getPositionBuffer();
		if (extrude) {
			extrudeVertices(esrPositionBuffer, egi->vertexData->vertexCount, lightPos, extrusionDistance);
		}
		esrPositionBuffer->suppressHardwareUpdate(false);
	}

	updateEdgeListLightFacing(edgeList, lightPos);
	generateShadowVolume(edgeList, *indexBuffer, light, m_unk0x130, flags);

	return ShadowRenderableListIterator(m_unk0x130.begin(), m_unk0x130.end());
}

} // namespace Ogre
