#include "appdraw/VisualEngine.h"

#include <OgreRenderTarget.h>
#include <OgreSceneManager.h>
#include <algorithm>
#include <vector>

namespace Ogre {

// GLOBAL: ROBLOX 0x0096134c
Root* g_unk0x0096134c;

// STUB: ROBLOX 0x00671e60
VisualEngine::Unk0x18::Unk0x18() : m_unk0x00(0)
{
}

// FUNCTION: ROBLOX 0x00671eb0
void VisualEngine::Unk0x18::FUN_00671eb0(const Unk0x08& entry)
{
	m_unk0x08[m_unk0x00 % 100] = entry;
	m_unk0x00++;
}

// STUB: ROBLOX 0x00671ef0
VisualEngine::Unk0x18::Unk0x08& VisualEngine::Unk0x18::FUN_00671ef0(int back)
{
	return m_unk0x08[(m_unk0x00 - back - 1) % 100];
}

// FUNCTION: ROBLOX 0x00671f80
VisualEngine::VisualEngine()
	: m_unk0x04(NULL), m_unk0x08(NULL), m_unk0x0c(NULL), m_unk0x10(NULL), m_unk0x14(NULL), m_unk0x7f8(-1)
{
}

VisualEngine::~VisualEngine()
{
	if (g_unk0x0096134c) {
		if (m_unk0x04) {
			g_unk0x0096134c->destroySceneManager(m_unk0x04);
		}
		if (m_unk0x0c) {
			g_unk0x0096134c->detachRenderTarget(m_unk0x0c);
		}
		delete m_unk0x14;
	}
}

// FUNCTION: ROBLOX 0x00672010
void FUN_00672010()
{
	if (g_unk0x0096134c) {
		delete g_unk0x0096134c;
		g_unk0x0096134c = NULL;
	}
}

// FUNCTION: ROBLOX 0x00672980
float VisualEngine::Unk0x18::FUN_00672980(int count)
{
	if (count <= 99 && m_unk0x00 >= count) {
		std::vector<float> values(count, 0.0f);
		values.clear();
		for (int i = 1; i <= count; i++) {
			values.push_back(m_unk0x08[(m_unk0x00 - i) % 100].m_unk0x00);
		}
		std::sort(values.begin(), values.end());
		return values[count / 2];
	}
	return -1.0f;
}

} // namespace Ogre
