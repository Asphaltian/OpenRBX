#ifndef GUI_EQUATIONDISPLAY_H
#define GUI_EQUATIONDISPLAY_H

#include "gui/GUI.h"
#include "placeholder.h"

#include <string>

namespace RBX {

class EquationDisplay : public TextDisplay
{
private:
	std::string equation;

protected:
	virtual std::string getLabel() const;

public:
	EquationDisplay(const std::string& name, const std::string& label, const std::string& equation);
	EquationDisplay(const std::string& name, const std::string& equation);
};

} // namespace RBX

#endif // GUI_EQUATIONDISPLAY_H
