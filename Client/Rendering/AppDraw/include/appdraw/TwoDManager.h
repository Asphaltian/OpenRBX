#ifndef APPDRAW_TWODMANAGER_H
#define APPDRAW_TWODMANAGER_H

#include "placeholder.h"

#include <G3D/Color4.h>
#include <OgreHardwareIndexBuffer.h>
#include <OgreHardwareVertexBuffer.h>
#include <OgreRenderOperation.h>
#include <OgreRenderQueueListener.h>
#include <list>
#include <string>

namespace Ogre {

class TwoDManager : public RenderQueueListener
{
public:
	struct Unk0x30
	{
		float m_unk0x00;
		float m_unk0x04;
		float m_unk0x08;
		float m_unk0x0c;
		float m_unk0x10;
		float m_unk0x14;
		float m_unk0x18;
		float m_unk0x1c;
		uint32 m_unk0x20;
		int m_unk0x24;
		int m_unk0x28;
		int m_unk0x2c;
	};

	struct Unk0x10
	{
		uint32 m_unk0x00;
		int m_unk0x04;
		int m_unk0x08;
		int m_unk0x0c;
	};

	virtual ~TwoDManager();
	TwoDManager();

	void FUN_0067c6c0(
		SceneManager* sceneManager, RenderTarget* target, uint8 queueGroupId, bool p_unk0x09, bool p_unk0x0a
	);
	void FUN_0067c710();
	void FUN_0067cb20(size_t numVertices);
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

	virtual void renderQueueStarted(uint8 queueGroupId, const String& invocation, bool& skipThisInvocation);
	virtual void renderQueueEnded(uint8 queueGroupId, const String& invocation, bool& repeatThisInvocation);

private:
	SceneManager* m_unk0x04;
	uint8 m_unk0x08;
	bool m_unk0x09;
	bool m_unk0x0a;
	RenderTarget* m_unk0x0c;
	RenderOperation m_unk0x10;
	HardwareVertexBufferSharedPtr m_unk0x24;
	HardwareIndexBufferSharedPtr m_unk0x30;
	std::list<Unk0x30> m_unk0x3c;
};

} // namespace Ogre

#endif // APPDRAW_TWODMANAGER_H
