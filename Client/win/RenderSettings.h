#ifndef RENDERSETTINGS_H
#define RENDERSETTINGS_H

#include "decomp.h"
#include "v8datamodel/GlobalSettings.h"

#include <G3D/Vector2int16.h>
#include <string>

extern const char sRenderSettings[];

extern undefined4 g_unk0x0091a2e8;
extern G3D::Vector2int16 g_unk0x0091a2ec;

class CRenderSettings;

// clang-format off
// STUB: ROBLOX 0x0040fc50
// RBX::Creatable<RBX::Instance>::create<CRenderSettings>
// TEMPLATE: ROBLOX 0x0040fce0
// RBX::GlobalSettingsItem<CRenderSettings,&sRenderSettings>::singleton
// TEMPLATE: ROBLOX 0x00448640
// RBX::FactoryProduct<CRenderSettings,RBX::GlobalSettings::Item,&sRenderSettings>::~FactoryProduct<CRenderSettings,RBX::GlobalSettings::Item,&sRenderSettings>
// TEMPLATE: ROBLOX 0x004486d0
// RBX::FactoryProduct<CRenderSettings,RBX::GlobalSettings::Item,&sRenderSettings>::FactoryProduct<CRenderSettings,RBX::GlobalSettings::Item,&sRenderSettings>
// TEMPLATE: ROBLOX 0x00448d70
// RBX::GlobalSettingsItem<CRenderSettings,&sRenderSettings>::~GlobalSettingsItem<CRenderSettings,&sRenderSettings>
// TEMPLATE: ROBLOX 0x0044ad20
// RBX::Reflection::Described<CRenderSettings,&sRenderSettings,RBX::FactoryProduct<CRenderSettings,RBX::GlobalSettings::Item,&sRenderSettings> >::classDescriptor
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
	unsigned int FUN_00448320() const;

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
	undefined m_unk0x11c;         // 0x11c
	std::string m_unk0x120;       // 0x120
	G3D::Vector2int16 m_unk0x13c; // 0x13c
	G3D::Vector2int16 m_unk0x140; // 0x140
};

DECOMP_SIZE_ASSERT(CRenderSettings, 0x144)

// clang-format off
// SYNTHETIC: ROBLOX 0x007efe50
// `RBX::Reflection::Described<CRenderSettings,&sRenderSettings,RBX::FactoryProduct<CRenderSettings,RBX::GlobalSettings::Item,&sRenderSettings> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// clang-format on

#endif // RENDERSETTINGS_H
