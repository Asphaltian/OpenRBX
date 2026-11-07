#ifndef APPDRAW_RBXSCENEMANAGER_H
#define APPDRAW_RBXSCENEMANAGER_H

#include "decomp.h"

#include <OgreSceneManager.h>
#include <OgreSceneManagerEnumerator.h>

namespace Ogre {

// SIZE 0xc4
class RbxSky
{
public:
	virtual ~RbxSky(); // vtable+0x00

private:
	undefined m_unk0x04[0xc4 - 0x04]; // 0x04
};

// SIZE 0x8ab0
class RbxSceneManager : public SceneManager
{
public:
	// SIZE 0x14
	class SceneMgrQueuedRenderableVisitor : public QueuedRenderableVisitor
	{
	protected:
		const Pass* mUsedPass; // 0x04

	public:
		SceneMgrQueuedRenderableVisitor() : transparentShadowCastersMode(false) {}
		~SceneMgrQueuedRenderableVisitor() {}

		void visit(const Renderable* r);
		bool visit(const Pass* p);
		void visit(const RenderablePass* rp);

		RbxSceneManager* targetSceneMgr;   // 0x08
		bool transparentShadowCastersMode; // 0x0c
		bool autoLights;                   // 0x0d
		const LightList* manualLightList;  // 0x10

		// SYNTHETIC: ROBLOX 0x00685660
		// Ogre::RbxSceneManager::SceneMgrQueuedRenderableVisitor::`scalar deleting destructor'
	};

	// SIZE 0x20
	class ShadowCasterSceneQueryListener : public SceneQueryListener
	{
	protected:
		SceneManager* mSceneMgr;                            // 0x04
		ShadowCasterList* mCasterList;                      // 0x08
		bool mIsLightInFrustum;                             // 0x0c
		const PlaneBoundedVolumeList* mLightClipVolumeList; // 0x10
		const Camera* mCamera;                              // 0x14
		const Light* mLight;                                // 0x18
		Real mFarDistSquared;                               // 0x1c

	public:
		ShadowCasterSceneQueryListener(SceneManager* sm)
			: mSceneMgr(sm), mCasterList(NULL), mIsLightInFrustum(false), mLightClipVolumeList(NULL), mCamera(NULL)
		{
		}

		void prepare(
			bool lightInFrustum,
			const PlaneBoundedVolumeList* lightClipVolumes,
			const Light* light,
			const Camera* cam,
			ShadowCasterList* casterList,
			Real farDistSquared
		)
		{
			mCasterList = casterList;
			mIsLightInFrustum = lightInFrustum;
			mLightClipVolumeList = lightClipVolumes;
			mCamera = cam;
			mLight = light;
			mFarDistSquared = farDistSquared;
		}

		bool queryResult(MovableObject* object);
		bool queryResult(SceneQuery::WorldFragment* fragment) { return true; }

		// SYNTHETIC: ROBLOX 0x00687340
		// Ogre::RbxSceneManager::ShadowCasterSceneQueryListener::`scalar deleting destructor'
	};

protected:
	undefined4 m_unk0x8a64;                                          // 0x8a64
	String m_unk0x8a68;                                              // 0x8a68
	SharedPtr<RbxSky> m_unk0x8a84;                                   // 0x8a84
	undefined4 m_unk0x8a90;                                          // 0x8a90
	ShadowCasterSceneQueryListener* mShadowCasterQueryListener;      // 0x8a94
	SceneMgrQueuedRenderableVisitor* mActiveQueuedRenderableVisitor; // 0x8a98
	SceneMgrQueuedRenderableVisitor mDefaultQueuedRenderableVisitor; // 0x8a9c

	friend class SceneMgrQueuedRenderableVisitor;

	using SceneManager::getMovableObjectCollection;
	MovableObjectCollection* getMovableObjectCollection(const String& typeName);

	bool validatePassForRendering(const Pass* pass);
	bool validateRenderableForRendering(const Pass* pass, const Renderable* rend);

public:
	using SceneManager::createEntity;

	RbxSceneManager(const String& name);
	~RbxSceneManager();

