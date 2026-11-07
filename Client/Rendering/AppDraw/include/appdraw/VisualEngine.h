#ifndef APPDRAW_VISUALENGINE_H
#define APPDRAW_VISUALENGINE_H

#include "decomp.h"

#include <OgreRenderQueueListener.h>
#include <OgreRoot.h>

namespace Ogre {

// SIZE 0x48
class TwoDManager : public RenderQueueListener
{
private:
	undefined m_unk0x04[0x48 - 0x04]; // 0x04
};

// SIZE 0x808
class VisualEngine
{
public:
	// SIZE 0x7d8
	class Unk0x18
	{
	public:
		// SIZE 0x14
		struct Unk0x08
		{
			Unk0x08() : m_unk0x00(-1.0f), m_unk0x04(-1.0f), m_unk0x08(-1.0f), m_unk0x0c(-1.0f), m_unk0x10(-1.0f) {}

			float m_unk0x00; // 0x00
			float m_unk0x04; // 0x04
			float m_unk0x08; // 0x08
			float m_unk0x0c; // 0x0c
			float m_unk0x10; // 0x10
		};

		Unk0x18();

		void FUN_00671eb0(const Unk0x08& entry);
		Unk0x08& FUN_00671ef0(int back);
		float FUN_00672980(int count);

	private:
		int m_unk0x00;          // 0x00
		undefined4 m_unk0x04;   // 0x04
		Unk0x08 m_unk0x08[100]; // 0x08
	};

	VisualEngine();
	virtual ~VisualEngine(); // vtable+0x00

	Viewport* getViewport() const { return m_unk0x10; }

	// SYNTHETIC: ROBLOX 0x00671fb0
	// Ogre::VisualEngine::`scalar deleting destructor'

private:
	SceneManager* m_unk0x04;             // 0x04
	Camera* m_unk0x08;                   // 0x08
	RenderTarget* m_unk0x0c;             // 0x0c
	Viewport* m_unk0x10;                 // 0x10
	TwoDManager* m_unk0x14;              // 0x14
	Unk0x18 m_unk0x18;                   // 0x18
	unsigned int m_unk0x7f0;             // 0x7f0
	undefined4 m_unk0x7f4;               // 0x7f4
	int m_unk0x7f8;                      // 0x7f8
	undefined m_unk0x7fc[0x808 - 0x7fc]; // 0x7fc
};

DECOMP_SIZE_ASSERT(VisualEngine::Unk0x18::Unk0x08, 0x14)
DECOMP_SIZE_ASSERT(VisualEngine::Unk0x18, 0x7d8)
DECOMP_SIZE_ASSERT(VisualEngine, 0x808)

extern Root* g_unk0x0096134c;

void FUN_00672010();

} // namespace Ogre

#endif // APPDRAW_VISUALENGINE_H
