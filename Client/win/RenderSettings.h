#ifndef RENDERSETTINGS_H
#define RENDERSETTINGS_H

#include "decomp.h"
#include "v8datamodel/GlobalSettings.h"

#include <G3D/Vector2int16.h>
#include <string>

extern const char sRenderSettings[];

extern G3D::Vector2int16 g_unk0x0091a2ec;

class CRenderSettings;

// clang-format off
// TEMPLATE: ROBLOX 0x0040fc50
// RBX::Creatable<RBX::Instance>::create<CRenderSettings>
// TEMPLATE: ROBLOX 0x0040fce0
// RBX::GlobalSettingsItem<CRenderSettings,&sRenderSettings>::singleton
// TEMPLATE: ROBLOX 0x0040fed0
// ?create@Creator@?$FactoryProduct@VCRenderSettings@@VItem@GlobalSettings@RBX@@$1?sRenderSettings@@3QBDB@RBX@@UBE?AV?$shared_ptr@VObject@RBX@@@boost@@XZ
// TEMPLATE: ROBLOX 0x004485d0
// ?getValue@?$GetSetImpl@P8CRenderSettings@@BEHXZP81@AEXH@Z@?$PropDescriptor@VCRenderSettings@@M@Reflection@RBX@@UBEMPBVDescribedBase@34@@Z
// TEMPLATE: ROBLOX 0x00448610
// ?setValue@?$GetSetImpl@P8CRenderSettings@@BEHXZP81@AEXH@Z@?$PropDescriptor@VCRenderSettings@@M@Reflection@RBX@@UBEXPAVDescribedBase@34@ABM@Z
// TEMPLATE: ROBLOX 0x00448640
// RBX::FactoryProduct<CRenderSettings,RBX::GlobalSettings::Item,&sRenderSettings>::~FactoryProduct<CRenderSettings,RBX::GlobalSettings::Item,&sRenderSettings>
// TEMPLATE: ROBLOX 0x004486d0
// RBX::FactoryProduct<CRenderSettings,RBX::GlobalSettings::Item,&sRenderSettings>::FactoryProduct<CRenderSettings,RBX::GlobalSettings::Item,&sRenderSettings>
// TEMPLATE: ROBLOX 0x00448810
// ??0?$TypedPropertyDescriptor@VVector2int16@G3D@@@Reflection@RBX@@IAE@AAVClassDescriptor@12@PBD1V?$auto_ptr@VGetSet@?$TypedPropertyDescriptor@VVector2int16@G3D@@@Reflection@RBX@@@std@@W4Functionality@PropertyDescriptor@12@@Z
// TEMPLATE: ROBLOX 0x00448920
// RBX::Reflection::PropDescriptor<CRenderSettings,enum CRenderSettings::GraphicsMode>::getset<enum CRenderSettings::GraphicsMode (__thiscall CRenderSettings::*)(void)const ,void (__thiscall CRenderSettings::*)(enum CRenderSettings::GraphicsMode)>
// TEMPLATE: ROBLOX 0x00448980
// RBX::Reflection::PropDescriptor<CRenderSettings,enum CRenderSettings::FrameRateManagerMode>::getset<enum CRenderSettings::FrameRateManagerMode (__thiscall CRenderSettings::*)(void)const ,void (__thiscall CRenderSettings::*)(enum CRenderSettings::FrameRateManagerMode)>
// TEMPLATE: ROBLOX 0x004489e0
// RBX::Reflection::PropDescriptor<CRenderSettings,enum CRenderSettings::BevelMode>::getset<enum CRenderSettings::BevelMode (__thiscall CRenderSettings::*)(void)const ,void (__thiscall CRenderSettings::*)(enum CRenderSettings::BevelMode)>
// TEMPLATE: ROBLOX 0x00448a40
// RBX::Reflection::PropDescriptor<CRenderSettings,enum CRenderSettings::ShadowMode>::getset<enum CRenderSettings::ShadowMode (__thiscall CRenderSettings::*)(void)const ,void (__thiscall CRenderSettings::*)(enum CRenderSettings::ShadowMode)>
// TEMPLATE: ROBLOX 0x00448aa0
// RBX::Reflection::PropDescriptor<CRenderSettings,enum CRenderSettings::AntialiasingMode>::getset<enum CRenderSettings::AntialiasingMode (__thiscall CRenderSettings::*)(void)const ,void (__thiscall CRenderSettings::*)(enum CRenderSettings::AntialiasingMode)>
// TEMPLATE: ROBLOX 0x00448b00
// ??$getset@P8CRenderSettings@@BE?AVVector2int16@G3D@@XZP81@AEXV23@@Z@?$PropDescriptor@VCRenderSettings@@VVector2int16@G3D@@@Reflection@RBX@@SA?AV?$auto_ptr@VGetSet@?$TypedPropertyDescriptor@VVector2int16@G3D@@@Reflection@RBX@@@std@@P8CRenderSettings@@BE?AVVector2int16@G3D@@XZP85@AEXV67@@Z@Z
// TEMPLATE: ROBLOX 0x00448b60
// ??$getset@P8CRenderSettings@@BEMXZP81@AEXM@Z@?$PropDescriptor@VCRenderSettings@@M@Reflection@RBX@@SA?AV?$auto_ptr@VGetSet@?$TypedPropertyDescriptor@M@Reflection@RBX@@@std@@P8CRenderSettings@@BEMXZP85@AEXM@Z@Z
// TEMPLATE: ROBLOX 0x00448bc0
// ??$getset@P8CRenderSettings@@BEHXZP81@AEXH@Z@?$PropDescriptor@VCRenderSettings@@M@Reflection@RBX@@SA?AV?$auto_ptr@VGetSet@?$TypedPropertyDescriptor@M@Reflection@RBX@@@std@@P8CRenderSettings@@BEHXZP85@AEXH@Z@Z
// TEMPLATE: ROBLOX 0x00448c20
// RBX::Reflection::PropDescriptor<CRenderSettings,enum CRenderSettings::AASamples>::getset<enum CRenderSettings::AASamples (__thiscall CRenderSettings::*)(void)const ,void (__thiscall CRenderSettings::*)(enum CRenderSettings::AASamples)>
// TEMPLATE: ROBLOX 0x00448d70
// RBX::GlobalSettingsItem<CRenderSettings,&sRenderSettings>::~GlobalSettingsItem<CRenderSettings,&sRenderSettings>
// TEMPLATE: ROBLOX 0x004494a0
// RBX::Reflection::EnumDesc<enum CRenderSettings::AASamples>::~EnumDesc<enum CRenderSettings::AASamples>
// STUB: ROBLOX 0x00449660
// RBX::Reflection::EnumDesc<enum CRenderSettings::ShadowMode>::addPair
// TEMPLATE: ROBLOX 0x004498a0
// RBX::Reflection::EnumDesc<enum CRenderSettings::GraphicsMode>::~EnumDesc<enum CRenderSettings::GraphicsMode>
// TEMPLATE: ROBLOX 0x00449a60
// RBX::Reflection::EnumDesc<enum CRenderSettings::FrameRateManagerMode>::~EnumDesc<enum CRenderSettings::FrameRateManagerMode>
// TEMPLATE: ROBLOX 0x00449c20
// RBX::Reflection::EnumDesc<enum CRenderSettings::AntialiasingMode>::~EnumDesc<enum CRenderSettings::AntialiasingMode>
// TEMPLATE: ROBLOX 0x00449de0
// RBX::Reflection::EnumDesc<enum CRenderSettings::BevelMode>::~EnumDesc<enum CRenderSettings::BevelMode>
// TEMPLATE: ROBLOX 0x00449fa0
// RBX::Reflection::EnumDesc<enum CRenderSettings::ShadowMode>::~EnumDesc<enum CRenderSettings::ShadowMode>
// SYNTHETIC: ROBLOX 0x0044a2c0
// RBX::Reflection::EnumDesc<enum CRenderSettings::AASamples>::`scalar deleting destructor'
// SYNTHETIC: ROBLOX 0x0044a460
// RBX::Reflection::EnumDesc<enum CRenderSettings::GraphicsMode>::`scalar deleting destructor'
// SYNTHETIC: ROBLOX 0x0044a5e0
// RBX::Reflection::EnumDesc<enum CRenderSettings::FrameRateManagerMode>::`scalar deleting destructor'
// SYNTHETIC: ROBLOX 0x0044a8e0
// RBX::Reflection::EnumDesc<enum CRenderSettings::BevelMode>::`scalar deleting destructor'
// SYNTHETIC: ROBLOX 0x0044aa60
// RBX::Reflection::EnumDesc<enum CRenderSettings::ShadowMode>::`scalar deleting destructor'
// TEMPLATE: ROBLOX 0x0044aa80
// RBX::Reflection::EnumDesc<enum CRenderSettings::AASamples>::singleton
// TEMPLATE: ROBLOX 0x0044aaf0
// RBX::Reflection::EnumDesc<enum CRenderSettings::GraphicsMode>::singleton
// TEMPLATE: ROBLOX 0x0044ab60
// RBX::Reflection::EnumDesc<enum CRenderSettings::FrameRateManagerMode>::singleton
// TEMPLATE: ROBLOX 0x0044abd0
// RBX::Reflection::EnumDesc<enum CRenderSettings::AntialiasingMode>::singleton
// TEMPLATE: ROBLOX 0x0044ac40
// RBX::Reflection::EnumDesc<enum CRenderSettings::BevelMode>::singleton
// TEMPLATE: ROBLOX 0x0044acb0
// RBX::Reflection::EnumDesc<enum CRenderSettings::ShadowMode>::singleton
// TEMPLATE: ROBLOX 0x0044ad20
// RBX::Reflection::Described<CRenderSettings,&sRenderSettings,RBX::FactoryProduct<CRenderSettings,RBX::GlobalSettings::Item,&sRenderSettings> >::classDescriptor
// TEMPLATE: ROBLOX 0x0044ada0
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::GraphicsMode>::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::GraphicsMode><enum CRenderSettings::GraphicsMode (__thiscall CRenderSettings::*)(void)const ,void (__thiscall CRenderSettings::*)(enum CRenderSettings::GraphicsMode)>
// TEMPLATE: ROBLOX 0x0044aff0
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::FrameRateManagerMode>::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::FrameRateManagerMode><enum CRenderSettings::FrameRateManagerMode (__thiscall CRenderSettings::*)(void)const ,void (__thiscall CRenderSettings::*)(enum CRenderSettings::FrameRateManagerMode)>
// TEMPLATE: ROBLOX 0x0044b0a0
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::BevelMode>::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::BevelMode><enum CRenderSettings::BevelMode (__thiscall CRenderSettings::*)(void)const ,void (__thiscall CRenderSettings::*)(enum CRenderSettings::BevelMode)>
// TEMPLATE: ROBLOX 0x0044b150
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::ShadowMode>::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::ShadowMode><enum CRenderSettings::ShadowMode (__thiscall CRenderSettings::*)(void)const ,void (__thiscall CRenderSettings::*)(enum CRenderSettings::ShadowMode)>
// TEMPLATE: ROBLOX 0x0044b200
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::AntialiasingMode>::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::AntialiasingMode><enum CRenderSettings::AntialiasingMode (__thiscall CRenderSettings::*)(void)const ,void (__thiscall CRenderSettings::*)(enum CRenderSettings::AntialiasingMode)>
// TEMPLATE: ROBLOX 0x0044b380
// ??$?0P8CRenderSettings@@BE?AVVector2int16@G3D@@XZP80@AEXV12@@Z@?$PropDescriptor@VCRenderSettings@@VVector2int16@G3D@@@Reflection@RBX@@QAE@PBD0P8CRenderSettings@@BE?AVVector2int16@G3D@@XZP83@AEXV45@@ZW4Functionality@PropertyDescriptor@12@@Z
// TEMPLATE: ROBLOX 0x0044b430
// ??$?0P8CRenderSettings@@BEMXZP80@AEXM@Z@?$PropDescriptor@VCRenderSettings@@M@Reflection@RBX@@QAE@PBD0P8CRenderSettings@@BEMXZP83@AEXM@ZW4Functionality@PropertyDescriptor@12@@Z
// TEMPLATE: ROBLOX 0x0044b4e0
// ??$?0P8CRenderSettings@@BEHXZP80@AEXH@Z@?$PropDescriptor@VCRenderSettings@@M@Reflection@RBX@@QAE@PBD0P8CRenderSettings@@BEHXZP83@AEXH@ZW4Functionality@PropertyDescriptor@12@@Z
// TEMPLATE: ROBLOX 0x0044b590
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::AASamples>::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::AASamples><enum CRenderSettings::AASamples (__thiscall CRenderSettings::*)(void)const ,void (__thiscall CRenderSettings::*)(enum CRenderSettings::AASamples)>
// TEMPLATE: ROBLOX 0x0044b640
// ??$?0VCRenderSettings@@@?$BoundProp@_N$00@Reflection@RBX@@QAE@PBD0PQCRenderSettings@@_NW4Functionality@PropertyDescriptor@12@@Z
// TEMPLATE: ROBLOX 0x0044b720
// ??$?0VCRenderSettings@@@?$BoundProp@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@$00@Reflection@RBX@@QAE@PBD0PQCRenderSettings@@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@W4Functionality@PropertyDescriptor@12@@Z
// TEMPLATE: ROBLOX 0x0044b800
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::GraphicsMode>::setEnumValue
// TEMPLATE: ROBLOX 0x0044b840
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::GraphicsMode>::getIndexValue
// TEMPLATE: ROBLOX 0x0044b860
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::GraphicsMode>::setIndexValue
// TEMPLATE: ROBLOX 0x0044b8c0
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::GraphicsMode>::getStringValue
// TEMPLATE: ROBLOX 0x0044b9a0
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::GraphicsMode>::readValue
// TEMPLATE: ROBLOX 0x0044bae0
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::FrameRateManagerMode>::setEnumValue
// TEMPLATE: ROBLOX 0x0044bb20
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::FrameRateManagerMode>::getIndexValue
// TEMPLATE: ROBLOX 0x0044bb40
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::FrameRateManagerMode>::setIndexValue
// TEMPLATE: ROBLOX 0x0044bba0
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::FrameRateManagerMode>::getStringValue
// TEMPLATE: ROBLOX 0x0044bc80
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::FrameRateManagerMode>::readValue
// TEMPLATE: ROBLOX 0x0044bdc0
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::BevelMode>::setEnumValue
// TEMPLATE: ROBLOX 0x0044be00
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::BevelMode>::getIndexValue
// TEMPLATE: ROBLOX 0x0044be20
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::BevelMode>::setIndexValue
// TEMPLATE: ROBLOX 0x0044be80
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::BevelMode>::getStringValue
// TEMPLATE: ROBLOX 0x0044bf60
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::BevelMode>::readValue
// TEMPLATE: ROBLOX 0x0044c0a0
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::ShadowMode>::setEnumValue
// TEMPLATE: ROBLOX 0x0044c0e0
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::ShadowMode>::getIndexValue
// TEMPLATE: ROBLOX 0x0044c100
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::ShadowMode>::setIndexValue
// TEMPLATE: ROBLOX 0x0044c160
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::ShadowMode>::getStringValue
// TEMPLATE: ROBLOX 0x0044c240
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::ShadowMode>::readValue
// TEMPLATE: ROBLOX 0x0044c380
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::AntialiasingMode>::setEnumValue
// TEMPLATE: ROBLOX 0x0044c3c0
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::AntialiasingMode>::getIndexValue
// TEMPLATE: ROBLOX 0x0044c3e0
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::AntialiasingMode>::setIndexValue
// TEMPLATE: ROBLOX 0x0044c440
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::AntialiasingMode>::getStringValue
// TEMPLATE: ROBLOX 0x0044c520
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::AntialiasingMode>::readValue
// TEMPLATE: ROBLOX 0x0044c660
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::AASamples>::setEnumValue
// TEMPLATE: ROBLOX 0x0044c6a0
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::AASamples>::getIndexValue
// TEMPLATE: ROBLOX 0x0044c6c0
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::AASamples>::setIndexValue
// TEMPLATE: ROBLOX 0x0044c720
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::AASamples>::getStringValue
// TEMPLATE: ROBLOX 0x0044c800
// RBX::Reflection::EnumPropDescriptor<CRenderSettings,enum CRenderSettings::AASamples>::readValue
// TEMPLATE: ROBLOX 0x0044c940
// RBX::Reflection::Described<CRenderSettings,&sRenderSettings,RBX::FactoryProduct<CRenderSettings,RBX::GlobalSettings::Item,&sRenderSettings> >::Described<CRenderSettings,&sRenderSettings,RBX::FactoryProduct<CRenderSettings,RBX::GlobalSettings::Item,&sRenderSettings> >
// TEMPLATE: ROBLOX 0x0044c990
// RBX::DescribedCreatable<CRenderSettings,RBX::GlobalSettings::Item,&sRenderSettings>::DescribedCreatable<CRenderSettings,RBX::GlobalSettings::Item,&sRenderSettings>
// TEMPLATE: ROBLOX 0x0044ca50
// RBX::GlobalSettingsItem<CRenderSettings,&sRenderSettings>::GlobalSettingsItem<CRenderSettings,&sRenderSettings>
// SYNTHETIC: ROBLOX 0x007efe60
// `RBX::Reflection::EnumDesc<enum CRenderSettings::ShadowMode>::singleton'::`2'::`dynamic atexit destructor for 's''
// SYNTHETIC: ROBLOX 0x007efe70
// `RBX::Reflection::EnumDesc<enum CRenderSettings::BevelMode>::singleton'::`2'::`dynamic atexit destructor for 's''
// SYNTHETIC: ROBLOX 0x007efe80
// `RBX::Reflection::EnumDesc<enum CRenderSettings::AntialiasingMode>::singleton'::`2'::`dynamic atexit destructor for 's''
// SYNTHETIC: ROBLOX 0x007efe90
// `RBX::Reflection::EnumDesc<enum CRenderSettings::FrameRateManagerMode>::singleton'::`2'::`dynamic atexit destructor for 's''
// SYNTHETIC: ROBLOX 0x007efea0
// `RBX::Reflection::EnumDesc<enum CRenderSettings::GraphicsMode>::singleton'::`2'::`dynamic atexit destructor for 's''
// SYNTHETIC: ROBLOX 0x007efeb0
// `RBX::Reflection::EnumDesc<enum CRenderSettings::AASamples>::singleton'::`2'::`dynamic atexit destructor for 's''
// clang-format on

