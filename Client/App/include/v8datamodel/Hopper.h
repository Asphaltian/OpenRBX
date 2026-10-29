#ifndef V8DATAMODEL_HOPPER_H
#define V8DATAMODEL_HOPPER_H

#include "decomp.h"
#include "gui/GuiDraw.h"
#include "gui/Widget.h"
#include "util/TextureId.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

namespace RBX {

extern const char sLegacyHopperService[];

extern const char sStarterPackService[];

extern const char sHopperBin[];

extern const char sBackpackItem[];

// SIZE 0x168
class BackpackItem : public DescribedNonCreatable<BackpackItem, Widget, sBackpackItem>
{
private:
	GuiDrawImage guiImageDraw; // 0x110
	TextureId textureId;       // 0x148

	bool inBackpack();

	virtual bool askSetParent(const Instance* instance) const { return true; }
	virtual bool askAddChild(const Instance* instance) const { return true; }

protected:
	virtual G3D::Vector2 getSize() const;
	virtual void render2d(Adorn* adorn);

	// FUNCTION: WEBSERVICE 0x100c7fd0
	// FUNCTION: ROBLOX 0x005e20e0
	virtual bool isEnabled() { return inBackpack(); }

	int getBinId() const;

public:
	void setTextureId(const TextureId& value);
	const TextureId getTextureId() const;

	virtual bool drawEnabled() const { return true; }   // vtable+0x7c
	virtual bool drawSelected() const { return false; } // vtable+0x80
	virtual void onLocalClicked() {}                    // vtable+0x84
	virtual void onLocalOtherClicked() {}               // vtable+0x88

	// SYNTHETIC: WEBSERVICE 0x100c7e40
	// SYNTHETIC: ROBLOX 0x005e2010
	// RBX::BackpackItem::~BackpackItem

	// SYNTHETIC: WEBSERVICE 0x100c93b0
	// SYNTHETIC: ROBLOX 0x005e3460
	// RBX::BackpackItem::BackpackItem

	// SYNTHETIC: WEBSERVICE 0x100c9500
	// RBX::BackpackItem::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(BackpackItem, 0x168)

// SIZE 0x170
class HopperBin : public DescribedCreatable<HopperBin, BackpackItem, sHopperBin>
{
public:
	enum BinType
	{
		SCRIPT_BIN = 0,
		GAME_TOOL = 1,
		GRAB_TOOL = 2,
		CLONE_TOOL = 3,
		HAMMER_TOOL = 4,
		SLINGSHOT_TOOL = 5,
		ROCKET_TOOL = 6,
		LASER_TOOL = 7,
	};

private:
	BinType binType; // 0x168
	bool active;     // 0x16c

	void onSelectScript();
	void onSelectCommand();
	int getCursor();

	// FUNCTION: WEBSERVICE 0x100c9ca0
	virtual bool drawSelected() const { return active; }

public:
	HopperBin();

	// FUNCTION: WEBSERVICE 0x100c70e0
	BinType getBinType() const { return binType; }

	void setBinType(const BinType value);

	virtual void onLocalClicked();
	virtual void onLocalOtherClicked();

	void setLegacyCommand(const std::string& text);
	void setLegacyTextureName(const std::string& value);
};

DECOMP_SIZE_ASSERT(HopperBin, 0x170)

extern const char sHopper[];

// SIZE 0x130
class Hopper : public RelativePanel
{
public:
	Hopper();

protected:
	virtual bool askAddChild(const Instance* instance) const;
	virtual bool askSetParent(const Instance* instance) const;
};

DECOMP_SIZE_ASSERT(Hopper, 0x130)

// SIZE 0x130
class StarterPackService : public DescribedCreatable<StarterPackService, Hopper, sStarterPackService>, public Service
{
public:
	StarterPackService();

	virtual void render2d(Adorn* adorn);
};

DECOMP_SIZE_ASSERT(StarterPackService, 0x130)

// SIZE 0x130
class LegacyHopperService : public DescribedCreatable<LegacyHopperService, Hopper, sLegacyHopperService>, public Service
{
private:
	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);

public:
	LegacyHopperService();
	virtual ~LegacyHopperService();

	// SYNTHETIC: WEBSERVICE 0x100c7dc0
	// RBX::LegacyHopperService::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(LegacyHopperService, 0x130)

// clang-format off
// TEMPLATE: WEBSERVICE 0x100c73f0
// TEMPLATE: ROBLOX 0x005e1660
// RBX::NonFactoryProduct<RBX::Widget,&RBX::sBackpackItem>::NonFactoryProduct<RBX::Widget,&RBX::sBackpackItem>
// SYNTHETIC: WEBSERVICE 0x1021bf30
// RBX::`dynamic initializer for 'desc_TextureId''
// SYNTHETIC: WEBSERVICE 0x1021bf70
// RBX::`dynamic initializer for 'desc_BinType''
// SYNTHETIC: WEBSERVICE 0x1021bfb0
// RBX::`dynamic initializer for 'desc_Selected''
// SYNTHETIC: WEBSERVICE 0x1021bfd0
// RBX::`dynamic initializer for 'desc_Deselected''
// SYNTHETIC: WEBSERVICE 0x1021c000
// RBX::`dynamic initializer for 'desc_legacyCommand''
// SYNTHETIC: WEBSERVICE 0x1021c030
// RBX::`dynamic initializer for 'desc_legacyTextureName''
// SYNTHETIC: WEBSERVICE 0x10223b50
// RBX::`dynamic atexit destructor for 'desc_BinType''
// SYNTHETIC: WEBSERVICE 0x10223b70
// RBX::`dynamic atexit destructor for 'desc_TextureId''
// SYNTHETIC: WEBSERVICE 0x10223b90
// RBX::`dynamic atexit destructor for 'desc_legacyTextureName''
// SYNTHETIC: WEBSERVICE 0x10223bb0
// RBX::`dynamic atexit destructor for 'desc_legacyCommand''
// SYNTHETIC: WEBSERVICE 0x10223bf0
// RBX::`dynamic atexit destructor for 'desc_Deselected''
// SYNTHETIC: WEBSERVICE 0x10223c00
// RBX::`dynamic atexit destructor for 'desc_Selected''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_HOPPER_H
