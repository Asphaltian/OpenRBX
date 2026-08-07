#include "v8datamodel/ICharacterSubject.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x10107440
ICharacterSubject::ICharacterSubject()
	: focusERA(1.0f), focusOffsetERA(1.0f), lastOffset(G3D::Vector3::ZERO), rotationERA(1.0f), cursorLocked(false),
	  wasOccluded(false), hasGoalInFocus(false)
{
}

} // namespace RBX
