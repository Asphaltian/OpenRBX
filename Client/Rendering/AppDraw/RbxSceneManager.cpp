#include "appdraw/RbxSceneManager.h"

#include <OgreAnimation.h>
#include <OgreAnimationState.h>
#include <OgreBillboardChain.h>
#include <OgreBillboardSet.h>
#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreException.h>
#include <OgreLight.h>
#include <OgreManualObject.h>
#include <OgreMaterialManager.h>
#include <OgreMovableObject.h>
#include <OgreParticleSystemManager.h>
#include <OgrePass.h>
#include <OgreRenderQueue.h>
#include <OgreRenderQueueListener.h>
#include <OgreRenderSystem.h>
#include <OgreRibbonTrail.h>
#include <OgreSceneNode.h>
#include <OgreStaticGeometry.h>
#include <OgreTechnique.h>
#include <OgreViewport.h>

namespace Ogre {

// GLOBAL: ROBLOX 0x0096143c
const String RbxSceneManagerFactory::FACTORY_TYPE_NAME = "RbxSceneManager";

// FUNCTION: ROBLOX 0x00685690
RenderQueue* RbxSceneManager::getRenderQueue(void)
{
	if (!mRenderQueue) {
		initRenderQueue();
	}
	return mRenderQueue;
}

// FUNCTION: ROBLOX 0x00685770
void RbxSceneManager::setSpecialCaseRenderQueueMode(SceneManager::SpecialCaseRenderQueueMode mode)
{
	mSpecialCaseQueueMode = mode;
}

// FUNCTION: ROBLOX 0x00685780
SceneManager::SpecialCaseRenderQueueMode RbxSceneManager::getSpecialCaseRenderQueueMode(void)
{
	return mSpecialCaseQueueMode;
}

// FUNCTION: ROBLOX 0x00685790
void RbxSceneManager::setWorldGeometryRenderQueue(uint8 qid)
{
	mWorldGeometryRenderQueue = qid;
}

// FUNCTION: ROBLOX 0x006857a0
uint8 RbxSceneManager::getWorldGeometryRenderQueue(void)
{
	return mWorldGeometryRenderQueue;
}

// FUNCTION: ROBLOX 0x006857c0
Light* RbxSceneManager::createLight(const String& name)
{
	return static_cast<Light*>(createMovableObject(name, LightFactory::FACTORY_TYPE_NAME));
}

// FUNCTION: ROBLOX 0x006857e0
Light* RbxSceneManager::getLight(const String& name) const
{
	return static_cast<Light*>(getMovableObject(name, LightFactory::FACTORY_TYPE_NAME));
}

// FUNCTION: ROBLOX 0x00685800
bool RbxSceneManager::hasLight(const String& name) const
{
	return hasMovableObject(name, LightFactory::FACTORY_TYPE_NAME);
}

// FUNCTION: ROBLOX 0x00685820
void RbxSceneManager::destroyLight(const String& name)
{
	destroyMovableObject(name, LightFactory::FACTORY_TYPE_NAME);
}

// FUNCTION: ROBLOX 0x00685840
void RbxSceneManager::destroyAllLights(void)
{
	destroyAllMovableObjectsByType(LightFactory::FACTORY_TYPE_NAME);
}

// FUNCTION: ROBLOX 0x00685860
const LightList& RbxSceneManager::_getLightsAffectingFrustum(void) const
{
	return mLightsAffectingFrustum;
}

// FUNCTION: ROBLOX 0x00685a10
Entity* RbxSceneManager::getEntity(const String& name) const
{
	return static_cast<Entity*>(getMovableObject(name, EntityFactory::FACTORY_TYPE_NAME));
}

// FUNCTION: ROBLOX 0x00685a30
bool RbxSceneManager::hasEntity(const String& name) const
{
	return hasMovableObject(name, EntityFactory::FACTORY_TYPE_NAME);
}

// FUNCTION: ROBLOX 0x00685a50
void RbxSceneManager::destroyEntity(const String& name)
{
	destroyMovableObject(name, EntityFactory::FACTORY_TYPE_NAME);
}

// FUNCTION: ROBLOX 0x00685a70
void RbxSceneManager::destroyAllEntities(void)
{

	destroyAllMovableObjectsByType(EntityFactory::FACTORY_TYPE_NAME);
}

// FUNCTION: ROBLOX 0x00685a90
void RbxSceneManager::destroyAllBillboardSets(void)
{
	destroyAllMovableObjectsByType(BillboardSetFactory::FACTORY_TYPE_NAME);
}

// FUNCTION: ROBLOX 0x00685ab0
ManualObject* RbxSceneManager::createManualObject(const String& name)
{
	return static_cast<ManualObject*>(createMovableObject(name, ManualObjectFactory::FACTORY_TYPE_NAME));
}

// FUNCTION: ROBLOX 0x00685ad0
ManualObject* RbxSceneManager::getManualObject(const String& name) const
{
	return static_cast<ManualObject*>(getMovableObject(name, ManualObjectFactory::FACTORY_TYPE_NAME));
}

// FUNCTION: ROBLOX 0x00685af0
bool RbxSceneManager::hasManualObject(const String& name) const
{
	return hasMovableObject(name, ManualObjectFactory::FACTORY_TYPE_NAME);
}

// FUNCTION: ROBLOX 0x00685b10
void RbxSceneManager::destroyManualObject(const String& name)
{
	destroyMovableObject(name, ManualObjectFactory::FACTORY_TYPE_NAME);
}

// FUNCTION: ROBLOX 0x00685b30
void RbxSceneManager::destroyAllManualObjects(void)
{
	destroyAllMovableObjectsByType(ManualObjectFactory::FACTORY_TYPE_NAME);
}

// FUNCTION: ROBLOX 0x00685b50
BillboardChain* RbxSceneManager::createBillboardChain(const String& name)
{
	return static_cast<BillboardChain*>(createMovableObject(name, BillboardChainFactory::FACTORY_TYPE_NAME));
}

// FUNCTION: ROBLOX 0x00685b70
BillboardChain* RbxSceneManager::getBillboardChain(const String& name) const
{
	return static_cast<BillboardChain*>(getMovableObject(name, BillboardChainFactory::FACTORY_TYPE_NAME));
}

// FUNCTION: ROBLOX 0x00685b90
bool RbxSceneManager::hasBillboardChain(const String& name) const
{
	return hasMovableObject(name, BillboardChainFactory::FACTORY_TYPE_NAME);
}

// FUNCTION: ROBLOX 0x00685bb0
void RbxSceneManager::destroyBillboardChain(const String& name)
{
	destroyMovableObject(name, BillboardChainFactory::FACTORY_TYPE_NAME);
}

// FUNCTION: ROBLOX 0x00685bd0
void RbxSceneManager::destroyAllBillboardChains(void)
{
	destroyAllMovableObjectsByType(BillboardChainFactory::FACTORY_TYPE_NAME);
}

// FUNCTION: ROBLOX 0x00685bf0
RibbonTrail* RbxSceneManager::createRibbonTrail(const String& name)
{
	return static_cast<RibbonTrail*>(createMovableObject(name, RibbonTrailFactory::FACTORY_TYPE_NAME));
}

// FUNCTION: ROBLOX 0x00685c10
RibbonTrail* RbxSceneManager::getRibbonTrail(const String& name) const
{
	return static_cast<RibbonTrail*>(getMovableObject(name, RibbonTrailFactory::FACTORY_TYPE_NAME));
}

// FUNCTION: ROBLOX 0x00685c30
bool RbxSceneManager::hasRibbonTrail(const String& name) const
{
	return hasMovableObject(name, RibbonTrailFactory::FACTORY_TYPE_NAME);
}

// FUNCTION: ROBLOX 0x00685c50
void RbxSceneManager::destroyLight(Light* l)
{
	destroyMovableObject(l);
}

// FUNCTION: ROBLOX 0x00685c50 FOLDED
void RbxSceneManager::destroyBillboardChain(BillboardChain* obj)
{
	destroyMovableObject(obj);
}

// FUNCTION: ROBLOX 0x00685c50 FOLDED
void RbxSceneManager::destroyBillboardSet(BillboardSet* set)
{
	destroyMovableObject(set);
}

// FUNCTION: ROBLOX 0x00685c50 FOLDED
void RbxSceneManager::destroyEntity(Entity* e)
{
	destroyMovableObject(e);
}

// FUNCTION: ROBLOX 0x00685c50 FOLDED
void RbxSceneManager::destroyManualObject(ManualObject* obj)
{
	destroyMovableObject(obj);
}

// FUNCTION: ROBLOX 0x00685c50 FOLDED
void RbxSceneManager::destroyRibbonTrail(RibbonTrail* obj)
{
	destroyMovableObject(obj);
}

// FUNCTION: ROBLOX 0x00685c60
void RbxSceneManager::destroyRibbonTrail(const String& name)
{
	destroyMovableObject(name, RibbonTrailFactory::FACTORY_TYPE_NAME);
}

// FUNCTION: ROBLOX 0x00685c80
void RbxSceneManager::destroyAllRibbonTrails(void)
{
	destroyAllMovableObjectsByType(RibbonTrailFactory::FACTORY_TYPE_NAME);
}

// FUNCTION: ROBLOX 0x00685ca0
ParticleSystem* RbxSceneManager::getParticleSystem(const String& name) const
{
	return static_cast<ParticleSystem*>(getMovableObject(name, ParticleSystemFactory::FACTORY_TYPE_NAME));
}

// FUNCTION: ROBLOX 0x00685cd0
bool RbxSceneManager::hasParticleSystem(const String& name) const
{
	return hasMovableObject(name, ParticleSystemFactory::FACTORY_TYPE_NAME);
}

// FUNCTION: ROBLOX 0x00685cf0
void RbxSceneManager::destroyParticleSystem(ParticleSystem* obj)
{
	destroyMovableObject(obj);
}

// FUNCTION: ROBLOX 0x00685d10
void RbxSceneManager::destroyParticleSystem(const String& name)
{
	destroyMovableObject(name, ParticleSystemFactory::FACTORY_TYPE_NAME);
}

// FUNCTION: ROBLOX 0x00685d30
void RbxSceneManager::destroyAllParticleSystems(void)
{
	destroyAllMovableObjectsByType(ParticleSystemFactory::FACTORY_TYPE_NAME);
}

// FUNCTION: ROBLOX 0x00685e90
void RbxSceneManager::_updateSceneGraph(Camera* cam)
{

	Node::processQueuedUpdates();

	mSceneRoot->_update(true, false);
}

// FUNCTION: ROBLOX 0x00685eb0
void RbxSceneManager::_findVisibleObjects(Camera* cam, VisibleObjectsBoundsInfo* visibleBounds, bool onlyShadowCasters)
{

	mSceneRoot->_findVisibleObjects(cam, getRenderQueue(), visibleBounds, true, mDisplayNodes, onlyShadowCasters);
}

// FUNCTION: ROBLOX 0x00685ef0
void RbxSceneManager::_renderVisibleObjects(void)
{
	RenderQueueInvocationSequence* invocationSequence = mCurrentViewport->_getRenderQueueInvocationSequence();

	if (invocationSequence && mIlluminationStage != IRS_RENDER_TO_TEXTURE) {
		renderVisibleObjectsCustomSequence(invocationSequence);
	}
	else {
		renderVisibleObjectsDefaultSequence();
	}
}

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

// FUNCTION: ROBLOX 0x00686010
void RbxSceneManager::renderObjects(
	const QueuedRenderableCollection& objs,
	QueuedRenderableCollection::OrganisationMode om,
	bool doLightIteration,
	const LightList* manualLightList
)
{
	mActiveQueuedRenderableVisitor->autoLights = doLightIteration;
	mActiveQueuedRenderableVisitor->manualLightList = manualLightList;
	mActiveQueuedRenderableVisitor->transparentShadowCastersMode = false;

	objs.acceptVisitor(mActiveQueuedRenderableVisitor, om);
}

// FUNCTION: ROBLOX 0x006861d0
const ColourValue& RbxSceneManager::getFogColour(void) const
{
	return mFogColour;
}

// FUNCTION: ROBLOX 0x006861e0
Real RbxSceneManager::getFogStart(void) const
{
	return mFogStart;
}

// FUNCTION: ROBLOX 0x006861f0
Real RbxSceneManager::getFogEnd(void) const
{
	return mFogEnd;
}

// FUNCTION: ROBLOX 0x00686200
Real RbxSceneManager::getFogDensity(void) const
{
	return mFogDensity;
}

// FUNCTION: ROBLOX 0x00686210
BillboardSet* RbxSceneManager::getBillboardSet(const String& name) const
{
	return static_cast<BillboardSet*>(getMovableObject(name, BillboardSetFactory::FACTORY_TYPE_NAME));
}

// FUNCTION: ROBLOX 0x00686230
bool RbxSceneManager::hasBillboardSet(const String& name) const
{
	return hasMovableObject(name, BillboardSetFactory::FACTORY_TYPE_NAME);
}

// FUNCTION: ROBLOX 0x00686250
void RbxSceneManager::destroyBillboardSet(const String& name)
{
	destroyMovableObject(name, BillboardSetFactory::FACTORY_TYPE_NAME);
}

// FUNCTION: ROBLOX 0x00686270
void RbxSceneManager::setDisplaySceneNodes(bool display)
{
	mDisplayNodes = display;
}

// FUNCTION: ROBLOX 0x006862d0
AnimationState* RbxSceneManager::getAnimationState(const String& animName) const
{
	return mAnimationStates.getAnimationState(animName);
}

// FUNCTION: ROBLOX 0x006862e0
bool RbxSceneManager::hasAnimationState(const String& name) const
{
	return mAnimationStates.hasAnimationState(name);
}

// FUNCTION: ROBLOX 0x006862f0
void RbxSceneManager::destroyAnimationState(const String& name)
{
	mAnimationStates.removeAnimationState(name);
}

// FUNCTION: ROBLOX 0x00686300
void RbxSceneManager::destroyAllAnimationStates(void)
{
	mAnimationStates.removeAllAnimationStates();
}

// FUNCTION: ROBLOX 0x00686480
void RbxSceneManager::setViewport(Viewport* vp)
{
	mCurrentViewport = vp;

	mDestRenderSystem->_setViewport(vp);

	MaterialManager::getSingleton().setActiveScheme(vp->getMaterialScheme());
}

// FUNCTION: ROBLOX 0x006864b0
void RbxSceneManager::showBoundingBoxes(bool bShow)
{
	mShowBoundingBoxes = bShow;
}

// FUNCTION: ROBLOX 0x006864c0
bool RbxSceneManager::getShowBoundingBoxes() const
{
	return mShowBoundingBoxes;
}

// FUNCTION: ROBLOX 0x006864d0
void RbxSceneManager::_suppressShadows(bool suppress)
{
	mSuppressShadows = suppress;
}

// FUNCTION: ROBLOX 0x006864e0
void RbxSceneManager::_suppressRenderStateChanges(bool suppress)
{
	mSuppressRenderStateChanges = suppress;
}

// FUNCTION: ROBLOX 0x00686690
void RbxSceneManager::_notifyLightsDirty(void)
{
	++mLightsDirtyCounter;
}

// FUNCTION: ROBLOX 0x00686820
const ColourValue& RbxSceneManager::getShadowColour(void) const
{
	return mShadowColour;
}

// FUNCTION: ROBLOX 0x00686830
void RbxSceneManager::setShadowFarDistance(Real distance)
{
	mShadowFarDist = distance;
	mShadowFarDistSquared = distance * distance;
}

// FUNCTION: ROBLOX 0x00686850
void RbxSceneManager::setShadowDirectionalLightExtrusionDistance(Real dist)
{
	mShadowDirLightExtrudeDist = dist;
}

// FUNCTION: ROBLOX 0x00686860
Real RbxSceneManager::getShadowDirectionalLightExtrusionDistance(void) const
{
	return mShadowDirLightExtrudeDist;
}

// FUNCTION: ROBLOX 0x00686930
void RbxSceneManager::setShadowTextureConfig(
	size_t shadowIndex, unsigned short width, unsigned short height, PixelFormat format
)
{
	ShadowTextureConfig conf;
	conf.width = width;
	conf.height = height;
	conf.format = format;

	setShadowTextureConfig(shadowIndex, conf);
}

// FUNCTION: ROBLOX 0x00686970
void RbxSceneManager::setShadowTextureSelfShadow(bool selfShadow)
{
	mShadowTextureSelfShadow = selfShadow;
	if (isShadowTechniqueTextureBased()) {
		getRenderQueue()->setShadowCastersCannotBeReceivers(!selfShadow);
	}
}

// FUNCTION: ROBLOX 0x006869b0
const ShadowCameraSetupPtr& RbxSceneManager::getShadowCameraSetup() const
{
	return mDefaultShadowCameraSetup;
}

// FUNCTION: ROBLOX 0x006869c0
void RbxSceneManager::destroyStaticGeometry(StaticGeometry* geom)
{
	destroyStaticGeometry(geom->getName());
}

// FUNCTION: ROBLOX 0x006869e0
void RbxSceneManager::destroyInstancedGeometry(InstancedGeometry* geom)
{
	destroyInstancedGeometry(geom->getName());
}

// FUNCTION: ROBLOX 0x00686db0
void RbxSceneManager::destroyQuery(SceneQuery* query)
{
	delete query;
}

// FUNCTION: ROBLOX 0x00686dd0
void RbxSceneManager::destroyMovableObject(MovableObject* m)
{
	destroyMovableObject(m->getName(), m->getMovableType());
}

// FUNCTION: ROBLOX 0x00686e00
void RbxSceneManager::extractMovableObject(MovableObject* m)
{
	extractMovableObject(m->getName(), m->getMovableType());
}

// FUNCTION: ROBLOX 0x00686e30
void RbxSceneManager::_injectRenderWithPass(Pass* pass, Renderable* rend, bool shadowDerivation)
{

	const Pass* usedPass = _setPass(pass, false, shadowDerivation);
	renderSingleObject(rend, usedPass, false);
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

// FUNCTION: ROBLOX 0x0068cea0
void RbxSceneManager::clearSpecialCaseRenderQueues(void)
{
	mSpecialCaseQueueList.clear();
}

// FUNCTION: ROBLOX 0x0068cfe0
bool RbxSceneManager::hasCamera(const String& name) const
{
	return (mCameras.find(name) != mCameras.end());
}

// FUNCTION: ROBLOX 0x0068d140
bool RbxSceneManager::hasSceneNode(const String& name) const
{
	return (mSceneNodes.find(name) != mSceneNodes.end());
}

// FUNCTION: ROBLOX 0x0068e230
bool RbxSceneManager::hasStaticGeometry(const String& name) const
{
	return (mStaticGeometryList.find(name) != mStaticGeometryList.end());
}

// FUNCTION: ROBLOX 0x0068f580
void RbxSceneManager::addSpecialCaseRenderQueue(uint8 qid)
{
	mSpecialCaseQueueList.insert(qid);
}

// FUNCTION: ROBLOX 0x00691650
void RbxSceneManager::_notifyAutotrackingSceneNode(SceneNode* node, bool autoTrack)
{
	if (autoTrack) {
		mAutoTrackingSceneNodes.insert(node);
	}
	else {
		mAutoTrackingSceneNodes.erase(node);
	}
}

// FUNCTION: ROBLOX 0x00692e60
void RbxSceneManager::removeSpecialCaseRenderQueue(uint8 qid)
{
	mSpecialCaseQueueList.erase(qid);
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

// FUNCTION: ROBLOX 0x00695e60
void RbxSceneManager::addRenderQueueListener(RenderQueueListener* newListener)
{
	mRenderQueueListeners.push_back(newListener);
}

// FUNCTION: ROBLOX 0x00695e80
void RbxSceneManager::addShadowListener(ShadowListener* newListener)
{
	mShadowListeners.push_back(newListener);
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

// FUNCTION: ROBLOX 0x00697660
SceneManager::MovableObjectCollection* RbxSceneManager::getMovableObjectCollection(const String& typeName)
{
	MovableObjectCollectionMap::iterator i = mMovableObjectCollectionMap.find(typeName);
	if (i == mMovableObjectCollectionMap.end()) {
		MovableObjectCollection* newCollection = new MovableObjectCollection();
		mMovableObjectCollectionMap[typeName] = newCollection;
		return newCollection;
	}
	else {
		return i->second;
	}
}

// FUNCTION: ROBLOX 0x00697a90
void RbxSceneManager::injectMovableObject(MovableObject* m)
{
	MovableObjectCollection* objectMap = getMovableObjectCollection(m->getMovableType());
	{
		OGRE_LOCK_MUTEX(objectMap->mutex)

		objectMap->map[m->getName()] = m;
	}
}

// FUNCTION: ROBLOX 0x00697b30
void RbxSceneManager::extractAllMovableObjectsByType(const String& typeName)
{
	MovableObjectCollection* objectMap = getMovableObjectCollection(typeName);
	{
		OGRE_LOCK_MUTEX(objectMap->mutex)

		objectMap->map.clear();
	}
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
