#ifndef V8TREE_VERB_H
#define V8TREE_VERB_H

#include "decomp.h"

#include <map>
#include <string>

namespace RBX {

class Name;
class Verb;

// SIZE 0x14
class __declspec(novtable) VerbContainer
{
private:
	std::map<const Name*, Verb*> verbs; // 0x04
	VerbContainer* parent;              // 0x10

	void addVerb(Verb* verb);
	void removeVerb(Verb* verb);

public:
	VerbContainer(VerbContainer* parent);

	virtual ~VerbContainer(); // vtable+0x00

	Verb* getVerb(const Name& name);
	Verb* getVerb(const std::string& name);

	void setVerbParent(VerbContainer* value);
	VerbContainer* getVerbParent() const;

	friend class Verb;
};

DECOMP_SIZE_ASSERT(VerbContainer, 0x14)

} // namespace RBX

#endif // V8TREE_VERB_H
