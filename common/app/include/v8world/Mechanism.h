#ifndef V8WORLD_MECHANISM_H
#define V8WORLD_MECHANISM_H

#include "decomp.h"

#include <boost/noncopyable.hpp>
#include <cstddef>
#include <list>
#include <set>
#include <vector>

namespace RBX {

class Assembly;
class Mechanism;
class Primitive;

// SIZE 0x04
class MechanismTracker
{
public:
	MechanismTracker() : mechanism(NULL) {}
	~MechanismTracker() { stopTracking(); }

	bool tracking();

	Mechanism* getMechanism();
	void setMechanism(Mechanism* value);

	static void transferTrackers(Mechanism* from, Mechanism* to);

private:
	bool containedBy(Mechanism* value);

	void stopTracking();

	Mechanism* mechanism; // 0x00
};

DECOMP_SIZE_ASSERT(MechanismTracker, 0x04)

// SIZE 0x24
class Mechanism : public boost::noncopyable
{
public:
	typedef std::set<Assembly*> AssemblySet;

	static Mechanism* getMechanismFromPrimitive(const Primitive* primitive);

	// FUNCTION: WEBSERVICE 0x1011ca90
	~Mechanism() {}

	// SYNTHETIC: WEBSERVICE 0x1011cae0
	// RBX::Mechanism::Mechanism

	const AssemblySet& getAssemblies() const { return assemblies; }
	AssemblySet& getAssemblies() { return assemblies; }

	void insertAssembly(Assembly* assembly);
	void removeAssembly(Assembly* assembly);

	void absorb(Mechanism* other);

	void notifyMovingPrimitives();

private:
	friend class MechanismTracker;

	AssemblySet assemblies;                  // 0x00
	std::vector<MechanismTracker*> trackers; // 0x0c

public:
	std::list<Mechanism*>::iterator myIt; // 0x1c
};

DECOMP_SIZE_ASSERT(Mechanism, 0x24)

} // namespace RBX

#endif // V8WORLD_MECHANISM_H
