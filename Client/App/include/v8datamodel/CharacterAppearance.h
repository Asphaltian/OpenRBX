#ifndef V8DATAMODEL_CHARACTERAPPEARANCE_H
#define V8DATAMODEL_CHARACTERAPPEARANCE_H

#include "humanoid/Humanoid.h"
#include "network/Players.h"
#include "placeholder.h"
#include "reflection/property.h"
#include "util/TextureId.h"
#include "v8datamodel/BrickColor.h"
#include "v8tree/Instance.h"

namespace RBX {

extern const char sSkin[];

extern const char sBodyColors[];

extern const char sShirt[];

extern const char sShirtGraphic[];

class __declspec(novtable) CharacterAppearance : public Instance
{
private:
	virtual void apply(Humanoid* humanoid) = 0;

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

class ShirtGraphic : public DescribedCreatable<ShirtGraphic, CharacterAppearance, sShirtGraphic>
{
private:
	TextureId graphic;

public:
	static Reflection::BoundProp<TextureId, 1> prop_Graphic;

	ShirtGraphic();

protected:
	virtual void apply(Humanoid* humanoid);

private:
	void dataChanged(const Reflection::PropertyDescriptor&) { CharacterAppearance::apply(); }
};

class Shirt : public DescribedCreatable<Shirt, ShirtGraphic, sShirt>
{
private:
	BrickColor leftSleeveColor;
	BrickColor rightSleeveColor;
	BrickColor torsoColor;

public:
	static Reflection::BoundProp<BrickColor, 1> prop_leftSleeveColor;
	static Reflection::BoundProp<BrickColor, 1> prop_rightSleeveColor;
	static Reflection::BoundProp<BrickColor, 1> prop_torsoColor;

	Shirt();

private:
	virtual void apply(Humanoid* humanoid);

	void dataChanged(const Reflection::PropertyDescriptor&) { CharacterAppearance::apply(); }
};

class Skin : public DescribedCreatable<Skin, CharacterAppearance, sSkin>
{
private:
	BrickColor skinColor;

public:
	static Reflection::BoundProp<BrickColor, 1> prop_skinColor;

	Skin();

private:
	virtual void apply(Humanoid* humanoid);

	void dataChanged(const Reflection::PropertyDescriptor&) { CharacterAppearance::apply(); }
};

class BodyColors : public DescribedCreatable<BodyColors, CharacterAppearance, sBodyColors>
{
private:
	BrickColor headColor;
	BrickColor leftArmColor;
	BrickColor rightArmColor;
	BrickColor torsoColor;
	BrickColor leftLegColor;
	BrickColor rightLegColor;

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

	void dataChanged(const Reflection::PropertyDescriptor&) { CharacterAppearance::apply(); }
};

} // namespace RBX

#endif // V8DATAMODEL_CHARACTERAPPEARANCE_H
