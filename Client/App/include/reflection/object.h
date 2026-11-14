#ifndef REFLECTION_OBJECT_H
#define REFLECTION_OBJECT_H

#include "reflection/descriptor.h"
#include "reflection/function.h"
#include "reflection/member.h"
#include "reflection/property.h"
#include "reflection/signal.h"
#include "placeholder.h"

#include <vector>

namespace RBX {
namespace Reflection {

class ClassDescriptor : public Descriptor,
						public MemberDescriptorContainer<PropertyDescriptor>,
						public MemberDescriptorContainer<SignalDescriptor>,
						public MemberDescriptorContainer<FunctionDescriptor>
{
public:
	ClassDescriptor(ClassDescriptor& base, const char* name);

	const ClassDescriptor* getBase() const { return base; }
	bool isBaseOf(const ClassDescriptor&) const;
	bool isBaseOf(const char*) const;
	bool isA(const ClassDescriptor&) const;
	bool isA(const char*) const;

	bool operator==(const ClassDescriptor& other) const;
	bool operator!=(const ClassDescriptor& other) const;

	static ClassDescriptor& rootDescriptor()
	{
		static ClassDescriptor root;

		return root;
	}
	std::vector<ClassDescriptor*>::const_iterator derivedClasses_begin() const;
	std::vector<ClassDescriptor*>::const_iterator derivedClasses_end() const;

	static bool lockedDown;

private:
	ClassDescriptor();

	std::vector<ClassDescriptor*> derivedClasses;
	ClassDescriptor* base;
};

class __declspec(novtable) DescribedBase : public SignalSource
{
public:
	static ClassDescriptor& classDescriptor() { return ClassDescriptor::rootDescriptor(); }

	DescribedBase()
	{
		ClassDescriptor::lockedDown = true;

		descriptor = &classDescriptor();
	}

	const ClassDescriptor& getDescriptor() const { return *descriptor; }

	MemberDescriptorContainer<PropertyDescriptor>::ConstIterator findProperty(const Name& name) const
	{
		return getDescriptor().MemberDescriptorContainer<PropertyDescriptor>::findConstMember(name, this);
	}
	MemberDescriptorContainer<PropertyDescriptor>::Iterator findProperty(const Name& name)
	{
		return getDescriptor().MemberDescriptorContainer<PropertyDescriptor>::findMember(name, this);
	}

	MemberDescriptorContainer<PropertyDescriptor>::Iterator properties_begin()
	{
		return getDescriptor().MemberDescriptorContainer<PropertyDescriptor>::members_begin(this);
	}
	MemberDescriptorContainer<PropertyDescriptor>::ConstIterator properties_begin() const
	{
		return getDescriptor().MemberDescriptorContainer<PropertyDescriptor>::members_begin(this);
	}

	MemberDescriptorContainer<PropertyDescriptor>::Iterator properties_end()
	{
		return getDescriptor().MemberDescriptorContainer<PropertyDescriptor>::members_end(this);
	}
	MemberDescriptorContainer<PropertyDescriptor>::ConstIterator properties_end() const
	{
		return getDescriptor().MemberDescriptorContainer<PropertyDescriptor>::members_end(this);
	}

	MemberDescriptorContainer<FunctionDescriptor>::ConstIterator findFunction(const Name& name) const
	{
		return getDescriptor().MemberDescriptorContainer<FunctionDescriptor>::findConstMember(name, this);
	}
	MemberDescriptorContainer<FunctionDescriptor>::ConstIterator functions_begin() const
	{
		return getDescriptor().MemberDescriptorContainer<FunctionDescriptor>::members_begin(this);
	}
	MemberDescriptorContainer<FunctionDescriptor>::ConstIterator functions_end() const
	{
		return getDescriptor().MemberDescriptorContainer<FunctionDescriptor>::members_end(this);
	}

	MemberDescriptorContainer<SignalDescriptor>::ConstIterator findSignal(const Name& name) const
	{
		return getDescriptor().MemberDescriptorContainer<SignalDescriptor>::findConstMember(name, this);
	}
	MemberDescriptorContainer<SignalDescriptor>::Iterator findSignal(const Name& name)
	{
		return getDescriptor().MemberDescriptorContainer<SignalDescriptor>::findMember(name, this);
	}

	MemberDescriptorContainer<SignalDescriptor>::Iterator signals_begin()
	{
		return getDescriptor().MemberDescriptorContainer<SignalDescriptor>::members_begin(this);
	}
	MemberDescriptorContainer<SignalDescriptor>::ConstIterator signals_begin() const
	{
		return getDescriptor().MemberDescriptorContainer<SignalDescriptor>::members_begin(this);
	}

	MemberDescriptorContainer<SignalDescriptor>::Iterator signals_end()
	{
		return getDescriptor().MemberDescriptorContainer<SignalDescriptor>::members_end(this);
	}
	MemberDescriptorContainer<SignalDescriptor>::ConstIterator signals_end() const
	{
		return getDescriptor().MemberDescriptorContainer<SignalDescriptor>::members_end(this);
	}

protected:
	const ClassDescriptor* descriptor;
};

} // namespace Reflection
} // namespace RBX

#endif // REFLECTION_OBJECT_H
