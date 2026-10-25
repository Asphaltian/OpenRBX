#include "v8tree/Instance.h"

#include "decomp.h"
#include "v8xml/SerializerV2.h"

#include <algorithm>
#include <boost/bind.hpp>
#include <boost/scoped_ptr.hpp>
#include <cstddef>

namespace RBX {

const char sInstance[] = "Instance";

Reflection::BoundProp<bool, 1> Instance::propArchivable("archivable", "Behavior", &Instance::archivable);

static Reflection::BoundFuncDesc<Instance, boost::shared_ptr<Instance>(std::string, bool), 2> findFirstChild(
	&Instance::findFirstChildByName2,
	"FindFirstChild",
	"name",
	"recursive",
	false,
	Reflection::FunctionDescriptor::AnyCaller
);
static Reflection::BoundFuncDesc<Instance, boost::shared_ptr<Instance>(), 0> func_clone(
	&Instance::clone,
	"Clone",
	Reflection::FunctionDescriptor::AnyCaller
);
static Reflection::BoundFuncDesc<Instance, void(), 0> func_Remove(
	&Instance::remove,
	"Remove",
	Reflection::FunctionDescriptor::AnyCaller
);
static Reflection::BoundFuncDesc<Instance, boost::shared_ptr<const std::vector<shared_ptr<Instance> > >(), 0>
	func_childrenOld(&Instance::getChildren2, "children", Reflection::FunctionDescriptor::AnyCaller);
static Reflection::BoundFuncDesc<Instance, boost::shared_ptr<const std::vector<shared_ptr<Instance> > >(), 0>
	func_children(&Instance::getChildren2, "GetChildren", Reflection::FunctionDescriptor::AnyCaller);
static Reflection::BoundFuncDesc<Instance, bool(shared_ptr<Instance>), 1> func_isDescendentOf(
	&Instance::isDescendentOf2,
	"IsDescendentOf",
	"ancestor",
	Reflection::FunctionDescriptor::AnyCaller
);
static Reflection::BoundFuncDesc<Instance, bool(shared_ptr<Instance>), 1> func_isAncestorOf(
	&Instance::isAncestorOf2,
	"IsAncestorOf",
	"descendent",
	Reflection::FunctionDescriptor::AnyCaller
);
static const Reflection::PropDescriptor<Instance, std::string> prop_className(
	"className",
	"Data",
	&Instance::getClassNameStr,
	NULL,
	Reflection::PropertyDescriptor::UI
);

const Reflection::PropDescriptor<Instance, std::string> Instance::desc_Name(
	"Name",
	"Data",
	&Instance::getName,
	&Instance::setName,
	Reflection::PropertyDescriptor::STANDARD
);

const Reflection::RefPropDescriptor<Instance, Instance> Instance::propParent(
	"Parent",
	"Data",
	&Instance::getParent,
	&Instance::setParent,
	Reflection::PropertyDescriptor::UI
);

Reflection::SignalDesc<Instance, void(shared_ptr<Instance>)> Instance::event_childAdded("ChildAdded", "child");

Reflection::SignalDesc<Instance, void(shared_ptr<Instance>)> Instance::event_childRemoved("ChildRemoved", "child");

Reflection::SignalDesc<Instance, void(shared_ptr<Instance>)> Instance::event_descendentAdded(
	"DescendentAdded",
	"descendent"
);

Reflection::SignalDesc<Instance, void(shared_ptr<Instance>)> Instance::event_descendentRemoving(
	"DescendentRemoving",
	"descendent"
);

Reflection::SignalDesc<Instance, void(shared_ptr<Instance>)> Instance::event_ancestryChanged(
	"AncestryChanged",
	"child"
);

Reflection::SignalDesc<Instance, void(const Reflection::PropertyDescriptor*)> Instance::event_propertyChanged(
	"Changed",
	"property"
);

static const Name& nameName = Name::declare("Name", 3000);
static const Name& nameParent = Name::declare("Parent", 3001);

ChildAdded::ChildAdded(Instance* child) : child(shared_from(child))
{
}

ChildRemoved::ChildRemoved(Instance* child) : child(shared_from(child))
{
}

// FUNCTION: WEBSERVICE 0x10047af0
void Instance::onChildChanged(Instance* instance, const PropertyChanged& event)
{
	if (getParent() != NULL) {
		getParent()->onChildChanged(instance, event);
	}
}

// FUNCTION: WEBSERVICE 0x10047b20
bool Instance::contains(const Instance* child) const
{
	while (child != 0) {
		if (child == this) {
			return true;
		}

		child = child->parent;
	}

	return false;
}

// FUNCTION: WEBSERVICE 0x10048340
void Instance::readProperties(const XmlElement* container, IReferenceBinder& binder)
{
	for (XmlElement* child = container->firstChild(); child != NULL; child = child->nextSibling()) {
		readProperty(child, binder);
	}
}

// FUNCTION: WEBSERVICE 0x10048370
void Instance::writeChildren(XmlElement* container)
{
	const std::vector<shared_ptr<Instance> >* kids = children.operator->();

	if (kids != NULL) {
		for (std::vector<shared_ptr<Instance> >::const_iterator it = kids->begin(); it != kids->end(); ++it) {
			XmlElement* child = (*it)->write();

			if (child != NULL) {
				container->addChild(child);
			}
		}
	}
}

// STUB: WEBSERVICE 0x100483f0
Instance* Instance::findFirstChildByName(const std::string& findName) const
{
	if (!children) {
		return NULL;
	}

	for (unsigned int i = 0; i < children->size(); ++i) {
		if ((*children)[i]->getName() == findName) {
			return (*children)[i].get();
		}
	}

	return NULL;
}

// STUB: WEBSERVICE 0x10048f30
Instance* Instance::findFirstChildByNameRecursive(const std::string& findName) const
{
	Instance* result = findFirstChildByName(findName);
	if (result) {
		return result;
	}

	if (!children) {
		return NULL;
	}

	for (unsigned int i = 0; i < children->size(); ++i) {
		result = (*children)[i]->findFirstChildByNameRecursive(findName);
		if (result) {
			return result;
		}
	}

	return NULL;
}

// FUNCTION: WEBSERVICE 0x10048fb0
void Instance::signalDescendentAdded(Instance* instance, Instance* beginParent, Instance* oldParent)
{
	for (Instance* walk = beginParent; walk != NULL; walk = walk->parent) {
		if (walk == oldParent) {
			break;
		}

		if (walk->isAncestorOf(oldParent)) {
			break;
		}

		walk->onDescendentAdded(instance);
	}

	boost::shared_ptr<const std::vector<shared_ptr<Instance> > > c = instance->children.read();

	if (c) {
		for (std::vector<shared_ptr<Instance> >::const_iterator it = c->begin(); it != c->end(); ++it) {
			signalDescendentAdded(it->get(), beginParent, oldParent);
		}
	}
}

// FUNCTION: WEBSERVICE 0x100490d0
void Instance::signalDescendentRemoving(
	const shared_ptr<Instance>& instance,
	Instance* beginParent,
	Instance* newParent
)
{
	for (Instance* walk = beginParent; walk != NULL; walk = walk->parent) {
		if (walk == newParent) {
			break;
		}

		if (walk->isAncestorOf(newParent)) {
			break;
		}

		walk->onDescendentRemoving(instance);
	}

	boost::shared_ptr<const std::vector<shared_ptr<Instance> > > c = instance->children.read();

	if (c) {
		for (std::vector<shared_ptr<Instance> >::const_iterator it = c->begin(); it != c->end(); ++it) {
			signalDescendentRemoving(*it, beginParent, newParent);
		}
	}
}

// FUNCTION: WEBSERVICE 0x10049b30
boost::shared_ptr<Instance> Instance::findFirstChildByName2(std::string findName, bool recursive)
{
	return recursive ? shared_from(findFirstChildByNameRecursive(findName))
					 : shared_from(findFirstChildByName(findName));
}

// STUB: WEBSERVICE 0x10049d40
XmlElement* Instance::write()
{
	STUB(0x10049d40);
	return NULL;
}

// FUNCTION: WEBSERVICE 0x10049e80
int Instance::findChildIndex(const Instance* instance) const
{
	const std::vector<shared_ptr<Instance> >& c = *children;
	std::vector<shared_ptr<Instance> >::const_iterator iter =
		std::find(c.begin(), c.end(), instance->shared_from_this());
	return iter - c.begin();
}

// STUB: WEBSERVICE 0x1004a010
void Instance::readProperty(const XmlElement* propertyElement, IReferenceBinder& binder)
{
	STUB(0x1004a010);
}

// FUNCTION: WEBSERVICE 0x1004a0e0
void Instance::onAddListener(Listener<Instance, ChildAdded>* listener) const
{
	if (children) {
		boost::shared_ptr<const std::vector<shared_ptr<Instance> > > c = children.read();
		std::vector<shared_ptr<Instance> >::const_iterator end = c->end();
		for (std::vector<shared_ptr<Instance> >::const_iterator iter = c->begin(); iter != end; ++iter) {
			Notifier<Instance, ChildAdded>::raise(ChildAdded(iter->get()), listener);
		}
	}
}

// SIZE 0x8
class RaiseDescendentAdded2
{
	const Instance* const notifier;                      // 0x00
	Listener<Instance, DescendentAdded>* const listener; // 0x04

public:
	RaiseDescendentAdded2(const Instance* notifier, Listener<Instance, DescendentAdded>* listener)
		: notifier(notifier), listener(listener)
	{
	}

