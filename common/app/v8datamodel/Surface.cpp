#include "v8datamodel/Surface.h"

#include "v8datamodel/PartInstance.h"
#include "v8world/Primitive.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x10104280
SurfaceType Surface::getSurfaceType() const
{
	return partInstance->getPrimitive()->getSurfaceType(surfId);
}

// FUNCTION: WEBSERVICE 0x101042a0
Controller::InputType Surface::getInput() const
{
	return partInstance->getPrimitive()->getSurfaceData(surfId).inputType;
}

// FUNCTION: WEBSERVICE 0x101043c0
bool Surface::isControllable() const
{
	Controller::InputType input = getInput();

	return input != Controller::NO_INPUT && input != Controller::CONSTANT_INPUT && input != Controller::SIN_INPUT;
}

} // namespace RBX
