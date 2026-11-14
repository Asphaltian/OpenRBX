#include "appdraw/RbxSceneManager.h"

#include "appdraw/RbxEntity.h"

#include <OgreAnimation.h>
#include <OgreAnimationState.h>
#include <OgreAnimationTrack.h>
#include <OgreBillboardChain.h>
#include <OgreBillboardSet.h>
#include <OgreCamera.h>
#include <OgreControllerManager.h>
#include <OgreDataStream.h>
#include <OgreEntity.h>
#include <OgreException.h>
#include <OgreGpuProgram.h>
#include <OgreGpuProgramManager.h>
#include <OgreHardwareBufferManager.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreInstancedGeometry.h>
#include <OgreLight.h>
#include <OgreLogManager.h>
#include <OgreManualObject.h>
#include <OgreMaterialManager.h>
#include <OgreMath.h>
#include <OgreMesh.h>
#include <OgreMeshManager.h>
#include <OgreMovableObject.h>
#include <OgreOverlay.h>
#include <OgreOverlayManager.h>
#include <OgreParticleSystemManager.h>
#include <OgrePass.h>
#include <OgreRenderQueue.h>
#include <OgreRenderQueueInvocation.h>
#include <OgreRenderQueueListener.h>
#include <OgreRenderQueueSortingGrouping.h>
#include <OgreRenderSystem.h>
#include <OgreRibbonTrail.h>
#include <OgreRoot.h>
#include <OgreSceneNode.h>
#include <OgreShadowVolumeExtrudeProgram.h>
#include <OgreSpotShadowFadePng.h>
#include <OgreStaticGeometry.h>
#include <OgreStringConverter.h>
#include <OgreSubEntity.h>
#include <OgreSubMesh.h>
#include <OgreTechnique.h>
#include <OgreTextureManager.h>
#include <OgreTextureUnitState.h>
#include <OgreTimer.h>
#include <OgreViewport.h>

namespace Ogre {

bool g_unk0x00961438;

const String RbxSceneManagerFactory::FACTORY_TYPE_NAME = "RbxSceneManager";

RenderQueue* RbxSceneManager::getRenderQueue(void)
{
	if (!mRenderQueue) {
		initRenderQueue();
	}
	return mRenderQueue;
}

void RbxSceneManager::initRenderQueue(void)
{
	mRenderQueue = new RenderQueue();

	mRenderQueue->getQueueGroup(RENDER_QUEUE_BACKGROUND)->setShadowsEnabled(false);
	mRenderQueue->getQueueGroup(RENDER_QUEUE_OVERLAY)->setShadowsEnabled(false);
	mRenderQueue->getQueueGroup(RENDER_QUEUE_SKIES_EARLY)->setShadowsEnabled(false);
	mRenderQueue->getQueueGroup(RENDER_QUEUE_SKIES_LATE)->setShadowsEnabled(false);
}

void RbxSceneManager::setSpecialCaseRenderQueueMode(SceneManager::SpecialCaseRenderQueueMode mode)
{
	mSpecialCaseQueueMode = mode;
}

SceneManager::SpecialCaseRenderQueueMode RbxSceneManager::getSpecialCaseRenderQueueMode(void)
{
	return mSpecialCaseQueueMode;
}

void RbxSceneManager::setWorldGeometryRenderQueue(uint8 qid)
{
	mWorldGeometryRenderQueue = qid;
}

uint8 RbxSceneManager::getWorldGeometryRenderQueue(void)
{
	return mWorldGeometryRenderQueue;
}

Light* RbxSceneManager::createLight(const String& name)
{
	return static_cast<Light*>(createMovableObject(name, LightFactory::FACTORY_TYPE_NAME));
}

Light* RbxSceneManager::getLight(const String& name) const
{
	return static_cast<Light*>(getMovableObject(name, LightFactory::FACTORY_TYPE_NAME));
}

bool RbxSceneManager::hasLight(const String& name) const
{
	return hasMovableObject(name, LightFactory::FACTORY_TYPE_NAME);
}

void RbxSceneManager::destroyLight(const String& name)
{
	destroyMovableObject(name, LightFactory::FACTORY_TYPE_NAME);
}

void RbxSceneManager::destroyAllLights(void)
{
	destroyAllMovableObjectsByType(LightFactory::FACTORY_TYPE_NAME);
}

const LightList& RbxSceneManager::_getLightsAffectingFrustum(void) const
{
	return mLightsAffectingFrustum;
}

Entity* RbxSceneManager::createEntity(const String& entityName, PrefabType ptype)
{
	switch (ptype) {
	case PT_PLANE:
		return createEntity(entityName, "Prefab_Plane");
	case PT_CUBE:
		return createEntity(entityName, "Prefab_Cube");
	case PT_SPHERE:
		return createEntity(entityName, "Prefab_Sphere");
	}

	OGRE_EXCEPT(
		Exception::ERR_ITEM_NOT_FOUND, "Unknown prefab type for entity " + entityName, "RbxSceneManager::createEntity"
	);
}

Entity* RbxSceneManager::getEntity(const String& name) const
{
	return static_cast<Entity*>(getMovableObject(name, EntityFactory::FACTORY_TYPE_NAME));
}

bool RbxSceneManager::hasEntity(const String& name) const
{
	return hasMovableObject(name, EntityFactory::FACTORY_TYPE_NAME);
}

void RbxSceneManager::destroyEntity(const String& name)
{
	destroyMovableObject(name, EntityFactory::FACTORY_TYPE_NAME);
}

void RbxSceneManager::destroyAllEntities(void)
{

	destroyAllMovableObjectsByType(EntityFactory::FACTORY_TYPE_NAME);
}

void RbxSceneManager::destroyAllBillboardSets(void)
{
	destroyAllMovableObjectsByType(BillboardSetFactory::FACTORY_TYPE_NAME);
}

ManualObject* RbxSceneManager::createManualObject(const String& name)
{
	return static_cast<ManualObject*>(createMovableObject(name, ManualObjectFactory::FACTORY_TYPE_NAME));
}

ManualObject* RbxSceneManager::getManualObject(const String& name) const
{
	return static_cast<ManualObject*>(getMovableObject(name, ManualObjectFactory::FACTORY_TYPE_NAME));
}

bool RbxSceneManager::hasManualObject(const String& name) const
{
	return hasMovableObject(name, ManualObjectFactory::FACTORY_TYPE_NAME);
}

void RbxSceneManager::destroyManualObject(const String& name)
{
	destroyMovableObject(name, ManualObjectFactory::FACTORY_TYPE_NAME);
}

void RbxSceneManager::destroyAllManualObjects(void)
{
	destroyAllMovableObjectsByType(ManualObjectFactory::FACTORY_TYPE_NAME);
}

BillboardChain* RbxSceneManager::createBillboardChain(const String& name)
{
	return static_cast<BillboardChain*>(createMovableObject(name, BillboardChainFactory::FACTORY_TYPE_NAME));
}

BillboardChain* RbxSceneManager::getBillboardChain(const String& name) const
{
	return static_cast<BillboardChain*>(getMovableObject(name, BillboardChainFactory::FACTORY_TYPE_NAME));
}

bool RbxSceneManager::hasBillboardChain(const String& name) const
{
	return hasMovableObject(name, BillboardChainFactory::FACTORY_TYPE_NAME);
}

void RbxSceneManager::destroyBillboardChain(const String& name)
{
	destroyMovableObject(name, BillboardChainFactory::FACTORY_TYPE_NAME);
}

void RbxSceneManager::destroyAllBillboardChains(void)
{
	destroyAllMovableObjectsByType(BillboardChainFactory::FACTORY_TYPE_NAME);
}

RibbonTrail* RbxSceneManager::createRibbonTrail(const String& name)
{
	return static_cast<RibbonTrail*>(createMovableObject(name, RibbonTrailFactory::FACTORY_TYPE_NAME));
}

RibbonTrail* RbxSceneManager::getRibbonTrail(const String& name) const
{
	return static_cast<RibbonTrail*>(getMovableObject(name, RibbonTrailFactory::FACTORY_TYPE_NAME));
}

bool RbxSceneManager::hasRibbonTrail(const String& name) const
{
	return hasMovableObject(name, RibbonTrailFactory::FACTORY_TYPE_NAME);
}

void RbxSceneManager::destroyLight(Light* l)
{
	destroyMovableObject(l);
}

void RbxSceneManager::destroyBillboardChain(BillboardChain* obj)
{
	destroyMovableObject(obj);
}

void RbxSceneManager::destroyBillboardSet(BillboardSet* set)
{
	destroyMovableObject(set);
}

void RbxSceneManager::destroyEntity(Entity* e)
{
	destroyMovableObject(e);
}

void RbxSceneManager::destroyManualObject(ManualObject* obj)
{
	destroyMovableObject(obj);
}

void RbxSceneManager::destroyRibbonTrail(RibbonTrail* obj)
{
	destroyMovableObject(obj);
}

void RbxSceneManager::destroyRibbonTrail(const String& name)
{
	destroyMovableObject(name, RibbonTrailFactory::FACTORY_TYPE_NAME);
}

void RbxSceneManager::destroyAllRibbonTrails(void)
{
	destroyAllMovableObjectsByType(RibbonTrailFactory::FACTORY_TYPE_NAME);
}

ParticleSystem* RbxSceneManager::getParticleSystem(const String& name) const
{
	return static_cast<ParticleSystem*>(getMovableObject(name, ParticleSystemFactory::FACTORY_TYPE_NAME));
}

bool RbxSceneManager::hasParticleSystem(const String& name) const
{
	return hasMovableObject(name, ParticleSystemFactory::FACTORY_TYPE_NAME);
}

void RbxSceneManager::destroyParticleSystem(ParticleSystem* obj)
{
	destroyMovableObject(obj);
}

void RbxSceneManager::destroyParticleSystem(const String& name)
{
	destroyMovableObject(name, ParticleSystemFactory::FACTORY_TYPE_NAME);
}

void RbxSceneManager::destroyAllParticleSystems(void)
{
	destroyAllMovableObjectsByType(ParticleSystemFactory::FACTORY_TYPE_NAME);
}

void RbxSceneManager::setWorldGeometry(const String& filename)
{

	OGRE_EXCEPT(
		Exception::ERR_INVALIDPARAMS,
		"World geometry is not supported by the generic SceneManager.",
		"RbxSceneManager::setWorldGeometry"
	);
}

void RbxSceneManager::setWorldGeometry(DataStreamPtr& stream, const String& typeName)
{

	OGRE_EXCEPT(
		Exception::ERR_INVALIDPARAMS,
		"World geometry is not supported by the generic SceneManager.",
		"RbxSceneManager::setWorldGeometry"
	);
}

void RbxSceneManager::_updateSceneGraph(Camera* cam)
{

	Node::processQueuedUpdates();

	mSceneRoot->_update(true, false);
}

void RbxSceneManager::_findVisibleObjects(Camera* cam, VisibleObjectsBoundsInfo* visibleBounds, bool onlyShadowCasters)
{

	mSceneRoot->_findVisibleObjects(cam, getRenderQueue(), visibleBounds, true, mDisplayNodes, onlyShadowCasters);
}

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

void RbxSceneManager::_renderQueueGroupObjects(
	RenderQueueGroup* pGroup, QueuedRenderableCollection::OrganisationMode om
)
{
	bool doShadows = pGroup->getShadowsEnabled() && mCurrentViewport->getShadowsEnabled() && !mSuppressShadows &&
					 !mSuppressRenderStateChanges;

	if (doShadows && mShadowTechnique == SHADOWTYPE_STENCIL_ADDITIVE) {

		renderAdditiveStencilShadowedQueueGroupObjects(pGroup, om);
	}
	else if (doShadows && mShadowTechnique == SHADOWTYPE_STENCIL_MODULATIVE) {

		renderModulativeStencilShadowedQueueGroupObjects(pGroup, om);
	}
	else if (isShadowTechniqueTextureBased()) {

		if (mIlluminationStage == IRS_RENDER_TO_TEXTURE) {

			if (mCurrentViewport->getShadowsEnabled() && !mSuppressShadows && !mSuppressRenderStateChanges) {
				renderTextureShadowCasterQueueGroupObjects(pGroup, om);
			}
		}
		else {

			if (doShadows && !isShadowTechniqueIntegrated()) {

				if (isShadowTechniqueAdditive()) {

					renderAdditiveTextureShadowedQueueGroupObjects(pGroup, om);
				}
				else {

					renderModulativeTextureShadowedQueueGroupObjects(pGroup, om);
				}
			}
			else {
				renderBasicQueueGroupObjects(pGroup, om);
			}
		}
	}
	else {

		renderBasicQueueGroupObjects(pGroup, om);
	}
}

void RbxSceneManager::renderTransparentShadowCasterObjects(
	const QueuedRenderableCollection& objs,
	QueuedRenderableCollection::OrganisationMode om,
	bool doLightIteration,
	const LightList* manualLightList
)
{
	mActiveQueuedRenderableVisitor->transparentShadowCastersMode = true;
	mActiveQueuedRenderableVisitor->autoLights = doLightIteration;
	mActiveQueuedRenderableVisitor->manualLightList = manualLightList;

	objs.acceptVisitor(mActiveQueuedRenderableVisitor, QueuedRenderableCollection::OM_SORT_DESCENDING);

	mActiveQueuedRenderableVisitor->transparentShadowCastersMode = false;
}

const ColourValue& RbxSceneManager::getFogColour(void) const
{
	return mFogColour;
}

Real RbxSceneManager::getFogStart(void) const
{
	return mFogStart;
}

Real RbxSceneManager::getFogEnd(void) const
{
	return mFogEnd;
}

Real RbxSceneManager::getFogDensity(void) const
{
	return mFogDensity;
}

BillboardSet* RbxSceneManager::getBillboardSet(const String& name) const
{
	return static_cast<BillboardSet*>(getMovableObject(name, BillboardSetFactory::FACTORY_TYPE_NAME));
}

bool RbxSceneManager::hasBillboardSet(const String& name) const
{
	return hasMovableObject(name, BillboardSetFactory::FACTORY_TYPE_NAME);
}

void RbxSceneManager::destroyBillboardSet(const String& name)
{
	destroyMovableObject(name, BillboardSetFactory::FACTORY_TYPE_NAME);
}

void RbxSceneManager::setDisplaySceneNodes(bool display)
{
	mDisplayNodes = display;
}

AnimationState* RbxSceneManager::createAnimationState(const String& animName)
{

	Animation* anim = getAnimation(animName);

	return mAnimationStates.createAnimationState(animName, 0, anim->getLength());
}

AnimationState* RbxSceneManager::getAnimationState(const String& animName) const
{
	return mAnimationStates.getAnimationState(animName);
}

bool RbxSceneManager::hasAnimationState(const String& name) const
{
	return mAnimationStates.hasAnimationState(name);
}

void RbxSceneManager::destroyAnimationState(const String& name)
{
	mAnimationStates.removeAnimationState(name);
}

void RbxSceneManager::destroyAllAnimationStates(void)
{
	mAnimationStates.removeAllAnimationStates();
}

void RbxSceneManager::manualRender(
	RenderOperation* rend,
	Pass* pass,
	Viewport* vp,
	const Matrix4& worldMatrix,
	const Matrix4& viewMatrix,
	const Matrix4& projMatrix,
	bool doBeginEndFrame
)
{
	mDestRenderSystem->_setViewport(vp);
	mDestRenderSystem->_setWorldMatrix(worldMatrix);
	mDestRenderSystem->_setViewMatrix(viewMatrix);
	mDestRenderSystem->_setProjectionMatrix(projMatrix);

	if (doBeginEndFrame) {
		mDestRenderSystem->_beginFrame();
	}

	_setPass(pass);

	mDestRenderSystem->_render(*rend);

	if (doBeginEndFrame) {
		mDestRenderSystem->_endFrame();
	}
}

void RbxSceneManager::useRenderableViewProjMode(const Renderable* pRend)
{

	bool useIdentityView = pRend->getUseIdentityView();
	if (useIdentityView) {

		mDestRenderSystem->_setViewMatrix(Matrix4::IDENTITY);
		mResetIdentityView = true;
	}

	bool useIdentityProj = pRend->getUseIdentityProjection();
	if (useIdentityProj) {

		Matrix4 mat;
		mDestRenderSystem->_convertProjectionMatrix(Matrix4::IDENTITY, mat);
		mDestRenderSystem->_setProjectionMatrix(mat);

		mResetIdentityProj = true;
	}
}

void RbxSceneManager::resetViewProjMode(void)
{
	if (mResetIdentityView) {

		mDestRenderSystem->_setViewMatrix(mCameraInProgress->getViewMatrix(true));
		mResetIdentityView = false;
	}

	if (mResetIdentityProj) {

		mDestRenderSystem->_setProjectionMatrix(mCameraInProgress->getProjectionMatrixRS());
		mResetIdentityProj = false;
	}
}

void RbxSceneManager::setViewport(Viewport* vp)
{
	mCurrentViewport = vp;

	mDestRenderSystem->_setViewport(vp);

	MaterialManager::getSingleton().setActiveScheme(vp->getMaterialScheme());
}

void RbxSceneManager::showBoundingBoxes(bool bShow)
{
	mShowBoundingBoxes = bShow;
}

bool RbxSceneManager::getShowBoundingBoxes() const
{
	return mShowBoundingBoxes;
}

void RbxSceneManager::_suppressShadows(bool suppress)
{
	mSuppressShadows = suppress;
}

void RbxSceneManager::_suppressRenderStateChanges(bool suppress)
{
	mSuppressRenderStateChanges = suppress;
}

void RbxSceneManager::updateRenderQueueSplitOptions(void)
{
	if (isShadowTechniqueStencilBased()) {

		getRenderQueue()->setShadowCastersCannotBeReceivers(false);
	}
	else {
		getRenderQueue()->setShadowCastersCannotBeReceivers(!mShadowTextureSelfShadow);
	}

	if (isShadowTechniqueAdditive() && !isShadowTechniqueIntegrated() && mCurrentViewport->getShadowsEnabled()) {

		getRenderQueue()->setSplitPassesByLightingType(true);
	}
	else {
		getRenderQueue()->setSplitPassesByLightingType(false);
	}

	if (isShadowTechniqueInUse() && mCurrentViewport->getShadowsEnabled() && !isShadowTechniqueIntegrated()) {

		getRenderQueue()->setSplitNoShadowPasses(true);
	}
	else {
		getRenderQueue()->setSplitNoShadowPasses(false);
	}
}

void RbxSceneManager::updateRenderQueueGroupSplitOptions(
	RenderQueueGroup* group, bool suppressShadows, bool suppressRenderState
)
{
	if (isShadowTechniqueStencilBased()) {

		group->setShadowCastersCannotBeReceivers(false);
	}
	else if (isShadowTechniqueTextureBased()) {
		group->setShadowCastersCannotBeReceivers(!mShadowTextureSelfShadow);
	}

	if (!suppressShadows && mCurrentViewport->getShadowsEnabled() && isShadowTechniqueAdditive() &&
		!isShadowTechniqueIntegrated()) {

		group->setSplitPassesByLightingType(true);
	}
	else {
		group->setSplitPassesByLightingType(false);
	}

	if (!suppressShadows && mCurrentViewport->getShadowsEnabled() && isShadowTechniqueInUse()) {

		group->setSplitNoShadowPasses(true);
	}
	else {
		group->setSplitNoShadowPasses(false);
	}
}

void RbxSceneManager::_notifyLightsDirty(void)
{
	++mLightsDirtyCounter;
}

bool RbxSceneManager::lightsForShadowTextureLess::operator()(const Ogre::Light* l1, const Ogre::Light* l2) const
{
	if (l1 == l2) {
		return false;
	}

	if (l1->getCastShadows() != l2->getCastShadows()) {
		return l1->getCastShadows();
	}

	return l1->tempSquareDist < l2->tempSquareDist;
}

void RbxSceneManager::setShadowVolumeStencilState(bool secondpass, bool zfail, bool twosided)
{

	StencilOperation incrOp, decrOp;
	if (mDestRenderSystem->getCapabilities()->hasCapability(RSC_STENCIL_WRAP)) {
		incrOp = SOP_INCREMENT_WRAP;
		decrOp = SOP_DECREMENT_WRAP;
	}
	else {
		incrOp = SOP_INCREMENT;
		decrOp = SOP_DECREMENT;
	}

	if (!twosided && ((secondpass || zfail) && !(secondpass && zfail))) {
		mDestRenderSystem->_setCullingMode(twosided ? CULL_NONE : CULL_ANTICLOCKWISE);
		mDestRenderSystem->setStencilBufferParams(
			CMPF_ALWAYS_PASS, 0, 0xFFFFFFFF, SOP_KEEP, zfail ? incrOp : SOP_KEEP, zfail ? SOP_KEEP : decrOp, twosided
		);
	}
	else {
		mDestRenderSystem->_setCullingMode(twosided ? CULL_NONE : CULL_CLOCKWISE);
		mDestRenderSystem->setStencilBufferParams(
			CMPF_ALWAYS_PASS, 0, 0xFFFFFFFF, SOP_KEEP, zfail ? decrOp : SOP_KEEP, zfail ? SOP_KEEP : incrOp, twosided
		);
	}
}

void RbxSceneManager::setShadowColour(const ColourValue& colour)
{
	mShadowColour = colour;

	if (mShadowModulativePass) {
		mShadowModulativePass->getTextureUnitState(0)->setColourOperationEx(
			LBX_MODULATE, LBS_MANUAL, LBS_CURRENT, colour
		);
	}
}

const ColourValue& RbxSceneManager::getShadowColour(void) const
{
	return mShadowColour;
}

void RbxSceneManager::setShadowFarDistance(Real distance)
{
	mShadowFarDist = distance;
	mShadowFarDistSquared = distance * distance;
}

void RbxSceneManager::setShadowDirectionalLightExtrusionDistance(Real dist)
{
	mShadowDirLightExtrudeDist = dist;
}

Real RbxSceneManager::getShadowDirectionalLightExtrusionDistance(void) const
{
	return mShadowDirLightExtrudeDist;
}

void RbxSceneManager::setShadowIndexBufferSize(size_t size)
{
	if (!mShadowIndexBuffer.isNull() && size != mShadowIndexBufferSize) {

		mShadowIndexBuffer = HardwareBufferManager::getSingleton().createIndexBuffer(
			HardwareIndexBuffer::IT_16BIT, size, HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE, false
		);
	}
	mShadowIndexBufferSize = size;
}

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

void RbxSceneManager::setShadowTextureSelfShadow(bool selfShadow)
{
	mShadowTextureSelfShadow = selfShadow;
	if (isShadowTechniqueTextureBased()) {
		getRenderQueue()->setShadowCastersCannotBeReceivers(!selfShadow);
	}
}

const ShadowCameraSetupPtr& RbxSceneManager::getShadowCameraSetup() const
{
	return mDefaultShadowCameraSetup;
}

void RbxSceneManager::destroyStaticGeometry(StaticGeometry* geom)
{
	destroyStaticGeometry(geom->getName());
}

void RbxSceneManager::destroyInstancedGeometry(InstancedGeometry* geom)
{
	destroyInstancedGeometry(geom->getName());
}

AxisAlignedBoxSceneQuery* RbxSceneManager::createAABBQuery(const AxisAlignedBox& box, unsigned long mask)
{
	DefaultAxisAlignedBoxSceneQuery* q = new DefaultAxisAlignedBoxSceneQuery(this);
	q->setBox(box);
	q->setQueryMask(mask);
	return q;
}

SphereSceneQuery* RbxSceneManager::createSphereQuery(const Sphere& sphere, unsigned long mask)
{
	DefaultSphereSceneQuery* q = new DefaultSphereSceneQuery(this);
	q->setSphere(sphere);
	q->setQueryMask(mask);
	return q;
}

PlaneBoundedVolumeListSceneQuery* RbxSceneManager::createPlaneBoundedVolumeQuery(
	const PlaneBoundedVolumeList& volumes, unsigned long mask
)
{
	DefaultPlaneBoundedVolumeListSceneQuery* q = new DefaultPlaneBoundedVolumeListSceneQuery(this);
	q->setVolumes(volumes);
	q->setQueryMask(mask);
	return q;
}

RaySceneQuery* RbxSceneManager::createRayQuery(const Ray& ray, unsigned long mask)
{
	DefaultRaySceneQuery* q = new DefaultRaySceneQuery(this);
	q->setRay(ray);
	q->setQueryMask(mask);
	return q;
}

IntersectionSceneQuery* RbxSceneManager::createIntersectionQuery(unsigned long mask)
{

	DefaultIntersectionSceneQuery* q = new DefaultIntersectionSceneQuery(this);
	q->setQueryMask(mask);
	return q;
}

void RbxSceneManager::destroyQuery(SceneQuery* query)
{
	delete query;
}

void RbxSceneManager::destroyMovableObject(MovableObject* m)
{
	destroyMovableObject(m->getName(), m->getMovableType());
}

void RbxSceneManager::extractMovableObject(MovableObject* m)
{
	extractMovableObject(m->getName(), m->getMovableType());
}

void RbxSceneManager::_injectRenderWithPass(Pass* pass, Renderable* rend, bool shadowDerivation)
{

	const Pass* usedPass = _setPass(pass, false, shadowDerivation);
	renderSingleObject(rend, usedPass, false);
}

const String& RbxSceneManager::getTypeName() const
{
	return RbxSceneManagerFactory::FACTORY_TYPE_NAME;
}

void RbxSceneManagerFactory::initMetaData() const
{
	mMetaData.typeName = FACTORY_TYPE_NAME;
	mMetaData.description = "The Roblox scene manager";
	mMetaData.sceneTypeMask = ST_GENERIC;
	mMetaData.worldGeometrySupported = false;
}

void RbxSceneManagerFactory::destroyInstance(SceneManager* instance)
{
	delete instance;
}

void RbxSceneManager::SceneMgrQueuedRenderableVisitor::visit(const Renderable* r)
{
	if (targetSceneMgr->validateRenderableForRendering(mUsedPass, r)) {
		targetSceneMgr->renderSingleObject(r, mUsedPass, autoLights, manualLightList);
	}
}

bool RbxSceneManager::SceneMgrQueuedRenderableVisitor::visit(const Pass* p)
{
	if (!targetSceneMgr->validatePassForRendering(p)) {
		return false;
	}

	mUsedPass = targetSceneMgr->_setPass(p);
	return true;
}

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

ViewPoint RbxSceneManager::getSuggestedViewpoint(bool random)
{

	ViewPoint vp;
	vp.position = Vector3::ZERO;
	vp.orientation = Quaternion::IDENTITY;
	return vp;
}

void RbxSceneManager::_queueSkiesForRendering(Camera* cam)
{

	if (mSkyPlaneNode) {

		mSkyPlaneNode->setPosition(cam->getDerivedPosition());
	}

	if (mSkyBoxNode) {
		mSkyBoxNode->setPosition(cam->getDerivedPosition());
		m_unk0x8a90->setPosition(cam->getDerivedPosition());
	}

	if (mSkyDomeNode) {
		mSkyDomeNode->setPosition(cam->getDerivedPosition());
	}

	RenderQueueGroupID qid;
	if (mSkyPlaneEnabled) {
		qid = mSkyPlaneDrawFirst ? RENDER_QUEUE_SKIES_EARLY : RENDER_QUEUE_SKIES_LATE;
		getRenderQueue()->addRenderable(mSkyPlaneEntity->getSubEntity(0), qid, OGRE_RENDERABLE_DEFAULT_PRIORITY);
	}

	uint plane;
	if (mSkyBoxEnabled) {
		qid = mSkyBoxDrawFirst ? RENDER_QUEUE_SKIES_EARLY : RENDER_QUEUE_SKIES_LATE;

		for (plane = 0; plane < 6; ++plane) {
			getRenderQueue()->addRenderable(
				mSkyBoxEntity[plane]->getSubEntity(0), qid, OGRE_RENDERABLE_DEFAULT_PRIORITY
			);
		}
		m_unk0x8a84->FUN_00678610(cam, getRenderQueue(), qid);
	}

	if (mSkyDomeEnabled) {
		qid = mSkyDomeDrawFirst ? RENDER_QUEUE_SKIES_EARLY : RENDER_QUEUE_SKIES_LATE;

		for (plane = 0; plane < 5; ++plane) {
			getRenderQueue()->addRenderable(
				mSkyDomeEntity[plane]->getSubEntity(0), qid, OGRE_RENDERABLE_DEFAULT_PRIORITY
			);
		}
	}
}

void RbxSceneManager::setShadowTechnique(ShadowTechnique technique)
{
	mShadowTechnique = technique;
	if (isShadowTechniqueStencilBased()) {

		if (!mDestRenderSystem->getCapabilities()->hasCapability(RSC_HWSTENCIL)) {
			LogManager::getSingleton().logMessage(
				"WARNING: Stencil shadows were requested, but this device does not "
				"have a hardware stencil. Shadows disabled."
			);
			mShadowTechnique = SHADOWTYPE_NONE;
		}
		else if (mShadowIndexBuffer.isNull()) {

			mShadowIndexBuffer = HardwareBufferManager::getSingleton().createIndexBuffer(
				HardwareIndexBuffer::IT_16BIT,
				mShadowIndexBufferSize,
				HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE,
				false
			);

			MeshManager::getSingleton().setPrepareAllMeshesForShadowVolumes(true);
		}
	}

	if (!isShadowTechniqueTextureBased()) {

		destroyShadowTextures();
	}
	else {

		for (size_t i = 0; i < mShadowTextureCameras.size(); ++i) {
			Camera* texCam = mShadowTextureCameras[i];

			texCam->setCustomViewMatrix(false);
			texCam->setCustomProjectionMatrix(false);
		}
	}
}

void RbxSceneManager::setShadowTextureConfig(size_t shadowIndex, const ShadowTextureConfig& config)
{
	if (shadowIndex >= mShadowTextureConfigList.size()) {
		OGRE_EXCEPT(
			Exception::ERR_ITEM_NOT_FOUND, "shadowIndex out of bounds", "RbxSceneManager::setShadowTextureConfig"
		);
	}
	mShadowTextureConfigList[shadowIndex] = config;

	mShadowTextureConfigDirty = true;
}

const TexturePtr& RbxSceneManager::getShadowTexture(size_t shadowIndex)
{
	if (shadowIndex >= mShadowTextureConfigList.size()) {
		OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "shadowIndex out of bounds", "RbxSceneManager::getShadowTexture");
	}
	ensureShadowTexturesCreated();

