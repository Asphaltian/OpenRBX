#ifndef APPDRAW_TWODMANAGER_H
#define APPDRAW_TWODMANAGER_H

#include "decomp.h"

#include <G3D/Color4.h>
#include <OgreHardwareIndexBuffer.h>
#include <OgreHardwareVertexBuffer.h>
#include <OgreRenderOperation.h>
#include <OgreRenderQueueListener.h>
#include <list>
#include <string>

namespace Ogre {

// VTABLE: ROBLOX 0x0083dfbc
// SIZE 0x48
class TwoDManager : public RenderQueueListener
{
public:
	// SIZE 0x30
	struct Unk0x30
	{
		float m_unk0x00;  // 0x00
		float m_unk0x04;  // 0x04
		float m_unk0x08;  // 0x08
		float m_unk0x0c;  // 0x0c
		float m_unk0x10;  // 0x10
		float m_unk0x14;  // 0x14
		float m_unk0x18;  // 0x18
		float m_unk0x1c;  // 0x1c
		uint32 m_unk0x20; // 0x20
		int m_unk0x24;    // 0x24
		int m_unk0x28;    // 0x28
		int m_unk0x2c;    // 0x2c
	};

	virtual ~TwoDManager(); // vtable+0x00
	TwoDManager();

	void FUN_0067cec0();
	void FUN_0067d9e0(
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
	);
	void FUN_0067dcc0(int type, float x0, float y0, float x1, float y1, const G3D::Color4& color);
	void FUN_0067dd70(float x0, float y0, float x1, float y1, const G3D::Color4& color);

	virtual void renderQueueStarted(
		uint8 queueGroupId,
		const String& invocation,
		bool& skipThisInvocation
	); // vtable+0x04
	virtual void renderQueueEnded(
		uint8 queueGroupId,
		const String& invocation,
		bool& repeatThisInvocation
	); // vtable+0x08

	// SYNTHETIC: ROBLOX 0x0067cea0
	// Ogre::TwoDManager::`scalar deleting destructor'

private:
	undefined4 m_unk0x04;                    // 0x04
	uint8 m_unk0x08;                         // 0x08
	bool m_unk0x09;                          // 0x09
	undefined4 m_unk0x0c;                    // 0x0c
	RenderOperation m_unk0x10;               // 0x10
	HardwareVertexBufferSharedPtr m_unk0x24; // 0x24
	HardwareIndexBufferSharedPtr m_unk0x30;  // 0x30
	std::list<Unk0x30> m_unk0x3c;            // 0x3c
};

DECOMP_SIZE_ASSERT(TwoDManager::Unk0x30, 0x30)
DECOMP_SIZE_ASSERT(TwoDManager, 0x48)

} // namespace Ogre

#endif // APPDRAW_TWODMANAGER_H
