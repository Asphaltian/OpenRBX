#ifndef APPDRAW_RBXSCENEMANAGER_H
#define APPDRAW_RBXSCENEMANAGER_H

#include "appdraw/RbxSky.h"
#include "placeholder.h"

#include <OgreSceneManager.h>
#include <OgreSceneManagerEnumerator.h>

namespace RBX {
namespace ViewNew {
class View;
} // namespace ViewNew
} // namespace RBX

namespace Ogre {

class RbxSceneManager : public SceneManager
{
public:
	class SceneMgrQueuedRenderableVisitor : public QueuedRenderableVisitor
	{
	protected:
		const Pass* mUsedPass;

	public:
		SceneMgrQueuedRenderableVisitor() : transparentShadowCastersMode(false) {}
		~SceneMgrQueuedRenderableVisitor() {}

		void visit(const Renderable* r);
		bool visit(const Pass* p);
		void visit(const RenderablePass* rp);

		RbxSceneManager* targetSceneMgr;
		bool transparentShadowCastersMode;
		bool autoLights;
		const LightList* manualLightList;
	};

	class ShadowCasterSceneQueryListener : public SceneQueryListener
	{
	protected:
		SceneManager* mSceneMgr;
		ShadowCasterList* mCasterList;
		bool mIsLightInFrustum;
		const PlaneBoundedVolumeList* mLightClipVolumeList;
		const Camera* mCamera;
		const Light* mLight;
		Real mFarDistSquared;

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
	};

protected:
	struct lightsForShadowTextureLess
	{
		bool operator()(const Light* l1, const Light* l2) const;
	};

	bool m_unk0x8a64;
	bool m_unk0x8a65;
	String m_unk0x8a68;
	SharedPtr<RbxSky> m_unk0x8a84;
	SceneNode* m_unk0x8a90;
	ShadowCasterSceneQueryListener* mShadowCasterQueryListener;
	SceneMgrQueuedRenderableVisitor* mActiveQueuedRenderableVisitor;
	SceneMgrQueuedRenderableVisitor mDefaultQueuedRenderableVisitor;

	friend class SceneMgrQueuedRenderableVisitor;
	friend class RBX::ViewNew::View;

	MeshPtr createSkyboxPlane(BoxPlane bp, Real distance, const Quaternion& orientation, const String& groupName);
	MeshPtr createSkydomePlane(
		BoxPlane bp,
		Real curvature,
		Real tiling,
		Real distance,
		const Quaternion& orientation,
		int xsegments,
		int ysegments,
		int ySegmentsToKeep,
		const String& groupName
	);
	bool fireRenderQueueStarted(uint8 id, const String& invocation);
	bool fireRenderQueueEnded(uint8 id, const String& invocation);
	void fireShadowTexturesUpdated(size_t numberOfShadowTextures);
	void fireShadowTexturesPreCaster(Light* light, Camera* camera);
	void fireShadowTexturesPreReceiver(Light* light, Frustum* f);
	void useRenderableViewProjMode(const Renderable* pRend);
	void resetViewProjMode(void);
	const Pass* deriveShadowCasterPass(const Pass* pass);
	const Pass* deriveShadowReceiverPass(const Pass* pass);
	void renderShadowVolumeObjects(
		ShadowCaster::ShadowRenderableListIterator iShadowRenderables,
		Pass* pass,
		const LightList* manualLightList,
		unsigned long flags,
		bool secondpass,
		bool zfail,
		bool twosided
	);
	const MovableObjectCollection* getMovableObjectCollection(const String& typeName) const;
	MovableObjectCollection* getMovableObjectCollection(const String& typeName);

	bool validatePassForRendering(const Pass* pass);
	bool validateRenderableForRendering(const Pass* pass, const Renderable* rend);

public:
	enum PrefabType
	{
		PT_PLANE,
		PT_CUBE,
		PT_SPHERE
	};

	using SceneManager::createEntity;

