#include "v8tree/Verb.h"

#include "reflection/property.h"
#include "util/Handle.h"
#include "util/Name.h"
#include "v8tree/Instance.h"

namespace RBX {

void XmlState::addAllProperties(Instance& instance)
{
	addState(instance.write(), instance);
}

void XmlState::addParentProperty(Instance& instance)
{
	XmlElement* element = Instance::propParent.write(&instance, true);
	addState(element, instance);
}

void XmlState::addDelete(Instance& instance)
{
	addState(instance.writeDelete(), instance);
}

void XmlState::addProperty(const Reflection::Property& property)
{
	XmlElement* element = property.getDescriptor().write(property.getInstance(), false);
	if (element) {
		addState(element, *static_cast<Instance*>(property.getInstance()));
	}
}

void UndoState::addState(XmlElement* element, Instance& instance)
{
	if (element) {
		if (!element->findAttribute(name_referent)) {
			element->addAttribute(name_referent, InstanceHandle(&instance));
		}

		root.pushFrontChild(element);
	}
}

void RedoState::addState(XmlElement* element, Instance& instance)
{
	if (!element->findAttribute(name_referent)) {
		element->addAttribute(name_referent, InstanceHandle(&instance));
	}

	root.pushBackChild(element);
}

void VerbContainer::setVerbParent(VerbContainer* parent)
{
	this->parent = parent;
}

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

VerbContainer::~VerbContainer()
{
	while (verbs.begin() != verbs.end()) {
		delete verbs.begin()->second;
	}
}

Verb* VerbContainer::getVerb(std::string name)
{
	return getVerb(Name::lookup(name));
}

Verb::~Verb()
{
	if (container) {
		container->removeVerb(this);
	}
}

VerbContainer::VerbContainer(VerbContainer* parent) : parent(parent)
{
}

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
