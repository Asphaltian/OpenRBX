#include "v8datamodel/Filters.h"

#include "humanoid/Humanoid.h"
#include "network/Players.h"
#include "v8datamodel/ModelInstance.h"
#include "v8datamodel/PartInstance.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x10115ac0
bool Unlocked::unlocked(const Primitive* testMe)
{
	return !PartInstance::fromPrimitiveConst(testMe)->getPartLocked();
}

// FUNCTION: WEBSERVICE 0x10115ae0
HitTestFilter::Result PartByLocalCharacter::filterResult(const Primitive* testMe) const
{
	if (character && head) {
		const PartInstance* part = PartInstance::fromPrimitiveConst(testMe);

		if (part->isDescendentOf(character.get())) {
			return head->getIsTransparent() ? IGNORE_PRIM : STOP_TEST;
		}
	}

	return INCLUDE_PRIM;
}

// FUNCTION: WEBSERVICE 0x10115b30
HitTestFilter::Result IgnoreDescendantsOf::filterResult(const Primitive* testMe) const
{
	if (inst) {
		const PartInstance* part = PartInstance::fromPrimitiveConst(testMe);

		if (part->isDescendentOf(inst.get())) {
			return IGNORE_PRIM;
		}
	}

	return INCLUDE_PRIM;
}

// FUNCTION: WEBSERVICE 0x10115b80
PartByLocalCharacter::PartByLocalCharacter(Instance* root)
{
	character = shared_from(Network::Players::findLocalCharacter(root));
	head = shared_from(Humanoid::getHeadFromCharacter(character.get()));
}

// FUNCTION: WEBSERVICE 0x10115ca0
IgnoreDescendantsOf::IgnoreDescendantsOf(Instance* i)
{
	inst = shared_from(i);
}

} // namespace RBX
