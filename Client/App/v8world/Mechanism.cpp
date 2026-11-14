#include "v8world/Mechanism.h"

#include "placeholder.h"
#include "util/StlExtra.h"
#include "v8world/Assembly2.h"
#include "v8world/Primitive.h"

#include <cstddef>

namespace RBX {

Mechanism* MechanismTracker::getMechanism()
{
	return mechanism;
}

Mechanism* Mechanism::getMechanismFromPrimitive(const Primitive* primitive)
{
	if (primitive != NULL) {
		Assembly* assembly = primitive->getAssembly();

		if (assembly != NULL) {
			return assembly->getMechanism();
		}
	}

	return NULL;
}

bool MechanismTracker::tracking()
{
	return mechanism ? true : false;
}

void Mechanism::notifyMovingPrimitives()
{
	AssemblySet::iterator it;

	for (it = assemblies.begin(); it != assemblies.end(); ++it) {
		(*it)->notifyMoved();
	}
}

void Mechanism::insertAssembly(Assembly* a)
{
	assemblies.insert(a);

	a->setMechanism(this);
}

void Mechanism::removeAssembly(Assembly* a)
{
	assemblies.erase(a);

	a->setMechanism(NULL);
}

void Mechanism::absorb(Mechanism* smaller)
{
	while (smaller->assemblies.size() != 0) {
		Assembly* assembly = *smaller->assemblies.begin();

		smaller->removeAssembly(assembly);

		insertAssembly(assembly);
	}
}

void MechanismTracker::stopTracking()
{
	if (mechanism != NULL) {
		fastRemoveShort(mechanism->trackers, this);

		mechanism = NULL;
	}
}

void MechanismTracker::setMechanism(Mechanism* m)
{
	stopTracking();

	if (m != NULL) {
		m->trackers.push_back(this);

		mechanism = m;
	}
}

void MechanismTracker::transferTrackers(Mechanism* from, Mechanism* to)
{
	while (from->trackers.size() != 0) {
		MechanismTracker* tracker = from->trackers.back();

		tracker->setMechanism(to);
	}
}

} // namespace RBX