	const String& getTypeName() const;

	void clearScene();
	void destroyAllCameras();

	virtual void setViewport(Viewport* vp);
	virtual void renderObjects(
		const QueuedRenderableCollection& objs,
		QueuedRenderableCollection::OrganisationMode om,
		bool doLightIteration,
		const LightList* manualLightList = 0
	);
	virtual bool hasCamera(const String& name) const;
	virtual Light* createLight(const String& name);
	virtual Light* getLight(const String& name) const;
	virtual bool hasLight(const String& name) const;
	virtual void destroyLight(const String& name);
	virtual void destroyLight(Light* light);
	virtual void destroyAllLights(void);
	virtual void _notifyLightsDirty(void);
	virtual const LightList& _getLightsAffectingFrustum(void) const;
	virtual bool hasSceneNode(const String& name) const;
	virtual Entity* getEntity(const String& name) const;
	virtual bool hasEntity(const String& name) const;
	virtual void destroyEntity(Entity* ent);
	virtual void destroyEntity(const String& name);
	virtual void destroyAllEntities(void);
	virtual ManualObject* createManualObject(const String& name);
	virtual ManualObject* getManualObject(const String& name) const;
	virtual bool hasManualObject(const String& name) const;
	virtual void destroyManualObject(ManualObject* obj);
	virtual void destroyManualObject(const String& name);
	virtual void destroyAllManualObjects(void);
	virtual BillboardChain* createBillboardChain(const String& name);
	virtual BillboardChain* getBillboardChain(const String& name) const;
	virtual bool hasBillboardChain(const String& name) const;
	virtual void destroyBillboardChain(BillboardChain* obj);
	virtual void destroyBillboardChain(const String& name);
	virtual void destroyAllBillboardChains(void);
	virtual RibbonTrail* createRibbonTrail(const String& name);
	virtual RibbonTrail* getRibbonTrail(const String& name) const;
	virtual bool hasRibbonTrail(const String& name) const;
	virtual void destroyRibbonTrail(RibbonTrail* obj);
	virtual void destroyRibbonTrail(const String& name);
	virtual void destroyAllRibbonTrails(void);
	virtual ParticleSystem* getParticleSystem(const String& name) const;
	virtual bool hasParticleSystem(const String& name) const;
	virtual void destroyParticleSystem(ParticleSystem* obj);
	virtual void destroyParticleSystem(const String& name);
	virtual void destroyAllParticleSystems(void);
	virtual void _updateSceneGraph(Camera* cam);
	virtual void _findVisibleObjects(Camera* cam, VisibleObjectsBoundsInfo* visibleBounds, bool onlyShadowCasters);
	virtual void _renderVisibleObjects(void);
	// FUNCTION: ROBLOX 0x00688480
	virtual bool isSkyPlaneEnabled(void) const { return mSkyPlaneEnabled; }
	// FUNCTION: ROBLOX 0x00688490
	virtual SceneNode* getSkyPlaneNode(void) const { return mSkyPlaneNode; }
	// FUNCTION: ROBLOX 0x006884a0
	virtual const SkyPlaneGenParameters& getSkyPlaneGenParameters(void) const { return mSkyPlaneGenParameters; }
	// FUNCTION: ROBLOX 0x006884b0
	virtual bool isSkyBoxEnabled(void) const { return mSkyBoxEnabled; }
	// FUNCTION: ROBLOX 0x006884c0
	virtual SceneNode* getSkyBoxNode(void) const { return mSkyBoxNode; }
	// FUNCTION: ROBLOX 0x006884d0
	virtual const SkyBoxGenParameters& getSkyBoxGenParameters(void) const { return mSkyBoxGenParameters; }
	// FUNCTION: ROBLOX 0x006884e0
	virtual bool isSkyDomeEnabled(void) const { return mSkyDomeEnabled; }
	// FUNCTION: ROBLOX 0x006884f0
	virtual SceneNode* getSkyDomeNode(void) const { return mSkyDomeNode; }
	// FUNCTION: ROBLOX 0x00688500
	virtual const SkyDomeGenParameters& getSkyDomeGenParameters(void) const { return mSkyDomeGenParameters; }
	virtual const ColourValue& getFogColour(void) const;
	virtual Real getFogStart(void) const;
	virtual Real getFogEnd(void) const;
	virtual Real getFogDensity(void) const;
	virtual BillboardSet* getBillboardSet(const String& name) const;
	virtual bool hasBillboardSet(const String& name) const;
	virtual void destroyBillboardSet(BillboardSet* set);
	virtual void destroyBillboardSet(const String& name);
	virtual void destroyAllBillboardSets(void);
	virtual void setDisplaySceneNodes(bool display);
	// FUNCTION: ROBLOX 0x00688510
	virtual bool getDisplaySceneNodes(void) const { return mDisplayNodes; }
	virtual AnimationState* getAnimationState(const String& animName) const;
	virtual bool hasAnimationState(const String& name) const;
	virtual void destroyAnimationState(const String& name);
	virtual void destroyAllAnimationStates(void);
	virtual RenderQueue* getRenderQueue(void);
	virtual void addRenderQueueListener(RenderQueueListener* newListener);
	virtual void addSpecialCaseRenderQueue(uint8 qid);
	virtual void removeSpecialCaseRenderQueue(uint8 qid);
	virtual void clearSpecialCaseRenderQueues(void);
	virtual void setSpecialCaseRenderQueueMode(SpecialCaseRenderQueueMode mode);
	virtual SpecialCaseRenderQueueMode getSpecialCaseRenderQueueMode(void);
	virtual void setWorldGeometryRenderQueue(uint8 qid);
	virtual uint8 getWorldGeometryRenderQueue(void);
	virtual void showBoundingBoxes(bool bShow);
	virtual bool getShowBoundingBoxes() const;
	virtual void _notifyAutotrackingSceneNode(SceneNode* node, bool autoTrack);
	virtual void destroyQuery(SceneQuery* query);
	// FUNCTION: ROBLOX 0x00688520
	virtual ShadowTechnique getShadowTechnique(void) const { return mShadowTechnique; }
	// FUNCTION: ROBLOX 0x00688530
	virtual void setShowDebugShadows(bool debug) { mDebugShadows = debug; }
	// FUNCTION: ROBLOX 0x00688540
	virtual bool getShowDebugShadows(void) const { return mDebugShadows; }
	virtual const ColourValue& getShadowColour(void) const;
	virtual void setShadowDirectionalLightExtrusionDistance(Real dist);
	virtual Real getShadowDirectionalLightExtrusionDistance(void) const;
	virtual void setShadowFarDistance(Real distance);
	// FUNCTION: ROBLOX 0x00688550
	virtual Real getShadowFarDistance(void) const { return mShadowFarDist; }
	// FUNCTION: ROBLOX 0x00688560
	virtual size_t getShadowIndexBufferSize(void) const { return mShadowIndexBufferSize; }
	virtual void setShadowTextureConfig(
		size_t shadowIndex, unsigned short width, unsigned short height, PixelFormat format
	);
	// FUNCTION: ROBLOX 0x00688570
	virtual void setShadowDirLightTextureOffset(Real offset) { mShadowTextureOffset = offset; }
	// FUNCTION: ROBLOX 0x00688580
	virtual Real getShadowDirLightTextureOffset(void) const { return mShadowTextureOffset; }
	// FUNCTION: ROBLOX 0x00688590
	virtual void setShadowTextureFadeStart(Real fadeStart) { mShadowTextureFadeStart = fadeStart; }
	// FUNCTION: ROBLOX 0x006885a0
	virtual void setShadowTextureFadeEnd(Real fadeEnd) { mShadowTextureFadeEnd = fadeEnd; }
	virtual void setShadowTextureSelfShadow(bool selfShadow);
	// FUNCTION: ROBLOX 0x006885b0
	virtual bool getShadowTextureSelfShadow(void) const { return mShadowTextureSelfShadow; }
	// FUNCTION: ROBLOX 0x006885c0
	virtual void setShadowCasterRenderBackFaces(bool bf) { mShadowCasterRenderBackFaces = bf; }
	// FUNCTION: ROBLOX 0x006885d0
	virtual bool getShadowCasterRenderBackFaces() const { return mShadowCasterRenderBackFaces; }
	virtual const ShadowCameraSetupPtr& getShadowCameraSetup() const;
	// FUNCTION: ROBLOX 0x006885e0
	virtual void setShadowUseInfiniteFarPlane(bool enable) { mShadowUseInfiniteFarPlane = enable; }
	// FUNCTION: ROBLOX 0x006885f0
	virtual bool isShadowTechniqueStencilBased(void) const
	{
		return (mShadowTechnique & SHADOWDETAILTYPE_STENCIL) != 0;
	}
	// FUNCTION: ROBLOX 0x00688600
	virtual bool isShadowTechniqueTextureBased(void) const
	{
		return (mShadowTechnique & SHADOWDETAILTYPE_TEXTURE) != 0;
	}
	// FUNCTION: ROBLOX 0x00688610
	virtual bool isShadowTechniqueModulative(void) const
	{
		return (mShadowTechnique & SHADOWDETAILTYPE_MODULATIVE) != 0;
	}
	// FUNCTION: ROBLOX 0x00688620
	virtual bool isShadowTechniqueAdditive(void) const { return (mShadowTechnique & SHADOWDETAILTYPE_ADDITIVE) != 0; }
	// FUNCTION: ROBLOX 0x00688630
	virtual bool isShadowTechniqueIntegrated(void) const
	{
		return (mShadowTechnique & SHADOWDETAILTYPE_INTEGRATED) != 0;
	}
	// FUNCTION: ROBLOX 0x00688640
	virtual bool isShadowTechniqueInUse(void) const { return mShadowTechnique != SHADOWTYPE_NONE; }
	virtual void addShadowListener(ShadowListener* s);
	virtual bool hasStaticGeometry(const String& name) const;
	virtual void destroyStaticGeometry(StaticGeometry* geom);
	virtual void destroyInstancedGeometry(InstancedGeometry* geom);
	virtual void destroyMovableObject(MovableObject* m);
	virtual void injectMovableObject(MovableObject* m);
	virtual void extractMovableObject(MovableObject* m);
	virtual void extractAllMovableObjectsByType(const String& typeName);
	// FUNCTION: ROBLOX 0x00688650
	virtual void setVisibilityMask(uint32 vmask) { mVisibilityMask = vmask; }
	// FUNCTION: ROBLOX 0x00688660
	virtual uint32 getVisibilityMask(void) { return mVisibilityMask; }
	// FUNCTION: ROBLOX 0x00688670
	virtual void setFindVisibleObjects(bool find) { mFindVisibleObjects = find; }
	// FUNCTION: ROBLOX 0x00688680
	virtual bool getFindVisibleObjects(void) { return mFindVisibleObjects; }
	virtual void _injectRenderWithPass(Pass* pass, Renderable* rend, bool shadowDerivation = true);
	virtual void _suppressRenderStateChanges(bool suppress);
	// FUNCTION: ROBLOX 0x00688690
	virtual bool _areRenderStateChangesSuppressed(void) const { return mSuppressRenderStateChanges; }
	virtual void _suppressShadows(bool suppress);
	// FUNCTION: ROBLOX 0x006886a0
	virtual bool _areShadowsSuppressed(void) const { return mSuppressShadows; }

