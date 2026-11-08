#ifndef APPDRAW_RBXSKY_H
#define APPDRAW_RBXSKY_H

#include "decomp.h"

#include <OgreMaterial.h>
#include <OgreString.h>
#include <OgreTexture.h>
#include <OgreVector3.h>
#include <vector>

namespace G3D {
class LightingParameters;
} // namespace G3D

namespace Ogre {

class BillboardSet;
class Camera;
class RenderQueue;
class SceneManager;
class SceneNode;
class VisualEngine;

// VTABLE: ROBLOX 0x0083dbfc
// SIZE 0xc4
class RbxSky
{
public:
	RbxSky(SceneManager* sceneManager, const String& folder, bool p_unk0x0c);
	virtual ~RbxSky(); // vtable+0x00

	void FUN_00677eb0(VisualEngine* engine, const G3D::LightingParameters& parameters);
	void FUN_00678210(VisualEngine* engine, const G3D::LightingParameters& parameters);
	void FUN_00678540(VisualEngine* engine, const G3D::LightingParameters& parameters);
	void FUN_00678610(Camera* cam, RenderQueue* queue, uint8 queueGroupId);

	// SYNTHETIC: ROBLOX 0x00678d10
	// Ogre::RbxSky::`scalar deleting destructor'

private:
	TexturePtr m_unk0x04;             // 0x04
	TexturePtr m_unk0x10;             // 0x10
	TexturePtr m_unk0x1c;             // 0x1c
	TexturePtr m_unk0x28;             // 0x28
	BillboardSet* m_unk0x34;          // 0x34
	BillboardSet* m_unk0x38;          // 0x38
	BillboardSet* m_unk0x3c;          // 0x3c
	SceneNode* m_unk0x40;             // 0x40
	SceneNode* m_unk0x44;             // 0x44
	SceneNode* m_unk0x48;             // 0x48
	SceneNode* m_unk0x4c;             // 0x4c
	undefined m_unk0x50[0x54 - 0x50]; // 0x50
	MaterialPtr m_unk0x54;            // 0x54
	MaterialPtr m_unk0x60;            // 0x60
	MaterialPtr m_unk0x6c;            // 0x6c
	MaterialPtr m_unk0x78;            // 0x78
	bool m_unk0x84;                   // 0x84
	bool m_unk0x85;                   // 0x85
	std::vector<Vector3> m_unk0x88;   // 0x88
	std::vector<Real> m_unk0x98;      // 0x98
	bool m_unk0xa8;                   // 0xa8
	Vector3 m_unk0xac;                // 0xac
	Vector3 m_unk0xb8;                // 0xb8
};

DECOMP_SIZE_ASSERT(RbxSky, 0xc4)

} // namespace Ogre

#endif // APPDRAW_RBXSKY_H
