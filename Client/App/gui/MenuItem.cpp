#include "gui/MenuItem.h"

#include "v8tree/Verb.h"

namespace RBX {

// FUNCTION: ROBLOX 0x00657850
MenuItem::MenuItem(Verb* p_unk0x04, const std::string& p_unk0x08) : VerbWidget(p_unk0x04)
{
	setName(p_unk0x08);
	setGuiSize(G3D::Vector2(200, 24));
}

// FUNCTION: ROBLOX 0x00657a10
void MenuItem::render2d(Adorn* p_unk0x04)
{
	if (widgetState == HOVER || widgetState == DOWN_OVER) {
		p_unk0x04->rect2d(getMyRect2D(), menuSelect());
	}

	bool enabled = isEnabled();
	const G3D::Color4& fontColor = isEnabled() ? menuEnabledFill() : menuDisabledFill();

	label2d(
		p_unk0x04,
		getVerb() != NULL && getVerb()->isChecked() ? "x " + getTitle() : getTitle(),
		fontColor,
		G3D::Color4::clear(),
		Adorn::XALIGN_LEFT
	);

	if (accelerator.size() > 0) {
		label2d(
			p_unk0x04,
			accelerator,
			isEnabled() ? menuEnabledFill() : menuDisabledFill(),
			G3D::Color4::clear(),
			Adorn::XALIGN_RIGHT
		);
	}
}

} // namespace RBX
