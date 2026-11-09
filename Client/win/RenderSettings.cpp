#include "RenderSettings.h"

#include "ViewBase.h"
#include "renderlib/AggregatingSceneManager.h"

#include <GLG3D/DXCaps.h>

const char sRenderSettings[] = "RenderSettings";

CRenderSettings::AASamples g_unk0x0091a2e8 = CRenderSettings::c_aaSamplesUnk0x1;
G3D::Vector2int16 g_unk0x0091a2ec(800, 600);
undefined4 g_unk0x00953ae0;

static RBX::Reflection::EnumPropDescriptor<CRenderSettings, CRenderSettings::GraphicsMode> g_unk0x00953ae4(
	"graphicsMode", "General", &CRenderSettings::FUN_00468a60, &CRenderSettings::FUN_0044d0b0
);

static RBX::Reflection::EnumPropDescriptor<CRenderSettings, CRenderSettings::FrameRateManagerMode> g_unk0x00953b04(
	"frameRateManager", "General", &CRenderSettings::FUN_00776c70, &CRenderSettings::FUN_0044d0e0
);

static RBX::Reflection::EnumPropDescriptor<CRenderSettings, CRenderSettings::BevelMode> g_unk0x00953b24(
	"Bevels", "General", &CRenderSettings::FUN_004482d0, &CRenderSettings::FUN_0044d110
);

static RBX::Reflection::EnumPropDescriptor<CRenderSettings, CRenderSettings::ShadowMode> g_unk0x00953b44(
	"Shadow", "General", &CRenderSettings::FUN_00776c60, &CRenderSettings::FUN_0044d170
);

static RBX::Reflection::EnumPropDescriptor<CRenderSettings, CRenderSettings::AntialiasingMode> g_unk0x00953b64(
	"Antialiasing", "General", &CRenderSettings::FUN_004482e0, &CRenderSettings::FUN_0044d140
);

static RBX::Reflection::PropDescriptor<CRenderSettings, G3D::Vector2int16> g_unk0x00953b84(
	"WindowSize", "Screen", &CRenderSettings::FUN_00448310, &CRenderSettings::FUN_0044ce00
);

static RBX::Reflection::PropDescriptor<CRenderSettings, G3D::Vector2int16> g_unk0x00953ba0(
	"FullscreenSize", "Screen", &CRenderSettings::FUN_00448300, &CRenderSettings::FUN_0044cdd0
);

static RBX::Reflection::PropDescriptor<CRenderSettings, float> g_unk0x00953bbc(
	"minShadingQuality", "Qaulity", &CRenderSettings::FUN_00448290, &CRenderSettings::FUN_0044ce30
);

static RBX::Reflection::PropDescriptor<CRenderSettings, float> g_unk0x00953bd8(
	"maxShadingQuality", "Qaulity", &CRenderSettings::FUN_004482a0, &CRenderSettings::FUN_0044ced0
);

static RBX::Reflection::PropDescriptor<CRenderSettings, float> g_unk0x00953bf4(
	"minMeshDetail", "Qaulity", &CRenderSettings::FUN_004482b0, &CRenderSettings::FUN_0044cf70
);

static RBX::Reflection::PropDescriptor<CRenderSettings, float> g_unk0x00953c10(
	"maxMeshDetail", "Qaulity", &CRenderSettings::FUN_004482c0, &CRenderSettings::FUN_0044cfe0
);

static RBX::Reflection::PropDescriptor<CRenderSettings, float> g_unk0x00953c2c(
	"_AggregateSize", "Performance", &CRenderSettings::FUN_00448320, &CRenderSettings::FUN_0044d050
);

static RBX::Reflection::EnumPropDescriptor<CRenderSettings, CRenderSettings::AASamples> g_unk0x00953c48(
	"AASamples", "Qaulity", &CRenderSettings::FUN_004482f0, &CRenderSettings::FUN_0044cda0
);

static RBX::Reflection::BoundProp<bool, 1> g_unk0x00953c68(
	"delaySwapBuffer", "Performance", &CRenderSettings::m_unk0x11c
);

static RBX::Reflection::BoundProp<std::string, 1> g_unk0x00953c84(
	"profileName", "Qaulity", &CRenderSettings::m_unk0x120, RBX::Reflection::PropertyDescriptor::STREAMING
);

static G3D::Vector2int16 defaultFullscreenSize()
{
	if (static_cast<unsigned int>(G3D::DXCaps::videoMemorySize()) >= 16000000) {
		return G3D::Vector2int16(1024, 768);
	}
	return G3D::Vector2int16(800, 600);
}

// FUNCTION: ROBLOX 0x004482d0
CRenderSettings::BevelMode CRenderSettings::FUN_004482d0() const
{
	return m_unk0xfc;
}

// FUNCTION: ROBLOX 0x004482e0
CRenderSettings::AntialiasingMode CRenderSettings::FUN_004482e0() const
{
	return m_unk0x100;
}

// FUNCTION: ROBLOX 0x004482f0
CRenderSettings::AASamples CRenderSettings::FUN_004482f0() const
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
int CRenderSettings::FUN_00448320() const
{
	return RBX::Render::AggregatingSceneManager::aggregationSize;
}

