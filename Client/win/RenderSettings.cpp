#include "RenderSettings.h"

#include "renderlib/AggregatingSceneManager.h"

#include <GLG3D/DXCaps.h>

const char sRenderSettings[] = "RenderSettings";

G3D::Vector2int16 g_unk0x0091a2ec(800, 600);

static G3D::Vector2int16 defaultFullscreenSize()
{
	if (static_cast<unsigned int>(G3D::DXCaps::videoMemorySize()) >= 16000000) {
		return G3D::Vector2int16(1024, 768);
	}
	return G3D::Vector2int16(800, 600);
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

template class RBX::FactoryProduct<CRenderSettings, RBX::GlobalSettings::Item, sRenderSettings>;
template class RBX::Reflection::Described<
	CRenderSettings,
	sRenderSettings,
	RBX::FactoryProduct<CRenderSettings, RBX::GlobalSettings::Item, sRenderSettings> >;
template class RBX::DescribedCreatable<CRenderSettings, RBX::GlobalSettings::Item, sRenderSettings>;
