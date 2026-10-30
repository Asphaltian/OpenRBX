#ifndef GUI_BUTTON_H
#define GUI_BUTTON_H

#include "decomp.h"
#include "gui/GuiDraw.h"
#include "gui/Widget.h"

#include <SDL_keysym.h>
#include <string>

namespace RBX {

class Verb;

// SIZE 0x118
class KeyButton : public Widget
{
private:
	SDLKey keyCode; // 0x110

protected:
	bool keyIsDown; // 0x114

	virtual void onDown(const UIEvent& p_unk0x04);
	virtual void onUp(const UIEvent& p_unk0x04);
	virtual bool isEnabled() { return true; }
	virtual GuiResponse process(const GuiEvent& p_unk0x04);

public:
	KeyButton(const std::string& p_unk0x04, SDLKey p_unk0x08);
};

DECOMP_SIZE_ASSERT(KeyButton, 0x118)

// SIZE 0x150
class ImageKeyButton : public KeyButton
{
private:
	GuiDrawImage imageDraw; // 0x118

public:
	ImageKeyButton(Adorn* p_unk0x04, const std::string& p_unk0x08, SDLKey p_unk0x0c);

	virtual void render2d(Adorn* p_unk0x04);

	// FUNCTION: ROBLOX 0x006581f0
	virtual G3D::Vector2 getSize() const { return imageDraw.getImageSize(); }

	// SYNTHETIC: ROBLOX 0x00658270
	// RBX::ImageKeyButton::~ImageKeyButton
	// SYNTHETIC: ROBLOX 0x006582c0
	// RBX::ImageKeyButton::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(ImageKeyButton, 0x150)

// SIZE 0x148
class ImageWidget : public Widget
{
private:
	GuiDrawImage guiImageDraw; // 0x110

public:
	ImageWidget(Adorn* p_unk0x04, const std::string& p_unk0x08);

	virtual void render2d(Adorn* p_unk0x04);

	// FUNCTION: ROBLOX 0x00657ee0
	virtual G3D::Vector2 getSize() const { return guiImageDraw.getImageSize(); }

	// SYNTHETIC: ROBLOX 0x00657f30
	// RBX::ImageWidget::~ImageWidget
	// SYNTHETIC: ROBLOX 0x006582e0
	// RBX::ImageWidget::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(ImageWidget, 0x148)

// SIZE 0x14c
class ImageButton : public VerbWidget
{
protected:
	GuiDrawImage guiImageDraw; // 0x114

public:
	ImageButton(Adorn* p_unk0x04, Verb* p_unk0x08, const std::string& p_unk0x0c);

	virtual void render2d(Adorn* p_unk0x04);

	void setSize(const G3D::Vector2& p_unk0x04) { guiImageDraw.setImageSize(p_unk0x04); }

	// FUNCTION: ROBLOX 0x00658050
	virtual G3D::Vector2 getSize() const { return guiImageDraw.getImageSize(); }

	// SYNTHETIC: ROBLOX 0x006580f0
	// RBX::ImageButton::~ImageButton
	// SYNTHETIC: ROBLOX 0x00658300
	// RBX::ImageButton::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(ImageButton, 0x14c)

} // namespace RBX

#endif // GUI_BUTTON_H
