#ifndef V8DATAMODEL_CHARACTERAPPEARANCE_H
#define V8DATAMODEL_CHARACTERAPPEARANCE_H

#include "decomp.h"
#include "humanoid/Humanoid.h"
#include "network/Players.h"
#include "reflection/property.h"
#include "util/TextureId.h"
#include "v8datamodel/BrickColor.h"
#include "v8tree/Instance.h"

namespace RBX {

extern const char sSkin[];

extern const char sBodyColors[];

extern const char sShirt[];

extern const char sShirtGraphic[];

// SIZE 0xf8
class __declspec(novtable) CharacterAppearance : public Instance
{
private:
	virtual void apply(Humanoid* humanoid) = 0; // vtable+0x40

public:
	void apply()
	{
		if (Network::Players::backendProcessing(this, false)) {
			if (Humanoid* humanoid = Humanoid::modelIsCharacter(getParent())) {
				apply(humanoid);
			}
		}
	}

protected:
	virtual void onAncestorChanged(const AncestorChanged& event);
	virtual bool askSetParent(const Instance* instance) const;
};

DECOMP_SIZE_ASSERT(CharacterAppearance, 0xf8)

// SIZE 0x118
class ShirtGraphic : public DescribedCreatable<ShirtGraphic, CharacterAppearance, sShirtGraphic>
{
private:
	TextureId graphic; // 0x0f8

public:
	static Reflection::BoundProp<TextureId, 1> prop_Graphic;

	ShirtGraphic();

protected:
	virtual void apply(Humanoid* humanoid);

private:
	// FUNCTION: WEBSERVICE 0x100e0460 FOLDED
	// FUNCTION: ROBLOX 0x005e5f90 FOLDED
	void dataChanged(const Reflection::PropertyDescriptor&) { CharacterAppearance::apply(); }
};

DECOMP_SIZE_ASSERT(ShirtGraphic, 0x118)

// SIZE 0x124
class Shirt : public DescribedCreatable<Shirt, ShirtGraphic, sShirt>
{
private:
	BrickColor leftSleeveColor;  // 0x118
	BrickColor rightSleeveColor; // 0x11c
	BrickColor torsoColor;       // 0x120

public:
	static Reflection::BoundProp<BrickColor, 1> prop_leftSleeveColor;
	static Reflection::BoundProp<BrickColor, 1> prop_rightSleeveColor;
	static Reflection::BoundProp<BrickColor, 1> prop_torsoColor;

	Shirt();

private:
	virtual void apply(Humanoid* humanoid);

	// FUNCTION: WEBSERVICE 0x100e0460 FOLDED
	// FUNCTION: ROBLOX 0x005e5f90 FOLDED
	void dataChanged(const Reflection::PropertyDescriptor&) { CharacterAppearance::apply(); }
};

DECOMP_SIZE_ASSERT(Shirt, 0x124)

// SIZE 0xfc
class Skin : public DescribedCreatable<Skin, CharacterAppearance, sSkin>
{
private:
	BrickColor skinColor; // 0x0f8

public:
	static Reflection::BoundProp<BrickColor, 1> prop_skinColor;

	Skin();

private:
	virtual void apply(Humanoid* humanoid);

	// FUNCTION: WEBSERVICE 0x100e0460 FOLDED
	// FUNCTION: ROBLOX 0x005e5f90 FOLDED
	void dataChanged(const Reflection::PropertyDescriptor&) { CharacterAppearance::apply(); }
};

DECOMP_SIZE_ASSERT(Skin, 0xfc)

// SIZE 0x110
class BodyColors : public DescribedCreatable<BodyColors, CharacterAppearance, sBodyColors>
{
private:
	BrickColor headColor;     // 0x0f8
	BrickColor leftArmColor;  // 0x0fc
	BrickColor rightArmColor; // 0x100
	BrickColor torsoColor;    // 0x104
	BrickColor leftLegColor;  // 0x108
	BrickColor rightLegColor; // 0x10c

public:
	static Reflection::BoundProp<BrickColor, 1> prop_HeadColor;
	static Reflection::BoundProp<BrickColor, 1> prop_LeftArmColor;
	static Reflection::BoundProp<BrickColor, 1> prop_RightArmColor;
	static Reflection::BoundProp<BrickColor, 1> prop_TorsoColor;
	static Reflection::BoundProp<BrickColor, 1> prop_LeftLegColor;
	static Reflection::BoundProp<BrickColor, 1> prop_RightLegColor;

	BodyColors();

private:
	virtual void apply(Humanoid* humanoid);

