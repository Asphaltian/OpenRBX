#ifndef REFLECTION_MEMBER_H
#define REFLECTION_MEMBER_H

#include "decomp.h"
#include "reflection/descriptor.h"

#include <vector>

namespace RBX {
namespace Reflection {

class ClassDescriptor;

// SIZE 0x10
class __declspec(novtable) MemberDescriptor : public Descriptor
{
public:
	const Name& category;         // 0x08
	const ClassDescriptor& owner; // 0x0c

protected:
	// FUNCTION: WEBSERVICE 0x10071340
	MemberDescriptor(const ClassDescriptor& owner, const char* name, const char* category)
		: Descriptor(name), category(Name::declare(category, -1)), owner(owner)
	{
	}

	virtual ~MemberDescriptor() // vtable+0x00
	{
	}
};

DECOMP_SIZE_ASSERT(MemberDescriptor, 0x10)

// SIZE 0x24
template <class T>
class MemberDescriptorContainer
{
public:
	typedef std::vector<T*> Collection;

	void declare(T& descriptor);

protected:
	MemberDescriptorContainer(MemberDescriptorContainer* base) : base(base) {}

	Collection descriptors;                                    // 0x00
	std::vector<MemberDescriptorContainer*> derivedContainers; // 0x10
	MemberDescriptorContainer* base;                           // 0x20
};

// clang-format off
// STUB: WEBSERVICE 0x100475e0
// RBX::Reflection::MemberDescriptorContainer<RBX::Reflection::FunctionDescriptor>::declare
// clang-format on
template <class T>
DECOMP_NOINLINE void MemberDescriptorContainer<T>::declare(T& descriptor)
{
	boost::recursive_mutex::scoped_lock lock(sync());

	STUB(0x100475e0);
}

} // namespace Reflection
} // namespace RBX

#endif // REFLECTION_MEMBER_H
