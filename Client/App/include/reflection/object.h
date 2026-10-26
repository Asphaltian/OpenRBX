#ifndef REFLECTION_OBJECT_H
#define REFLECTION_OBJECT_H

#include "decomp.h"
#include "reflection/descriptor.h"
#include "reflection/function.h"
#include "reflection/member.h"
#include "reflection/property.h"
#include "reflection/signal.h"

#include <vector>

namespace RBX {
namespace Reflection {

// SYNTHETIC: WEBSERVICE 0x100478a0
// RBX::Reflection::ClassDescriptor::`scalar deleting destructor'

// VTABLE: WEBSERVICE 0x1022e264
// class ClassDescriptor
// SIZE 0x88
class ClassDescriptor : public Descriptor,
						public MemberDescriptorContainer<PropertyDescriptor>,
						public MemberDescriptorContainer<SignalDescriptor>,
						public MemberDescriptorContainer<FunctionDescriptor>
{
public:
	ClassDescriptor(ClassDescriptor& base, const char* name);

	// STUB: WEBSERVICE 0x10005bd0
	virtual ~ClassDescriptor() // vtable+0x00
	{
	}

	const ClassDescriptor* getBase() const { return base; }

	bool operator==(const ClassDescriptor& other) const;
	bool operator!=(const ClassDescriptor& other) const;

	// FUNCTION: WEBSERVICE 0x10005b60
	static ClassDescriptor& rootDescriptor()
	{
		static ClassDescriptor root;

		return root;
	}

	static bool lockedDown;

private:
	ClassDescriptor();

	std::vector<ClassDescriptor*> derivedClasses; // 0x74
	ClassDescriptor* base;                        // 0x84
};

DECOMP_SIZE_ASSERT(ClassDescriptor, 0x88)

// SIZE 0x0c
class __declspec(novtable) DescribedBase : public SignalSource
{
public:
	static ClassDescriptor& classDescriptor() { return ClassDescriptor::rootDescriptor(); }

	// FUNCTION: WEBSERVICE 0x1004e900
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
	// clang-format off
	// FUNCTION: WEBSERVICE 0x100ac2c0
	// ?findSignal@DescribedBase@Reflection@RBX@@QAE?AVIterator@?$MemberDescriptorContainer@VSignalDescriptor@Reflection@RBX@@@23@ABVName@3@@Z
	// clang-format on
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

	// clang-format off
	// FUNCTION: WEBSERVICE 0x100abd10
	// ?signals_end@DescribedBase@Reflection@RBX@@QAE?AVIterator@?$MemberDescriptorContainer@VSignalDescriptor@Reflection@RBX@@@23@XZ
	// clang-format on
	MemberDescriptorContainer<SignalDescriptor>::Iterator signals_end()
	{
		return getDescriptor().MemberDescriptorContainer<SignalDescriptor>::members_end(this);
	}
	MemberDescriptorContainer<SignalDescriptor>::ConstIterator signals_end() const
	{
		return getDescriptor().MemberDescriptorContainer<SignalDescriptor>::members_end(this);
	}

protected:
	const ClassDescriptor* descriptor; // 0x08
};

DECOMP_SIZE_ASSERT(DescribedBase, 0x0c)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x10220fa0
// `RBX::Reflection::ClassDescriptor::rootDescriptor'::`2'::`dynamic atexit destructor for 'root''
// clang-format on

} // namespace Reflection
} // namespace RBX

#endif // REFLECTION_OBJECT_H