	return mShadowTextures[shadowIndex];
}

RbxSceneManager::RbxSceneManager(const String& name) : SceneManager(name)
{
	mShadowCasterQueryListener = new ShadowCasterSceneQueryListener(this);
	mActiveQueuedRenderableVisitor = &mDefaultQueuedRenderableVisitor;
}

void RbxSceneManager::setSkyPlane(
	bool enable,
	const Plane& plane,
	const String& materialName,
	Real gscale,
	Real tiling,
	bool drawFirst,
	Real bow,
	int xsegments,
	int ysegments,
	const String& groupName
)
{
	if (enable) {
		String meshName = mName + "SkyPlane";
		mSkyPlane = plane;

		MaterialPtr m = MaterialManager::getSingleton().getByName(materialName);
		if (m.isNull()) {
			OGRE_EXCEPT(
				Exception::ERR_INVALIDPARAMS,
				"Sky plane material '" + materialName + "' not found.",
				"RbxSceneManager::setSkyPlane"
			);
		}

		m->setDepthWriteEnabled(false);

		m->load();

		mSkyPlaneDrawFirst = drawFirst;

		MeshPtr planeMesh = MeshManager::getSingleton().getByName(meshName);
		if (!planeMesh.isNull()) {

			MeshManager::getSingleton().remove(planeMesh->getHandle());
		}

		Vector3 up = plane.normal.crossProduct(Vector3::UNIT_X);
		if (up == Vector3::ZERO) {
			up = plane.normal.crossProduct(-Vector3::UNIT_Z);
		}

		if (bow > 0) {

			planeMesh = MeshManager::getSingleton().createCurvedPlane(
				meshName,
				groupName,
				plane,
				gscale * 100,
				gscale * 100,
				gscale * bow * 100,
				xsegments,
				ysegments,
				false,
				1,
				tiling,
				tiling,
				up
			);
		}
		else {
			planeMesh = MeshManager::getSingleton().createPlane(
				meshName,
				groupName,
				plane,
				gscale * 100,
				gscale * 100,
				xsegments,
				ysegments,
				false,
				1,
				tiling,
				tiling,
				up
			);
		}

		if (mSkyPlaneEntity) {

			destroyEntity(meshName);
		}

		mSkyPlaneEntity = createEntity(meshName, meshName);
		mSkyPlaneEntity->setMaterialName(materialName);
		mSkyPlaneEntity->setCastShadows(false);

		if (!mSkyPlaneNode) {
			mSkyPlaneNode = createSceneNode(meshName + "Node");
		}
		else {
			mSkyPlaneNode->detachAllObjects();
		}
		mSkyPlaneNode->attachObject(mSkyPlaneEntity);
	}
	mSkyPlaneEnabled = enable;
	mSkyPlaneGenParameters.skyPlaneBow = bow;
	mSkyPlaneGenParameters.skyPlaneScale = gscale;
	mSkyPlaneGenParameters.skyPlaneTiling = tiling;
	mSkyPlaneGenParameters.skyPlaneXSegments = xsegments;
	mSkyPlaneGenParameters.skyPlaneYSegments = ysegments;
}

MeshPtr RbxSceneManager::createSkyboxPlane(
	BoxPlane bp, Real distance, const Quaternion& orientation, const String& groupName
)
{
	Plane plane;
	String meshName;
	Vector3 up;

	meshName = mName + "SkyBoxPlane_";
	plane.d = distance;
	switch (bp) {
	case BP_FRONT:
		plane.normal = Vector3::UNIT_Z;
		up = Vector3::UNIT_Y;
		meshName += "Front";
		break;
	case BP_BACK:
		plane.normal = -Vector3::UNIT_Z;
		up = Vector3::UNIT_Y;
		meshName += "Back";
		break;
	case BP_LEFT:
		plane.normal = Vector3::UNIT_X;
		up = Vector3::UNIT_Y;
		meshName += "Left";
		break;
	case BP_RIGHT:
		plane.normal = -Vector3::UNIT_X;
		up = Vector3::UNIT_Y;
		meshName += "Right";
		break;
	case BP_UP:
		plane.normal = -Vector3::UNIT_Y;
		up = Vector3::UNIT_Z;
		meshName += "Up";
		break;
	case BP_DOWN:
		plane.normal = Vector3::UNIT_Y;
		up = -Vector3::UNIT_Z;
		meshName += "Down";
		break;
	}

	plane.normal = orientation * plane.normal;
	up = orientation * up;

	MeshManager& mm = MeshManager::getSingleton();
	MeshPtr planeMesh = mm.getByName(meshName);
	if (!planeMesh.isNull()) {
		mm.remove(planeMesh->getHandle());
	}

	Real planeSize = distance * 2;
	const int BOX_SEGMENTS = 1;
	planeMesh = mm.createPlane(
		meshName, groupName, plane, planeSize, planeSize, BOX_SEGMENTS, BOX_SEGMENTS, false, 1, 1, 1, up
	);

	return planeMesh;
}

