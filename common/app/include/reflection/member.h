#ifndef REFLECTION_MEMBER_H
#define REFLECTION_MEMBER_H

#include "decomp.h"
#include "reflection/descriptor.h"

#include <algorithm>
#include <vector>

namespace RBX {
namespace Reflection {

class ClassDescriptor;
class DescribedBase;

// SIZE 0x24
template <class T>
class MemberDescriptorContainer
{
public:
	typedef std::vector<T*> Collection;

	// SIZE 0x10
	class Iterator : public std::iterator<
						 std::forward_iterator_tag,
						 typename T::MemberType,
						 void,
						 typename T::MemberType,
						 typename T::MemberType&>
	{
	public:
		Iterator(DescribedBase* instance, typename Collection::const_iterator iter) : instance(instance), iter(iter) {}

		typename T::MemberType operator*() const { return typename T::MemberType(*iter, instance); }

		bool operator==(const Iterator& other) const { return iter == other.iter; }

		bool operator!=(const Iterator& other) const { return iter != other.iter; }

		Iterator& operator++()
		{
			++iter;
			return *this;
		}

		Iterator operator++(int)
		{
			Iterator result = *this;
			++iter;
			return result;
		}

	private:
		DescribedBase* instance;                  // 0x04
		typename Collection::const_iterator iter; // 0x08
	};
	typedef std::vector<MemberDescriptorContainer*> DerivedContainers;

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x10047440
	// RBX::Reflection::MemberDescriptorContainer<RBX::Reflection::FunctionDescriptor>::compare
	// clang-format on
	static bool compare(const T* a, const T* b)
	{
		const Name* bn = &b->name;
		const Name* an = &a->name;

		return an != bn && an->name < bn->name;
	}

	void declare(T* descriptor);

protected:
	MemberDescriptorContainer(MemberDescriptorContainer* base);

	void mergeMembers(const MemberDescriptorContainer* source);

	Collection descriptors;                // 0x00
	DerivedContainers derivedContainers;   // 0x10
	const MemberDescriptorContainer* base; // 0x20
};

// clang-format off
// TEMPLATE: WEBSERVICE 0x100475e0
// RBX::Reflection::MemberDescriptorContainer<RBX::Reflection::FunctionDescriptor>::declare
// clang-format on
template <class T>
void MemberDescriptorContainer<T>::declare(T* descriptor)
{
	boost::recursive_mutex::scoped_lock lock(sync());

	typename Collection::iterator where = std::lower_bound(descriptors.begin(), descriptors.end(), descriptor, compare);
	if (where == descriptors.end() || *where != descriptor) {
		descriptors.insert(where, descriptor);
	}

	for (typename DerivedContainers::iterator it = derivedContainers.begin(); it != derivedContainers.end(); ++it) {
		(*it)->declare(descriptor);
	}
}

// clang-format off
// TEMPLATE: WEBSERVICE 0x10047700
// RBX::Reflection::MemberDescriptorContainer<RBX::Reflection::FunctionDescriptor>::mergeMembers
// clang-format on
template <class T>
void MemberDescriptorContainer<T>::mergeMembers(const MemberDescriptorContainer* source)
{
	for (typename Collection::const_iterator it = source->descriptors.begin(); it != source->descriptors.end(); ++it) {
		declare(*it);
	}

	if (source->base != NULL) {
		mergeMembers(source->base);
	}
}

// clang-format off
// TEMPLATE: WEBSERVICE 0x10047760
// RBX::Reflection::MemberDescriptorContainer<RBX::Reflection::FunctionDescriptor>::MemberDescriptorContainer<RBX::Reflection::FunctionDescriptor>
// clang-format on
template <class T>
MemberDescriptorContainer<T>::MemberDescriptorContainer(MemberDescriptorContainer* base) : base(base)
{
	if (base != NULL) {
		boost::recursive_mutex::scoped_lock lock(sync());

		mergeMembers(base);

		base->derivedContainers.push_back(this);
	}
}

// SIZE 0x10
class __declspec(novtable) MemberDescriptor : public Descriptor
{
public:
	bool isMemberOf(const ClassDescriptor& classDescriptor) const;
	bool isMemberOf(const DescribedBase* instance) const;

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

} // namespace Reflection
} // namespace RBX

#endif // REFLECTION_MEMBER_H