	virtual void initRenderQueue(void);
	virtual void renderVisibleObjectsDefaultSequence(void);
	virtual void renderVisibleObjectsCustomSequence(RenderQueueInvocationSequence* s);
	virtual void prepareRenderQueue(void);
	virtual void renderSingleObject(
		const Renderable* rend, const Pass* pass, bool doLightIteration, const LightList* manualLightList = 0
	);
	virtual void findLightsAffectingFrustum(const Camera* camera);
	virtual void initShadowVolumeMaterials(void);
	virtual void ensureShadowTexturesCreated();
	virtual void destroyShadowTextures(void);
	virtual void prepareShadowTextures(Camera* cam, Viewport* vp);
	virtual void renderShadowVolumesToStencil(const Light* light, const Camera* cam);
	virtual void setShadowVolumeStencilState(bool secondpass, bool zfail, bool twosided);
	virtual const ShadowCasterList& findShadowCastersForLight(const Light* light, const Camera* camera);
	virtual void renderBasicQueueGroupObjects(
		RenderQueueGroup* pGroup, QueuedRenderableCollection::OrganisationMode om
	);
	virtual void renderAdditiveStencilShadowedQueueGroupObjects(
		RenderQueueGroup* group, QueuedRenderableCollection::OrganisationMode om
	);
	virtual void renderModulativeStencilShadowedQueueGroupObjects(
		RenderQueueGroup* group, QueuedRenderableCollection::OrganisationMode om
	);
	virtual void renderTextureShadowCasterQueueGroupObjects(
		RenderQueueGroup* group, QueuedRenderableCollection::OrganisationMode om
	);
	virtual void renderTextureShadowReceiverQueueGroupObjects(
		RenderQueueGroup* group, QueuedRenderableCollection::OrganisationMode om
	);
	virtual void renderModulativeTextureShadowedQueueGroupObjects(
		RenderQueueGroup* group, QueuedRenderableCollection::OrganisationMode om
	);
	virtual void renderAdditiveTextureShadowedQueueGroupObjects(
		RenderQueueGroup* group, QueuedRenderableCollection::OrganisationMode om
	);
	virtual void renderTransparentShadowCasterObjects(
		const QueuedRenderableCollection& objs,
		QueuedRenderableCollection::OrganisationMode om,
		bool doLightIteration,
		const LightList* manualLightList = 0
	);
	virtual void updateRenderQueueSplitOptions(void);
	virtual void updateRenderQueueGroupSplitOptions(
		RenderQueueGroup* group, bool suppressShadows, bool suppressRenderState
	);
	virtual Camera* createCamera(const String& name);
	virtual Camera* getCamera(const String& name) const;
	virtual void destroyCamera(Camera* cam);
	virtual void destroyCamera(const String& name);
	virtual void _populateLightList(const Vector3& position, Real radius, LightList& destList);
	virtual SceneNode* createSceneNode(void);
	virtual SceneNode* createSceneNode(const String& name);
	virtual void destroySceneNode(const String& name);
	virtual SceneNode* getSceneNode(const String& name) const;
	virtual Entity* createEntity(const String& entityName, const String& meshName);
	virtual ParticleSystem* createParticleSystem(const String& name, const String& templateName);
	virtual ParticleSystem* createParticleSystem(
		const String& name,
		size_t quota = 500,
		const String& resourceGroup = ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
	);
	virtual void setWorldGeometry(const String& filename);
	virtual void setWorldGeometry(DataStreamPtr& stream, const String& typeName = StringUtil::BLANK);
	virtual ViewPoint getSuggestedViewpoint(bool random = false);
	virtual void _applySceneAnimations(void);
	virtual void _renderScene(Camera* camera, Viewport* vp, bool includeOverlays);
	virtual void _queueSkiesForRendering(Camera* cam);
	virtual void setSkyPlane(
		bool enable,
		const Plane& plane,
		const String& materialName,
		Real scale = 1000,
		Real tiling = 10,
		bool drawFirst = true,
		Real bow = 0,
		int xsegments = 1,
		int ysegments = 1,
		const String& groupName = ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
	);
	virtual void setSkyBox(
		bool enable,
		const String& materialName,
		Real distance = 5000,
		bool drawFirst = true,
		const Quaternion& orientation = Quaternion::IDENTITY,
		const String& groupName = ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
	);
	virtual void setSkyDome(
		bool enable,
		const String& materialName,
		Real curvature = 10,
		Real tiling = 8,
		Real distance = 4000,
		bool drawFirst = true,
		const Quaternion& orientation = Quaternion::IDENTITY,
		int xsegments = 16,
		int ysegments = 16,
		int ysegments_keep = -1,
		const String& groupName = ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
	);
	virtual BillboardSet* createBillboardSet(const String& name, unsigned int poolSize = 20);
	virtual Animation* createAnimation(const String& name, Real length);
	virtual Animation* getAnimation(const String& name) const;
	virtual bool hasAnimation(const String& name) const;
	virtual void destroyAnimation(const String& name);
	virtual void destroyAllAnimations(void);
	virtual AnimationState* createAnimationState(const String& animName);
	virtual void manualRender(
		RenderOperation* rend,
		Pass* pass,
		Viewport* vp,
		const Matrix4& worldMatrix,
		const Matrix4& viewMatrix,
		const Matrix4& projMatrix,
		bool doBeginEndFrame = false
	);
	virtual void removeRenderQueueListener(RenderQueueListener* delListener);
	virtual bool isRenderQueueToBeProcessed(uint8 qid);
	virtual void setShadowTechnique(ShadowTechnique technique);
	virtual void setShadowColour(const ColourValue& colour);
	virtual void setShadowIndexBufferSize(size_t size);
	virtual void setShadowTextureSize(unsigned short size);
	virtual void setShadowTextureConfig(size_t shadowIndex, const ShadowTextureConfig& config);
	virtual void setShadowTexturePixelFormat(PixelFormat fmt);
	virtual void setShadowTextureCount(size_t count);
	virtual void setShadowTextureSettings(unsigned short size, unsigned short count, PixelFormat fmt = PF_X8R8G8B8);
	virtual const TexturePtr& getShadowTexture(size_t shadowIndex);
	virtual void setShadowTextureCasterMaterial(const String& name);
	virtual void setShadowTextureReceiverMaterial(const String& name);
	virtual void removeShadowListener(ShadowListener* s);
	virtual StaticGeometry* createStaticGeometry(const String& name);
	virtual StaticGeometry* getStaticGeometry(const String& name) const;
	virtual void destroyStaticGeometry(const String& name);
	virtual void destroyAllStaticGeometry(void);
	virtual InstancedGeometry* createInstancedGeometry(const String& name);
	virtual InstancedGeometry* getInstancedGeometry(const String& name) const;
	virtual void destroyInstancedGeometry(const String& name);
	virtual void destroyAllInstancedGeometry(void);
	virtual MovableObject* createMovableObject(
		const String& name, const String& typeName, const NameValuePairList* params = 0
	);
	virtual void destroyMovableObject(const String& name, const String& typeName);
	virtual void destroyAllMovableObjectsByType(const String& typeName);
	virtual void destroyAllMovableObjects(void);
	virtual MovableObject* getMovableObject(const String& name, const String& typeName) const;
	virtual bool hasMovableObject(const String& name, const String& typeName) const;
	virtual void extractMovableObject(const String& name, const String& typeName);
	virtual const Pass* _setPass(const Pass* pass, bool evenIfSuppressed = false, bool shadowDerivation = true);
	virtual void _renderQueueGroupObjects(RenderQueueGroup* group, QueuedRenderableCollection::OrganisationMode om);

	// SYNTHETIC: ROBLOX 0x00698370
	// Ogre::RbxSceneManager::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(RbxSceneManager::SceneMgrQueuedRenderableVisitor, 0x14)
DECOMP_SIZE_ASSERT(RbxSceneManager::ShadowCasterSceneQueryListener, 0x20)
DECOMP_SIZE_ASSERT(RbxSceneManager, 0x8ab0)

// SIZE 0x40
class RbxSceneManagerFactory : public SceneManagerFactory
{
protected:
	void initMetaData() const;

public:
	RbxSceneManagerFactory() {}
	~RbxSceneManagerFactory() {}

	static const String FACTORY_TYPE_NAME;

	SceneManager* createInstance(const String& instanceName);
	void destroyInstance(SceneManager* instance);
};

DECOMP_SIZE_ASSERT(RbxSceneManagerFactory, 0x40)

} // namespace Ogre

#endif // APPDRAW_RBXSCENEMANAGER_H
