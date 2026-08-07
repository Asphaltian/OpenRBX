#include "gui/Widget.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x10116aa0
GuiResponse Widget::processMouse(const GuiEvent& event)
{
	bool inRect = getMyRect().pointInRect(event.mousePosition);

	switch (widgetState) {
	case NOTHING:
	case HOVER:
		if (inRect) {
			if (!isEnabled()) {
				widgetState = NOTHING;

				return GuiResponse::used();
			}

			switch (event.eventType) {
			case UIEvent::MOUSE_LEFT_BUTTON_DOWN:
				widgetState = DOWN_OVER;
				onDown(event);
				break;

			case UIEvent::MOUSE_MOVE:
				widgetState = HOVER;
				break;

			default:
				return GuiResponse::notUsed();
			}

			return GuiResponse::used();
		}
		else {
			widgetState = NOTHING;

			return GuiResponse::notUsed();
		}
		break;

	case DOWN_OVER:
		switch (event.eventType) {
		case UIEvent::MOUSE_LEFT_BUTTON_DOWN:
			return GuiResponse::used();

		case UIEvent::MOUSE_MOVE:
			if (!inRect) {
				widgetState = onDrag(event) ? NOTHING : DOWN_AWAY;
			}

			return GuiResponse::used();

		case UIEvent::MOUSE_LEFT_BUTTON_UP:
			widgetState = NOTHING;
			onUp(event);

			if (inRect) {
				onClick(event);

				return GuiResponse::finished();
			}

			return GuiResponse::notUsed();
		}
		break;

	case DOWN_AWAY:
		switch (event.eventType) {
		case UIEvent::MOUSE_LEFT_BUTTON_DOWN:
			return GuiResponse::used();

		case UIEvent::MOUSE_MOVE:
			widgetState = inRect ? DOWN_OVER : DOWN_AWAY;

			return GuiResponse::used();

		case UIEvent::MOUSE_LEFT_BUTTON_UP:
			widgetState = NOTHING;
			onUp(event);

			if (inRect) {
				onClick(event);

				return GuiResponse::used();
			}

			return GuiResponse::notUsed();
		}
		break;
	}

	return GuiResponse::notUsed();
}

// FUNCTION: WEBSERVICE 0x10116cd0
GuiResponse Widget::process(const GuiEvent& event)
{
	if (isEnabled() && event.isMouseEvent()) {
		return processMouse(event);
	}

	return GuiResponse::notUsed();
}

// FUNCTION: WEBSERVICE 0x10116d40
Widget::Widget() : widgetState(NOTHING)
{
	setGuiSize(G3D::Vector2(100, 24));
}

// STUB: WEBSERVICE 0x10116dd0
void Widget::render2d(Adorn* adorn)
{
	STUB(0x10116dd0);
}

} // namespace RBX
