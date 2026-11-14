#ifndef V8TREE_VERB_H
#define V8TREE_VERB_H

#include "placeholder.h"
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

class __declspec(novtable) XmlState : public virtual Debugable
{
protected:
	XmlElement root;

public:
	XmlState() : root(name_root) {}

	const XmlElement* getData() const { return &root; }

	virtual void addState(XmlElement*, Instance&) = 0;

	void addAllProperties(Instance&);
	void addParentProperty(Instance&);
	void addDelete(Instance&);
	void addProperty(const Reflection::Property&);
};

class UndoState : public XmlState
{
public:
	virtual void addState(XmlElement* element, Instance& instance);
};

class RedoState : public XmlState
{
public:
	virtual void addState(XmlElement* element, Instance& instance);
};

class __declspec(novtable) Verb : public Debugable
{
private:
	const Name& name;
	VerbContainer* container;

protected:
	Verb(VerbContainer* container, const Name& name);
	Verb(VerbContainer* container, std::string name);

public:
	virtual ~Verb();

	virtual bool isEnabled() const;
	virtual bool isChecked() const;

	const Name& getName() const { return name; }

	virtual void doIt(IDataState* dataState) = 0;

	VerbContainer* getContainer() const { return container; }
};

class VerbContainer
{
private:
	std::map<const Name*, Verb*> verbs;
	VerbContainer* parent;

public:
	VerbContainer(VerbContainer* parent);

	virtual ~VerbContainer();

	Verb* getVerb(std::string name);
	Verb* getVerb(const Name& name);

	void setVerbParent(VerbContainer* parent);
	VerbContainer* getVerbParent() const;

private:
	void addVerb(Verb* verb);
	void removeVerb(Verb* verb);

	friend class Verb;
};

class __declspec(novtable) IDataState : public StateStack<XmlState>
{
public:
	virtual void setDirty(bool) = 0;
	virtual bool isDirty() const = 0;
};

} // namespace RBX

#endif // V8TREE_VERB_H
