#include "gui/Button.h"

#include "util/UserInputBase.h"
#include "v8tree/Verb.h"

namespace RBX {

// FUNCTION: ROBLOX 0x00657bf0
GuiResponse KeyButton::process(const GuiEvent& p_unk0x04)
{
	if (p_unk0x04.eventType == UIEvent::KEY_DOWN && p_unk0x04.key == keyCode) {
		keyIsDown = true;
	}

	if (p_unk0x04.eventType == UIEvent::KEY_UP && p_unk0x04.key == keyCode) {
		keyIsDown = false;
	}

	return Widget::process(p_unk0x04);
}

// FUNCTION: ROBLOX 0x00657c30
void KeyButton::onDown(const UIEvent& p_unk0x04)
{
	p_unk0x04.userInput->setKeyState(keyCode, true);
}

// FUNCTION: ROBLOX 0x00657c50
void KeyButton::onUp(const UIEvent& p_unk0x04)
{
	p_unk0x04.userInput->setKeyState(keyCode, false);
}

// FUNCTION: ROBLOX 0x00657c70
void ImageKeyButton::render2d(Adorn* p_unk0x04)
{
	if (isVisible()) {
		imageDraw.render2d(
			p_unk0x04,
			isEnabled(),
			const_cast<Rect&>(static_cast<const Rect&>(getMyRect())),
			keyIsDown ? DOWN_OVER : widgetState
		);
	}
}

// FUNCTION: ROBLOX 0x00657cd0
void ImageWidget::render2d(Adorn* p_unk0x04)
{
	if (isVisible()) {
		guiImageDraw.render2d(
			p_unk0x04, isEnabled(), const_cast<Rect&>(static_cast<const Rect&>(getMyRect())), NOTHING
		);
	}
}

// FUNCTION: ROBLOX 0x00657d20
void ImageButton::render2d(Adorn* p_unk0x04)
{
	if (isVisible()) {
		guiImageDraw.render2d(
			p_unk0x04, isEnabled(), const_cast<Rect&>(static_cast<const Rect&>(getMyRect())), widgetState
		);
	}
}

// FUNCTION: ROBLOX 0x00657d70
KeyButton::KeyButton(const std::string& p_unk0x04, SDLKey p_unk0x08) : keyCode(p_unk0x08), keyIsDown(false)
{
	setName(p_unk0x04);
}

// FUNCTION: ROBLOX 0x00657e20
ImageWidget::ImageWidget(Adorn* p_unk0x04, const std::string& p_unk0x08) : guiImageDraw(p_unk0x04, p_unk0x08)
{
	setName(p_unk0x08);
}

// FUNCTION: ROBLOX 0x00657f80
ImageButton::ImageButton(Adorn* p_unk0x04, Verb* p_unk0x08, const std::string& p_unk0x0c)
	: VerbWidget(p_unk0x08), guiImageDraw(p_unk0x04, p_unk0x08->getName().toString())
{
	setName(p_unk0x0c);
}

// FUNCTION: ROBLOX 0x00658140
ImageKeyButton::ImageKeyButton(Adorn* p_unk0x04, const std::string& p_unk0x08, SDLKey p_unk0x0c)
	: KeyButton(p_unk0x08, p_unk0x0c), imageDraw(p_unk0x04, p_unk0x08)
{
}

} // namespace RBX