	void operator()(Instance* child)
	{
		notifier->Notifier<Instance, DescendentAdded>::raise(DescendentAdded(child, child->getParent()), listener);
		child->for_eachChild(*this);
	}
};

// FUNCTION: WEBSERVICE 0x1004a470
void Instance::onAddListener(Listener<Instance, DescendentAdded>* listener) const
{
	for_eachChild(RaiseDescendentAdded2(this, listener));
}

// STUB: WEBSERVICE 0x1004a9c0
boost::shared_ptr<Instance> Instance::fromXmlRoot(XmlElement* root)
{
	STUB(0x1004a9c0);
	return boost::shared_ptr<Instance>();
}

// STUB: WEBSERVICE 0x1004ab00
boost::shared_ptr<Instance> Instance::clone()
{
	boost::scoped_ptr<XmlElement> root(SerializerV2::newRootElement());
	root->addChild(write());
	return fromXmlRoot(root.get());
}

// FUNCTION: WEBSERVICE 0x1004cdf0
shared_ptr<Instance> Instance::createChild(const Name& className)
{
	return Instance::create(className);
}

// FUNCTION: WEBSERVICE 0x1004d0b0
Instance::~Instance()
{
}

// FUNCTION: WEBSERVICE 0x1004da80
void Instance::setParent(Instance* newParent)
{
	if (newParent == parent) {
		return;
	}

	shared_ptr<Instance> oldParent = shared_from(parent);
	shared_ptr<Instance> self = shared_from_this();

	if (oldParent) {
		if (!oldParent->contains(newParent)) {
			signalDescendentRemoving(self, oldParent.get(), newParent);
		}

		oldParent->onChildRemoving(this);

		shared_ptr<std::vector<shared_ptr<Instance> > >& children = oldParent->children.write();

		children->erase(std::find(children->begin(), children->end(), self));

		if (children->size() == 0) {
			oldParent->children.reset();
		}

		parent = NULL;

		event_childRemoved.fire(oldParent.get(), self);

		oldParent->Notifier<Instance, ChildRemoved>::raise(this);
		oldParent->onChildRemoved(this);

		if (oldParent->numChildren() == 0) {
			oldParent->onLastChildRemoved();
		}
	}

	if (newParent != NULL) {
		newParent->children.write()->push_back(self);
	}

	parent = newParent;

	if (newParent != NULL) {
		newParent->onChildAdded(this);

		if (!newParent->contains(oldParent.get())) {
			signalDescendentAdded(this, newParent, oldParent.get());
		}

		newParent->Notifier<Instance, ChildAdded>::raise(this);

		event_childAdded.fire(newParent, self);
	}

	onAncestorChanged(AncestorChanged(this, oldParent.get(), newParent));
	raisePropertyChanged(propParent);
}

// FUNCTION: WEBSERVICE 0x1004ddb0
void Instance::onAncestorChanged(const AncestorChanged& event)
{
	if (children) {
		boost::shared_ptr<const std::vector<shared_ptr<Instance> > > c = children.read();
		for (std::vector<shared_ptr<Instance> >::const_iterator it = c->begin(); it != c->end(); ++it) {
			(*it)->onAncestorChanged(event);
		}
	}

	event_ancestryChanged.fire(this, shared_from(event.child));

	Notifier<Instance, AncestorChanged>::raise(event);
}

// FUNCTION: WEBSERVICE 0x1004dee0
void Instance::onDescendentAdded(Instance* instance)
{
	if (Notifier<Instance, DescendentAdded>::hasListeners()) {
		Notifier<Instance, DescendentAdded>::raise(DescendentAdded(instance, instance->getParent()));
	}
	event_descendentAdded.fire(this, shared_from(instance));
}

// FUNCTION: WEBSERVICE 0x1004df50
void Instance::onDescendentRemoving(const shared_ptr<Instance>& instance)
{
	event_descendentRemoving.fire(this, instance);
	if (Notifier<Instance, DescendentRemoving>::hasListeners()) {
		Notifier<Instance, DescendentRemoving>::raise(DescendentRemoving(instance, shared_from(instance->getParent())));
	}
}

// FUNCTION: WEBSERVICE 0x1004e040
void Instance::setName(const std::string& value)
{
	if (name != value) {
		name = value;
		raisePropertyChanged(desc_Name);
	}
}

// FUNCTION: WEBSERVICE 0x1004e080
void Instance::removeAllChildren()
{
	while (children) {
		children->back()->setParent(NULL);
	}
}

// STUB: WEBSERVICE 0x1004e0f0
void Instance::predelete()
{
	STUB(0x1004e0f0);
}

// FUNCTION: WEBSERVICE 0x1004e4f0
void Instance::remove()
{
	boost::shared_ptr<const std::vector<shared_ptr<Instance> > > c = children.read();
	setParent(NULL);
	if (c) {
		std::for_each(c->begin(), c->end(), boost::bind(&Instance::remove, _1));
	}
}

// FUNCTION: WEBSERVICE 0x1004e5e0
void Instance::readChild(const XmlElement* childElement, IReferenceBinder& binder)
{
	const Name* className = NULL;
	if (childElement->findAttributeValue(tag_class, className)) {
		shared_ptr<Instance> childInstance = createChild(*className);

		if (childInstance) {
			childInstance->read(childElement, binder);
			if (childInstance) {
				childInstance->setParent(this);
			}
		}
		else {
			const XmlAttribute* referent = childElement->findAttribute(name_referent);
			if (referent) {
				binder.announceID(referent, NULL);
			}
		}
	}
}

// FUNCTION: WEBSERVICE 0x1004e6e0
void Instance::readChildren(const XmlElement* element, IReferenceBinder& binder)
{
	if (element != NULL) {
		const XmlElement* item = element->findFirstChildByTag(tag_Item);

		while (item != NULL) {
			readChild(item, binder);
			item = element->findNextChildWithSameTag(item);
		}
	}
}

// FUNCTION: WEBSERVICE 0x1004e730
void Instance::read(const XmlElement* element, IReferenceBinder& binder)
{
	const XmlAttribute* referent = element->findAttribute(name_referent);
	if (referent) {
		binder.announceID(referent, this);
	}

	if (&element->getTag() == &tag_Item) {
		const XmlElement* properties = element->findFirstChildByTag(tag_Properties);
		if (properties) {
			readProperties(properties, binder);
		}

		readChildren(element, binder);
	}
	else {
		readProperty(element, binder);
	}
}

// FUNCTION: WEBSERVICE 0x1004e7b0
void Instance::predelete(Instance* instance)
{
	instance->predelete();
}

// FUNCTION: WEBSERVICE 0x1004eb10
Instance::Instance() : parent(NULL), name("Instance"), archivable(true)
{
}

// FUNCTION: WEBSERVICE 0x1004ec70
Instance::Instance(const char* name) : parent(NULL), name(name), archivable(true)
{
}

// FUNCTION: WEBSERVICE 0x101019f0 FOLDED
void Instance::onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider)
{
}

} // namespace RBX
