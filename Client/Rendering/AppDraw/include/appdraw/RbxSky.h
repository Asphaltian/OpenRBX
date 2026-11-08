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

class SceneManager;
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

	// SYNTHETIC: ROBLOX 0x00678d10
	// Ogre::RbxSky::`scalar deleting destructor'

private:
	TexturePtr m_unk0x04;             // 0x04
	TexturePtr m_unk0x10;             // 0x10
	TexturePtr m_unk0x1c;             // 0x1c
	TexturePtr m_unk0x28;             // 0x28
	undefined m_unk0x34[0x54 - 0x34]; // 0x34
	MaterialPtr m_unk0x54;            // 0x54
	MaterialPtr m_unk0x60;            // 0x60
	MaterialPtr m_unk0x6c;            // 0x6c
	MaterialPtr m_unk0x78;            // 0x78
	undefined4 m_unk0x84;             // 0x84
	std::vector<Vector3> m_unk0x88;   // 0x88
	std::vector<Real> m_unk0x98;      // 0x98
	bool m_unk0xa8;                   // 0xa8
	undefined m_unk0xa9[0xc4 - 0xa9]; // 0xa9
};

DECOMP_SIZE_ASSERT(RbxSky, 0xc4)

} // namespace Ogre

#endif // APPDRAW_RBXSKY_H
