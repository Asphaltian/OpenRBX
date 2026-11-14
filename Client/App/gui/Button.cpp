#include "gui/Button.h"

#include "util/UserInputBase.h"
#include "v8tree/Verb.h"

namespace RBX {

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

void KeyButton::onDown(const UIEvent& event)
{
	event.userInput->setKeyState(keyCode, true);
}

void KeyButton::onUp(const UIEvent& event)
{
	event.userInput->setKeyState(keyCode, false);
}

void ImageKeyButton::render2d(Adorn* adorn)
{
	if (isVisible()) {
		imageDraw.render2d(
			adorn,
			isEnabled(),
			const_cast<Rect&>(static_cast<const Rect&>(getMyRect())),
			keyIsDown ? DOWN_OVER : widgetState
		);
	}
}

void ImageWidget::render2d(Adorn* adorn)
{
	if (isVisible()) {
		guiImageDraw.render2d(adorn, isEnabled(), const_cast<Rect&>(static_cast<const Rect&>(getMyRect())), NOTHING);
	}
}

void ImageButton::render2d(Adorn* adorn)
{
	if (isVisible()) {
		guiImageDraw.render2d(
			adorn, isEnabled(), const_cast<Rect&>(static_cast<const Rect&>(getMyRect())), widgetState
		);
	}
}

KeyButton::KeyButton(const std::string& p_unk0x04, SDLKey p_unk0x08) : keyCode(p_unk0x08), keyIsDown(false)
{
	setName(p_unk0x04);
}

ImageWidget::ImageWidget(Adorn* p_unk0x04, const std::string& p_unk0x08) : guiImageDraw(p_unk0x04, p_unk0x08)
{
	setName(p_unk0x08);
}

ImageButton::ImageButton(Adorn* p_unk0x04, Verb* p_unk0x08, const std::string& p_unk0x0c)
	: VerbWidget(p_unk0x08), guiImageDraw(p_unk0x04, p_unk0x08->getName().toString())
{
	setName(p_unk0x0c);
}

ImageKeyButton::ImageKeyButton(Adorn* p_unk0x04, const std::string& p_unk0x08, SDLKey p_unk0x0c)
	: KeyButton(p_unk0x08, p_unk0x0c), imageDraw(p_unk0x04, p_unk0x08)
{
}

} // namespace RBX
