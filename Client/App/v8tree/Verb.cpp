#include "v8tree/Verb.h"

#include "util/Name.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x100d09b0
// FUNCTION: ROBLOX 0x00751f70
void VerbContainer::setVerbParent(VerbContainer* parent)
{
	this->parent = parent;
}

// FUNCTION: WEBSERVICE 0x100d09c0
// FUNCTION: ROBLOX 0x0059ceb0
Verb* VerbContainer::getVerb(const Name& name)
{
	if (name != Name::getNullName()) {
		std::map<const Name*, Verb*>::iterator iter = verbs.find(&name);
		if (iter != verbs.end()) {
			return iter->second;
		}

		if (parent) {
			return parent->getVerb(name);
		}
	}

	return NULL;
}

// FUNCTION: WEBSERVICE 0x100d0a60
// FUNCTION: ROBLOX 0x0059cf50
VerbContainer::~VerbContainer()
{
	while (verbs.begin() != verbs.end()) {
		delete verbs.begin()->second;
	}
}

// FUNCTION: WEBSERVICE 0x100d0b10
// FUNCTION: ROBLOX 0x0059d000
Verb* VerbContainer::getVerb(std::string name)
{
	return getVerb(Name::lookup(name));
}

// FUNCTION: WEBSERVICE 0x100d0b70
// FUNCTION: ROBLOX 0x0059d080
VerbContainer::VerbContainer(VerbContainer* parent) : parent(parent)
{
}

} // namespace RBX