MeshPtr RbxSceneManager::createSkydomePlane(
	BoxPlane bp,
	Real curvature,
	Real tiling,
	Real distance,
	const Quaternion& orientation,
	int xsegments,
	int ysegments,
	int ysegments_keep,
	const String& groupName
)
{

	Plane plane;
	String meshName;
	Vector3 up;

	meshName = mName + "SkyDomePlane_";

	plane.d = distance;
	switch (bp) {
	case BP_FRONT:
		plane.normal = Vector3::UNIT_Z;
		up = Vector3::UNIT_Y;
		meshName += "Front";
		break;
	case BP_BACK:
		plane.normal = -Vector3::UNIT_Z;
		up = Vector3::UNIT_Y;
		meshName += "Back";
		break;
	case BP_LEFT:
		plane.normal = Vector3::UNIT_X;
		up = Vector3::UNIT_Y;
		meshName += "Left";
		break;
	case BP_RIGHT:
		plane.normal = -Vector3::UNIT_X;
		up = Vector3::UNIT_Y;
		meshName += "Right";
		break;
	case BP_UP:
		plane.normal = -Vector3::UNIT_Y;
		up = Vector3::UNIT_Z;
		meshName += "Up";
		break;
	case BP_DOWN:

		return MeshPtr();
	}

	plane.normal = orientation * plane.normal;
	up = orientation * up;

	MeshManager& mm = MeshManager::getSingleton();
	MeshPtr planeMesh = mm.getByName(meshName);
	if (!planeMesh.isNull()) {

		mm.remove(planeMesh->getHandle());
	}

	Real planeSize = distance * 2;
	planeMesh = mm.createCurvedIllusionPlane(
		meshName,
		groupName,
		plane,
		planeSize,
		planeSize,
		curvature,
		xsegments,
		ysegments,
		false,
		1,
		tiling,
		tiling,
		up,
		orientation,
		HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY,
		HardwareBuffer::HBU_STATIC_WRITE_ONLY,
		false,
		false,
		ysegments_keep
	);

	return planeMesh;
}

const Pass* RbxSceneManager::deriveShadowCasterPass(const Pass* pass)
{
	if (isShadowTechniqueTextureBased()) {
		Pass* retPass = mShadowTextureCustomCasterPass ? mShadowTextureCustomCasterPass : mShadowCasterPlainBlackPass;

		if ((pass->getSourceBlendFactor() == SBF_SOURCE_ALPHA &&
			 pass->getDestBlendFactor() == SBF_ONE_MINUS_SOURCE_ALPHA) ||
			pass->getAlphaRejectFunction() != CMPF_ALWAYS_PASS) {

			retPass->setAlphaRejectSettings(pass->getAlphaRejectFunction(), pass->getAlphaRejectValue());
			retPass->setSceneBlending(pass->getSourceBlendFactor(), pass->getDestBlendFactor());
			retPass->getParent()->getParent()->setTransparencyCastsShadows(true);

			unsigned short origPassTUCount = pass->getNumTextureUnitStates();
			for (unsigned short t = 0; t < origPassTUCount; ++t) {
				TextureUnitState* tex;
				if (retPass->getNumTextureUnitStates() <= t) {
					tex = retPass->createTextureUnitState();
				}
				else {
					tex = retPass->getTextureUnitState(t);
				}

				(*tex) = *(pass->getTextureUnitState(t));

				tex->setColourOperationEx(
					LBX_SOURCE1,
					LBS_MANUAL,
					LBS_CURRENT,
					isShadowTechniqueAdditive() ? ColourValue::Black : mShadowColour
				);
			}

			while (retPass->getNumTextureUnitStates() > origPassTUCount) {
				retPass->removeTextureUnitState(origPassTUCount);
			}
		}
		else {

			retPass->setSceneBlending(SBT_REPLACE);
			retPass->setAlphaRejectFunction(CMPF_ALWAYS_PASS);
			while (retPass->getNumTextureUnitStates() > 0) {
				retPass->removeTextureUnitState(0);
			}
		}

		retPass->setCullingMode(pass->getCullingMode());
		retPass->setManualCullingMode(pass->getManualCullingMode());

		if (!pass->getShadowCasterVertexProgramName().empty()) {

			retPass->setVertexProgram(pass->getShadowCasterVertexProgramName(), false);
			const GpuProgramPtr& prg = retPass->getVertexProgram();

			if (!prg->isLoaded()) {
				prg->load();
			}

			retPass->setVertexProgramParameters(pass->getShadowCasterVertexProgramParameters());
		}
		else {
			if (retPass == mShadowTextureCustomCasterPass) {

				if (mShadowTextureCustomCasterPass->getVertexProgramName() != mShadowTextureCustomCasterVertexProgram) {
					mShadowTextureCustomCasterPass->setVertexProgram(mShadowTextureCustomCasterVertexProgram, false);
					if (mShadowTextureCustomCasterPass->hasVertexProgram()) {
						mShadowTextureCustomCasterPass->setVertexProgramParameters(mShadowTextureCustomCasterVPParams);
					}
				}
			}
			else {

				retPass->setVertexProgram(StringUtil::BLANK);
			}
		}
		return retPass;
	}
	else {
		return pass;
	}
}

const Pass* RbxSceneManager::deriveShadowReceiverPass(const Pass* pass)
{

	if (isShadowTechniqueTextureBased()) {
		Pass* retPass = mShadowTextureCustomReceiverPass ? mShadowTextureCustomReceiverPass : mShadowReceiverPass;

		if (!pass->getShadowReceiverVertexProgramName().empty()) {

			retPass->setVertexProgram(pass->getShadowReceiverVertexProgramName(), false);
			const GpuProgramPtr& prg = retPass->getVertexProgram();

			if (!prg->isLoaded()) {
				prg->load();
			}

			retPass->setVertexProgramParameters(pass->getShadowReceiverVertexProgramParameters());
		}
		else {
			if (retPass == mShadowTextureCustomReceiverPass) {

				if (mShadowTextureCustomReceiverPass->getVertexProgramName() !=
					mShadowTextureCustomReceiverVertexProgram) {
					mShadowTextureCustomReceiverPass->setVertexProgram(
						mShadowTextureCustomReceiverVertexProgram, false
					);
					if (mShadowTextureCustomReceiverPass->hasVertexProgram()) {
						mShadowTextureCustomReceiverPass->setVertexProgramParameters(
							mShadowTextureCustomReceiverVPParams
						);
					}
				}
			}
			else {

				retPass->setVertexProgram(StringUtil::BLANK);
			}
		}

		unsigned short keepTUCount;

		if (isShadowTechniqueAdditive()) {
			retPass->setLightingEnabled(true);
			retPass->setAmbient(pass->getAmbient());
			retPass->setSelfIllumination(pass->getSelfIllumination());
			retPass->setDiffuse(pass->getDiffuse());
			retPass->setSpecular(pass->getSpecular());
			retPass->setShininess(pass->getShininess());
			retPass->setIteratePerLight(
				pass->getIteratePerLight(), pass->getRunOnlyForOneLightType(), pass->getOnlyLightType()
			);

			retPass->setAlphaRejectSettings(pass->getAlphaRejectFunction(), pass->getAlphaRejectValue());

			unsigned short origPassTUCount = pass->getNumTextureUnitStates();
			for (unsigned short t = 0; t < origPassTUCount; ++t) {
				unsigned short targetIndex = t + 1;
				TextureUnitState* tex;
				if (retPass->getNumTextureUnitStates() <= targetIndex) {
					tex = retPass->createTextureUnitState();
				}
				else {
					tex = retPass->getTextureUnitState(targetIndex);
				}
				(*tex) = *(pass->getTextureUnitState(t));

				if (retPass->hasVertexProgram()) {
					tex->setTextureCoordSet(targetIndex);
				}
			}
			keepTUCount = origPassTUCount + 1;
		}
		else {

			keepTUCount = retPass->getNumTextureUnitStates();
		}

		if (!pass->getShadowReceiverFragmentProgramName().empty()) {

			retPass->setFragmentProgram(pass->getShadowReceiverFragmentProgramName(), false);
			const GpuProgramPtr& prg = retPass->getFragmentProgram();

			if (!prg->isLoaded()) {
				prg->load();
			}

			retPass->setFragmentProgramParameters(pass->getShadowReceiverFragmentProgramParameters());

			if (pass->hasVertexProgram() && !retPass->hasVertexProgram()) {

				retPass->setVertexProgram(pass->getVertexProgramName(), false);
				const GpuProgramPtr& prg = retPass->getVertexProgram();

				if (!prg->isLoaded()) {
					prg->load();
				}

				retPass->setVertexProgramParameters(pass->getVertexProgramParameters());
			}
		}
		else {

			if (retPass == mShadowTextureCustomReceiverPass) {

				if (mShadowTextureCustomReceiverPass->getFragmentProgramName() !=
					mShadowTextureCustomReceiverFragmentProgram) {
					mShadowTextureCustomReceiverPass->setFragmentProgram(
						mShadowTextureCustomReceiverFragmentProgram, false
					);
					if (mShadowTextureCustomReceiverPass->hasFragmentProgram()) {
						mShadowTextureCustomReceiverPass->setFragmentProgramParameters(
							mShadowTextureCustomReceiverFPParams
						);
					}
				}
			}
			else {

				retPass->setFragmentProgram(StringUtil::BLANK);
			}
		}

		while (retPass->getNumTextureUnitStates() > keepTUCount) {
			retPass->removeTextureUnitState(keepTUCount);
		}

		retPass->_load();

		return retPass;
	}
	else {
		return pass;
	}
}

SceneManager* RbxSceneManagerFactory::createInstance(const String& instanceName)
{
	return new RbxSceneManager(instanceName);
}

void RbxSceneManager::setSkyDome(
	bool enable,
	const String& materialName,
	Real curvature,
	Real tiling,
	Real distance,
	bool drawFirst,
	const Quaternion& orientation,
	int xsegments,
	int ysegments,
	int ySegmentsToKeep,
	const String& groupName
)
{
	if (enable) {
		MaterialPtr m = MaterialManager::getSingleton().getByName(materialName);
		if (m.isNull()) {
			OGRE_EXCEPT(
				Exception::ERR_INVALIDPARAMS,
				"Sky dome material '" + materialName + "' not found.",
				"RbxSceneManager::setSkyDome"
			);
		}

		m->setDepthWriteEnabled(false);

		m->load();

		mSkyDomeDrawFirst = drawFirst;

		if (!mSkyDomeNode) {
			mSkyDomeNode = createSceneNode("SkyDomeNode");
		}
		else {
			mSkyDomeNode->detachAllObjects();
		}

		for (int i = 0; i < 5; ++i) {
			MeshPtr planeMesh = createSkydomePlane(
				(BoxPlane) i,
				curvature,
				tiling,
				distance,
				orientation,
				xsegments,
				ysegments,
				i != BP_UP ? ySegmentsToKeep : -1,
				groupName
			);

			String entName = "SkyDomePlane" + StringConverter::toString(i);

			if (mSkyDomeEntity[i]) {

				destroyEntity(entName);
			}
			mSkyDomeEntity[i] = createEntity(entName, planeMesh->getName());
			mSkyDomeEntity[i]->setMaterialName(m->getName());
			mSkyDomeEntity[i]->setCastShadows(false);

			mSkyDomeNode->attachObject(mSkyDomeEntity[i]);
		}
	}
	mSkyDomeEnabled = enable;
	mSkyDomeGenParameters.skyDomeCurvature = curvature;
	mSkyDomeGenParameters.skyDomeDistance = distance;
	mSkyDomeGenParameters.skyDomeTiling = tiling;
	mSkyDomeGenParameters.skyDomeXSegments = xsegments;
	mSkyDomeGenParameters.skyDomeYSegments = ysegments;
	mSkyDomeGenParameters.skyDomeYSegments_keep = ySegmentsToKeep;
}

bool RbxSceneManager::fireRenderQueueStarted(uint8 id, const String& invocation)
{
	RenderQueueListenerList::iterator i, iend;
	bool skip = false;

	iend = mRenderQueueListeners.end();
	for (i = mRenderQueueListeners.begin(); i != iend; ++i) {
		(*i)->renderQueueStarted(id, invocation, skip);
	}
	return skip;
}

bool RbxSceneManager::fireRenderQueueEnded(uint8 id, const String& invocation)
{
	RenderQueueListenerList::iterator i, iend;
	bool repeat = false;

	iend = mRenderQueueListeners.end();
	for (i = mRenderQueueListeners.begin(); i != iend; ++i) {
		(*i)->renderQueueEnded(id, invocation, repeat);
	}
	return repeat;
}

void RbxSceneManager::fireShadowTexturesUpdated(size_t numberOfShadowTextures)
{
	ShadowListenerList::iterator i, iend;

	iend = mShadowListeners.end();
	for (i = mShadowListeners.begin(); i != iend; ++i) {
		(*i)->shadowTexturesUpdated(numberOfShadowTextures);
	}
}

void RbxSceneManager::fireShadowTexturesPreCaster(Light* light, Camera* camera)
{
	ShadowListenerList::iterator i, iend;

	iend = mShadowListeners.end();
	for (i = mShadowListeners.begin(); i != iend; ++i) {
		(*i)->shadowTextureCasterPreViewProj(light, camera);
	}
}

void RbxSceneManager::fireShadowTexturesPreReceiver(Light* light, Frustum* f)
{
	ShadowListenerList::iterator i, iend;

	iend = mShadowListeners.end();
	for (i = mShadowListeners.begin(); i != iend; ++i) {
		(*i)->shadowTextureReceiverPreViewProj(light, f);
	}
}

