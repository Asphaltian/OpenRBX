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
	// clang-format off
	// TEMPLATE: WEBSERVICE 0x10047440
	// TEMPLATE: ROBLOX 0x005a9410
	// RBX::Reflection::MemberDescriptorContainer<RBX::Reflection::FunctionDescriptor>::compare
	// clang-format on
	static bool compare(const T* a, const T* b)
	{
		const Name* bn = &b->name;
		const Name* an = &a->name;

		return an != bn && an->name < bn->name;
	}

	typedef std::vector<T*> Collection;
	typedef typename T::ConstMember ConstMemberType;
	typedef typename T::Member MemberType;

	// SIZE 0x10
	class ConstIterator
		: public std::iterator<std::forward_iterator_tag, ConstMemberType, void, ConstMemberType, ConstMemberType&>
	{
	public:
		ConstIterator(const typename Collection::const_iterator& iter, const DescribedBase* instance)
			: instance(instance), iter(iter)
		{
		}

		ConstMemberType operator*() const { return ConstMemberType(**iter, instance); }

		bool operator==(const ConstIterator& other) const { return iter == other.iter; }

		bool operator!=(const ConstIterator& other) const { return iter != other.iter; }

		ConstIterator operator++(int)
		{
			ConstIterator result = *this;
			++iter;
			return result;
		}

		ConstIterator& operator++()
		{
			++iter;
			return *this;
		}

		const T& getDescriptor() const { return **iter; }

	private:
		const DescribedBase* instance;            // 0x04
		typename Collection::const_iterator iter; // 0x08
	};

	// SIZE 0x10
	class Iterator : public std::iterator<std::forward_iterator_tag, MemberType, void, MemberType, MemberType&>
	{
	public:
		Iterator(const typename Collection::const_iterator& iter, DescribedBase* instance)
			: instance(instance), iter(iter)
		{
		}

		// clang-format off
		// TEMPLATE: WEBSERVICE 0x10047dd0
		// TEMPLATE: ROBLOX 0x005fc620
		// RBX::Reflection::MemberDescriptorContainer<RBX::Reflection::PropertyDescriptor>::Iterator::operator*
		// clang-format on
		MemberType operator*() const { return MemberType(**iter, instance); }

		bool operator==(const Iterator& other) const { return iter == other.iter; }

		bool operator!=(const Iterator& other) const { return iter != other.iter; }

		Iterator operator++(int)
		{
			Iterator result = *this;
			++iter;
			return result;
		}

		Iterator& operator++()
		{
			++iter;
			return *this;
		}

	private:
		DescribedBase* instance;                  // 0x04
		typename Collection::const_iterator iter; // 0x08
	};

	void declare(T* descriptor);

	typename Collection::const_iterator descriptors_begin() const { return descriptors.begin(); }
	typename Collection::const_iterator descriptors_end() const { return descriptors.end(); }

	typename Collection::const_iterator findDescriptor(const Name& name) const;

	Iterator findMember(const Name& name, DescribedBase* instance) const
	{
		return Iterator(findDescriptor(name), instance);
	}

	ConstIterator findConstMember(const Name& name, const DescribedBase* instance) const
	{
		return ConstIterator(findDescriptor(name), instance);
	}

	Iterator members_begin(DescribedBase* instance) const { return Iterator(descriptors.begin(), instance); }
	ConstIterator members_begin(const DescribedBase* instance) const
	{
		return ConstIterator(descriptors.begin(), instance);
	}

	Iterator members_end(DescribedBase* instance) const { return Iterator(descriptors.end(), instance); }
	ConstIterator members_end(const DescribedBase* instance) const
	{
		return ConstIterator(descriptors.end(), instance);
	}

protected:
	MemberDescriptorContainer(MemberDescriptorContainer* base);

	void mergeMembers(const MemberDescriptorContainer* source);

	Collection descriptors;                                    // 0x00
	std::vector<MemberDescriptorContainer*> derivedContainers; // 0x10
	const MemberDescriptorContainer* base;                     // 0x20
};

// clang-format off
// TEMPLATE: WEBSERVICE 0x100495f0
// TEMPLATE: ROBLOX 0x005fc920
// RBX::Reflection::MemberDescriptorContainer<RBX::Reflection::PropertyDescriptor>::findDescriptor
// clang-format on
template <class T>
typename MemberDescriptorContainer<T>::Collection::const_iterator MemberDescriptorContainer<T>::findDescriptor(
	const Name& name
) const
{
	size_t min = 0, max = descriptors.size();
	while (min < max) {
		size_t mid = (min + max) / 2;
		int result = name.compare(descriptors[mid]->name);
		if (result < 0) {
			max = mid;
		}
		else if (result > 0) {
			min = mid + 1;
		}
		else {
			return descriptors.begin() + mid;
		}
	}

	return descriptors.end();
}

// clang-format off
// TEMPLATE: WEBSERVICE 0x100475e0
// TEMPLATE: ROBLOX 0x005a9790
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

	for (typename std::vector<MemberDescriptorContainer*>::iterator it = derivedContainers.begin();
		 it != derivedContainers.end();
		 ++it) {
		(*it)->declare(descriptor);
	}
}

// clang-format off
// TEMPLATE: WEBSERVICE 0x10047700
// TEMPLATE: ROBLOX 0x005a98b0
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
// STUB: ROBLOX 0x005a9a10
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
	// FUNCTION: ROBLOX 0x005a8be0
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
