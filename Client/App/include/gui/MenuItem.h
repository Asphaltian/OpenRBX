#ifndef GUI_MENUITEM_H
#define GUI_MENUITEM_H

#include "gui/Widget.h"
#include "placeholder.h"

#include <string>

namespace RBX {

class Verb;

class MenuItem : public VerbWidget
{
private:
	std::string accelerator;

protected:
	virtual bool canLoseFocus() { return true; }

public:
	MenuItem(Verb* p_unk0x04, const std::string& p_unk0x08, std::string p_unk0x0c);
	MenuItem(Verb* p_unk0x04, const std::string& p_unk0x08);

	virtual void render2d(Adorn* adorn);
};

} // namespace RBX

#endif // GUI_MENUITEM_H