void RbxSceneManager::initShadowVolumeMaterials(void)
{

	assert(mDestRenderSystem);

	if (mShadowMaterialInitDone) {
		return;
	}

	if (!mShadowDebugPass) {
		MaterialPtr matDebug = MaterialManager::getSingleton().getByName("Ogre/Debug/ShadowVolumes");
		if (matDebug.isNull()) {

			matDebug = MaterialManager::getSingleton().create(
				"Ogre/Debug/ShadowVolumes", ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME
			);
			mShadowDebugPass = matDebug->getTechnique(0)->getPass(0);
			mShadowDebugPass->setSceneBlending(SBT_ADD);
			mShadowDebugPass->setLightingEnabled(false);
			mShadowDebugPass->setDepthWriteEnabled(false);
			TextureUnitState* t = mShadowDebugPass->createTextureUnitState();
			t->setColourOperationEx(LBX_MODULATE, LBS_MANUAL, LBS_CURRENT, ColourValue(0.7, 0.0, 0.2));
			mShadowDebugPass->setCullingMode(CULL_NONE);

			if (mDestRenderSystem->getCapabilities()->hasCapability(RSC_VERTEX_PROGRAM)) {
				ShadowVolumeExtrudeProgram::initialise();

				mShadowDebugPass->setVertexProgram(
					ShadowVolumeExtrudeProgram::programNames[ShadowVolumeExtrudeProgram::POINT_LIGHT]
				);
				mInfiniteExtrusionParams = mShadowDebugPass->getVertexProgramParameters();
				mInfiniteExtrusionParams->setAutoConstant(0, GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);
				mInfiniteExtrusionParams->setAutoConstant(4, GpuProgramParameters::ACT_LIGHT_POSITION_OBJECT_SPACE);
			}
			matDebug->compile();
		}
		else {
			mShadowDebugPass = matDebug->getTechnique(0)->getPass(0);

			if (mDestRenderSystem->getCapabilities()->hasCapability(RSC_VERTEX_PROGRAM)) {
				mInfiniteExtrusionParams = mShadowDebugPass->getVertexProgramParameters();
			}
		}
	}

	if (!mShadowStencilPass) {

		MaterialPtr matStencil = MaterialManager::getSingleton().getByName("Ogre/StencilShadowVolumes");
		if (matStencil.isNull()) {

			matStencil = MaterialManager::getSingleton().create(
				"Ogre/StencilShadowVolumes", ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME
			);
			mShadowStencilPass = matStencil->getTechnique(0)->getPass(0);

			if (mDestRenderSystem->getCapabilities()->hasCapability(RSC_VERTEX_PROGRAM)) {

				mShadowStencilPass->setVertexProgram(
					ShadowVolumeExtrudeProgram::programNames[ShadowVolumeExtrudeProgram::POINT_LIGHT_FINITE]
				);
				mFiniteExtrusionParams = mShadowStencilPass->getVertexProgramParameters();
				mFiniteExtrusionParams->setAutoConstant(0, GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);
				mFiniteExtrusionParams->setAutoConstant(4, GpuProgramParameters::ACT_LIGHT_POSITION_OBJECT_SPACE);

				mFiniteExtrusionParams->setAutoConstant(5, GpuProgramParameters::ACT_SHADOW_EXTRUSION_DISTANCE);
			}
			matStencil->compile();
		}
		else {
			mShadowStencilPass = matStencil->getTechnique(0)->getPass(0);

			if (mDestRenderSystem->getCapabilities()->hasCapability(RSC_VERTEX_PROGRAM)) {
				mFiniteExtrusionParams = mShadowStencilPass->getVertexProgramParameters();
			}
		}
	}

	if (!mShadowModulativePass) {

		MaterialPtr matModStencil = MaterialManager::getSingleton().getByName("Ogre/StencilShadowModulationPass");
		if (matModStencil.isNull()) {

			matModStencil = MaterialManager::getSingleton().create(
				"Ogre/StencilShadowModulationPass", ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME
			);
			mShadowModulativePass = matModStencil->getTechnique(0)->getPass(0);
			mShadowModulativePass->setSceneBlending(SBF_DEST_COLOUR, SBF_ZERO);
			mShadowModulativePass->setLightingEnabled(false);
			mShadowModulativePass->setDepthWriteEnabled(false);
			mShadowModulativePass->setDepthCheckEnabled(false);
			TextureUnitState* t = mShadowModulativePass->createTextureUnitState();
			t->setColourOperationEx(LBX_MODULATE, LBS_MANUAL, LBS_CURRENT, mShadowColour);
			mShadowModulativePass->setCullingMode(CULL_NONE);
		}
		else {
			mShadowModulativePass = matModStencil->getTechnique(0)->getPass(0);
		}
	}

	if (!mFullScreenQuad) {
		mFullScreenQuad = new Rectangle2D();
		mFullScreenQuad->setCorners(-1, 1, 1, -1);
	}

	if (!mShadowCasterPlainBlackPass) {
		MaterialPtr matPlainBlack = MaterialManager::getSingleton().getByName("Ogre/TextureShadowCaster");
		if (matPlainBlack.isNull()) {
			matPlainBlack = MaterialManager::getSingleton().create(
				"Ogre/TextureShadowCaster", ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME
			);
			mShadowCasterPlainBlackPass = matPlainBlack->getTechnique(0)->getPass(0);

			mShadowCasterPlainBlackPass->setAmbient(ColourValue::White);
			mShadowCasterPlainBlackPass->setDiffuse(ColourValue::Black);
			mShadowCasterPlainBlackPass->setSelfIllumination(ColourValue::Black);
			mShadowCasterPlainBlackPass->setSpecular(ColourValue::Black);

			mShadowCasterPlainBlackPass->setFog(true, FOG_NONE);
		}
		else {
			mShadowCasterPlainBlackPass = matPlainBlack->getTechnique(0)->getPass(0);
		}
	}

	if (!mShadowReceiverPass) {
		MaterialPtr matShadRec = MaterialManager::getSingleton().getByName("Ogre/TextureShadowReceiver");
		if (matShadRec.isNull()) {
			matShadRec = MaterialManager::getSingleton().create(
				"Ogre/TextureShadowReceiver", ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME
			);
			mShadowReceiverPass = matShadRec->getTechnique(0)->getPass(0);

			TextureUnitState* t = mShadowReceiverPass->createTextureUnitState();
			t->setTextureAddressingMode(TextureUnitState::TAM_CLAMP);
		}
		else {
			mShadowReceiverPass = matShadRec->getTechnique(0)->getPass(0);
		}
	}

	TexturePtr spotShadowFadeTex = TextureManager::getSingleton().getByName("spot_shadow_fade.png");
	if (spotShadowFadeTex.isNull()) {

		DataStreamPtr stream(new MemoryDataStream(SPOT_SHADOW_FADE_PNG, SPOT_SHADOW_FADE_PNG_SIZE, false));
		Image img;
		img.load(stream, "png");
		spotShadowFadeTex = TextureManager::getSingleton().loadImage(
			"spot_shadow_fade.png", ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME, img, TEX_TYPE_2D
		);
	}

	mShadowMaterialInitDone = true;
}

void RbxSceneManager::setShadowTextureSize(unsigned short size)
{

	for (ShadowTextureConfigList::iterator i = mShadowTextureConfigList.begin(); i != mShadowTextureConfigList.end();
		 ++i) {
		if (i->width != size || i->height != size) {
			i->width = i->height = size;
			mShadowTextureConfigDirty = true;
		}
	}
}

void RbxSceneManager::setShadowTexturePixelFormat(PixelFormat fmt)
{
	for (ShadowTextureConfigList::iterator i = mShadowTextureConfigList.begin(); i != mShadowTextureConfigList.end();
		 ++i) {
		if (i->format != fmt) {
			i->format = fmt;
			mShadowTextureConfigDirty = true;
		}
	}
}

void RbxSceneManager::setShadowTextureSettings(unsigned short size, unsigned short count, PixelFormat fmt)
{
	setShadowTextureCount(count);
	for (ShadowTextureConfigList::iterator i = mShadowTextureConfigList.begin(); i != mShadowTextureConfigList.end();
		 ++i) {
		if (i->width != size || i->height != size || i->format != fmt) {
			i->width = i->height = size;
			i->format = fmt;
			mShadowTextureConfigDirty = true;
		}
	}
}

void RbxSceneManager::setShadowTextureCasterMaterial(const String& name)
{
	if (name.empty()) {
		mShadowTextureCustomCasterPass = 0;
	}
	else {
		MaterialPtr mat = MaterialManager::getSingleton().getByName(name);
		if (mat.isNull()) {
			OGRE_EXCEPT(
				Exception::ERR_ITEM_NOT_FOUND,
				"Cannot locate material called '" + name + "'",
				"RbxSceneManager::setShadowTextureCasterMaterial"
			);
		}
		mat->load();
		mShadowTextureCustomCasterPass = mat->getBestTechnique()->getPass(0);
		if (mShadowTextureCustomCasterPass->hasVertexProgram()) {

			mShadowTextureCustomCasterVertexProgram = mShadowTextureCustomCasterPass->getVertexProgramName();
			mShadowTextureCustomCasterVPParams = mShadowTextureCustomCasterPass->getVertexProgramParameters();
		}
	}
}

void RbxSceneManager::setShadowTextureReceiverMaterial(const String& name)
{
	if (name.empty()) {
		mShadowTextureCustomReceiverPass = 0;
	}
	else {
		MaterialPtr mat = MaterialManager::getSingleton().getByName(name);
		if (mat.isNull()) {
			OGRE_EXCEPT(
				Exception::ERR_ITEM_NOT_FOUND,
				"Cannot locate material called '" + name + "'",
				"RbxSceneManager::setShadowTextureReceiverMaterial"
			);
		}
		mat->load();
		mShadowTextureCustomReceiverPass = mat->getBestTechnique()->getPass(0);
		if (mShadowTextureCustomReceiverPass->hasVertexProgram()) {

			mShadowTextureCustomReceiverVertexProgram = mShadowTextureCustomReceiverPass->getVertexProgramName();
			mShadowTextureCustomReceiverVPParams = mShadowTextureCustomReceiverPass->getVertexProgramParameters();
		}
		else {
			mShadowTextureCustomReceiverVertexProgram = StringUtil::BLANK;
		}
		if (mShadowTextureCustomReceiverPass->hasFragmentProgram()) {

			mShadowTextureCustomReceiverFragmentProgram = mShadowTextureCustomReceiverPass->getFragmentProgramName();
			mShadowTextureCustomReceiverFPParams = mShadowTextureCustomReceiverPass->getFragmentProgramParameters();
		}
		else {
			mShadowTextureCustomReceiverFragmentProgram = StringUtil::BLANK;
		}
	}
}

void RbxSceneManager::setShadowCameraSetup(const ShadowCameraSetupPtr& shadowSetup)
{
	mDefaultShadowCameraSetup = shadowSetup;
}

void RbxSceneManager::clearSpecialCaseRenderQueues(void)
{
	mSpecialCaseQueueList.clear();
}

Camera* RbxSceneManager::getCamera(const String& name) const
{
	CameraList::const_iterator i = mCameras.find(name);
	if (i == mCameras.end()) {
		OGRE_EXCEPT(
			Exception::ERR_ITEM_NOT_FOUND, "Cannot find Camera with name " + name, "RbxSceneManager::getCamera"
		);
	}
	else {
		return i->second;
	}
}

bool RbxSceneManager::hasCamera(const String& name) const
{
	return (mCameras.find(name) != mCameras.end());
}

SceneNode* RbxSceneManager::getSceneNode(const String& name) const
{
	SceneNodeList::const_iterator i = mSceneNodes.find(name);

	if (i == mSceneNodes.end()) {
		OGRE_EXCEPT(
			Exception::ERR_ITEM_NOT_FOUND, "SceneNode '" + name + "' not found.", "RbxSceneManager::getSceneNode"
		);
	}

	return i->second;
}

bool RbxSceneManager::hasSceneNode(const String& name) const
{
	return (mSceneNodes.find(name) != mSceneNodes.end());
}

void RbxSceneManager::setSkyBox(
	bool enable,
	const String& materialName,
	Real distance,
	bool drawFirst,
	const Quaternion& orientation,
	const String& groupName
)
{
	if (enable) {
		if (m_unk0x8a84.isNull()) {
			m_unk0x8a84.bind(new RbxSky(this, m_unk0x8a68, true));
		}

		MaterialPtr m = MaterialManager::getSingleton().getByName(materialName);
		if (m.isNull()) {
			OGRE_EXCEPT(
				Exception::ERR_INVALIDPARAMS,
				"Sky box material '" + materialName + "' not found.",
				"RbxSceneManager::setSkyBox"
			);
		}

		m->setDepthWriteEnabled(false);
		m->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setNumMipmaps(0);
		m->load();

		m->getBestTechnique()->getPass(0)->getTextureUnitState(0)->setTextureAddressingMode(
			TextureUnitState::TAM_CLAMP
		);

		mSkyBoxDrawFirst = drawFirst;

		if (!mSkyBoxNode) {
			mSkyBoxNode = createSceneNode("SkyBoxNode");
			m_unk0x8a90 = createSceneNode("SkyBoxTBNode");
			m_unk0x8a90->setOrientation(Quaternion(Radian(Degree(-90.0)), Vector3(0.0f, 1.0f, 0.0f)));
		}
		else {
			mSkyBoxNode->detachAllObjects();
			m_unk0x8a90->detachAllObjects();
		}

		MaterialManager& matMgr = MaterialManager::getSingleton();

		for (int i = 0; i < 6; ++i) {
			MeshPtr planeMesh = createSkyboxPlane((BoxPlane) i, distance, orientation, groupName);
			String entName = mName + "SkyBoxPlane" + StringConverter::toString(i);

			if (mSkyBoxEntity[i]) {
				destroyEntity(entName);
			}
			mSkyBoxEntity[i] = createEntity(entName, planeMesh->getName());
			mSkyBoxEntity[i]->setCastShadows(false);

			MaterialPtr boxMat = matMgr.getByName(entName);
			if (boxMat.isNull()) {
				boxMat = m->clone(entName);
			}
			else {
				m->copyDetailsTo(boxMat);
			}

			boxMat->setDepthWriteEnabled(false);
			boxMat->setSceneBlending(SBT_MODULATE);
			boxMat->load();

			Material::TechniqueIterator ti = boxMat->getSupportedTechniqueIterator();
			while (ti.hasMoreElements()) {
				Technique* tech = ti.getNext();
				tech->getPass(0)->getTextureUnitState(0)->setCurrentFrame(i);
			}

			mSkyBoxEntity[i]->setMaterialName(boxMat->getName());

			if (i < 4) {
				mSkyBoxNode->attachObject(mSkyBoxEntity[i]);
			}
			else {
				m_unk0x8a90->attachObject(mSkyBoxEntity[i]);
			}
		}
	}
	mSkyBoxEnabled = enable;
	mSkyBoxGenParameters.skyBoxDistance = distance;
}

void RbxSceneManager::renderVisibleObjectsCustomSequence(RenderQueueInvocationSequence* seq)
{
	RenderQueueInvocationIterator invocationIt = seq->iterator();
	while (invocationIt.hasMoreElements()) {
		RenderQueueInvocation* invocation = invocationIt.getNext();
		uint8 qId = invocation->getRenderQueueGroupID();

		if (!isRenderQueueToBeProcessed(qId)) {
			continue;
		}

		bool repeatQueue = false;
		const String& invocationName = invocation->getInvocationName();
		RenderQueueGroup* queueGroup = getRenderQueue()->getQueueGroup(qId);
		do {

			if (fireRenderQueueStarted(qId, invocationName)) {

				break;
			}

			invocation->invoke(queueGroup, this);

			if (fireRenderQueueEnded(qId, invocationName)) {

				repeatQueue = true;
			}
			else {
				repeatQueue = false;
			}
		} while (repeatQueue);
	}
}

Animation* RbxSceneManager::getAnimation(const String& name) const
{
	OGRE_LOCK_MUTEX(mAnimationsListMutex)

	AnimationList::const_iterator i = mAnimationsList.find(name);
	if (i == mAnimationsList.end()) {
		OGRE_EXCEPT(
			Exception::ERR_ITEM_NOT_FOUND, "Cannot find animation with name " + name, "RbxSceneManager::getAnimation"
		);
	}
	return i->second;
}

bool RbxSceneManager::hasAnimation(const String& name) const
{
	OGRE_LOCK_MUTEX(mAnimationsListMutex)
	return (mAnimationsList.find(name) != mAnimationsList.end());
}

void RbxSceneManager::removeRenderQueueListener(RenderQueueListener* delListener)
{
	RenderQueueListenerList::iterator i, iend;
	iend = mRenderQueueListeners.end();
	for (i = mRenderQueueListeners.begin(); i != iend; ++i) {
		if (*i == delListener) {
			mRenderQueueListeners.erase(i);
			break;
		}
	}
}

void RbxSceneManager::removeShadowListener(ShadowListener* delListener)
{
	ShadowListenerList::iterator i, iend;
	iend = mShadowListeners.end();
	for (i = mShadowListeners.begin(); i != iend; ++i) {
		if (*i == delListener) {
			mShadowListeners.erase(i);
			break;
		}
	}
}

void RbxSceneManager::renderShadowVolumeObjects(
	ShadowCaster::ShadowRenderableListIterator iShadowRenderables,
	Pass* pass,
	const LightList* manualLightList,
	unsigned long flags,
	bool secondpass,
	bool zfail,
	bool twosided
)
{

	while (iShadowRenderables.hasMoreElements()) {
		ShadowRenderable* sr = iShadowRenderables.getNext();

		renderSingleObject(sr, pass, false, manualLightList);

		if (sr->isLightCapSeparate() && (flags & SRF_INCLUDE_LIGHT_CAP)) {
			ShadowRenderable* lightCap = sr->getLightCapRenderable();
			assert(lightCap && "Shadow renderable is missing a separate light cap renderable!");

			if (twosided) {

				mDestRenderSystem->_setCullingMode(CULL_ANTICLOCKWISE);

				renderSingleObject(lightCap, pass, false, manualLightList);

				mDestRenderSystem->_setCullingMode(CULL_CLOCKWISE);

				mDestRenderSystem->_setDepthBufferFunction(CMPF_ALWAYS_FAIL);
				renderSingleObject(lightCap, pass, false, manualLightList);

				mDestRenderSystem->_setDepthBufferFunction(CMPF_LESS);

				mDestRenderSystem->_setCullingMode(CULL_NONE);
			}
			else if ((secondpass || zfail) && !(secondpass && zfail)) {

				renderSingleObject(lightCap, pass, false, manualLightList);
			}
			else {

				mDestRenderSystem->_setDepthBufferFunction(CMPF_ALWAYS_FAIL);
				renderSingleObject(lightCap, pass, false, manualLightList);

				mDestRenderSystem->_setDepthBufferFunction(CMPF_LESS);
			}
		}
	}
}

void RbxSceneManager::prepareShadowTextures(Camera* cam, Viewport* vp)
{

	ensureShadowTexturesCreated();

	IlluminationRenderStage savedStage = mIlluminationStage;
	mIlluminationStage = IRS_RENDER_TO_TEXTURE;

	Real shadowDist = mShadowFarDist;
	if (!shadowDist) {

		shadowDist = cam->getNearClipDistance() * 300;
	}
	Real shadowOffset = shadowDist * mShadowTextureOffset;

	Real shadowEnd = shadowDist + shadowOffset;
	Real fadeStart = shadowEnd * mShadowTextureFadeStart;
	Real fadeEnd = shadowEnd * mShadowTextureFadeEnd;

	if (!isShadowTechniqueAdditive()) {

		mShadowReceiverPass->setFog(true, FOG_LINEAR, ColourValue::White, 0, fadeStart, fadeEnd);
	}
	else {

		mShadowReceiverPass->setFog(true, FOG_NONE);
	}

	LightList::iterator i, iend;
	ShadowTextureList::iterator si, siend;
	ShadowTextureCameraList::iterator ci;
	iend = mLightsAffectingFrustum.end();
	siend = mShadowTextures.end();
	ci = mShadowTextureCameras.begin();
	for (i = mLightsAffectingFrustum.begin(), si = mShadowTextures.begin(); i != iend && si != siend; ++i) {
		Light* light = *i;

		if (!light->getCastShadows()) {
			continue;
		}

		TexturePtr& shadowTex = *si;
		RenderTarget* shadowRTT = shadowTex->getBuffer()->getRenderTarget();
		Viewport* shadowView = shadowRTT->getViewport(0);
		Camera* texCam = *ci;

		shadowView->setCamera(texCam);

		ShadowCamLightMapping::iterator camLightIt = mShadowCamLightMapping.find(texCam);
		assert(camLightIt != mShadowCamLightMapping.end());
		camLightIt->second = light;

		if (light->getCustomShadowCameraSetup().isNull()) {
			mDefaultShadowCameraSetup->getShadowCamera(this, cam, vp, light, texCam);
		}
		else {
			light->getCustomShadowCameraSetup()->getShadowCamera(this, cam, vp, light, texCam);
		}

		shadowView->setBackgroundColour(ColourValue::White);

		fireShadowTexturesPreCaster(light, texCam);

		shadowRTT->update();

		++si;
		++ci;
	}

	mIlluminationStage = savedStage;

	fireShadowTexturesUpdated(std::min(mLightsAffectingFrustum.size(), mShadowTextures.size()));

	ShadowTextureManager::getSingleton().clearUnused();
}

