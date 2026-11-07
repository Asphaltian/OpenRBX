#include "appdraw/TwoDManager.h"

#include <OgreRenderSystem.h>
#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreTextureManager.h>

namespace Ogre {

// FUNCTION: ROBLOX 0x0067c6c0
void TwoDManager::FUN_0067c6c0(
	SceneManager* sceneManager, RenderTarget* target, uint8 queueGroupId, bool p_unk0x09, bool p_unk0x0a
)
{
	m_unk0x09 = p_unk0x09;
	m_unk0x08 = queueGroupId;
	m_unk0x04 = sceneManager;
	m_unk0x0c = target;
	m_unk0x0a = p_unk0x0a;
	m_unk0x24.setNull();
	sceneManager->addRenderQueueListener(this);
}

// FUNCTION: ROBLOX 0x0067cda0
TwoDManager::~TwoDManager()
{
}

// FUNCTION: ROBLOX 0x0067ce10
TwoDManager::TwoDManager()
{
}

// STUB: ROBLOX 0x0067cec0
void TwoDManager::FUN_0067cec0()
{
	STUB(0x67cec0);
}

// FUNCTION: ROBLOX 0x0067d9e0
void TwoDManager::FUN_0067d9e0(
	std::string texture,
	const G3D::Color4& color,
	float x0,
	float y0,
	float x1,
	float y1,
	float u0,
	float v0,
	float u1,
	float v1
)
{
	TexturePtr tex;
	Unk0x30 item;
	item.m_unk0x00 = x0;
	item.m_unk0x04 = y0;
	item.m_unk0x08 = x1;
	item.m_unk0x0c = y1;
	item.m_unk0x10 = u0;
	item.m_unk0x14 = v0;
	item.m_unk0x18 = u1;
	item.m_unk0x1c = v1;
	RenderSystem* renderSystem = Root::getSingleton().getRenderSystem();
	renderSystem->convertColourValue(ColourValue(color.r, color.g, color.b, color.a), &item.m_unk0x20);
	item.m_unk0x28 = 4;
	item.m_unk0x2c = 0;
	if (texture != "") {
		tex = TextureManager::getSingleton().getByName(texture);
		if (tex.isNull()) {
			tex = TextureManager::getSingleton().load(texture, "General");
		}
		if (tex.isNull()) {
			item.m_unk0x24 = 0;
		}
		else {
			item.m_unk0x24 = tex->getHandle();
			if (x0 == x1 && y0 == y1) {
				x1 = x0 + tex->getWidth();
				y1 = y0 + tex->getHeight();
			}
		}
	}
	else {
		item.m_unk0x24 = 0;
	}
	m_unk0x3c.push_back(item);
}

// FUNCTION: ROBLOX 0x0067dcc0
void TwoDManager::FUN_0067dcc0(int type, float x0, float y0, float x1, float y1, const G3D::Color4& color)
{
	Unk0x30 item;
	item.m_unk0x00 = x0;
	item.m_unk0x04 = y0;
	item.m_unk0x08 = x1;
	item.m_unk0x0c = y1;
	Root::getSingleton().getRenderSystem()->convertColourValue(
		ColourValue(color.r, color.g, color.b, color.a), &item.m_unk0x20
	);
	item.m_unk0x24 = 0;
	item.m_unk0x28 = type;
	item.m_unk0x2c = 0;
	m_unk0x3c.push_back(item);
}

// FUNCTION: ROBLOX 0x0067dd70
void TwoDManager::FUN_0067dd70(float x0, float y0, float x1, float y1, const G3D::Color4& color)
{
	Unk0x30 item;
	item.m_unk0x00 = x0;
	item.m_unk0x04 = y0;
	item.m_unk0x08 = x1;
	item.m_unk0x0c = y1;
	Root::getSingleton().getRenderSystem()->convertColourValue(
		ColourValue(color.r, color.g, color.b, color.a), &item.m_unk0x20
	);
	item.m_unk0x24 = 0;
	item.m_unk0x28 = 2;
	item.m_unk0x2c = 1;
	m_unk0x3c.push_back(item);
}

// STUB: ROBLOX 0x0067de30
void TwoDManager::renderQueueStarted(uint8 queueGroupId, const String& invocation, bool& skipThisInvocation)
{
	if (!m_unk0x09 && queueGroupId == m_unk0x08) {
		FUN_0067cec0();
	}
}

// STUB: ROBLOX 0x0067de50
void TwoDManager::renderQueueEnded(uint8 queueGroupId, const String& invocation, bool& repeatThisInvocation)
{
	if (m_unk0x09 && queueGroupId == m_unk0x08) {
		FUN_0067cec0();
	}
}

} // namespace Ogre
