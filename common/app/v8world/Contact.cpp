#include "v8world/Contact.h"

#include "decomp.h"
#include "v8world/Primitive.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x100d1fe0
bool Contact::step(int uiStepId)
{
	bool touching = stepContact();

	if (touching) {
		if (lastContactStep == -1) {
			Primitive::onNewTouch(getPrimitive(0), getPrimitive(1));
		}

		lastContactStep = uiStepId;
	}
	else {
		if (lastContactStep < uiStepId) {
			lastContactStep = -1;
		}
	}

	return touching;
}

} // namespace RBX