StaticGeometry* RbxSceneManager::getStaticGeometry(const String& name) const
{
	StaticGeometryList::const_iterator i = mStaticGeometryList.find(name);
	if (i == mStaticGeometryList.end()) {
		OGRE_EXCEPT(
			Exception::ERR_ITEM_NOT_FOUND,
			"StaticGeometry with name '" + name + "' not found",
			"RbxSceneManager::createStaticGeometry"
		);
	}
	return i->second;
}

bool RbxSceneManager::hasStaticGeometry(const String& name) const
{
	return (mStaticGeometryList.find(name) != mStaticGeometryList.end());
}

InstancedGeometry* RbxSceneManager::getInstancedGeometry(const String& name) const
{
	InstancedGeometryList::const_iterator i = mInstancedGeometryList.find(name);
	if (i == mInstancedGeometryList.end()) {
		OGRE_EXCEPT(
			Exception::ERR_ITEM_NOT_FOUND,
			"InstancedGeometry with name '" + name + "' not found",
			"RbxSceneManager::createInstancedGeometry"
		);
	}
	return i->second;
}

const SceneManager::MovableObjectCollection* RbxSceneManager::getMovableObjectCollection(const String& typeName) const
{
	MovableObjectCollectionMap::const_iterator i = mMovableObjectCollectionMap.find(typeName);
	if (i == mMovableObjectCollectionMap.end()) {
		OGRE_EXCEPT(
			Exception::ERR_ITEM_NOT_FOUND,
			"Object collection named '" + typeName + "' does not exist.",
			"RbxSceneManager::getMovableObjectCollection"
		);
	}
	else {
		return i->second;
	}
}

MovableObject* RbxSceneManager::getMovableObject(const String& name, const String& typeName) const
{
	const MovableObjectCollection* objectMap = getMovableObjectCollection(typeName);

	{
		OGRE_LOCK_MUTEX(objectMap->mutex)
		MovableObjectMap::const_iterator mi = objectMap->map.find(name);
		if (mi == objectMap->map.end()) {
			OGRE_EXCEPT(
				Exception::ERR_ITEM_NOT_FOUND,
				"Object named '" + name + "' does not exist.",
				"RbxSceneManager::getMovableObject"
			);
		}
		return mi->second;
	}
}

void RbxSceneManager::addSpecialCaseRenderQueue(uint8 qid)
{
	mSpecialCaseQueueList.insert(qid);
}

bool RbxSceneManager::isRenderQueueToBeProcessed(uint8 qid)
{
	bool inList = mSpecialCaseQueueList.find(qid) != mSpecialCaseQueueList.end();
	return (inList && mSpecialCaseQueueMode == SCRQM_INCLUDE) || (!inList && mSpecialCaseQueueMode == SCRQM_EXCLUDE);
}

void RbxSceneManager::destroyCamera(Camera* cam)
{

	CamVisibleObjectsMap::iterator camVisObjIt = mCamVisibleObjectsMap.find(cam);
	if (camVisObjIt != mCamVisibleObjectsMap.end()) {
		mCamVisibleObjectsMap.erase(camVisObjIt);
	}

	ShadowCamLightMapping::iterator camLightIt = mShadowCamLightMapping.find(cam);
	if (camLightIt != mShadowCamLightMapping.end()) {
		mShadowCamLightMapping.erase(camLightIt);
	}

	CameraList::iterator i = mCameras.begin();
	for (; i != mCameras.end(); ++i) {
		if (i->second == cam) {
			mCameras.erase(i);

			mDestRenderSystem->_notifyCameraRemoved(cam);
			delete cam;
			break;
		}
	}
}

void RbxSceneManager::destroyCamera(const String& name)
{

	CameraList::iterator i = mCameras.find(name);
	if (i != mCameras.end()) {

		mDestRenderSystem->_notifyCameraRemoved(i->second);
		delete i->second;
		mCameras.erase(i);
	}
}

void RbxSceneManager::destroySceneNode(const String& name)
{
	SceneNodeList::iterator i = mSceneNodes.find(name);

	if (i == mSceneNodes.end()) {
		OGRE_EXCEPT(
			Exception::ERR_ITEM_NOT_FOUND, "SceneNode '" + name + "' not found.", "RbxSceneManager::destroySceneNode"
		);
	}

	AutoTrackingSceneNodes::iterator ai, aiend;
	aiend = mAutoTrackingSceneNodes.end();
	for (ai = mAutoTrackingSceneNodes.begin(); ai != aiend;) {

		AutoTrackingSceneNodes::iterator curri = ai++;
		SceneNode* n = *curri;

		if (n->getAutoTrackTarget() == i->second) {

			n->setAutoTracking(false);
		}

		else if (n == i->second) {
			mAutoTrackingSceneNodes.erase(curri);
		}
	}

	Node* parentNode = i->second->getParent();
	if (parentNode) {
		parentNode->removeChild(i->second);
	}
	delete i->second;
	mSceneNodes.erase(i);
}

const Pass* RbxSceneManager::_setPass(const Pass* pass, bool evenIfSuppressed, bool shadowDerivation)
{
	if (!mSuppressRenderStateChanges || evenIfSuppressed) {
		m_unk0x8a64 = pass->getLightingEnabled() ? true : false;

		if (mIlluminationStage == IRS_RENDER_TO_TEXTURE && shadowDerivation) {

			pass = deriveShadowCasterPass(pass);
		}
		else if (mIlluminationStage == IRS_RENDER_RECEIVER_PASS && shadowDerivation) {
			pass = deriveShadowReceiverPass(pass);
		}

		mAutoParamDataSource.setCurrentPass(pass);

		bool passSurfaceAndLightParams = true;

		if (pass->hasVertexProgram()) {
			mDestRenderSystem->bindGpuProgram(pass->getVertexProgram()->_getBindingDelegate());

			passSurfaceAndLightParams = pass->getVertexProgram()->getPassSurfaceAndLightStates();
		}
		else {

			if (mDestRenderSystem->isGpuProgramBound(GPT_VERTEX_PROGRAM)) {
				mDestRenderSystem->unbindGpuProgram(GPT_VERTEX_PROGRAM);
			}
		}

		if (passSurfaceAndLightParams) {

			if (pass->getLightingEnabled()) {
				mDestRenderSystem->_setSurfaceParams(
					pass->getAmbient(),
					pass->getDiffuse(),
					pass->getSpecular(),
					pass->getSelfIllumination(),
					pass->getShininess(),
					pass->getVertexColourTracking()
				);
			}

			mDestRenderSystem->setLightingEnabled(pass->getLightingEnabled());
		}

		if (pass->hasFragmentProgram()) {
			mDestRenderSystem->bindGpuProgram(pass->getFragmentProgram()->_getBindingDelegate());
		}
		else {

			if (mDestRenderSystem->isGpuProgramBound(GPT_FRAGMENT_PROGRAM)) {
				mDestRenderSystem->unbindGpuProgram(GPT_FRAGMENT_PROGRAM);
			}
		}

		FogMode newFogMode;
		ColourValue newFogColour;
		Real newFogStart, newFogEnd, newFogDensity;
		if (pass->getFogOverride()) {

			newFogMode = pass->getFogMode();
			newFogColour = pass->getFogColour();
			newFogStart = pass->getFogStart();
			newFogEnd = pass->getFogEnd();
			newFogDensity = pass->getFogDensity();
		}
		else {

			newFogMode = mFogMode;
			newFogColour = mFogColour;
			newFogStart = mFogStart;
			newFogEnd = mFogEnd;
			newFogDensity = mFogDensity;
		}
		mDestRenderSystem->_setFog(newFogMode, newFogColour, newFogDensity, newFogStart, newFogEnd);

		mAutoParamDataSource.setFog(mFogMode, mFogColour, mFogDensity, mFogStart, mFogEnd);

		mDestRenderSystem->_setSceneBlending(pass->getSourceBlendFactor(), pass->getDestBlendFactor());

		mDestRenderSystem->_setPointParameters(
			pass->getPointSize(),
			pass->isPointAttenuationEnabled(),
			pass->getPointAttenuationConstant(),
			pass->getPointAttenuationLinear(),
			pass->getPointAttenuationQuadratic(),
			pass->getPointMinSize(),
			pass->getPointMaxSize()
		);

		mDestRenderSystem->_setPointSpritesEnabled(pass->getPointSpritesEnabled());

		Pass::ConstTextureUnitStateIterator texIter = pass->getTextureUnitStateIterator();
		size_t unit = 0;

		size_t shadowTexIndex = pass->getStartLight();
		while (texIter.hasMoreElements()) {
			TextureUnitState* pTex = texIter.getNext();
			if (!pass->getIteratePerLight() && isShadowTechniqueTextureBased() &&
				pTex->getContentType() == TextureUnitState::CONTENT_SHADOW) {

				TexturePtr shadowTex;
				if (shadowTexIndex < mShadowTextures.size()) {
					shadowTex = getShadowTexture(shadowTexIndex);

					Camera* cam = shadowTex->getBuffer()->getRenderTarget()->getViewport(0)->getCamera();

					pTex->setProjectiveTexturing(!pass->hasVertexProgram(), cam);
					mAutoParamDataSource.setTextureProjector(cam, shadowTexIndex);
				}
				else {

					shadowTex = mNullShadowTexture;
					pTex->setProjectiveTexturing(false);
					mAutoParamDataSource.setTextureProjector(0, shadowTexIndex);
				}
				pTex->_setTexturePtr(shadowTex);

				++shadowTexIndex;
			}
			else if (mIlluminationStage == IRS_NONE && pass->hasVertexProgram()) {

				TextureUnitState::EffectMap::const_iterator effi =
					pTex->getEffects().find(TextureUnitState::ET_PROJECTIVE_TEXTURE);
				if (effi != pTex->getEffects().end()) {
					mAutoParamDataSource.setTextureProjector(effi->second.frustum, unit);
				}
			}
			mDestRenderSystem->_setTextureUnitSettings(unit, *pTex);
			++unit;
		}

		mDestRenderSystem->_disableTextureUnitsFrom(pass->getNumTextureUnitStates());

		mDestRenderSystem->_setDepthBufferFunction(pass->getDepthFunction());
		mDestRenderSystem->_setDepthBufferCheckEnabled(pass->getDepthCheckEnabled());
		mDestRenderSystem->_setDepthBufferWriteEnabled(pass->getDepthWriteEnabled());
		mDestRenderSystem->_setDepthBias(pass->getDepthBiasConstant(), pass->getDepthBiasSlopeScale());

		mDestRenderSystem->_setAlphaRejectSettings(pass->getAlphaRejectFunction(), pass->getAlphaRejectValue());

		bool colWrite = pass->getColourWriteEnabled();
		mDestRenderSystem->_setColourBufferWriteEnabled(colWrite, colWrite, colWrite, colWrite);

		if (isShadowTechniqueTextureBased() && mIlluminationStage == IRS_RENDER_TO_TEXTURE &&
			mShadowCasterRenderBackFaces && pass->getCullingMode() == CULL_CLOCKWISE) {

			mDestRenderSystem->_setCullingMode(CULL_ANTICLOCKWISE);
		}
		else {
			mDestRenderSystem->_setCullingMode(pass->getCullingMode());
		}

		mDestRenderSystem->setShadingType(pass->getShadingMode());

		mDestRenderSystem->_setPolygonMode(pass->getPolygonMode());

		mAutoParamDataSource.setPassNumber(pass->getIndex());
	}

	return pass;
}

void RbxSceneManager::_renderScene(Camera* camera, Viewport* vp, bool includeOverlays)
{
	g_unk0x00961438 = false;
	m_unk0x8a64 = true;
	m_unk0x8a65 = false;
	Root::getSingleton()._setCurrentSceneManager(this);
	mActiveQueuedRenderableVisitor->targetSceneMgr = this;
	mAutoParamDataSource.setCurrentSceneManager(this);

	if (isShadowTechniqueInUse()) {

		initShadowVolumeMaterials();
	}

	if (isShadowTechniqueStencilBased() && camera->getProjectionType() == PT_PERSPECTIVE &&
		camera->getFarClipDistance() != 0 &&
		mDestRenderSystem->getCapabilities()->hasCapability(RSC_INFINITE_FAR_PLANE) && mShadowUseInfiniteFarPlane) {

		camera->setFarClipDistance(0);
	}

	mCameraInProgress = camera;

	ControllerManager::getSingleton().updateAllControllers();

	unsigned long thisFrameNumber = Root::getSingleton().getCurrentFrameNumber();
	if (thisFrameNumber != mLastFrameNumber) {

		_applySceneAnimations();
		mLastFrameNumber = thisFrameNumber;
	}

	Timer timer;
	{

		OGRE_LOCK_MUTEX(sceneGraphMutex)

		timer.reset();
		_updateSceneGraph(camera);
		timer.getMicroseconds();

		AutoTrackingSceneNodes::iterator atsni, atsniend;
		atsniend = mAutoTrackingSceneNodes.end();
		for (atsni = mAutoTrackingSceneNodes.begin(); atsni != atsniend; ++atsni) {
			(*atsni)->_autoTrack();
		}

		camera->_autoTrack();

		if (mIlluminationStage != IRS_RENDER_TO_TEXTURE && mFindVisibleObjects) {

			findLightsAffectingFrustum(camera);

			if (isShadowTechniqueInUse() && vp->getShadowsEnabled()) {

				if (isShadowTechniqueTextureBased()) {

					prepareShadowTextures(camera, vp);

					mCameraInProgress = camera;
				}
			}
		}

		if (camera->isReflected()) {
			mDestRenderSystem->setInvertVertexWinding(true);
		}
		else {
			mDestRenderSystem->setInvertVertexWinding(false);
		}

		mAutoParamDataSource.setCurrentViewport(vp);

		setViewport(vp);

		mAutoParamDataSource.setCurrentCamera(camera);

		mAutoParamDataSource.setShadowDirLightExtrusionDistance(mShadowDirLightExtrudeDist);

		mAutoParamDataSource.setAmbientLightColour(mAmbientLight);

		mDestRenderSystem->setAmbientLight(mAmbientLight.r, mAmbientLight.g, mAmbientLight.b);

		mAutoParamDataSource.setCurrentRenderTarget(vp->getTarget());

		if (mDestRenderSystem->getCapabilities()->hasCapability(RSC_USER_CLIP_PLANES)) {
			if (camera->isWindowSet()) {
				const std::vector<Plane>& planeList = camera->getWindowPlanes();
				for (ushort i = 0; i < 4; ++i) {
					mDestRenderSystem->enableClipPlane(i, true);
					mDestRenderSystem->setClipPlane(i, planeList[i]);
				}
			}
			else {
				for (ushort i = 0; i < 4; ++i) {
					mDestRenderSystem->enableClipPlane(i, false);
				}
			}
		}

		timer.reset();
		prepareRenderQueue();
		timer.getMicroseconds();

		if (mFindVisibleObjects) {

			CamVisibleObjectsMap::iterator camVisObjIt = mCamVisibleObjectsMap.find(camera);

			assert(
				camVisObjIt != mCamVisibleObjectsMap.end() &&
				"Should never fail to find a visible object bound for a camera, "
				"did you override SceneManager::createCamera or something?"
			);

			camVisObjIt->second.reset();

			_findVisibleObjects(
				camera, &(camVisObjIt->second), mIlluminationStage == IRS_RENDER_TO_TEXTURE ? true : false
			);

			mAutoParamDataSource.setMainCamBoundsInfo(&(camVisObjIt->second));
		}

		if (vp->getOverlaysEnabled() && mIlluminationStage != IRS_RENDER_TO_TEXTURE) {
			OverlayManager::getSingleton()._queueOverlaysForRendering(camera, getRenderQueue(), vp);
		}

		if (vp->getSkiesEnabled() && mFindVisibleObjects && mIlluminationStage != IRS_RENDER_TO_TEXTURE) {
			_queueSkiesForRendering(camera);
		}
	}

	mDestRenderSystem->_beginGeometryCount();

	if (mCurrentViewport->getClearEveryFrame()) {
		mDestRenderSystem->clearFrameBuffer(
			mCurrentViewport->getClearBuffers(), mCurrentViewport->getBackgroundColour()
		);
	}

	mDestRenderSystem->_beginFrame();

	mDestRenderSystem->_setPolygonMode(camera->getPolygonMode());

	mDestRenderSystem->_setProjectionMatrix(mCameraInProgress->getProjectionMatrixRS());
	mDestRenderSystem->_setViewMatrix(mCameraInProgress->getViewMatrix(true));

	timer.reset();
	_renderVisibleObjects();
	timer.getMicroseconds();

	mDestRenderSystem->_endFrame();

	camera->_notifyRenderedFaces(mDestRenderSystem->_getFaceCount());

	camera->_notifyRenderedBatches(mDestRenderSystem->_getBatchCount());
}

