#ifndef GUI_BUTTON_H
#define GUI_BUTTON_H

#include "gui/GuiDraw.h"
#include "gui/Widget.h"
#include "placeholder.h"

#include <SDL_keysym.h>
#include <string>

namespace RBX {

class Verb;

class KeyButton : public Widget
{
private:
	SDLKey keyCode;

protected:
	bool keyIsDown;

	virtual void onDown(const UIEvent& event);
	virtual void onUp(const UIEvent& event);
	virtual bool isEnabled() { return isVisible(); }
	virtual GuiResponse process(const GuiEvent& p_unk0x04);

public:
	KeyButton(const std::string& p_unk0x04, SDLKey p_unk0x08);
};

class ImageKeyButton : public KeyButton
{
private:
	GuiDrawImage imageDraw;

public:
	ImageKeyButton(Adorn* p_unk0x04, const std::string& p_unk0x08, SDLKey p_unk0x0c);

	virtual void render2d(Adorn* adorn);

	virtual G3D::Vector2 getSize() const { return imageDraw.getImageSize(); }
};

class ImageWidget : public Widget
{
private:
	GuiDrawImage guiImageDraw;

public:
	ImageWidget(Adorn* p_unk0x04, const std::string& p_unk0x08);

	virtual void render2d(Adorn* adorn);

	virtual G3D::Vector2 getSize() const { return guiImageDraw.getImageSize(); }
};

class ImageButton : public VerbWidget
{
protected:
	GuiDrawImage guiImageDraw;

public:
	ImageButton(Adorn* p_unk0x04, Verb* p_unk0x08, const std::string& p_unk0x0c);

	virtual void render2d(Adorn* adorn);

	void setSize(const G3D::Vector2& p_unk0x04) { guiImageDraw.setImageSize(p_unk0x04); }

	virtual G3D::Vector2 getSize() const { return guiImageDraw.getImageSize(); }
};

} // namespace RBX

#endif // GUI_BUTTON_H
