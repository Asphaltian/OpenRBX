#include "v8tree/Verb.h"

#include "reflection/property.h"
#include "util/Handle.h"
#include "util/Name.h"
#include "v8tree/Instance.h"

namespace RBX {

// FUNCTION: ROBLOX 0x0059cd30
void XmlState::addAllProperties(Instance& instance)
{
	addState(instance.write(), instance);
}

// FUNCTION: ROBLOX 0x0059cd50
void XmlState::addParentProperty(Instance& instance)
{
	XmlElement* element = Instance::propParent.write(&instance, true);
	addState(element, instance);
}

// FUNCTION: ROBLOX 0x0059cd80
void XmlState::addDelete(Instance& instance)
{
	addState(instance.writeDelete(), instance);
}

// FUNCTION: ROBLOX 0x0059cda0
void XmlState::addProperty(const Reflection::Property& property)
{
	XmlElement* element = property.getDescriptor().write(property.getInstance(), false);
	if (element) {
		addState(element, *static_cast<Instance*>(property.getInstance()));
	}
}

// FUNCTION: ROBLOX 0x0059cdf0
void UndoState::addState(XmlElement* element, Instance& instance)
{
	if (element) {
		if (!element->findAttribute(name_referent)) {
			element->addAttribute(name_referent, InstanceHandle(&instance));
		}

		root.pushFrontChild(element);
	}
}

// FUNCTION: ROBLOX 0x0059ce50
void RedoState::addState(XmlElement* element, Instance& instance)
{
	if (!element->findAttribute(name_referent)) {
		element->addAttribute(name_referent, InstanceHandle(&instance));
	}

	root.pushBackChild(element);
}

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

// FUNCTION: ROBLOX 0x0059d060
Verb::~Verb()
{
	if (container) {
		container->removeVerb(this);
	}
}

// FUNCTION: WEBSERVICE 0x100d0b70
// FUNCTION: ROBLOX 0x0059d080
VerbContainer::VerbContainer(VerbContainer* parent) : parent(parent)
{
}

// FUNCTION: ROBLOX 0x0059d0f0
Verb::Verb(VerbContainer* container, std::string name) : name(Name::declare(name.c_str(), -1)), container(container)
{
	container->addVerb(this);
}

void VerbContainer::addVerb(Verb* verb)
{
	verbs[&verb->getName()] = verb;
}

void VerbContainer::removeVerb(Verb* verb)
{
	verbs.erase(&verb->getName());
}

bool Verb::isEnabled() const
{
	return true;
}

bool Verb::isChecked() const
{
	return false;
}

} // namespace RBX
