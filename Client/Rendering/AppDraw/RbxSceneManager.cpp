#include "appdraw/RbxSceneManager.h"

#include <OgreCamera.h>
#include <OgreLight.h>
#include <OgreMovableObject.h>
#include <OgrePass.h>
#include <OgreRenderQueue.h>
#include <OgreRenderSystem.h>
#include <OgreSceneNode.h>
#include <OgreTechnique.h>
#include <OgreViewport.h>

namespace Ogre {

// GLOBAL: ROBLOX 0x0096143c
const String RbxSceneManagerFactory::FACTORY_TYPE_NAME = "RbxSceneManager";

// FUNCTION: ROBLOX 0x00685f20
bool RbxSceneManager::validatePassForRendering(const Pass* pass)
{
	if (!mSuppressShadows && mCurrentViewport->getShadowsEnabled() &&
		((isShadowTechniqueModulative() && mIlluminationStage == IRS_RENDER_RECEIVER_PASS) ||
		 mIlluminationStage == IRS_RENDER_TO_TEXTURE || mSuppressRenderStateChanges) &&
		pass->getIndex() > 0) {
		return false;
	}

	return true;
}

// FUNCTION: ROBLOX 0x00685f80
bool RbxSceneManager::validateRenderableForRendering(const Pass* pass, const Renderable* rend)
{
	if (!mSuppressShadows && mCurrentViewport->getShadowsEnabled() && isShadowTechniqueTextureBased()) {
		if (mIlluminationStage == IRS_RENDER_RECEIVER_PASS && rend->getCastsShadows() && !mShadowTextureSelfShadow) {
			return false;
		}
		if (((isShadowTechniqueModulative() && mIlluminationStage == IRS_RENDER_RECEIVER_PASS) ||
			 mIlluminationStage == IRS_RENDER_TO_TEXTURE || mSuppressRenderStateChanges) &&
			pass->getIndex() > 0) {
			return false;
		}
	}

	return true;
}

// FUNCTION: ROBLOX 0x00686e70
const String& RbxSceneManager::getTypeName() const
{
	return RbxSceneManagerFactory::FACTORY_TYPE_NAME;
}

// FUNCTION: ROBLOX 0x00686e80
void RbxSceneManagerFactory::initMetaData() const
{
	mMetaData.typeName = FACTORY_TYPE_NAME;
	mMetaData.description = "The Roblox scene manager";
	mMetaData.sceneTypeMask = ST_GENERIC;
	mMetaData.worldGeometrySupported = false;
}

// FUNCTION: ROBLOX 0x00686eb0
void RbxSceneManagerFactory::destroyInstance(SceneManager* instance)
{
	delete instance;
}

// FUNCTION: ROBLOX 0x00687360
void RbxSceneManager::SceneMgrQueuedRenderableVisitor::visit(const Renderable* r)
{
	if (targetSceneMgr->validateRenderableForRendering(mUsedPass, r)) {
		targetSceneMgr->renderSingleObject(r, mUsedPass, autoLights, manualLightList);
	}
}

// FUNCTION: ROBLOX 0x006873a0
bool RbxSceneManager::SceneMgrQueuedRenderableVisitor::visit(const Pass* p)
{
	if (!targetSceneMgr->validatePassForRendering(p)) {
		return false;
	}

	mUsedPass = targetSceneMgr->_setPass(p);
	return true;
}

// FUNCTION: ROBLOX 0x006873e0
void RbxSceneManager::SceneMgrQueuedRenderableVisitor::visit(const RenderablePass* rp)
{
	if (transparentShadowCastersMode && !rp->pass->getParent()->getParent()->getTransparencyCastsShadows()) {
		return;
	}

	if (targetSceneMgr->validateRenderableForRendering(rp->pass, rp->renderable)) {
		mUsedPass = targetSceneMgr->_setPass(rp->pass);
		targetSceneMgr->renderSingleObject(rp->renderable, mUsedPass, autoLights, manualLightList);
	}
}

// FUNCTION: ROBLOX 0x006883b0
RbxSceneManager::RbxSceneManager(const String& name) : SceneManager(name)
{
	mShadowCasterQueryListener = new ShadowCasterSceneQueryListener(this);
	mActiveQueuedRenderableVisitor = &mDefaultQueuedRenderableVisitor;
}

// FUNCTION: ROBLOX 0x00689b90
SceneManager* RbxSceneManagerFactory::createInstance(const String& instanceName)
{
	return new RbxSceneManager(instanceName);
}

// FUNCTION: ROBLOX 0x00692e80
void RbxSceneManager::destroyAllCameras()
{
	CameraList::iterator i = mCameras.begin();
	for (; i != mCameras.end(); ++i) {
		mDestRenderSystem->_notifyCameraRemoved(i->second);
		delete i->second;
	}
	mCameras.clear();
}

// FUNCTION: ROBLOX 0x00692f40
void RbxSceneManager::clearScene()
{
	destroyAllStaticGeometry();
	destroyAllMovableObjects();

	mSceneRoot->removeAllChildren();
	mSceneRoot->detachAllObjects();

	for (SceneNodeList::iterator i = mSceneNodes.begin(); i != mSceneNodes.end(); ++i) {
		delete i->second;
	}
	mSceneNodes.clear();
	mAutoTrackingSceneNodes.clear();

	destroyAllAnimations();

	mSkyBoxNode = mSkyPlaneNode = mSkyDomeNode = NULL;
	mSkyBoxEnabled = mSkyPlaneEnabled = mSkyDomeEnabled = false;

	if (mRenderQueue) {
		mRenderQueue->clear(true);
	}
}

// FUNCTION: ROBLOX 0x00695ea0
bool RbxSceneManager::ShadowCasterSceneQueryListener::queryResult(MovableObject* object)
{
	if (object->getCastShadows() && object->isVisible() &&
		mSceneMgr->isRenderQueueToBeProcessed(object->getRenderQueueGroup()) &&
		((mSceneMgr->getShadowTechnique() & SHADOWDETAILTYPE_TEXTURE) ||
		 (mSceneMgr->getShadowTechnique() & SHADOWDETAILTYPE_STENCIL) && object->hasEdgeList())) {
		if (mFarDistSquared) {
			Vector3 toObj = object->getParentNode()->_getDerivedPosition() - mCamera->getDerivedPosition();
			Real radius = object->getWorldBoundingSphere().getRadius();
			Real dist = toObj.squaredLength();
			if (dist - (radius * radius) > mFarDistSquared) {
				return true;
			}
		}

		if (mCamera->isVisible(object->getWorldBoundingBox())) {
			mCasterList->push_back(object);
			return true;
		}

		if (!mIsLightInFrustum || mLight->getType() == Light::LT_DIRECTIONAL) {
			PlaneBoundedVolumeList::const_iterator i, iend;
			iend = mLightClipVolumeList->end();
			for (i = mLightClipVolumeList->begin(); i != iend; ++i) {
				if (i->intersects(object->getWorldBoundingBox())) {
					mCasterList->push_back(object);
					return true;
				}
			}
		}
	}
	return true;
}

// FUNCTION: ROBLOX 0x00697b70
RbxSceneManager::~RbxSceneManager()
{
	clearScene();
	destroyAllCameras();

	for (MovableObjectCollectionMap::iterator i = mMovableObjectCollectionMap.begin();
		 i != mMovableObjectCollectionMap.end();
		 ++i) {
		delete i->second;
	}
	mMovableObjectCollectionMap.clear();

	delete mShadowCasterQueryListener;
}

} // namespace Ogre