void RbxSceneManager::renderModulativeStencilShadowedQueueGroupObjects(
	RenderQueueGroup* pGroup, QueuedRenderableCollection::OrganisationMode om
)
{

	RenderQueueGroup::PriorityMapIterator groupIt = pGroup->getIterator();

	while (groupIt.hasMoreElements()) {
		RenderPriorityGroup* pPriorityGrp = groupIt.getNext();

		pPriorityGrp->sort(mCameraInProgress);

		renderObjects(pPriorityGrp->getSolidsBasic(), om, true);
	}

	LightList::const_iterator li, liend;
	liend = mLightsAffectingFrustum.end();

	for (li = mLightsAffectingFrustum.begin(); li != liend; ++li) {
		Light* l = *li;
		if (l->getCastShadows()) {

			mDestRenderSystem->clearFrameBuffer(FBT_STENCIL);
			renderShadowVolumesToStencil(l, mCameraInProgress);

			_setPass(mShadowModulativePass);

			mDestRenderSystem->setStencilCheckEnabled(true);

			mDestRenderSystem->setStencilBufferParams(CMPF_NOT_EQUAL, 0);
			renderSingleObject(mFullScreenQuad, mShadowModulativePass, false);

			mDestRenderSystem->setStencilBufferParams();
			mDestRenderSystem->setStencilCheckEnabled(false);
			mDestRenderSystem->_setDepthBufferParams();
		}
	}

	RenderQueueGroup::PriorityMapIterator groupIt2 = pGroup->getIterator();
	while (groupIt2.hasMoreElements()) {
		RenderPriorityGroup* pPriorityGrp = groupIt2.getNext();

		renderObjects(pPriorityGrp->getSolidsNoShadowReceive(), om, true);
	}

	RenderQueueGroup::PriorityMapIterator groupIt3 = pGroup->getIterator();
	while (groupIt3.hasMoreElements()) {
		RenderPriorityGroup* pPriorityGrp = groupIt3.getNext();

		renderObjects(pPriorityGrp->getTransparents(), QueuedRenderableCollection::OM_SORT_DESCENDING, true);
	}
}

void RbxSceneManager::renderTextureShadowCasterQueueGroupObjects(
	RenderQueueGroup* pGroup, QueuedRenderableCollection::OrganisationMode om
)
{
	static LightList nullLightList;

	RenderQueueGroup::PriorityMapIterator groupIt = pGroup->getIterator();

	if (isShadowTechniqueAdditive()) {

		mAutoParamDataSource.setAmbientLightColour(ColourValue::Black);
		mDestRenderSystem->setAmbientLight(0, 0, 0);
	}
	else {

		mAutoParamDataSource.setAmbientLightColour(mShadowColour);
		mDestRenderSystem->setAmbientLight(mShadowColour.r, mShadowColour.g, mShadowColour.b);
	}

	while (groupIt.hasMoreElements()) {
		RenderPriorityGroup* pPriorityGrp = groupIt.getNext();

		pPriorityGrp->sort(mCameraInProgress);

		renderObjects(pPriorityGrp->getSolidsBasic(), om, false, &nullLightList);
		renderObjects(pPriorityGrp->getSolidsNoShadowReceive(), om, false, &nullLightList);

		renderTransparentShadowCasterObjects(
			pPriorityGrp->getTransparents(), QueuedRenderableCollection::OM_SORT_DESCENDING, false, &nullLightList
		);
	}

	mAutoParamDataSource.setAmbientLightColour(mAmbientLight);
	mDestRenderSystem->setAmbientLight(mAmbientLight.r, mAmbientLight.g, mAmbientLight.b);
}

void RbxSceneManager::renderModulativeTextureShadowedQueueGroupObjects(
	RenderQueueGroup* pGroup, QueuedRenderableCollection::OrganisationMode om
)
{

	RenderQueueGroup::PriorityMapIterator groupIt = pGroup->getIterator();

	while (groupIt.hasMoreElements()) {
		RenderPriorityGroup* pPriorityGrp = groupIt.getNext();

		pPriorityGrp->sort(mCameraInProgress);

		renderObjects(pPriorityGrp->getSolidsBasic(), om, true);
		renderObjects(pPriorityGrp->getSolidsNoShadowReceive(), om, true);
	}

	if (mIlluminationStage == IRS_NONE) {
		mIlluminationStage = IRS_RENDER_RECEIVER_PASS;

		LightList::iterator i, iend;
		ShadowTextureList::iterator si, siend;
		iend = mLightsAffectingFrustum.end();
		siend = mShadowTextures.end();
		for (i = mLightsAffectingFrustum.begin(), si = mShadowTextures.begin(); i != iend && si != siend; ++i) {
			Light* l = *i;

			if (!l->getCastShadows()) {
				continue;
			}

			mCurrentShadowTexture = si->getPointer();

			Camera* cam = mCurrentShadowTexture->getBuffer()->getRenderTarget()->getViewport(0)->getCamera();

			Pass* targetPass =
				mShadowTextureCustomReceiverPass ? mShadowTextureCustomReceiverPass : mShadowReceiverPass;
			targetPass->getTextureUnitState(0)->setTextureName(mCurrentShadowTexture->getName());

			TextureUnitState* texUnit = targetPass->getTextureUnitState(0);
			texUnit->setProjectiveTexturing(!targetPass->hasVertexProgram(), cam);

			texUnit->setTextureAddressingMode(TextureUnitState::TAM_BORDER);
			texUnit->setTextureBorderColour(ColourValue::White);

			mAutoParamDataSource.setTextureProjector(cam, 0);

			if (l->getType() == Light::LT_SPOTLIGHT) {

				while (targetPass->getNumTextureUnitStates() > 2) {
					targetPass->removeTextureUnitState(2);
				}

				if (targetPass->getNumTextureUnitStates() == 2 &&
					targetPass->getTextureUnitState(1)->getTextureName() == "spot_shadow_fade.png") {

					TextureUnitState* t = targetPass->getTextureUnitState(1);
					t->setProjectiveTexturing(!targetPass->hasVertexProgram(), cam);
				}
				else {

					while (targetPass->getNumTextureUnitStates() > 1) {
						targetPass->removeTextureUnitState(1);
					}

					TextureUnitState* t = targetPass->createTextureUnitState("spot_shadow_fade.png");
					t->setProjectiveTexturing(!targetPass->hasVertexProgram(), cam);
					t->setColourOperation(LBO_ADD);
					t->setTextureAddressingMode(TextureUnitState::TAM_CLAMP);
				}
			}
			else {

				while (targetPass->getNumTextureUnitStates() > 1) {
					targetPass->removeTextureUnitState(1);
				}
			}

			targetPass->setSceneBlending(SBF_DEST_COLOUR, SBF_ZERO);
			targetPass->setLightingEnabled(false);

			targetPass->_load();

			fireShadowTexturesPreReceiver(l, cam);

			renderTextureShadowReceiverQueueGroupObjects(pGroup, om);

			++si;
		}

		mIlluminationStage = IRS_NONE;
	}

	RenderQueueGroup::PriorityMapIterator groupIt3 = pGroup->getIterator();
	while (groupIt3.hasMoreElements()) {
		RenderPriorityGroup* pPriorityGrp = groupIt3.getNext();

		renderObjects(pPriorityGrp->getTransparents(), QueuedRenderableCollection::OM_SORT_DESCENDING, true);
	}
}

void RbxSceneManager::renderTextureShadowReceiverQueueGroupObjects(
	RenderQueueGroup* pGroup, QueuedRenderableCollection::OrganisationMode om
)
{
	static LightList nullLightList;

	RenderQueueGroup::PriorityMapIterator groupIt = pGroup->getIterator();

	mAutoParamDataSource.setAmbientLightColour(ColourValue::White);
	mDestRenderSystem->setAmbientLight(1, 1, 1);

	while (groupIt.hasMoreElements()) {
		RenderPriorityGroup* pPriorityGrp = groupIt.getNext();

		renderObjects(pPriorityGrp->getSolidsBasic(), om, false, &nullLightList);
	}

	mAutoParamDataSource.setAmbientLightColour(mAmbientLight);
	mDestRenderSystem->setAmbientLight(mAmbientLight.r, mAmbientLight.g, mAmbientLight.b);
}

void RbxSceneManager::renderBasicQueueGroupObjects(
	RenderQueueGroup* pGroup, QueuedRenderableCollection::OrganisationMode om
)
{

	RenderQueueGroup::PriorityMapIterator groupIt = pGroup->getIterator();
	Timer timer;

	while (groupIt.hasMoreElements()) {
		RenderPriorityGroup* pPriorityGrp = groupIt.getNext();

		timer.reset();
		pPriorityGrp->sort(mCameraInProgress);
		timer.getMicroseconds();

		timer.reset();
		renderObjects(pPriorityGrp->getSolidsBasic(), om, true);
		timer.getMicroseconds();

		timer.reset();
		renderObjects(pPriorityGrp->getTransparents(), QueuedRenderableCollection::OM_SORT_DESCENDING, true);
		timer.getMicroseconds();
	}
}

void RbxSceneManager::destroyAnimation(const String& name)
{
	OGRE_LOCK_MUTEX(mAnimationsListMutex)

	mAnimationStates.removeAnimationState(name);

	AnimationList::iterator i = mAnimationsList.find(name);
	if (i == mAnimationsList.end()) {
		OGRE_EXCEPT(
			Exception::ERR_ITEM_NOT_FOUND, "Cannot find animation with name " + name, "RbxSceneManager::getAnimation"
		);
	}

	delete i->second;

	mAnimationsList.erase(i);
}

void RbxSceneManager::_applySceneAnimations(void)
{

	OGRE_LOCK_MUTEX(mAnimationStates.OGRE_AUTO_MUTEX_NAME)

	ConstEnabledAnimationStateIterator stateIt = mAnimationStates.getEnabledAnimationStateIterator();

	while (stateIt.hasMoreElements()) {
		const AnimationState* state = stateIt.getNext();
		Animation* anim = getAnimation(state->getAnimationName());

		Animation::NodeTrackIterator nodeTrackIt = anim->getNodeTrackIterator();
		while (nodeTrackIt.hasMoreElements()) {
			Node* nd = nodeTrackIt.getNext()->getAssociatedNode();
			nd->resetToInitialState();
		}

		Animation::NumericTrackIterator numTrackIt = anim->getNumericTrackIterator();
		while (numTrackIt.hasMoreElements()) {
			const AnimableValuePtr& anim = numTrackIt.getNext()->getAssociatedAnimable();
			anim->resetToBaseValue();
		}

		anim->apply(state->getTimePosition(), state->getWeight());
	}
}

void RbxSceneManager::_notifyAutotrackingSceneNode(SceneNode* node, bool autoTrack)
{
	if (autoTrack) {
		mAutoTrackingSceneNodes.insert(node);
	}
	else {
		mAutoTrackingSceneNodes.erase(node);
	}
}

const SceneManager::ShadowCasterList& RbxSceneManager::findShadowCastersForLight(
	const Light* light, const Camera* camera
)
{
	if (g_unk0x00961438 == true) {
		throw std::exception("findShadowCastersForLight call repeated");
	}
	g_unk0x00961438 = true;

	mShadowCasterList.clear();

	SceneNode* root = dynamic_cast<SceneNode*>(mSceneRoot->getChild("rbxroot"));
	if (!root) {
		LogManager::getSingleton().logMessage("RBX Missing or duplicate world node");
	}
	else {
		if (root->getAttachedObjectIterator().hasMoreElements()) {
			RbxEntity* entity = dynamic_cast<RbxEntity*>(root->getAttachedObject((unsigned short) 0));
			entity->FUN_006772d0(const_cast<Camera*>(camera), light, &mShadowCasterList);
		}
	}

	return mShadowCasterList;
}

void RbxSceneManager::destroyStaticGeometry(const String& name)
{
	StaticGeometryList::iterator i = mStaticGeometryList.find(name);
	if (i != mStaticGeometryList.end()) {
		delete i->second;
		mStaticGeometryList.erase(i);
	}
}

void RbxSceneManager::destroyInstancedGeometry(const String& name)
{
	InstancedGeometryList::iterator i = mInstancedGeometryList.find(name);
	if (i != mInstancedGeometryList.end()) {
		delete i->second;
		mInstancedGeometryList.erase(i);
	}
}

bool RbxSceneManager::hasMovableObject(const String& name, const String& typeName) const
{
	OGRE_LOCK_MUTEX(mMovableObjectCollectionMapMutex)

	MovableObjectCollectionMap::const_iterator i = mMovableObjectCollectionMap.find(typeName);
	if (i == mMovableObjectCollectionMap.end()) {
		return false;
	}

	{
		OGRE_LOCK_MUTEX(i->second->mutex)
		return (i->second->map.find(name) != i->second->map.end());
	}
}

void RbxSceneManager::removeSpecialCaseRenderQueue(uint8 qid)
{
	mSpecialCaseQueueList.erase(qid);
}

void RbxSceneManager::destroyAllCameras()
{
	CameraList::iterator i = mCameras.begin();
	for (; i != mCameras.end(); ++i) {
		mDestRenderSystem->_notifyCameraRemoved(i->second);
		delete i->second;
	}
	mCameras.clear();
}

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

void RbxSceneManager::destroyAllAnimations(void)
{
	OGRE_LOCK_MUTEX(mAnimationsListMutex)

	destroyAllAnimationStates();

	AnimationList::iterator i;
	for (i = mAnimationsList.begin(); i != mAnimationsList.end(); ++i) {

		delete i->second;
	}
	mAnimationsList.clear();
}

void RbxSceneManager::destroyAllStaticGeometry(void)
{
	StaticGeometryList::iterator i, iend;
	iend = mStaticGeometryList.end();
	for (i = mStaticGeometryList.begin(); i != iend; ++i) {
		delete i->second;
	}
	mStaticGeometryList.clear();
}

void RbxSceneManager::destroyAllInstancedGeometry(void)
{
	InstancedGeometryList::iterator i, iend;
	iend = mInstancedGeometryList.end();
	for (i = mInstancedGeometryList.begin(); i != iend; ++i) {
		delete i->second;
	}
	mInstancedGeometryList.clear();
}

void RbxSceneManager::destroyAllMovableObjects(void)
{

	OGRE_LOCK_MUTEX(mMovableObjectCollectionMapMutex)

	MovableObjectCollectionMap::iterator ci = mMovableObjectCollectionMap.begin();

	for (; ci != mMovableObjectCollectionMap.end(); ++ci) {
		MovableObjectCollection* coll = ci->second;

		OGRE_LOCK_MUTEX(coll->mutex)

		if (Root::getSingleton().hasMovableObjectFactory(ci->first)) {

			MovableObjectFactory* factory = Root::getSingleton().getMovableObjectFactory(ci->first);
			MovableObjectMap::iterator i = coll->map.begin();
			for (; i != coll->map.end(); ++i) {
				if (i->second->_getManager() == this) {
					factory->destroyInstance(i->second);
				}
			}
		}
		coll->map.clear();
	}
}

Camera* RbxSceneManager::createCamera(const String& name)
{

	if (mCameras.find(name) != mCameras.end()) {
		OGRE_EXCEPT(
			Exception::ERR_DUPLICATE_ITEM,
			"A camera with the name " + name + " already exists",
			"RbxSceneManager::createCamera"
		);
	}

	Camera* c = new Camera(name, this);
	mCameras.insert(CameraList::value_type(name, c));

	mCamVisibleObjectsMap[c] = VisibleObjectsBoundsInfo();

	return c;
}

SceneNode* RbxSceneManager::createSceneNode(void)
{
	SceneNode* sn = new SceneNode(this);
	assert(mSceneNodes.find(sn->getName()) == mSceneNodes.end());
	mSceneNodes[sn->getName()] = sn;
	return sn;
}

SceneNode* RbxSceneManager::createSceneNode(const String& name)
{

	if (mSceneNodes.find(name) != mSceneNodes.end()) {
		OGRE_EXCEPT(
			Exception::ERR_DUPLICATE_ITEM,
			"A scene node with the name " + name + " already exists",
			"RbxSceneManager::createSceneNode"
		);
	}

	SceneNode* sn = new SceneNode(this, name);
	mSceneNodes[sn->getName()] = sn;
	return sn;
}

void RbxSceneManager::prepareRenderQueue(void)
{
	RenderQueue* q = getRenderQueue();

	q->clear();

	RenderQueueInvocationSequence* seq = mCurrentViewport->_getRenderQueueInvocationSequence();
	if (seq) {

		RenderQueueInvocationIterator invokeIt = seq->iterator();
		while (invokeIt.hasMoreElements()) {
			RenderQueueInvocation* invocation = invokeIt.getNext();
			RenderQueueGroup* group = q->getQueueGroup(invocation->getRenderQueueGroupID());
			group->resetOrganisationModes();
		}

		invokeIt = seq->iterator();
		while (invokeIt.hasMoreElements()) {
			RenderQueueInvocation* invocation = invokeIt.getNext();
			RenderQueueGroup* group = q->getQueueGroup(invocation->getRenderQueueGroupID());
			group->addOrganisationMode(invocation->getSolidsOrganisation());

			updateRenderQueueGroupSplitOptions(
				group, invocation->getSuppressShadows(), invocation->getSuppressRenderStateChanges()
			);
		}
	}
	else {

		RenderQueue::QueueGroupIterator groupIter = q->_getQueueGroupIterator();
		while (groupIter.hasMoreElements()) {
			RenderQueueGroup* g = groupIter.getNext();
			g->defaultOrganisationMode();
		}

		updateRenderQueueSplitOptions();
	}
}

