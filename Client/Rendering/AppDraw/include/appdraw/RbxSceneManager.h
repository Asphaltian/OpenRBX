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

	bool validatePassForRendering(const Pass* pass);
	bool validateRenderableForRendering(const Pass* pass, const Renderable* rend);

public:
	RbxSceneManager(const String& name);
	~RbxSceneManager();

	const String& getTypeName() const;

	void clearScene();
	void destroyAllCameras();

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
