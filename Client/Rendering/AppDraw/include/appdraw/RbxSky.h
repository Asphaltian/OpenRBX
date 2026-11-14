#ifndef APPDRAW_RBXSKY_H
#define APPDRAW_RBXSKY_H

#include "placeholder.h"

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

class RbxSky
{
public:
	RbxSky(SceneManager* sceneManager, const String& folder, bool p_unk0x0c);
	virtual ~RbxSky();

	void FUN_00677eb0(VisualEngine* engine, const G3D::LightingParameters& parameters);
	void FUN_00678210(VisualEngine* engine, const G3D::LightingParameters& parameters);
	void FUN_00678540(VisualEngine* engine, const G3D::LightingParameters& parameters);
	void FUN_00678610(Camera* cam, RenderQueue* queue, uint8 queueGroupId);

private:
	TexturePtr m_unk0x04;
	TexturePtr m_unk0x10;
	TexturePtr m_unk0x1c;
	TexturePtr m_unk0x28;
	BillboardSet* m_unk0x34;
	BillboardSet* m_unk0x38;
	BillboardSet* m_unk0x3c;
	SceneNode* m_unk0x40;
	SceneNode* m_unk0x44;
	SceneNode* m_unk0x48;
	SceneNode* m_unk0x4c;
	undefined m_unk0x50[0x54 - 0x50];
	MaterialPtr m_unk0x54;
	MaterialPtr m_unk0x60;
	MaterialPtr m_unk0x6c;
	MaterialPtr m_unk0x78;
	bool m_unk0x84;
	bool m_unk0x85;
	std::vector<Vector3> m_unk0x88;
	std::vector<Real> m_unk0x98;
	bool m_unk0xa8;
	Vector3 m_unk0xac;
	Vector3 m_unk0xb8;
};

} // namespace Ogre

#endif // APPDRAW_RBXSKY_H
