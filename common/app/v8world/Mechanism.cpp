#include "v8world/Mechanism.h"

#include "decomp.h"
#include "util/StlExtra.h"
#include "v8world/Assembly2.h"
#include "v8world/Primitive.h"

#include <cstddef>

namespace RBX {

// FUNCTION: WEBSERVICE 0x1005ecd0
Mechanism* MechanismTracker::getMechanism()
{
	return mechanism;
}

// FUNCTION: WEBSERVICE 0x10127280
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

// FUNCTION: WEBSERVICE 0x101272a0
bool MechanismTracker::tracking()
{
	return mechanism ? true : false;
}

// FUNCTION: WEBSERVICE 0x101272b0
void Mechanism::notifyMovingPrimitives()
{
	AssemblySet::iterator it;

	for (it = assemblies.begin(); it != assemblies.end(); ++it) {
		(*it)->notifyMoved();
	}
}

// FUNCTION: WEBSERVICE 0x10127320
void Mechanism::insertAssembly(Assembly* assembly)
{
	assemblies.insert(assembly);

	assembly->setMechanism(this);
}

// FUNCTION: WEBSERVICE 0x10127350
void Mechanism::removeAssembly(Assembly* assembly)
{
	assemblies.erase(assembly);

	assembly->setMechanism(NULL);
}

// FUNCTION: WEBSERVICE 0x10127370
void Mechanism::absorb(Mechanism* other)
{
	while (other->assemblies.size() != 0) {
		Assembly* assembly = *other->assemblies.begin();

		other->removeAssembly(assembly);

		insertAssembly(assembly);
	}
}

// FUNCTION: WEBSERVICE 0x101274c0
void MechanismTracker::stopTracking()
{
	if (mechanism != NULL) {
		fastRemoveShort(mechanism->trackers, this);

		mechanism = NULL;
	}
}

// FUNCTION: WEBSERVICE 0x101274f0
void MechanismTracker::setMechanism(Mechanism* value)
{
	stopTracking();

	if (value != NULL) {
		value->trackers.push_back(this);

		mechanism = value;
	}
}

// FUNCTION: WEBSERVICE 0x10127540
void MechanismTracker::transferTrackers(Mechanism* from, Mechanism* to)
{
	while (from->trackers.size() != 0) {
		MechanismTracker* tracker = from->trackers.back();

		tracker->setMechanism(to);
	}
}

} // namespace RBX
