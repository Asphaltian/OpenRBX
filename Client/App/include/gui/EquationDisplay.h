#ifndef GUI_EQUATIONDISPLAY_H
#define GUI_EQUATIONDISPLAY_H

#include "decomp.h"
#include "gui/GUI.h"

#include <string>

namespace RBX {

// SIZE 0x16c
class EquationDisplay : public TextDisplay
{
private:
	std::string equation; // 0x150

protected:
	virtual std::string getLabel() const; // vtable+0x64

public:
	EquationDisplay(const std::string& name, const std::string& label, const std::string& equation);
	EquationDisplay(const std::string& name, const std::string& equation);

	// SYNTHETIC: ROBLOX 0x0065f8a0
	// RBX::EquationDisplay::~EquationDisplay

	// SYNTHETIC: ROBLOX 0x0065f8f0
	// RBX::EquationDisplay::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(EquationDisplay, 0x16c)

} // namespace RBX

#endif // GUI_EQUATIONDISPLAY_H
