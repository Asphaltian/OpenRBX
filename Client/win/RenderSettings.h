#ifndef RENDERSETTINGS_H
#define RENDERSETTINGS_H

#include "decomp.h"
#include "v8datamodel/GlobalSettings.h"

#include <G3D/Vector2int16.h>
#include <string>

extern const char sRenderSettings[];

extern undefined4 g_unk0x0091a2e8;
extern G3D::Vector2int16 g_unk0x0091a2ec;
extern undefined4 g_unk0x00953ae0;

class CRenderSettings;

// clang-format off
// TEMPLATE: ROBLOX 0x0040fc50
// RBX::Creatable<RBX::Instance>::create<CRenderSettings>
// TEMPLATE: ROBLOX 0x0040fce0
// RBX::GlobalSettingsItem<CRenderSettings,&sRenderSettings>::singleton
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
// TEMPLATE: ROBLOX 0x00448b00
// ??$getset@P8CRenderSettings@@BE?AVVector2int16@G3D@@XZP81@AEXV23@@Z@?$PropDescriptor@VCRenderSettings@@VVector2int16@G3D@@@Reflection@RBX@@SA?AV?$auto_ptr@VGetSet@?$TypedPropertyDescriptor@VVector2int16@G3D@@@Reflection@RBX@@@std@@P8CRenderSettings@@BE?AVVector2int16@G3D@@XZP85@AEXV67@@Z@Z
// TEMPLATE: ROBLOX 0x00448b60
// ??$getset@P8CRenderSettings@@BEMXZP81@AEXM@Z@?$PropDescriptor@VCRenderSettings@@M@Reflection@RBX@@SA?AV?$auto_ptr@VGetSet@?$TypedPropertyDescriptor@M@Reflection@RBX@@@std@@P8CRenderSettings@@BEMXZP85@AEXM@Z@Z
// TEMPLATE: ROBLOX 0x00448bc0
// ??$getset@P8CRenderSettings@@BEHXZP81@AEXH@Z@?$PropDescriptor@VCRenderSettings@@M@Reflection@RBX@@SA?AV?$auto_ptr@VGetSet@?$TypedPropertyDescriptor@M@Reflection@RBX@@@std@@P8CRenderSettings@@BEHXZP85@AEXH@Z@Z
// TEMPLATE: ROBLOX 0x00448d70
// RBX::GlobalSettingsItem<CRenderSettings,&sRenderSettings>::~GlobalSettingsItem<CRenderSettings,&sRenderSettings>
// TEMPLATE: ROBLOX 0x0044ad20
// RBX::Reflection::Described<CRenderSettings,&sRenderSettings,RBX::FactoryProduct<CRenderSettings,RBX::GlobalSettings::Item,&sRenderSettings> >::classDescriptor
// TEMPLATE: ROBLOX 0x0044b380
// ??$?0P8CRenderSettings@@BE?AVVector2int16@G3D@@XZP80@AEXV12@@Z@?$PropDescriptor@VCRenderSettings@@VVector2int16@G3D@@@Reflection@RBX@@QAE@PBD0P8CRenderSettings@@BE?AVVector2int16@G3D@@XZP83@AEXV45@@ZW4Functionality@PropertyDescriptor@12@@Z
// TEMPLATE: ROBLOX 0x0044b430
// ??$?0P8CRenderSettings@@BEMXZP80@AEXM@Z@?$PropDescriptor@VCRenderSettings@@M@Reflection@RBX@@QAE@PBD0P8CRenderSettings@@BEMXZP83@AEXM@ZW4Functionality@PropertyDescriptor@12@@Z
// TEMPLATE: ROBLOX 0x0044b4e0
// ??$?0P8CRenderSettings@@BEHXZP80@AEXH@Z@?$PropDescriptor@VCRenderSettings@@M@Reflection@RBX@@QAE@PBD0P8CRenderSettings@@BEHXZP83@AEXH@ZW4Functionality@PropertyDescriptor@12@@Z
// TEMPLATE: ROBLOX 0x0044b640
// ??$?0VCRenderSettings@@@?$BoundProp@_N$00@Reflection@RBX@@QAE@PBD0PQCRenderSettings@@_NW4Functionality@PropertyDescriptor@12@@Z
// TEMPLATE: ROBLOX 0x0044b720
// ??$?0VCRenderSettings@@@?$BoundProp@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@$00@Reflection@RBX@@QAE@PBD0PQCRenderSettings@@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@W4Functionality@PropertyDescriptor@12@@Z
// TEMPLATE: ROBLOX 0x0044c940
// RBX::Reflection::Described<CRenderSettings,&sRenderSettings,RBX::FactoryProduct<CRenderSettings,RBX::GlobalSettings::Item,&sRenderSettings> >::Described<CRenderSettings,&sRenderSettings,RBX::FactoryProduct<CRenderSettings,RBX::GlobalSettings::Item,&sRenderSettings> >
// TEMPLATE: ROBLOX 0x0044c990
// RBX::DescribedCreatable<CRenderSettings,RBX::GlobalSettings::Item,&sRenderSettings>::DescribedCreatable<CRenderSettings,RBX::GlobalSettings::Item,&sRenderSettings>
// TEMPLATE: ROBLOX 0x0044ca50
// RBX::GlobalSettingsItem<CRenderSettings,&sRenderSettings>::GlobalSettingsItem<CRenderSettings,&sRenderSettings>
// clang-format on

