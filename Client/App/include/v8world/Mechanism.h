#ifndef V8WORLD_MECHANISM_H
#define V8WORLD_MECHANISM_H

#include "placeholder.h"

#include <boost/noncopyable.hpp>
#include <cstddef>
#include <list>
#include <set>
#include <vector>

namespace RBX {

class Assembly;
class Mechanism;
class Primitive;

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

	Mechanism* mechanism;
};

class Mechanism : public boost::noncopyable
{
public:
	typedef std::set<Assembly*> AssemblySet;

	static Mechanism* getMechanismFromPrimitive(const Primitive* primitive);

	~Mechanism() {}

	const AssemblySet& getAssemblies() const { return assemblies; }
	AssemblySet& getAssemblies() { return assemblies; }

	void insertAssembly(Assembly* assembly);
	void removeAssembly(Assembly* assembly);

	void absorb(Mechanism* other);

	void notifyMovingPrimitives();

private:
	friend class MechanismTracker;

	AssemblySet assemblies;
	std::vector<MechanismTracker*> trackers;

public:
	std::list<Mechanism*>::iterator myIt;
};

} // namespace RBX

#endif // V8WORLD_MECHANISM_H
