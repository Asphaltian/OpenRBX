#include "appdraw/RbxSubEntity.h"

#include <OgreCamera.h>
#include <OgreEntity.h>
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
	m_unk0x12c = 0;
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
	m_unk0x12c = 0;
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
