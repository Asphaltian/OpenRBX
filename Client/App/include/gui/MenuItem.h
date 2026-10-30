#ifndef GUI_MENUITEM_H
#define GUI_MENUITEM_H

#include "decomp.h"
#include "gui/Widget.h"

#include <string>

namespace RBX {

class Verb;

// SIZE 0x130
class MenuItem : public VerbWidget
{
private:
	std::string accelerator; // 0x114

protected:
	virtual bool canLoseFocus() { return true; }

public:
	MenuItem(Verb* p_unk0x04, const std::string& p_unk0x08, std::string p_unk0x0c);
	MenuItem(Verb* p_unk0x04, const std::string& p_unk0x08);

	virtual void render2d(Adorn* p_unk0x04);

	// SYNTHETIC: ROBLOX 0x00657930
	// RBX::MenuItem::~MenuItem
	// SYNTHETIC: ROBLOX 0x006579f0
	// RBX::MenuItem::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(MenuItem, 0x130)

} // namespace RBX

#endif // GUI_MENUITEM_H
