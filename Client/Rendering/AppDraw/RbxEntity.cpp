#include "appdraw/RbxEntity.h"

#include "appdraw/RbxSubEntity.h"

#include <OgreMesh.h>
#include <OgreRenderQueue.h>

namespace Ogre {

// FUNCTION: ROBLOX 0x00674ea0
void RbxEntity::_notifyCurrentCamera(Camera* cam)
{
	if (!m_unk0x245) {
		FUN_00676870(cam);
		m_unk0x245 = true;
	}
}

// FUNCTION: ROBLOX 0x00674fd0
void RbxEntity::FUN_00674fd0(bool visible)
{
	for (int i = 0; i < (int) mSubEntityList.size(); i++) {
		mSubEntityList[i]->setVisible(visible);
	}
}

// FUNCTION: ROBLOX 0x006754f0
void RbxEntity::_updateRenderQueue(RenderQueue* queue)
{
	if (!mInitialised) {
		return;
	}

	if (mMeshLodIndex > 0 && mMesh->isLodManual()) {
		mLodEntityList[mMeshLodIndex - 1]->_updateRenderQueue(queue);
		return;
	}

	SubEntityList::iterator i, iend;
	iend = mSubEntityList.end();
	for (i = mSubEntityList.begin(); i != iend; ++i) {
		if ((*i)->isVisible()) {
			if (mRenderQueueIDSet) {
				queue->addRenderable(*i, mRenderQueueID);
			}
			else {
				queue->addRenderable(*i);
			}
		}
	}
}

// FUNCTION: ROBLOX 0x00675bc0
RbxEntity::RbxEntity()
{
	m_unk0x268 = 20.0f;
	mInitialised = true;
	m_unk0x244 = true;
	m_unk0x246 = true;
	m_unk0x240 = 0;
	m_unk0x26c = 888;
}

// FUNCTION: ROBLOX 0x00675cd0
RbxEntity::~RbxEntity()
{
	for (int i = 0; i < (int) getNumSubEntities(); i++) {
		RbxSubEntity* subEntity = dynamic_cast<RbxSubEntity*>(mSubEntityList[i]);
		if (subEntity) {
			delete subEntity;
		}
	}
	mSubEntityList.clear();
}

// STUB: ROBLOX 0x00676870
void RbxEntity::FUN_00676870(Camera* cam)
{
	STUB(0x676870);
}

} // namespace Ogre