// SIZE 0x144
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

	// FUNCTION: ROBLOX 0x00448290
	float FUN_00448290() const { return m_unk0x10c; }

	// FUNCTION: ROBLOX 0x004482a0
	float FUN_004482a0() const { return m_unk0x110; }

	// FUNCTION: ROBLOX 0x004482b0
	float FUN_004482b0() const { return m_unk0x114; }

	// FUNCTION: ROBLOX 0x004482c0
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

	// clang-format off
	// SYNTHETIC: ROBLOX 0x0044cd20
	// CRenderSettings::~CRenderSettings
	// SYNTHETIC: ROBLOX 0x0044cd80
	// CRenderSettings::`scalar deleting destructor'
	// clang-format on

	GraphicsMode FUN_00468a60() const { return m_unk0xf8; }
	ShadowMode FUN_00776c60() const { return m_unk0x104; }
	FrameRateManagerMode FUN_00776c70() const { return m_unk0x108; }

	GraphicsMode m_unk0xf8;          // 0x0f8
	BevelMode m_unk0xfc;             // 0x0fc
	AntialiasingMode m_unk0x100;     // 0x100
	ShadowMode m_unk0x104;           // 0x104
	FrameRateManagerMode m_unk0x108; // 0x108
	float m_unk0x10c;                // 0x10c
	float m_unk0x110;                // 0x110
	float m_unk0x114;                // 0x114
	float m_unk0x118;                // 0x118
	bool m_unk0x11c;                 // 0x11c
	std::string m_unk0x120;          // 0x120
	G3D::Vector2int16 m_unk0x13c;    // 0x13c
	G3D::Vector2int16 m_unk0x140;    // 0x140
};