	const ShadowCasterList& getShadowCasterList() const { return mShadowCasterList; }

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
	virtual Entity* createEntity(const String& entityName, PrefabType ptype);
	virtual SceneNode* getRootSceneNode(void) const;
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
	virtual bool isSkyPlaneEnabled(void) const { return mSkyPlaneEnabled; }
	virtual SceneNode* getSkyPlaneNode(void) const { return mSkyPlaneNode; }
	virtual const SkyPlaneGenParameters& getSkyPlaneGenParameters(void) const { return mSkyPlaneGenParameters; }
	virtual bool isSkyBoxEnabled(void) const { return mSkyBoxEnabled; }
	virtual SceneNode* getSkyBoxNode(void) const { return mSkyBoxNode; }
	virtual const SkyBoxGenParameters& getSkyBoxGenParameters(void) const { return mSkyBoxGenParameters; }
	virtual bool isSkyDomeEnabled(void) const { return mSkyDomeEnabled; }
	virtual SceneNode* getSkyDomeNode(void) const { return mSkyDomeNode; }
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
	virtual AxisAlignedBoxSceneQuery* createAABBQuery(const AxisAlignedBox& box, unsigned long mask = 0xFFFFFFFF);
	virtual SphereSceneQuery* createSphereQuery(const Sphere& sphere, unsigned long mask = 0xFFFFFFFF);
	virtual PlaneBoundedVolumeListSceneQuery* createPlaneBoundedVolumeQuery(
		const PlaneBoundedVolumeList& volumes, unsigned long mask = 0xFFFFFFFF
	);
	virtual RaySceneQuery* createRayQuery(const Ray& ray, unsigned long mask = 0xFFFFFFFF);
	virtual IntersectionSceneQuery* createIntersectionQuery(unsigned long mask = 0xFFFFFFFF);
	virtual void destroyQuery(SceneQuery* query);
	virtual ShadowTechnique getShadowTechnique(void) const { return mShadowTechnique; }
	virtual void setShowDebugShadows(bool debug) { mDebugShadows = debug; }
	virtual bool getShowDebugShadows(void) const { return mDebugShadows; }
	virtual const ColourValue& getShadowColour(void) const;
	virtual void setShadowDirectionalLightExtrusionDistance(Real dist);
	virtual Real getShadowDirectionalLightExtrusionDistance(void) const;
	virtual void setShadowFarDistance(Real distance);
	virtual Real getShadowFarDistance(void) const { return mShadowFarDist; }
	virtual size_t getShadowIndexBufferSize(void) const { return mShadowIndexBufferSize; }
	virtual void setShadowTextureConfig(
		size_t shadowIndex, unsigned short width, unsigned short height, PixelFormat format
	);
	virtual void setShadowDirLightTextureOffset(Real offset) { mShadowTextureOffset = offset; }
	virtual Real getShadowDirLightTextureOffset(void) const { return mShadowTextureOffset; }
	virtual void setShadowTextureFadeStart(Real fadeStart) { mShadowTextureFadeStart = fadeStart; }
	virtual void setShadowTextureFadeEnd(Real fadeEnd) { mShadowTextureFadeEnd = fadeEnd; }
	virtual void setShadowTextureSelfShadow(bool selfShadow);
	virtual bool getShadowTextureSelfShadow(void) const { return mShadowTextureSelfShadow; }
	virtual void setShadowCasterRenderBackFaces(bool bf) { mShadowCasterRenderBackFaces = bf; }
	virtual bool getShadowCasterRenderBackFaces() const { return mShadowCasterRenderBackFaces; }
	virtual const ShadowCameraSetupPtr& getShadowCameraSetup() const;
	virtual void setShadowUseInfiniteFarPlane(bool enable) { mShadowUseInfiniteFarPlane = enable; }
	virtual bool isShadowTechniqueStencilBased(void) const
	{
		return (mShadowTechnique & SHADOWDETAILTYPE_STENCIL) != 0;
	}
	virtual bool isShadowTechniqueTextureBased(void) const
	{
		return (mShadowTechnique & SHADOWDETAILTYPE_TEXTURE) != 0;
	}
	virtual bool isShadowTechniqueModulative(void) const
	{
		return (mShadowTechnique & SHADOWDETAILTYPE_MODULATIVE) != 0;
	}
	virtual bool isShadowTechniqueAdditive(void) const { return (mShadowTechnique & SHADOWDETAILTYPE_ADDITIVE) != 0; }
	virtual bool isShadowTechniqueIntegrated(void) const
	{
		return (mShadowTechnique & SHADOWDETAILTYPE_INTEGRATED) != 0;
	}
	virtual bool isShadowTechniqueInUse(void) const { return mShadowTechnique != SHADOWTYPE_NONE; }
	virtual void addShadowListener(ShadowListener* s);
	virtual bool hasStaticGeometry(const String& name) const;
	virtual void destroyStaticGeometry(StaticGeometry* geom);
	virtual void destroyInstancedGeometry(InstancedGeometry* geom);
	virtual void destroyMovableObject(MovableObject* m);
	virtual void injectMovableObject(MovableObject* m);
	virtual void extractMovableObject(MovableObject* m);
	virtual void extractAllMovableObjectsByType(const String& typeName);
	virtual void setVisibilityMask(uint32 vmask) { mVisibilityMask = vmask; }
	virtual uint32 getVisibilityMask(void) { return mVisibilityMask; }
	virtual void setFindVisibleObjects(bool find) { mFindVisibleObjects = find; }
	virtual bool getFindVisibleObjects(void) { return mFindVisibleObjects; }
	virtual void _injectRenderWithPass(Pass* pass, Renderable* rend, bool shadowDerivation = true);
	virtual void _suppressRenderStateChanges(bool suppress);
	virtual bool _areRenderStateChangesSuppressed(void) const { return mSuppressRenderStateChanges; }
	virtual void _suppressShadows(bool suppress);
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
	virtual size_t estimateWorldGeometry(const String& filename) { return 0; }
	virtual size_t estimateWorldGeometry(DataStreamPtr& stream, const String& typeName = StringUtil::BLANK)
	{
		return 0;
	}
	virtual bool setOption(const String& strKey, const void* pValue) { return false; }
	virtual bool getOption(const String& strKey, void* pDestValue) { return false; }
	virtual bool hasOption(const String& strKey) const { return false; }
	virtual bool getOptionValues(const String& strKey, StringVector& refValueList) { return false; }
	virtual bool getOptionKeys(StringVector& refKeys) { return false; }
	virtual void _setDestinationRenderSystem(RenderSystem* sys);
	virtual FogMode getFogMode(void) const;
	virtual void setShadowCameraSetup(const ShadowCameraSetupPtr& shadowSetup);
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
	size_t getShadowTextureCount(void) const { return mShadowTextureConfigList.size(); }
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
	virtual MovableObjectIterator getMovableObjectIterator(const String& typeName);
	virtual const Pass* _setPass(const Pass* pass, bool evenIfSuppressed = false, bool shadowDerivation = true);
	virtual void _renderQueueGroupObjects(RenderQueueGroup* group, QueuedRenderableCollection::OrganisationMode om);
};

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

} // namespace Ogre

#endif // APPDRAW_RBXSCENEMANAGER_H
