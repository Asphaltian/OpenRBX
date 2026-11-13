#ifndef APPDRAW_RBXENTITY_H
#define APPDRAW_RBXENTITY_H

#include "appdraw/VisualEngine.h"
#include "decomp.h"

#include <OgreAxisAlignedBox.h>
#include <OgreEntity.h>
#include <vector>

namespace RBX {
namespace RenderNew {
class RenderScene;
} // namespace RenderNew
} // namespace RBX

namespace Ogre {

class RbxSubEntity;

// VTABLE: ROBLOX 0x0083dacc Ogre::ShadowCaster
// VTABLE: ROBLOX 0x0083dab8 Ogre::AnimableObject
// VTABLE: ROBLOX 0x0083daac
// SIZE 0x280
class RbxEntity : public Entity
{
public:
	// SIZE 0x08
	struct Unknown00676870
	{
		bool operator<(const Unknown00676870& other) const { return m_unk0x04 < other.m_unk0x04; }

		int m_unk0x00;  // 0x00
		Real m_unk0x04; // 0x04
	};

	RbxEntity();
	virtual ~RbxEntity();

	virtual void _notifyCurrentCamera(Camera* cam);
	virtual void _updateRenderQueue(RenderQueue* queue);

	// FUNCTION: ROBLOX 0x0052c8b0
	void FUN_0052c8b0(AxisAlignedBox box) { m_unk0x248 = box; }

	void FUN_00674e30(VisualEngine::Unk0x18* p_unk0x240);
	void FUN_00674e40(MeshPtr mesh);
	void FUN_00675a70(RbxSubEntity* subEntity);
	void FUN_00675c90();
	RbxSubEntity* FUN_00676680(SubMesh* subMesh, int lodIndex);
	void FUN_00676730(RbxSubEntity* subEntity);
	void FUN_006772d0(Camera* camera, const Light* light, std::vector<ShadowCaster*>* casters);

	virtual void FUN_00676870(Camera* cam);
	virtual void FUN_00674fd0(bool visible);

	// SYNTHETIC: ROBLOX 0x00675f10
	// Ogre::RbxEntity::`scalar deleting destructor'

private:
	friend class RBX::AdornRbxGfx;
	friend class RBX::RenderNew::RenderScene;

	VisualEngine::Unk0x18* m_unk0x240;     // 0x240
	bool m_unk0x244;                       // 0x244
	bool m_unk0x245;                       // 0x245
	bool m_unk0x246;                       // 0x246
	AxisAlignedBox m_unk0x248;             // 0x248
	Real m_unk0x268;                       // 0x268
	int m_unk0x26c;                        // 0x26c
	std::vector<unsigned char> m_unk0x270; // 0x270
};

DECOMP_SIZE_ASSERT(Entity, 0x240)
DECOMP_SIZE_ASSERT(RbxEntity, 0x280)

class RbxEntityFactory : public MovableObjectFactory
{
public:
	static String FACTORY_TYPE_NAME;
};

} // namespace Ogre

#endif // APPDRAW_RBXENTITY_H
