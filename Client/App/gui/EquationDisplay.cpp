#include "gui/EquationDisplay.h"

#include "util/Evaluator.h"

namespace RBX {

// FUNCTION: ROBLOX 0x0065f660
std::string EquationDisplay::getLabel() const
{
	const Evaluator* evaluator = dynamic_cast<const Evaluator*>(Instance::getRootAncestor(this));
	if (evaluator != NULL) {
		std::string value = evaluator->evaluate(equation);
		return label + " " + value;
	}

	return label;
}

// FUNCTION: ROBLOX 0x0065f770
EquationDisplay::EquationDisplay(const std::string& name, const std::string& equation)
	: TextDisplay(name, name), equation(equation)
{
}

} // namespace RBX
