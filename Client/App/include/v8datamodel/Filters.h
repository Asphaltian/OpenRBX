#ifndef V8DATAMODEL_FILTERS_H
#define V8DATAMODEL_FILTERS_H

#include "decomp.h"
#include "util/HitTestFilter.h"

#include <boost/shared_ptr.hpp>

namespace RBX {

class Instance;
class ModelInstance;
class PartInstance;

// VTABLE: WEBSERVICE 0x10239d58
// SIZE 0x04
class Unlocked : public HitTestFilter
{
public:
	static bool unlocked(const Primitive* testMe);

	// FUNCTION: WEBSERVICE 0x100bead0
	virtual Result filterResult(const Primitive* testMe) const // vtable+0x00
	{
		return unlocked(testMe) ? INCLUDE_PRIM : STOP_TEST;
	}
};

DECOMP_SIZE_ASSERT(Unlocked, 0x04)

// VTABLE: WEBSERVICE 0x10247a34
// SIZE 0x14
class PartByLocalCharacter : public HitTestFilter
{
protected:
	boost::shared_ptr<ModelInstance> character; // 0x04
	boost::shared_ptr<PartInstance> head;       // 0x0c

public:
	PartByLocalCharacter(Instance* root);

	virtual Result filterResult(const Primitive* testMe) const; // vtable+0x00

	// SYNTHETIC: WEBSERVICE 0x100beb70
	// RBX::PartByLocalCharacter::~PartByLocalCharacter
};

DECOMP_SIZE_ASSERT(PartByLocalCharacter, 0x14)

// VTABLE: WEBSERVICE 0x10247a3c
// SIZE 0x0c
class IgnoreDescendantsOf : public HitTestFilter
{
protected:
	boost::shared_ptr<Instance> inst; // 0x04

public:
	IgnoreDescendantsOf(Instance* i);

	virtual Result filterResult(const Primitive* testMe) const; // vtable+0x00
};

DECOMP_SIZE_ASSERT(IgnoreDescendantsOf, 0x0c)

} // namespace RBX

#endif // V8DATAMODEL_FILTERS_H
