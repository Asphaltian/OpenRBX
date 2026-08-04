#ifndef V8WORLD_SLEEPSTAGE2_H
#define V8WORLD_SLEEPSTAGE2_H

#include "decomp.h"
#include "util/IndexArray.h"
#include "util/Profiling.h"
#include "v8world/Contact.h"
#include "v8world/IWorldStage.h"

#include <boost/scoped_ptr.hpp>
#include <set>

namespace RBX {

class Assembly;
class Joint;

// SIZE 0x90
class SleepStage : public IWorldStage
{
public:
	typedef std::set<Assembly*> AssemblySet;
	typedef std::set<Joint*> JointSet;
	typedef AssemblySet::iterator AssemblySetIt;
	typedef AssemblySet::const_iterator CAssemblySetIt;
	typedef IndexArray<Contact, &Contact::steppingIndexFunc> ContactList;

	SleepStage(IStage* upstream, World* world);

	virtual ~SleepStage(); // vtable+0x00

	virtual StageType getStageType(); // vtable+0x04

	virtual void onEdgeAdded(Edge* edge);         // vtable+0x10
	virtual void onEdgeRemoving(Edge* edge);      // vtable+0x14
	virtual int getMetric(MetricType metricType); // vtable+0x18

	void onAssemblyAdded(Assembly* assembly);
	void onAssemblyRemoving(Assembly* assembly);

	void onWakeUpRequest(Assembly* assembly, bool recursive);

private:
	bool externalBodyForceAdded;      // 0x10
	int numContactsInStage;           // 0x14
	int numContactsInKernel;          // 0x18
	bool throttling;                  // 0x1c
	int uiStepId;                     // 0x20
	AssemblySet recursiveWakePending; // 0x24
	AssemblySet wakePending;          // 0x30
	ContactList steppingContacts;     // 0x3c
	ContactList touchingContacts;     // 0x48
	JointSet steppingJoints;          // 0x54
	AssemblySet awake;                // 0x60
	AssemblySet sleepingChecking;     // 0x6c
	AssemblySet sleepingDeeply;       // 0x78

public:
	boost::scoped_ptr<Profiling::CodeProfiler> profilingCollision; // 0x84
	boost::scoped_ptr<Profiling::CodeProfiler> profilingWake;      // 0x88
	boost::scoped_ptr<Profiling::CodeProfiler> profilingSleep;     // 0x8c
};

DECOMP_SIZE_ASSERT(SleepStage, 0x90)

} // namespace RBX

#endif // V8WORLD_SLEEPSTAGE2_H
