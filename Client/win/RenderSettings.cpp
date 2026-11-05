#include "RenderSettings.h"

#include "renderlib/AggregatingSceneManager.h"

#include <GLG3D/DXCaps.h>

const char sRenderSettings[] = "RenderSettings";

undefined4 g_unk0x0091a2e8 = 1;
G3D::Vector2int16 g_unk0x0091a2ec(800, 600);
undefined4 g_unk0x00953ae0;

static G3D::Vector2int16 defaultFullscreenSize()
{
	if (static_cast<unsigned int>(G3D::DXCaps::videoMemorySize()) >= 16000000) {
		return G3D::Vector2int16(1024, 768);
	}
	return G3D::Vector2int16(800, 600);
}

// FUNCTION: ROBLOX 0x00448290
float CRenderSettings::FUN_00448290() const
{
	return m_unk0x10c;
}

// FUNCTION: ROBLOX 0x004482a0
float CRenderSettings::FUN_004482a0() const
{
	return m_unk0x110;
}

// FUNCTION: ROBLOX 0x004482b0
float CRenderSettings::FUN_004482b0() const
{
	return m_unk0x114;
}

// FUNCTION: ROBLOX 0x004482c0
float CRenderSettings::FUN_004482c0() const
{
	return m_unk0x118;
}

// FUNCTION: ROBLOX 0x004482d0
undefined4 CRenderSettings::FUN_004482d0() const
{
	return m_unk0xfc;
}

// FUNCTION: ROBLOX 0x004482e0
undefined4 CRenderSettings::FUN_004482e0() const
{
	return m_unk0x100;
}

// FUNCTION: ROBLOX 0x004482f0
undefined4 CRenderSettings::FUN_004482f0() const
{
	return g_unk0x0091a2e8;
}

// FUNCTION: ROBLOX 0x00448300
G3D::Vector2int16 CRenderSettings::FUN_00448300() const
{
	return m_unk0x13c;
}

// FUNCTION: ROBLOX 0x00448310
G3D::Vector2int16 CRenderSettings::FUN_00448310() const
{
	return m_unk0x140;
}

// FUNCTION: ROBLOX 0x00448320
unsigned int CRenderSettings::FUN_00448320() const
{
	return RBX::Render::AggregatingSceneManager::aggregationSize;
}

// STUB: ROBLOX 0x0044cb20
CRenderSettings::CRenderSettings()
	: m_unk0xf8(1), m_unk0xfc(0), m_unk0x100(2), m_unk0x104(0), m_unk0x108(0), m_unk0x10c(20.0f), m_unk0x110(20.0f),
	  m_unk0x114(50.0f), m_unk0x118(50.0f), m_unk0x11c(1), m_unk0x13c(defaultFullscreenSize()),
	  m_unk0x140(g_unk0x0091a2ec)
{
	setName("Rendering");
	RBX::Render::AggregatingSceneManager::aggregationSize = 1;
}

// STUB: ROBLOX 0x0044d0b0
void CRenderSettings::FUN_0044d0b0(undefined4 p_unk0x04)
{
	STUB(0x44d0b0);
}

// FUNCTION: ROBLOX 0x0044d430
undefined4 CRenderSettings::FUN_0044d430()
{
	if (g_unk0x00953ae0 == 0) {
		g_unk0x00953ae0 = m_unk0xf8;
	}
	return g_unk0x00953ae0;
}

template class RBX::FactoryProduct<CRenderSettings, RBX::GlobalSettings::Item, sRenderSettings>;
template class RBX::Reflection::Described<
	CRenderSettings,
	sRenderSettings,
	RBX::FactoryProduct<CRenderSettings, RBX::GlobalSettings::Item, sRenderSettings> >;
template class RBX::DescribedCreatable<CRenderSettings, RBX::GlobalSettings::Item, sRenderSettings>;