void RbxSceneManager::renderVisibleObjectsDefaultSequence(void)
{

	RenderQueue::QueueGroupIterator queueIt = getRenderQueue()->_getQueueGroupIterator();

	while (queueIt.hasMoreElements()) {

		uint8 qId = queueIt.peekNextKey();
		RenderQueueGroup* pGroup = queueIt.getNext();

		if (!isRenderQueueToBeProcessed(qId)) {
			continue;
		}

		bool repeatQueue = false;
		do {

			if (fireRenderQueueStarted(
					qId,
					mIlluminationStage == IRS_RENDER_TO_TEXTURE ? RenderQueueInvocation::RENDER_QUEUE_INVOCATION_SHADOWS
																: StringUtil::BLANK
				)) {

				break;
			}

			_renderQueueGroupObjects(pGroup, QueuedRenderableCollection::OM_PASS_GROUP);

			if (fireRenderQueueEnded(
					qId,
					mIlluminationStage == IRS_RENDER_TO_TEXTURE ? RenderQueueInvocation::RENDER_QUEUE_INVOCATION_SHADOWS
																: StringUtil::BLANK
				)) {

				repeatQueue = true;
			}
			else {
				repeatQueue = false;
			}
		} while (repeatQueue);
	}
}

Animation* RbxSceneManager::createAnimation(const String& name, Real length)
{
	OGRE_LOCK_MUTEX(mAnimationsListMutex)

	if (mAnimationsList.find(name) != mAnimationsList.end()) {
		OGRE_EXCEPT(
			Exception::ERR_DUPLICATE_ITEM,
			"An animation with the name " + name + " already exists",
			"RbxSceneManager::createAnimation"
		);
	}

	Animation* pAnim = new Animation(name, length);
	mAnimationsList[name] = pAnim;
	return pAnim;
}

void RbxSceneManager::destroyShadowTextures(void)
{

	ShadowTextureList::iterator i, iend;
	ShadowTextureCameraList::iterator ci;
	iend = mShadowTextures.end();
	ci = mShadowTextureCameras.begin();
	for (i = mShadowTextures.begin(); i != iend; ++i, ++ci) {
		TexturePtr& shadowTex = *i;

		String matName = shadowTex->getName() + "Mat" + getName();
		MaterialPtr mat = MaterialManager::getSingleton().getByName(matName);
		if (!mat.isNull()) {

			mat->getTechnique(0)->getPass(0)->removeAllTextureUnitStates();
			MaterialManager::getSingleton().remove(mat->getHandle());
		}

		destroyCamera(*ci);
	}
	mShadowTextures.clear();
	mShadowTextureCameras.clear();

	ShadowTextureManager::getSingleton().clearUnused();

	mShadowTextureConfigDirty = true;
}

StaticGeometry* RbxSceneManager::createStaticGeometry(const String& name)
{

	if (mStaticGeometryList.find(name) != mStaticGeometryList.end()) {
		OGRE_EXCEPT(
			Exception::ERR_DUPLICATE_ITEM,
			"StaticGeometry with name '" + name + "' already exists!",
			"RbxSceneManager::createStaticGeometry"
		);
	}
	StaticGeometry* ret = new StaticGeometry(this, name);
	mStaticGeometryList[name] = ret;
	return ret;
}

InstancedGeometry* RbxSceneManager::createInstancedGeometry(const String& name)
{

	if (mInstancedGeometryList.find(name) != mInstancedGeometryList.end()) {
		OGRE_EXCEPT(
			Exception::ERR_DUPLICATE_ITEM,
			"InstancedGeometry with name '" + name + "' already exists!",
			"RbxSceneManager::createInstancedGeometry"
		);
	}
	InstancedGeometry* ret = new InstancedGeometry(this, name);
	mInstancedGeometryList[name] = ret;
	return ret;
}

void RbxSceneManager::renderAdditiveStencilShadowedQueueGroupObjects(
	RenderQueueGroup* pGroup, QueuedRenderableCollection::OrganisationMode om
)
{
	RenderQueueGroup::PriorityMapIterator groupIt = pGroup->getIterator();
	LightList lightList;

	while (groupIt.hasMoreElements()) {
		RenderPriorityGroup* pPriorityGrp = groupIt.getNext();

		pPriorityGrp->sort(mCameraInProgress);

		lightList.clear();

		renderObjects(pPriorityGrp->getSolidsBasic(), om, false, &lightList);

		renderObjects(pPriorityGrp->getSolidsNoShadowReceive(), om, true);

		LightList::const_iterator li, liend;
		liend = mLightsAffectingFrustum.end();

		for (li = mLightsAffectingFrustum.begin(); li != liend; ++li) {
			Light* l = *li;

			if (l->getCastShadows()) {

				mDestRenderSystem->clearFrameBuffer(FBT_STENCIL);
				renderShadowVolumesToStencil(l, mCameraInProgress);

				mDestRenderSystem->setStencilCheckEnabled(true);

				mDestRenderSystem->setStencilBufferParams(CMPF_EQUAL, 0);
			}

			if (lightList.empty()) {
				lightList.push_back(l);
			}
			else {
				lightList[0] = l;
			}
			renderObjects(pPriorityGrp->getSolidsDiffuseSpecular(), om, false, &lightList);

			mDestRenderSystem->setStencilBufferParams();
			mDestRenderSystem->setStencilCheckEnabled(false);
			mDestRenderSystem->_setDepthBufferParams();
		}

		renderObjects(pPriorityGrp->getSolidsDecal(), om, false);
	}

	RenderQueueGroup::PriorityMapIterator groupIt2 = pGroup->getIterator();
	while (groupIt2.hasMoreElements()) {
		RenderPriorityGroup* pPriorityGrp = groupIt2.getNext();

		renderObjects(pPriorityGrp->getTransparents(), QueuedRenderableCollection::OM_SORT_DESCENDING, true);
	}
}

void RbxSceneManager::renderAdditiveTextureShadowedQueueGroupObjects(
	RenderQueueGroup* pGroup, QueuedRenderableCollection::OrganisationMode om
)
{
	RenderQueueGroup::PriorityMapIterator groupIt = pGroup->getIterator();
	LightList lightList;

	while (groupIt.hasMoreElements()) {
		RenderPriorityGroup* pPriorityGrp = groupIt.getNext();

		pPriorityGrp->sort(mCameraInProgress);

		lightList.clear();

		renderObjects(pPriorityGrp->getSolidsBasic(), om, false, &lightList);

		renderObjects(pPriorityGrp->getSolidsNoShadowReceive(), om, true);

		if (mIlluminationStage == IRS_NONE) {

			LightList::const_iterator li, liend;
			ShadowTextureList::iterator si, siend;
			liend = mLightsAffectingFrustum.end();
			siend = mShadowTextures.end();
			si = mShadowTextures.begin();

			for (li = mLightsAffectingFrustum.begin(); li != liend; ++li) {
				Light* l = *li;

				if (l->getCastShadows() && si != siend) {

					mCurrentShadowTexture = si->getPointer();

					Camera* cam = mCurrentShadowTexture->getBuffer()->getRenderTarget()->getViewport(0)->getCamera();

					Pass* targetPass =
						mShadowTextureCustomReceiverPass ? mShadowTextureCustomReceiverPass : mShadowReceiverPass;
					targetPass->getTextureUnitState(0)->setTextureName(mCurrentShadowTexture->getName());

					TextureUnitState* texUnit = targetPass->getTextureUnitState(0);
					texUnit->setProjectiveTexturing(!targetPass->hasVertexProgram(), cam);

					texUnit->setTextureAddressingMode(TextureUnitState::TAM_BORDER);
					texUnit->setTextureBorderColour(ColourValue::White);
					mAutoParamDataSource.setTextureProjector(cam, 0);

					if (targetPass->getNumTextureUnitStates() > 1 &&
						targetPass->getTextureUnitState(1)->getTextureName() == "spot_shadow_fade.png") {

						targetPass->removeTextureUnitState(1);
					}

					targetPass->setSceneBlending(SBF_ONE, SBF_ONE);
					targetPass->setLightingEnabled(true);
					targetPass->_load();

					++si;

					mIlluminationStage = IRS_RENDER_RECEIVER_PASS;
				}
				else {
					mIlluminationStage = IRS_NONE;
				}

				if (lightList.empty()) {
					lightList.push_back(l);
				}
				else {
					lightList[0] = l;
				}
				renderObjects(pPriorityGrp->getSolidsDiffuseSpecular(), om, false, &lightList);
			}

			mIlluminationStage = IRS_NONE;

			renderObjects(pPriorityGrp->getSolidsDecal(), om, false);
		}
	}

	RenderQueueGroup::PriorityMapIterator groupIt2 = pGroup->getIterator();
	while (groupIt2.hasMoreElements()) {
		RenderPriorityGroup* pPriorityGrp = groupIt2.getNext();

		renderObjects(pPriorityGrp->getTransparents(), QueuedRenderableCollection::OM_SORT_DESCENDING, true);
	}
}

void RbxSceneManager::renderSingleObject(
	const Renderable* rend, const Pass* pass, bool doLightIteration, const LightList* manualLightList
)
{
	unsigned short numMatrices;
	static RenderOperation ro;

	const_cast<Renderable*>(rend)->getRenderOperation(ro);
	ro.srcRenderable = rend;

	rend->getWorldTransforms(mTempXform);
	numMatrices = rend->getNumWorldTransforms();
	if (numMatrices > 1) {
		mDestRenderSystem->_setWorldMatrices(mTempXform, numMatrices);
	}
	else {
		mDestRenderSystem->_setWorldMatrix(*mTempXform);
	}

	useRenderableViewProjMode(rend);

	if (!mSuppressRenderStateChanges) {
		bool passSurfaceAndLightParams = true;

		if (pass->isProgrammable()) {

			mAutoParamDataSource.setCurrentRenderable(rend);

			mAutoParamDataSource.setWorldMatrices(mTempXform, numMatrices);
			pass->_updateAutoParamsNoLights(mAutoParamDataSource);
			if (pass->hasVertexProgram()) {
				passSurfaceAndLightParams = pass->getVertexProgram()->getPassSurfaceAndLightStates();
			}
		}

		Pass::ConstTextureUnitStateIterator texIter = pass->getTextureUnitStateIterator();
		size_t unit = 0;
		while (texIter.hasMoreElements()) {
			TextureUnitState* pTex = texIter.getNext();
			if (pTex->hasViewRelativeTextureCoordinateGeneration()) {
				mDestRenderSystem->_setTextureUnitSettings(unit, *pTex);
			}
			++unit;
		}

		mDestRenderSystem->setNormaliseNormals(rend->getNormaliseNormals());

		PolygonMode reqMode = pass->getPolygonMode();
		if (rend->getPolygonModeOverrideable()) {
			PolygonMode camPolyMode = mCameraInProgress->getPolygonMode();

			if (reqMode > camPolyMode) {

				reqMode = camPolyMode;
			}
		}
		mDestRenderSystem->_setPolygonMode(reqMode);

		mDestRenderSystem->setClipPlanes(rend->getClipPlanes());

		if (doLightIteration) {

			static LightList localLightList;

			const LightList& rendLightList = rend->getLights();

			bool iteratePerLight = pass->getIteratePerLight();

			int lightsLeft = 1;
			if (iteratePerLight) {
				lightsLeft = static_cast<int>(rendLightList.size()) - pass->getStartLight();

				if (lightsLeft > static_cast<int>(pass->getMaxSimultaneousLights())) {
					lightsLeft = static_cast<int>(pass->getMaxSimultaneousLights());
				}
			}

			const LightList* pLightListToUse;

			size_t lightIndex = pass->getStartLight();

			while (lightsLeft > 0) {

				if (iteratePerLight) {
					localLightList.resize(pass->getLightCountPerIteration());

					LightList::iterator destit = localLightList.begin();
					unsigned short numShadowTextureLights = 0;
					for (; destit != localLightList.end() && lightIndex < rendLightList.size();
						 ++lightIndex, --lightsLeft) {

						if (pass->getRunOnlyForOneLightType() &&
							pass->getOnlyLightType() != rendLightList[lightIndex]->getType()) {

							continue;
						}

						*destit++ = rendLightList[lightIndex];

						if (isShadowTechniqueTextureBased() && lightIndex < mShadowTextures.size()) {

							unsigned short tuindex = pass->_getTextureUnitWithContentTypeIndex(
								TextureUnitState::CONTENT_SHADOW, numShadowTextureLights
							);
							if (tuindex < pass->getNumTextureUnitStates()) {

								TextureUnitState* tu =
									const_cast<TextureUnitState*>(pass->getTextureUnitState(tuindex));
								const TexturePtr& shadowTex = mShadowTextures[lightIndex];
								tu->_setTexturePtr(shadowTex);
								Camera* cam = shadowTex->getBuffer()->getRenderTarget()->getViewport(0)->getCamera();
								tu->setProjectiveTexturing(!pass->hasVertexProgram(), cam);
								mAutoParamDataSource.setTextureProjector(cam, numShadowTextureLights);
								++numShadowTextureLights;

								mDestRenderSystem->_setTextureUnitSettings(tuindex, *tu);
							}
						}
					}

					if (destit != localLightList.end()) {
						localLightList.erase(destit, localLightList.end());
						lightsLeft = 0;
					}
					pLightListToUse = &localLightList;
				}
				else {

					if (pass->getStartLight() || pass->getMaxSimultaneousLights() != OGRE_MAX_SIMULTANEOUS_LIGHTS) {

						if (pass->getStartLight() >= rendLightList.size()) {
							lightsLeft = 0;
							break;
						}
						else {
							localLightList.clear();
							LightList::const_iterator copyStart = rendLightList.begin();
							std::advance(copyStart, pass->getStartLight());
							LightList::const_iterator copyEnd = copyStart;
							std::advance(copyEnd, pass->getMaxSimultaneousLights());
							localLightList.insert(localLightList.begin(), copyStart, copyEnd);
							pLightListToUse = &localLightList;
						}
					}
					else {
						pLightListToUse = &rendLightList;
					}
					lightsLeft = 0;
				}

				if (pass->isProgrammable()) {

					mAutoParamDataSource.setCurrentLightList(pLightListToUse);
					pass->_updateAutoParamsLightsOnly(mAutoParamDataSource);

					if (pass->hasVertexProgram()) {
						mDestRenderSystem->bindGpuProgramParameters(
							GPT_VERTEX_PROGRAM, pass->getVertexProgramParameters()
						);
					}
					if (pass->hasFragmentProgram()) {
						mDestRenderSystem->bindGpuProgramParameters(
							GPT_FRAGMENT_PROGRAM, pass->getFragmentProgramParameters()
						);
					}
				}

				if (pass->getLightingEnabled() && passSurfaceAndLightParams) {
					mDestRenderSystem->_useLights(*pLightListToUse, pass->getMaxSimultaneousLights());
				}

				mDestRenderSystem->setCurrentPassIterationCount(pass->getPassIterationCount());
				mDestRenderSystem->_render(ro);
			}
		}
		else {

			if (pass->isProgrammable()) {

				if (manualLightList) {

					mAutoParamDataSource.setCurrentLightList(manualLightList);
					pass->_updateAutoParamsLightsOnly(mAutoParamDataSource);
				}

				if (pass->hasVertexProgram()) {
					mDestRenderSystem->bindGpuProgramParameters(GPT_VERTEX_PROGRAM, pass->getVertexProgramParameters());
				}
				if (pass->hasFragmentProgram()) {
					mDestRenderSystem->bindGpuProgramParameters(
						GPT_FRAGMENT_PROGRAM, pass->getFragmentProgramParameters()
					);
				}
			}

			if (manualLightList && pass->getLightingEnabled() && passSurfaceAndLightParams) {
				mDestRenderSystem->_useLights(*manualLightList, pass->getMaxSimultaneousLights());
			}

			mDestRenderSystem->setCurrentPassIterationCount(pass->getPassIterationCount());
			mDestRenderSystem->_render(ro);
		}
	}
	else {

		mDestRenderSystem->setCurrentPassIterationCount(1);
		mDestRenderSystem->_render(ro);
	}

	resetViewProjMode();
}

void RbxSceneManager::addRenderQueueListener(RenderQueueListener* newListener)
{
	mRenderQueueListeners.push_back(newListener);
}

void RbxSceneManager::addShadowListener(ShadowListener* newListener)
{
	mShadowListeners.push_back(newListener);
}

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