	// FUNCTION: WEBSERVICE 0x100e0460 FOLDED
	// FUNCTION: ROBLOX 0x005e5f90 FOLDED
	void dataChanged(const Reflection::PropertyDescriptor&) { CharacterAppearance::apply(); }
};

DECOMP_SIZE_ASSERT(BodyColors, 0x110)

// clang-format off
// FUNCTION: WEBSERVICE 0x100e0930
// RBX::ShirtGraphic::~ShirtGraphic
// SYNTHETIC: WEBSERVICE 0x100e15a0
// RBX::ShirtGraphic::`scalar deleting destructor'
// SYNTHETIC: WEBSERVICE 0x1021c9c0
// `dynamic initializer for 'RBX::ShirtGraphic::prop_Graphic''
// SYNTHETIC: WEBSERVICE 0x1021c9f0
// `dynamic initializer for 'RBX::Shirt::prop_leftSleeveColor''
// SYNTHETIC: WEBSERVICE 0x1021ca20
// SYNTHETIC: ROBLOX 0x007ec090
// `dynamic initializer for 'RBX::Shirt::prop_rightSleeveColor''
// SYNTHETIC: WEBSERVICE 0x1021ca50
// SYNTHETIC: ROBLOX 0x007ec0c0
// `dynamic initializer for 'RBX::Shirt::prop_torsoColor''
// SYNTHETIC: WEBSERVICE 0x1021ca80
// `dynamic initializer for 'RBX::Skin::prop_skinColor''
// SYNTHETIC: WEBSERVICE 0x1021cab0
// `dynamic initializer for 'RBX::BodyColors::prop_HeadColor''
// SYNTHETIC: WEBSERVICE 0x1021cae0
// SYNTHETIC: ROBLOX 0x007ec150
// `dynamic initializer for 'RBX::BodyColors::prop_LeftArmColor''
// SYNTHETIC: WEBSERVICE 0x1021cb10
// SYNTHETIC: ROBLOX 0x007ec180
// `dynamic initializer for 'RBX::BodyColors::prop_RightArmColor''
// SYNTHETIC: WEBSERVICE 0x1021cb40
// SYNTHETIC: ROBLOX 0x007ec1b0
// `dynamic initializer for 'RBX::BodyColors::prop_TorsoColor''
// SYNTHETIC: WEBSERVICE 0x1021cb70
// SYNTHETIC: ROBLOX 0x007ec1e0
// `dynamic initializer for 'RBX::BodyColors::prop_LeftLegColor''
// SYNTHETIC: WEBSERVICE 0x1021cba0
// SYNTHETIC: ROBLOX 0x007ec210
// `dynamic initializer for 'RBX::BodyColors::prop_RightLegColor''
// SYNTHETIC: WEBSERVICE 0x10224000
// SYNTHETIC: ROBLOX 0x007f4090
// `dynamic atexit destructor for 'RBX::Shirt::prop_torsoColor''
// SYNTHETIC: WEBSERVICE 0x10224020
// `dynamic atexit destructor for 'RBX::Skin::prop_skinColor''
// SYNTHETIC: WEBSERVICE 0x10224040
// SYNTHETIC: ROBLOX 0x007f40d0
// `dynamic atexit destructor for 'RBX::BodyColors::prop_LeftArmColor''
// SYNTHETIC: WEBSERVICE 0x10224060
// `dynamic atexit destructor for 'RBX::BodyColors::prop_HeadColor''
// SYNTHETIC: WEBSERVICE 0x10224080
// SYNTHETIC: ROBLOX 0x007f4110
// `dynamic atexit destructor for 'RBX::BodyColors::prop_RightArmColor''
// SYNTHETIC: WEBSERVICE 0x102240a0
// SYNTHETIC: ROBLOX 0x007f4130
// `dynamic atexit destructor for 'RBX::BodyColors::prop_TorsoColor''
// SYNTHETIC: WEBSERVICE 0x102240c0
// SYNTHETIC: ROBLOX 0x007f4150
// `dynamic atexit destructor for 'RBX::BodyColors::prop_LeftLegColor''
// SYNTHETIC: WEBSERVICE 0x102240e0
// SYNTHETIC: ROBLOX 0x007f4170
// `dynamic atexit destructor for 'RBX::BodyColors::prop_RightLegColor''
// SYNTHETIC: WEBSERVICE 0x10224100
// SYNTHETIC: ROBLOX 0x007f4190
// `dynamic atexit destructor for 'RBX::Shirt::prop_rightSleeveColor''
// SYNTHETIC: WEBSERVICE 0x10224120
// `dynamic atexit destructor for 'RBX::Shirt::prop_leftSleeveColor''
// SYNTHETIC: WEBSERVICE 0x10224140
// `dynamic atexit destructor for 'RBX::ShirtGraphic::prop_Graphic''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_CHARACTERAPPEARANCE_H
