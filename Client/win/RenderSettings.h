#ifndef RENDERSETTINGS_H
#define RENDERSETTINGS_H

#include "placeholder.h"
#include "v8datamodel/GlobalSettings.h"

#include <G3D/Vector2int16.h>
#include <string>

extern const char sRenderSettings[];

extern const float g_unk0x008094f8;
extern const float g_unk0x008094fc;
extern const float g_unk0x00809500;
extern const float g_unk0x00809504;
extern G3D::Vector2int16 g_unk0x0091a2ec;

class CRenderSettings;

class CRenderSettings : public RBX::GlobalSettingsItem<CRenderSettings, sRenderSettings>
{
public:
	enum GraphicsMode
	{
		c_graphicsModeUnk0x1 = 1,
		c_graphicsModeUnk0x2,
		c_graphicsModeUnk0x3,
		c_graphicsModeUnk0x4,
		c_graphicsModeUnk0x5,
	};

	enum FrameRateManagerMode
	{
		c_frameRateManagerModeUnk0x0,
		c_frameRateManagerModeUnk0x1,
		c_frameRateManagerModeUnk0x2,
	};

	enum BevelMode
	{
		c_bevelModeUnk0x0,
		c_bevelModeUnk0x1,
		c_bevelModeUnk0x2,
	};

	enum ShadowMode
	{
		c_shadowModeUnk0x0,
		c_shadowModeUnk0x1,
		c_shadowModeUnk0x2,
	};

	enum AntialiasingMode
	{
		c_antialiasingModeUnk0x0,
		c_antialiasingModeUnk0x1,
		c_antialiasingModeUnk0x2,
	};

	enum AASamples
	{
		c_aaSamplesUnk0x1 = 1,
		c_aaSamplesUnk0x4 = 4,
		c_aaSamplesUnk0x8 = 8,
	};

	CRenderSettings();

	float FUN_00448290() const { return m_unk0x10c; }

	float FUN_004482a0() const { return m_unk0x110; }

	float FUN_004482b0() const { return m_unk0x114; }

	float FUN_004482c0() const { return m_unk0x118; }

	BevelMode FUN_004482d0() const;
	AntialiasingMode FUN_004482e0() const;
	AASamples FUN_004482f0() const;
	G3D::Vector2int16 FUN_00448300() const;
	G3D::Vector2int16 FUN_00448310() const;
	int FUN_00448320() const;
	void FUN_0044cda0(AASamples p_unk0x04);
	void FUN_0044cdd0(G3D::Vector2int16 p_unk0x04);
	void FUN_0044ce00(G3D::Vector2int16 p_unk0x04);
	void FUN_0044ce30(float p_unk0x04);
	void FUN_0044ced0(float p_unk0x04);
	void FUN_0044cf70(float p_unk0x04);
	void FUN_0044cfe0(float p_unk0x04);
	void FUN_0044d050(int p_unk0x04);
	void FUN_0044d0b0(GraphicsMode p_unk0x04);
	void FUN_0044d1b0(bool p_unk0x04);
	void FUN_0044d0e0(FrameRateManagerMode p_unk0x04);
	void FUN_0044d110(BevelMode p_unk0x04);
	void FUN_0044d140(AntialiasingMode p_unk0x04);
	void FUN_0044d170(ShadowMode p_unk0x04);
	GraphicsMode FUN_0044d430();

	GraphicsMode FUN_00468a60() const { return m_unk0xf8; }
	ShadowMode FUN_00776c60() const { return m_unk0x104; }
	FrameRateManagerMode FUN_00776c70() const { return m_unk0x108; }

	GraphicsMode m_unk0xf8;
	BevelMode m_unk0xfc;
	AntialiasingMode m_unk0x100;
	ShadowMode m_unk0x104;
	FrameRateManagerMode m_unk0x108;
	float m_unk0x10c;
	float m_unk0x110;
	float m_unk0x114;
	float m_unk0x118;
	bool m_unk0x11c;
	std::string m_unk0x120;
	G3D::Vector2int16 m_unk0x13c;
	G3D::Vector2int16 m_unk0x140;
};

extern CRenderSettings::GraphicsMode g_unk0x00953ae0;

inline CRenderSettings::GraphicsMode CRenderSettings::FUN_0044d430()
{
	if (g_unk0x00953ae0 == 0) {
		g_unk0x00953ae0 = m_unk0xf8;
	}
	return g_unk0x00953ae0;
}

extern RBX::Reflection::BoundProp<bool, 1> g_unk0x00953c68;

extern CRenderSettings::AASamples g_unk0x0091a2e8;

#endif // RENDERSETTINGS_H
