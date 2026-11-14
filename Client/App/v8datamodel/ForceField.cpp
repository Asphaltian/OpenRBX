#include "v8datamodel/ForceField.h"

#include "appdraw/Draw.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/Workspace.h"

#include <boost/bind.hpp>

namespace RBX {

char sForceField[] = "ForceField";

bool ForceField::askSetParent(const Instance* instance) const
{
	return dynamic_cast<const Workspace*>(instance) == NULL;
}

void renderForceField(boost::shared_ptr<Instance> descendent, Adorn* adorn, int cycle)
{
	if (PartInstance* part = dynamic_cast<PartInstance*>(descendent.get())) {
		int value = cycle < 15 ? cycle : 30 - cycle;
		G3D::Color4 color(value / 15.0f, 0.0f, 1.0f - value / 15.0f, 0.5f);
		Draw::selectionBox(part->getPart(), adorn, color);
	}
}

bool containsForceField(Instance* instance)
{
	for (unsigned int i = 0; i < instance->numChildren(); ++i) {
		if (dynamic_cast<ForceField*>(instance->getChild(i))) {
			return true;
		}
	}

	return false;
}

bool ancestorContainsForceField(Instance* instance)
{
	if (containsForceField(instance)) {
		return true;
	}

	Instance* parent = instance->getParent();
	if (parent != NULL && dynamic_cast<Workspace*>(parent) == NULL) {
		return ancestorContainsForceField(parent);
	}

	return false;
}

bool ForceField::partInForceField(PartInstance* part)
{
	return ancestorContainsForceField(part);
}

void ForceField::render3dAdorn(Adorn* adorn)
{
	cycle = (cycle + 1) % 30;

	getParent()->visitDescendents(boost::bind(&renderForceField, _1, adorn, cycle));
}

ForceField::ForceField() : cycle(0)
{
	setName("ForceField");
}

} // namespace RBX

template class RBX::FactoryProduct<RBX::ForceField, RBX::Instance, RBX::sForceField>;
template class RBX::Reflection::Described<
	RBX::ForceField,
	RBX::sForceField,
	RBX::FactoryProduct<RBX::ForceField, RBX::Instance, RBX::sForceField> >;
template class RBX::DescribedCreatable<RBX::ForceField, RBX::Instance, RBX::sForceField>;