// SIZE 0x144
class CRenderSettings : public RBX::GlobalSettingsItem<CRenderSettings, sRenderSettings>
{
public:
	CRenderSettings();

	float FUN_00448290() const;
	float FUN_004482a0() const;
	float FUN_004482b0() const;
	float FUN_004482c0() const;
	undefined4 FUN_004482d0() const;
	undefined4 FUN_004482e0() const;
	undefined4 FUN_004482f0() const;
	G3D::Vector2int16 FUN_00448300() const;
	G3D::Vector2int16 FUN_00448310() const;
	int FUN_00448320() const;
	void FUN_0044cdd0(G3D::Vector2int16 p_unk0x04);
	void FUN_0044ce00(G3D::Vector2int16 p_unk0x04);
	void FUN_0044ce30(float p_unk0x04);
	void FUN_0044ced0(float p_unk0x04);
	void FUN_0044cf70(float p_unk0x04);
	void FUN_0044cfe0(float p_unk0x04);
	void FUN_0044d050(int p_unk0x04);
	void FUN_0044d0b0(undefined4 p_unk0x04);
	undefined4 FUN_0044d430();

	// clang-format off
	// SYNTHETIC: ROBLOX 0x0044cd20
	// CRenderSettings::~CRenderSettings
	// SYNTHETIC: ROBLOX 0x0044cd80
	// CRenderSettings::`scalar deleting destructor'
	// clang-format on

	undefined4 m_unk0xf8;         // 0x0f8
	undefined4 m_unk0xfc;         // 0x0fc
	undefined4 m_unk0x100;        // 0x100
	undefined4 m_unk0x104;        // 0x104
	undefined4 m_unk0x108;        // 0x108
	float m_unk0x10c;             // 0x10c
	float m_unk0x110;             // 0x110
	float m_unk0x114;             // 0x114
	float m_unk0x118;             // 0x118
	bool m_unk0x11c;              // 0x11c
	std::string m_unk0x120;       // 0x120
	G3D::Vector2int16 m_unk0x13c; // 0x13c
	G3D::Vector2int16 m_unk0x140; // 0x140
};

DECOMP_SIZE_ASSERT(CRenderSettings, 0x144)

// clang-format off
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
// SYNTHETIC: ROBLOX 0x007e4cc0
// `dynamic initializer for 'g_unk0x00953c68''
// SYNTHETIC: ROBLOX 0x007e4cf0
// `dynamic initializer for 'g_unk0x00953c84''
// SYNTHETIC: ROBLOX 0x007efd10
// `dynamic atexit destructor for 'g_unk0x00953bf4''
// SYNTHETIC: ROBLOX 0x007efd30
// `dynamic atexit destructor for 'g_unk0x00953c10''
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