DECOMP_SIZE_ASSERT(CRenderSettings, 0x144)

extern CRenderSettings::GraphicsMode g_unk0x00953ae0;

// FUNCTION: ROBLOX 0x0044d430
inline CRenderSettings::GraphicsMode CRenderSettings::FUN_0044d430()
{
	if (g_unk0x00953ae0 == 0) {
		g_unk0x00953ae0 = m_unk0xf8;
	}
	return g_unk0x00953ae0;
}

extern RBX::Reflection::BoundProp<bool, 1> g_unk0x00953c68;

extern CRenderSettings::AASamples g_unk0x0091a2e8;

// clang-format off
// SYNTHETIC: ROBLOX 0x007e49c0
// `dynamic initializer for 'g_unk0x00953b04''
// SYNTHETIC: ROBLOX 0x007e4a00
// `dynamic initializer for 'g_unk0x00953b24''
// SYNTHETIC: ROBLOX 0x007e4a40
// `dynamic initializer for 'g_unk0x00953b44''
// SYNTHETIC: ROBLOX 0x007e4a80
// `dynamic initializer for 'g_unk0x00953b64''
// SYNTHETIC: ROBLOX 0x007e4ac0
// `dynamic initializer for 'g_unk0x00953b84''
// SYNTHETIC: ROBLOX 0x007e4b00
// `dynamic initializer for 'g_unk0x00953ba0''
// SYNTHETIC: ROBLOX 0x007e4b40
// `dynamic initializer for 'g_unk0x00953bbc''
// SYNTHETIC: ROBLOX 0x007e4b80
// `dynamic initializer for 'g_unk0x00953bd8''
// SYNTHETIC: ROBLOX 0x007e4bc0
// `dynamic initializer for 'g_unk0x00953bf4''
// SYNTHETIC: ROBLOX 0x007e4c00
// `dynamic initializer for 'g_unk0x00953c10''
// SYNTHETIC: ROBLOX 0x007e4c40
// `dynamic initializer for 'g_unk0x00953c2c''
// SYNTHETIC: ROBLOX 0x007e4c80
// `dynamic initializer for 'g_unk0x00953c48''
// SYNTHETIC: ROBLOX 0x007e4cc0
// `dynamic initializer for 'g_unk0x00953c68''
// SYNTHETIC: ROBLOX 0x007e4cf0
// `dynamic initializer for 'g_unk0x00953c84''
// SYNTHETIC: ROBLOX 0x007efc70
// `dynamic atexit destructor for 'g_unk0x00953b64''
// SYNTHETIC: ROBLOX 0x007efc90
// `dynamic atexit destructor for 'g_unk0x00953b44''
// SYNTHETIC: ROBLOX 0x007efcb0
// `dynamic atexit destructor for 'g_unk0x00953b24''
// SYNTHETIC: ROBLOX 0x007efcd0
// `dynamic atexit destructor for 'g_unk0x00953b04''
// SYNTHETIC: ROBLOX 0x007efcf0
// `dynamic atexit destructor for 'g_unk0x00953ae4''
// SYNTHETIC: ROBLOX 0x007efd10
// `dynamic atexit destructor for 'g_unk0x00953bf4''
// SYNTHETIC: ROBLOX 0x007efd30
// `dynamic atexit destructor for 'g_unk0x00953c10''
// SYNTHETIC: ROBLOX 0x007efd50
// `dynamic atexit destructor for 'g_unk0x00953c48''
// SYNTHETIC: ROBLOX 0x007efd70
// `dynamic atexit destructor for 'g_unk0x00953c2c''
// SYNTHETIC: ROBLOX 0x007efd90
// `dynamic atexit destructor for 'g_unk0x00953ba0''
// SYNTHETIC: ROBLOX 0x007efdb0
// `dynamic atexit destructor for 'g_unk0x00953b84''
// SYNTHETIC: ROBLOX 0x007efdd0
// `dynamic atexit destructor for 'g_unk0x00953bd8''
// SYNTHETIC: ROBLOX 0x007efdf0
// `dynamic atexit destructor for 'g_unk0x00953bbc''
// SYNTHETIC: ROBLOX 0x007efe10
// `dynamic atexit destructor for 'g_unk0x00953c84''
// SYNTHETIC: ROBLOX 0x007efe30
// `dynamic atexit destructor for 'g_unk0x00953c68''
// SYNTHETIC: ROBLOX 0x007efe50
// `RBX::Reflection::Described<CRenderSettings,&sRenderSettings,RBX::FactoryProduct<CRenderSettings,RBX::GlobalSettings::Item,&sRenderSettings> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// clang-format on

#endif // RENDERSETTINGS_H
