#ifndef V8DATAMODEL_FILTERS_H
#define V8DATAMODEL_FILTERS_H

#include "placeholder.h"
#include "util/HitTestFilter.h"

#include <boost/shared_ptr.hpp>

namespace RBX {

class Instance;
class ModelInstance;
class PartInstance;

class Unlocked : public HitTestFilter
{
public:
	static bool unlocked(const Primitive* testMe);

	virtual Result filterResult(const Primitive* testMe) const { return unlocked(testMe) ? INCLUDE_PRIM : STOP_TEST; }
};

class PartByLocalCharacter : public HitTestFilter
{
protected:
	boost::shared_ptr<ModelInstance> character;
	boost::shared_ptr<PartInstance> head;

public:
	PartByLocalCharacter(Instance* root);

	virtual Result filterResult(const Primitive* testMe) const;
};

class UnlockedPartByLocalCharacter : public PartByLocalCharacter
{
public:
	UnlockedPartByLocalCharacter(Instance* root) : PartByLocalCharacter(root) {}

	virtual Result filterResult(const Primitive* testMe) const;
};

class IgnoreDescendantsOf : public HitTestFilter
{
protected:
	boost::shared_ptr<Instance> inst;

public:
	IgnoreDescendantsOf(Instance* i);

	virtual Result filterResult(const Primitive* testMe) const;
};

} // namespace RBX

#endif // V8DATAMODEL_FILTERS_H