// FUNCTION: ROBLOX 0x0044a160
template <>
RBX::Reflection::EnumDesc<CRenderSettings::AASamples>::EnumDesc()
	: EnumDescriptor("AASamples", typeid(CRenderSettings::AASamples))
{
	addPair(CRenderSettings::c_aaSamplesUnk0x1, "None");
	addPair(CRenderSettings::c_aaSamplesUnk0x4, "4");
	addPair(CRenderSettings::c_aaSamplesUnk0x8, "8");
}

// FUNCTION: ROBLOX 0x0044a2e0
template <>
RBX::Reflection::EnumDesc<CRenderSettings::GraphicsMode>::EnumDesc()
	: EnumDescriptor("GraphicsMode", typeid(CRenderSettings::GraphicsMode))
{
	addPair(CRenderSettings::c_graphicsModeUnk0x1, "Automatic");
	addPair(CRenderSettings::c_graphicsModeUnk0x2, "OpenGL stable");
	addPair(CRenderSettings::c_graphicsModeUnk0x3, "Direct3D experimental");
	addPair(CRenderSettings::c_graphicsModeUnk0x4, "OpenGL experimental");
	addPair(CRenderSettings::c_graphicsModeUnk0x5, "No Graphics");
}

// FUNCTION: ROBLOX 0x0044a480
template <>
RBX::Reflection::EnumDesc<CRenderSettings::FrameRateManagerMode>::EnumDesc()
	: EnumDescriptor("Framerate Manager", typeid(CRenderSettings::FrameRateManagerMode))
{
	addPair(CRenderSettings::c_frameRateManagerModeUnk0x0, "Automatic");
	addPair(CRenderSettings::c_frameRateManagerModeUnk0x1, "On");
	addPair(CRenderSettings::c_frameRateManagerModeUnk0x2, "Off");
}

// FUNCTION: ROBLOX 0x0044a600
template <>
RBX::Reflection::EnumDesc<CRenderSettings::AntialiasingMode>::EnumDesc()
	: EnumDescriptor("Antialiasing", typeid(CRenderSettings::AntialiasingMode))
{
	addPair(CRenderSettings::c_antialiasingModeUnk0x0, "Automatic");
	addPair(CRenderSettings::c_antialiasingModeUnk0x2, "Off");
	addPair(CRenderSettings::c_antialiasingModeUnk0x1, "On");
}

// FUNCTION: ROBLOX 0x0044a780
template <>
RBX::Reflection::EnumDesc<CRenderSettings::BevelMode>::EnumDesc()
	: EnumDescriptor("Bevels", typeid(CRenderSettings::BevelMode))
{
	addPair(CRenderSettings::c_bevelModeUnk0x0, "Automatic");
	addPair(CRenderSettings::c_bevelModeUnk0x1, "On");
	addPair(CRenderSettings::c_bevelModeUnk0x2, "Off");
}

// FUNCTION: ROBLOX 0x0044a900
template <>
RBX::Reflection::EnumDesc<CRenderSettings::ShadowMode>::EnumDesc()
	: EnumDescriptor("Shadow", typeid(CRenderSettings::ShadowMode))
{
	addPair(CRenderSettings::c_shadowModeUnk0x0, "Automatic");
	addPair(CRenderSettings::c_shadowModeUnk0x1, "On");
	addPair(CRenderSettings::c_shadowModeUnk0x2, "Off");
}

// STUB: ROBLOX 0x0044cb20
CRenderSettings::CRenderSettings()
	: m_unk0xf8(c_graphicsModeUnk0x1), m_unk0xfc(c_bevelModeUnk0x0), m_unk0x100(c_antialiasingModeUnk0x2),
	  m_unk0x104(c_shadowModeUnk0x0), m_unk0x108(c_frameRateManagerModeUnk0x0), m_unk0x10c(20.0f), m_unk0x110(20.0f),
	  m_unk0x114(50.0f), m_unk0x118(50.0f), m_unk0x11c(1), m_unk0x13c(defaultFullscreenSize()),
	  m_unk0x140(g_unk0x0091a2ec)
{
	setName("Rendering");
	RBX::Render::AggregatingSceneManager::aggregationSize = 1;
}

// FUNCTION: ROBLOX 0x0044cda0
void CRenderSettings::FUN_0044cda0(AASamples p_unk0x04)
{
	if (p_unk0x04 != g_unk0x0091a2e8) {
		g_unk0x0091a2e8 = p_unk0x04;
		raisePropertyChanged(g_unk0x00953c48);
	}
}

// FUNCTION: ROBLOX 0x0044cdd0
void CRenderSettings::FUN_0044cdd0(G3D::Vector2int16 p_unk0x04)
{
	if (p_unk0x04 != m_unk0x13c) {
		m_unk0x13c = p_unk0x04;
		raisePropertyChanged(g_unk0x00953ba0);
	}
}

