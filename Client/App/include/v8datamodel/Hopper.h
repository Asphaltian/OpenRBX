#ifndef V8DATAMODEL_HOPPER_H
#define V8DATAMODEL_HOPPER_H

#include "gui/GuiDraw.h"
#include "gui/Widget.h"
#include "placeholder.h"
#include "util/TextureId.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

namespace RBX {

extern const char sLegacyHopperService[];

extern const char sStarterPackService[];

extern const char sHopperBin[];

extern const char sBackpackItem[];

class BackpackItem : public DescribedNonCreatable<BackpackItem, Widget, sBackpackItem>
{
private:
	GuiDrawImage guiImageDraw;
	TextureId textureId;

	bool inBackpack();

	virtual bool askSetParent(const Instance* instance) const { return true; }
	virtual bool askAddChild(const Instance* instance) const { return true; }

protected:
	virtual G3D::Vector2 getSize() const;
	virtual void render2d(Adorn* adorn);

	virtual bool isEnabled() { return inBackpack(); }

	int getBinId() const;

public:
	void setTextureId(const TextureId& value);
	const TextureId getTextureId() const;

	virtual bool drawEnabled() const { return true; }
	virtual bool drawSelected() const { return false; }
	virtual void onLocalClicked() {}
	virtual void onLocalOtherClicked() {}
};

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
	BinType binType;
	bool active;

	void onSelectScript();
	void onSelectCommand();
	int getCursor();

	virtual bool drawSelected() const { return active; }

public:
	HopperBin();

	BinType getBinType() const { return binType; }

	void setBinType(const BinType value);

	virtual void onLocalClicked();
	virtual void onLocalOtherClicked();

	void setLegacyCommand(const std::string& text);
	void setLegacyTextureName(const std::string& value);
};

extern const char sHopper[];

class Hopper : public RelativePanel
{
public:
	Hopper();

protected:
	virtual bool askAddChild(const Instance* instance) const;
	virtual bool askSetParent(const Instance* instance) const;
};

class StarterPackService : public DescribedCreatable<StarterPackService, Hopper, sStarterPackService>, public Service
{
public:
	StarterPackService();

	virtual void render2d(Adorn* adorn);
};

class LegacyHopperService : public DescribedCreatable<LegacyHopperService, Hopper, sLegacyHopperService>, public Service
{
private:
	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);

public:
	LegacyHopperService();
	virtual ~LegacyHopperService();
};

} // namespace RBX

#endif // V8DATAMODEL_HOPPER_H
