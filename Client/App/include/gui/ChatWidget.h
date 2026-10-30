#ifndef GUI_CHATWIDGET_H
#define GUI_CHATWIDGET_H

#include "decomp.h"
#include "gui/GUI.h"
#include "gui/GuiDraw.h"
#include "gui/Widget.h"

#include <string>

namespace RBX {

// SIZE 0x148
class UnifiedImageWidget : public UnifiedWidget
{
protected:
	GuiDrawImage guiImageDraw; // 0x110

public:
	// FUNCTION: ROBLOX 0x006133c0
	UnifiedImageWidget(Adorn* adorn, const std::string& image) : guiImageDraw(adorn, image) {}

	Widget::WidgetState getWidgetState() const
	{
		switch (getMenuState()) {
		case HOVER:
			return Widget::HOVER;
		case SHOWN_APPEARING:
		case SHOWN:
			return Widget::DOWN_OVER;
		default:
			return Widget::NOTHING;
		}
	}

	virtual void render2dMe(Adorn* p_unk0x04);

	void setSize(const G3D::Vector2& p_unk0x04) { guiImageDraw.setImageSize(p_unk0x04); }
	virtual G3D::Vector2 getSize() const { return guiImageDraw.getImageSize(); }
};

DECOMP_SIZE_ASSERT(UnifiedImageWidget, 0x148)

// SIZE 0x148
class ChatButton : public UnifiedImageWidget
{
private:
	virtual bool isVisible() const;

public:
	// FUNCTION: ROBLOX 0x006134a0
	ChatButton(Adorn* adorn, const std::string& image) : UnifiedImageWidget(adorn, image) {}
};

DECOMP_SIZE_ASSERT(ChatButton, 0x148)

// SIZE 0x12c
class ChatWidget : public UnifiedWidget
{
private:
	std::string findMenuString(GuiItem* p_unk0x04);

	std::string code; // 0x110

	virtual void onMenuStateChanged();
	virtual GuiResponse process(const GuiEvent& p_unk0x04);

public:
	ChatWidget(const std::string& p_unk0x04, std::string p_unk0x08);

	// SYNTHETIC: ROBLOX 0x00657660
	// RBX::ChatWidget::~ChatWidget
	// SYNTHETIC: ROBLOX 0x006576b0
	// RBX::ChatWidget::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(ChatWidget, 0x12c)

} // namespace RBX

#endif // GUI_CHATWIDGET_H