// FUNCTION: ROBLOX 0x0044ce00
void CRenderSettings::FUN_0044ce00(G3D::Vector2int16 p_unk0x04)
{
	if (p_unk0x04 != m_unk0x140) {
		m_unk0x140 = p_unk0x04;
		raisePropertyChanged(g_unk0x00953b84);
	}
}

// FUNCTION: ROBLOX 0x0044ce30
void CRenderSettings::FUN_0044ce30(float p_unk0x04)
{
	if (p_unk0x04 > m_unk0x110) {
		FUN_0044ced0(p_unk0x04);
	}

	p_unk0x04 = std::min(m_unk0x110, p_unk0x04);
	p_unk0x04 = std::max(0.0f, p_unk0x04);
	if (p_unk0x04 != m_unk0x10c) {
		m_unk0x10c = p_unk0x04;
		raisePropertyChanged(g_unk0x00953bbc);
	}
}

// FUNCTION: ROBLOX 0x0044ced0
void CRenderSettings::FUN_0044ced0(float p_unk0x04)
{
	if (p_unk0x04 < m_unk0x10c) {
		FUN_0044ce30(p_unk0x04);
	}

	p_unk0x04 = std::max(m_unk0x10c, p_unk0x04);
	p_unk0x04 = std::min(100.0f, p_unk0x04);
	if (p_unk0x04 != m_unk0x110) {
		m_unk0x110 = p_unk0x04;
		raisePropertyChanged(g_unk0x00953bd8);
	}
}

// FUNCTION: ROBLOX 0x0044cf70
void CRenderSettings::FUN_0044cf70(float p_unk0x04)
{
	p_unk0x04 = std::min(m_unk0x118, std::max(0.0f, p_unk0x04));
	if (p_unk0x04 != m_unk0x114) {
		m_unk0x114 = p_unk0x04;
		raisePropertyChanged(g_unk0x00953bf4);
	}
}

// FUNCTION: ROBLOX 0x0044cfe0
void CRenderSettings::FUN_0044cfe0(float p_unk0x04)
{
	p_unk0x04 = std::min(100.0f, std::max(m_unk0x114, p_unk0x04));
	if (p_unk0x04 != m_unk0x118) {
		m_unk0x118 = p_unk0x04;
		raisePropertyChanged(g_unk0x00953c10);
	}
}

// FUNCTION: ROBLOX 0x0044d050
void CRenderSettings::FUN_0044d050(int p_unk0x04)
{
	int size = std::min(1000, std::max(1, p_unk0x04));
	if (size != RBX::Render::AggregatingSceneManager::aggregationSize) {
		RBX::Render::AggregatingSceneManager::aggregationSize = size;
		raisePropertyChanged(g_unk0x00953c2c);
	}
}

// FUNCTION: ROBLOX 0x0044d0b0
void CRenderSettings::FUN_0044d0b0(GraphicsMode p_unk0x04)
{
	if (p_unk0x04 != m_unk0xf8) {
		m_unk0xf8 = p_unk0x04;
		raisePropertyChanged(g_unk0x00953ae4);
	}
}

// FUNCTION: ROBLOX 0x0044d0e0
void CRenderSettings::FUN_0044d0e0(FrameRateManagerMode p_unk0x04)
{
	if (p_unk0x04 != m_unk0x108) {
		m_unk0x108 = p_unk0x04;
		raisePropertyChanged(g_unk0x00953b04);
	}
}

// FUNCTION: ROBLOX 0x0044d110
void CRenderSettings::FUN_0044d110(BevelMode p_unk0x04)
{
	if (p_unk0x04 != m_unk0xfc) {
		m_unk0xfc = p_unk0x04;
		raisePropertyChanged(g_unk0x00953b24);
	}
}

// FUNCTION: ROBLOX 0x0044d140
void CRenderSettings::FUN_0044d140(AntialiasingMode p_unk0x04)
{
	if (p_unk0x04 != m_unk0x100) {
		m_unk0x100 = p_unk0x04;
		raisePropertyChanged(g_unk0x00953b64);
	}
}

// FUNCTION: ROBLOX 0x0044d170
void CRenderSettings::FUN_0044d170(ShadowMode p_unk0x04)
{
	if (p_unk0x04 != m_unk0x104) {
		m_unk0x104 = p_unk0x04;
		bool enabled = p_unk0x04 != c_shadowModeUnk0x0 && p_unk0x04 != c_shadowModeUnk0x2;
		if (enabled != RBX::ViewBase::getShadowsEnabled()) {
			RBX::ViewBase::getShadowsEnabled() = enabled;
			raisePropertyChanged(g_unk0x00953b44);
		}
	}
}

// STUB: ROBLOX 0x0044d1b0
void CRenderSettings::FUN_0044d1b0(bool p_unk0x04)
{
	STUB(0x44d1b0);
}

template class RBX::FactoryProduct<CRenderSettings, RBX::GlobalSettings::Item, sRenderSettings>;
template class RBX::Reflection::Described<
	CRenderSettings,
	sRenderSettings,
	RBX::FactoryProduct<CRenderSettings, RBX::GlobalSettings::Item, sRenderSettings> >;
template class RBX::DescribedCreatable<CRenderSettings, RBX::GlobalSettings::Item, sRenderSettings>;
