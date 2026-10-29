#ifndef V8TREE_VERB_H
#define V8TREE_VERB_H

#include "decomp.h"
#include "util/StateStack.h"
#include "util/Utilities.h"
#include "v8xml/XmlElement.h"

#include <map>
#include <string>

namespace RBX {

class IDataState;
class Instance;
class Name;
class VerbContainer;

namespace Reflection {
class Property;
}

// SIZE 0x3c
class __declspec(novtable) XmlState : public virtual Debugable
{
protected:
	XmlElement root; // 0x10

public:
	XmlState();

	const XmlElement* getData() const;

	virtual void addState(XmlElement*, Instance&) = 0; // vtable+0x00

	void addAllProperties(Instance&);
	void addParentProperty(Instance&);
	void addDelete(Instance&);
	void addProperty(const Reflection::Property&);
};

DECOMP_SIZE_ASSERT(XmlState, 0x3c)

// SIZE 0x0c
class __declspec(novtable) Verb : public Debugable
{
private:
	const Name& name;         // 0x04
	VerbContainer* container; // 0x08

protected:
	Verb(VerbContainer* container, const Name& name);
	Verb(VerbContainer* container, std::string name);

public:
	virtual ~Verb(); // vtable+0x04

	virtual bool isEnabled() const; // vtable+0x08
	virtual bool isChecked() const; // vtable+0x0c

	const Name& getName() const { return name; }

	virtual void doIt(IDataState* dataState) = 0; // vtable+0x10

	VerbContainer* getContainer() const { return container; }
};

DECOMP_SIZE_ASSERT(Verb, 0x0c)

// VTABLE: WEBSERVICE 0x1023c59c
// VTABLE: ROBLOX 0x0082420c
// SIZE 0x14
class VerbContainer
{
private:
	std::map<const Name*, Verb*> verbs; // 0x04
	VerbContainer* parent;              // 0x10

public:
	VerbContainer(VerbContainer* parent);

	virtual ~VerbContainer(); // vtable+0x00

	Verb* getVerb(std::string name);
	Verb* getVerb(const Name& name);

	void setVerbParent(VerbContainer* parent);
	VerbContainer* getVerbParent() const;

	// SYNTHETIC: WEBSERVICE 0x100d0bc0
	// RBX::VerbContainer::`scalar deleting destructor'

private:
	void addVerb(Verb* verb);
	void removeVerb(Verb* verb);

	friend class Verb;
};

DECOMP_SIZE_ASSERT(VerbContainer, 0x14)

// SIZE 0x18
class __declspec(novtable) IDataState : public StateStack<XmlState>
{
public:
	virtual void setDirty(bool) = 0;  // vtable+0x04
	virtual bool isDirty() const = 0; // vtable+0x08
};

DECOMP_SIZE_ASSERT(IDataState, 0x18)

} // namespace RBX

#endif // V8TREE_VERB_H