void RbxSceneManager::renderShadowVolumesToStencil(const Light* light, const Camera* camera)
{

	const ShadowCasterList& casters = findShadowCastersForLight(light, camera);

	if (casters.empty()) {

		return;
	}

	bool scissored = false;
	if (light->getType() != Light::LT_DIRECTIONAL &&
		mDestRenderSystem->getCapabilities()->hasCapability(RSC_SCISSOR_TEST)) {

		Real left, right, top, bottom;
		Sphere sphere(light->getDerivedPosition(), light->getAttenuationRange());
		if (camera->projectSphere(sphere, &left, &top, &right, &bottom)) {
			scissored = true;

			int iLeft, iTop, iWidth, iHeight;
			mCurrentViewport->getActualDimensions(iLeft, iTop, iWidth, iHeight);
			size_t szLeft, szRight, szTop, szBottom;

			szLeft = (size_t) (iLeft + ((left + 1) * 0.5 * iWidth));
			szRight = (size_t) (iLeft + ((right + 1) * 0.5 * iWidth));
			szTop = (size_t) (iTop + ((-top + 1) * 0.5 * iHeight));
			szBottom = (size_t) (iTop + ((-bottom + 1) * 0.5 * iHeight));

			mDestRenderSystem->setScissorTest(true, szLeft, szTop, szRight, szBottom);
		}
	}

	mDestRenderSystem->unbindGpuProgram(GPT_FRAGMENT_PROGRAM);

	bool stencil2sided = false;
	if (mDestRenderSystem->getCapabilities()->hasCapability(RSC_TWO_SIDED_STENCIL) &&
		mDestRenderSystem->getCapabilities()->hasCapability(RSC_STENCIL_WRAP)) {

		stencil2sided = true;
	}

	bool extrudeInSoftware = true;
	bool finiteExtrude =
		!mShadowUseInfiniteFarPlane || !mDestRenderSystem->getCapabilities()->hasCapability(RSC_INFINITE_FAR_PLANE);
	if (mDestRenderSystem->getCapabilities()->hasCapability(RSC_VERTEX_PROGRAM)) {
		extrudeInSoftware = false;

		mShadowStencilPass->setVertexProgram(
			ShadowVolumeExtrudeProgram::getProgramName(light->getType(), finiteExtrude, false), false
		);

		if (finiteExtrude) {
			mShadowStencilPass->setVertexProgramParameters(mFiniteExtrusionParams);
		}
		else {
			mShadowStencilPass->setVertexProgramParameters(mInfiniteExtrusionParams);
		}
		if (mDebugShadows) {
			mShadowDebugPass->setVertexProgram(
				ShadowVolumeExtrudeProgram::getProgramName(light->getType(), finiteExtrude, true), false
			);

			if (finiteExtrude) {
				mShadowDebugPass->setVertexProgramParameters(mFiniteExtrusionParams);
			}
			else {
				mShadowDebugPass->setVertexProgramParameters(mInfiniteExtrusionParams);
			}
		}

		mDestRenderSystem->bindGpuProgram(mShadowStencilPass->getVertexProgram()->_getBindingDelegate());
	}
	else {
		mDestRenderSystem->unbindGpuProgram(GPT_VERTEX_PROGRAM);
	}

	LightList lightList;

	lightList.push_back(const_cast<Light*>(light));

	mDestRenderSystem->_setColourBufferWriteEnabled(false, false, false, false);
	mDestRenderSystem->_disableTextureUnitsFrom(0);
	mDestRenderSystem->_setDepthBufferParams(true, false, CMPF_LESS);
	mDestRenderSystem->setStencilCheckEnabled(true);

	Real extrudeDist = mShadowDirLightExtrudeDist;

	const PlaneBoundedVolume& nearClipVol = light->_getNearClipVolume(camera);

	ShadowCasterList::const_iterator si, siend;
	siend = casters.end();

	for (si = casters.begin(); si != siend; ++si) {
		ShadowCaster* caster = *si;
		bool zfailAlgo = camera->isCustomNearClipPlaneEnabled();
		unsigned long flags = 0;

		if (light->getType() != Light::LT_DIRECTIONAL) {
			extrudeDist = caster->getPointExtrusionDistance(light);
		}

		if (!extrudeInSoftware && !finiteExtrude) {

			flags |= SRF_EXTRUDE_TO_INFINITY;
		}

		if (zfailAlgo || nearClipVol.intersects(caster->getWorldBoundingBox())) {

			zfailAlgo = true;

			if (camera->isVisible(caster->getLightCapBounds())) {
				flags |= SRF_INCLUDE_LIGHT_CAP;
			}

			if (!((flags & SRF_EXTRUDE_TO_INFINITY) && light->getType() == Light::LT_DIRECTIONAL) &&
				camera->isVisible(caster->getDarkCapBounds(*light, extrudeDist))) {
				flags |= SRF_INCLUDE_DARK_CAP;
			}
		}
		else {

			if ((flags & SRF_EXTRUDE_TO_INFINITY) && light->getType() != Light::LT_DIRECTIONAL &&
				isShadowTechniqueModulative() && camera->isVisible(caster->getDarkCapBounds(*light, extrudeDist))) {
				flags |= SRF_INCLUDE_DARK_CAP;
			}
			else if (
				!(flags & SRF_EXTRUDE_TO_INFINITY) && camera->isVisible(caster->getDarkCapBounds(*light, extrudeDist))
			) {
				flags |= SRF_INCLUDE_DARK_CAP;
			}
		}

		ShadowCaster::ShadowRenderableListIterator iShadowRenderables = caster->getShadowVolumeRenderableIterator(
			mShadowTechnique, light, &mShadowIndexBuffer, extrudeInSoftware, extrudeDist, flags
		);

		setShadowVolumeStencilState(false, zfailAlgo, stencil2sided);
		renderShadowVolumeObjects(
			iShadowRenderables, mShadowStencilPass, &lightList, flags, false, zfailAlgo, stencil2sided
		);
		if (!stencil2sided) {

			setShadowVolumeStencilState(true, zfailAlgo, false);
			renderShadowVolumeObjects(
				iShadowRenderables, mShadowStencilPass, &lightList, flags, true, zfailAlgo, false
			);
		}

		if (mDebugShadows) {

			mDestRenderSystem->setStencilBufferParams();
			mShadowDebugPass->getTextureUnitState(0)->setColourOperationEx(
				LBX_MODULATE,
				LBS_MANUAL,
				LBS_CURRENT,
				zfailAlgo ? ColourValue(0.7, 0.0, 0.2) : ColourValue(0.0, 0.7, 0.2)
			);
			_setPass(mShadowDebugPass);
			renderShadowVolumeObjects(iShadowRenderables, mShadowDebugPass, &lightList, flags, true, false, false);
			mDestRenderSystem->_setColourBufferWriteEnabled(false, false, false, false);
			mDestRenderSystem->_setDepthBufferFunction(CMPF_LESS);
		}
	}

	mDestRenderSystem->_setColourBufferWriteEnabled(true, true, true, true);

	mDestRenderSystem->_setDepthBufferParams();

	mDestRenderSystem->setStencilCheckEnabled(false);

	mDestRenderSystem->unbindGpuProgram(GPT_VERTEX_PROGRAM);

	if (scissored) {

		mDestRenderSystem->setScissorTest(false);
	}
}

void RbxSceneManager::ensureShadowTexturesCreated()
{
	if (mShadowTextureConfigDirty) {
		destroyShadowTextures();
		ShadowTextureManager::getSingleton().getShadowTextures(mShadowTextureConfigList, mShadowTextures);

		mShadowCamLightMapping.clear();

		for (ShadowTextureList::iterator i = mShadowTextures.begin(); i != mShadowTextures.end(); ++i) {
			const TexturePtr& shadowTex = *i;

			String camName = shadowTex->getName() + "Cam";

			String matName = shadowTex->getName() + "Mat" + getName();

			RenderTexture* shadowRTT = shadowTex->getBuffer()->getRenderTarget();

			Camera* cam = createCamera(camName);
			cam->setAspectRatio(shadowTex->getWidth() / shadowTex->getHeight());

			cam->setUseRenderingDistance(false);
			mShadowTextureCameras.push_back(cam);

			if (shadowRTT->getNumViewports() == 0) {

				Viewport* v = shadowRTT->addViewport(cam);
				v->setClearEveryFrame(true);

				v->setOverlaysEnabled(false);
			}

			shadowRTT->setAutoUpdated(false);

			MaterialPtr mat = MaterialManager::getSingleton().getByName(matName);
			if (mat.isNull()) {
				mat =
					MaterialManager::getSingleton().create(matName, ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);
			}
			Pass* p = mat->getTechnique(0)->getPass(0);
			if (p->getNumTextureUnitStates() != 1 || p->getTextureUnitState(0)->_getTexturePtr(0) != shadowTex) {
				mat->getTechnique(0)->getPass(0)->removeAllTextureUnitStates();

				TextureUnitState* texUnit = p->createTextureUnitState(shadowTex->getName());

				texUnit->setProjectiveTexturing(!p->hasVertexProgram(), cam);

				texUnit->setTextureAddressingMode(TextureUnitState::TAM_BORDER);
				texUnit->setTextureBorderColour(ColourValue::White);
				mat->touch();
			}

			mShadowCamLightMapping[cam] = 0;

			if (mShadowTextureConfigList.empty()) {
				mNullShadowTexture.setNull();
			}
			else {
				mNullShadowTexture =
					ShadowTextureManager::getSingleton().getNullShadowTexture(mShadowTextureConfigList[0].format);
			}
		}
		mShadowTextureConfigDirty = false;
	}
}

void RbxSceneManager::setShadowTextureCount(size_t count)
{

	if (count != mShadowTextureConfigList.size()) {
		mShadowTextureConfigList.resize(count);
		mShadowTextureConfigDirty = true;
	}
}

Entity* RbxSceneManager::createEntity(const String& entityName, const String& meshName)
{

	NameValuePairList params;
	params["mesh"] = meshName;
	return static_cast<Entity*>(createMovableObject(entityName, EntityFactory::FACTORY_TYPE_NAME, &params));
}

ParticleSystem* RbxSceneManager::createParticleSystem(const String& name, const String& templateName)
{
	NameValuePairList params;
	params["templateName"] = templateName;

	return static_cast<ParticleSystem*>(createMovableObject(name, ParticleSystemFactory::FACTORY_TYPE_NAME, &params));
}

ParticleSystem* RbxSceneManager::createParticleSystem(const String& name, size_t quota, const String& group)
{
	NameValuePairList params;
	params["quota"] = StringConverter::toString(quota);
	params["resourceGroup"] = group;

	return static_cast<ParticleSystem*>(createMovableObject(name, ParticleSystemFactory::FACTORY_TYPE_NAME, &params));
}

BillboardSet* RbxSceneManager::createBillboardSet(const String& name, unsigned int poolSize)
{
	NameValuePairList params;
	params["poolSize"] = StringConverter::toString(poolSize);
	return static_cast<BillboardSet*>(createMovableObject(name, BillboardSetFactory::FACTORY_TYPE_NAME, &params));
}

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

MovableObject* RbxSceneManager::createMovableObject(
	const String& name, const String& typeName, const NameValuePairList* params
)
{
	MovableObjectFactory* factory = Root::getSingleton().getMovableObjectFactory(typeName);

	MovableObjectCollection* objectMap = getMovableObjectCollection(typeName);

	{
		OGRE_LOCK_MUTEX(objectMap->mutex)

		if (objectMap->map.find(name) != objectMap->map.end()) {
			OGRE_EXCEPT(
				Exception::ERR_DUPLICATE_ITEM,
				"An object of type '" + typeName + "' with name '" + name + "' already exists.",
				"RbxSceneManager::createMovableObject"
			);
		}

		MovableObject* newObj = factory->createInstance(name, this, params);
		objectMap->map[name] = newObj;
		return newObj;
	}
}

void RbxSceneManager::destroyMovableObject(const String& name, const String& typeName)
{
	MovableObjectCollection* objectMap = getMovableObjectCollection(typeName);
	MovableObjectFactory* factory = Root::getSingleton().getMovableObjectFactory(typeName);

	{
		OGRE_LOCK_MUTEX(objectMap->mutex)

		MovableObjectMap::iterator mi = objectMap->map.find(name);
		if (mi != objectMap->map.end()) {
			factory->destroyInstance(mi->second);
			objectMap->map.erase(mi);
		}
	}
}

void RbxSceneManager::destroyAllMovableObjectsByType(const String& typeName)
{
	MovableObjectCollection* objectMap = getMovableObjectCollection(typeName);
	MovableObjectFactory* factory = Root::getSingleton().getMovableObjectFactory(typeName);

	{
		OGRE_LOCK_MUTEX(objectMap->mutex)
		MovableObjectMap::iterator i = objectMap->map.begin();
		for (; i != objectMap->map.end(); ++i) {

			if (i->second->_getManager() == this) {
				factory->destroyInstance(i->second);
			}
		}
		objectMap->map.clear();
	}
}

SceneManager::MovableObjectIterator RbxSceneManager::getMovableObjectIterator(const String& typeName)
{
	MovableObjectCollection* objectMap = getMovableObjectCollection(typeName);
	return MovableObjectIterator(objectMap->map.begin(), objectMap->map.end());
}

void RbxSceneManager::injectMovableObject(MovableObject* m)
{
	MovableObjectCollection* objectMap = getMovableObjectCollection(m->getMovableType());
	{
		OGRE_LOCK_MUTEX(objectMap->mutex)

		objectMap->map[m->getName()] = m;
	}
}

void RbxSceneManager::extractMovableObject(const String& name, const String& typeName)
{
	MovableObjectCollection* objectMap = getMovableObjectCollection(typeName);
	{
		OGRE_LOCK_MUTEX(objectMap->mutex)
		MovableObjectMap::iterator mi = objectMap->map.find(name);
		if (mi != objectMap->map.end()) {

			objectMap->map.erase(mi);
		}
	}
}

void RbxSceneManager::extractAllMovableObjectsByType(const String& typeName)
{
	MovableObjectCollection* objectMap = getMovableObjectCollection(typeName);
	{
		OGRE_LOCK_MUTEX(objectMap->mutex)

		objectMap->map.clear();
	}
}

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

void RbxSceneManager::_populateLightList(const Vector3& position, Real radius, LightList& destList)
{

	const LightList& candidateLights = _getLightsAffectingFrustum();

	destList.clear();
	destList.reserve(candidateLights.size());

	LightList::const_iterator it;
	for (it = candidateLights.begin(); it != candidateLights.end(); ++it) {
		Light* lt = *it;
		if (lt->getType() == Light::LT_DIRECTIONAL) {

			lt->tempSquareDist = 0.0f;
			destList.push_back(lt);
		}
		else {

			lt->tempSquareDist = (lt->getDerivedPosition() - position).squaredLength();

			Real range = lt->getAttenuationRange();
			Real maxDist = range + radius;
			if (lt->tempSquareDist <= Math::Sqr(maxDist)) {
				destList.push_back(lt);
			}
		}
	}

	if (isShadowTechniqueTextureBased()) {

		if (destList.size() > getShadowTextureCount()) {
			LightList::iterator start = destList.begin();
			std::advance(start, getShadowTextureCount());
			std::stable_sort(start, destList.end(), lightLess());
		}
	}
	else {
		std::stable_sort(destList.begin(), destList.end(), lightLess());
	}
}

void RbxSceneManager::findLightsAffectingFrustum(const Camera* camera)
{

	MovableObjectCollection* lights = getMovableObjectCollection(LightFactory::FACTORY_TYPE_NAME);

	{
		OGRE_LOCK_MUTEX(lights->mutex)

		mTestLightInfos.clear();
		mTestLightInfos.reserve(lights->map.size());

		MovableObjectIterator it(lights->map.begin(), lights->map.end());

		while (it.hasMoreElements()) {
			Light* l = static_cast<Light*>(it.getNext());
			if (l->isVisible()) {
				LightInfo lightInfo;
				lightInfo.light = l;
				lightInfo.type = l->getType();
				if (lightInfo.type == Light::LT_DIRECTIONAL) {

					lightInfo.position = Vector3::ZERO;
					lightInfo.range = 0;
					mTestLightInfos.push_back(lightInfo);
				}
				else {

					lightInfo.range = l->getAttenuationRange();
					lightInfo.position = l->getDerivedPosition();
					Sphere sphere(lightInfo.position, lightInfo.range);
					if (camera->isVisible(sphere)) {
						mTestLightInfos.push_back(lightInfo);
					}
				}
			}
		}
	}

	if (mCachedLightInfos != mTestLightInfos) {
		mLightsAffectingFrustum.resize(mTestLightInfos.size());
		LightInfoList::const_iterator i;
		LightList::iterator j = mLightsAffectingFrustum.begin();
		for (i = mTestLightInfos.begin(); i != mTestLightInfos.end(); ++i, ++j) {
			*j = i->light;

			if (isShadowTechniqueTextureBased()) {
				(*j)->tempSquareDist = (camera->getDerivedPosition() - (*j)->getDerivedPosition()).squaredLength();
			}
		}

		if (isShadowTechniqueTextureBased()) {

			bool overridden = false;
			for (ShadowListenerList::reverse_iterator ri = mShadowListeners.rbegin(); ri != mShadowListeners.rend();
				 ++ri) {
				overridden = (*ri)->sortLightsAffectingFrustum(mLightsAffectingFrustum);
				if (overridden) {
					break;
				}
			}
			if (!overridden) {

				std::stable_sort(
					mLightsAffectingFrustum.begin(), mLightsAffectingFrustum.end(), lightsForShadowTextureLess()
				);
			}
		}

		mCachedLightInfos.swap(mTestLightInfos);

		_notifyLightsDirty();
	}
}

SceneNode* RbxSceneManager::getRootSceneNode(void) const
{
	return mSceneRoot;
}

void RbxSceneManager::_setDestinationRenderSystem(RenderSystem* sys)
{
	mDestRenderSystem = sys;
}

FogMode RbxSceneManager::getFogMode(void) const
{
	return mFogMode;
}

} // namespace Ogre
