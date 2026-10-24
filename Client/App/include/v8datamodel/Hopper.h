#ifndef V8DATAMODEL_HOPPER_H
#define V8DATAMODEL_HOPPER_H

#include "decomp.h"
#include "gui/GuiDraw.h"
#include "gui/Widget.h"
#include "util/TextureId.h"
#include "v8tree/Instance.h"

namespace RBX {

extern const char sLegacyHopperService[];

extern const char sStarterPackService[];

extern const char sHopperBin[];

extern const char sBackpackItem[];

// SIZE 0x168
class BackpackItem : public DescribedNonCreatable<BackpackItem, Widget, sBackpackItem>
{
public:
	const TextureId& getTextureId() const { return textureId; }

private:
	GuiDrawImage guiImageDraw; // 0x110
	TextureId textureId;       // 0x148
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

	virtual ~HopperBin();

	BinType getBinType() const { return binType; }

private:
	BinType binType; // 0x168
	bool active;     // 0x16c
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

} // namespace RBX

#endif // V8DATAMODEL_HOPPER_H
