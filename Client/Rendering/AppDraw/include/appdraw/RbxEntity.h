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
	RbxEntity();
	virtual ~RbxEntity();

	virtual void _notifyCurrentCamera(Camera* cam);
	virtual void _updateRenderQueue(RenderQueue* queue);

	void FUN_00674e30(VisualEngine::Unk0x18* p_unk0x240);
	void FUN_00674e40(MeshPtr mesh);
	RbxSubEntity* FUN_00676680(SubMesh* subMesh, int lodIndex);

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

} // namespace Ogre

#endif // APPDRAW_RBXENTITY_H
