#ifndef REFLECTION_PROPERTY_H
#define REFLECTION_PROPERTY_H

#include "decomp.h"

namespace RBX {
namespace Reflection {

// SIZE 0x10
class MemberDescriptor
{
private:
	undefined m_unk0x00[0x10 - 0x00]; // 0x00
};

DECOMP_SIZE_ASSERT(MemberDescriptor, 0x10)

// SIZE 0x18
class PropertyDescriptor : public MemberDescriptor
{
private:
	undefined m_unk0x10[0x18 - 0x10]; // 0x10
};

DECOMP_SIZE_ASSERT(PropertyDescriptor, 0x18)

// SIZE 0x1c
template <class T>
class TypedPropertyDescriptor : public PropertyDescriptor
{
protected:
	undefined m_getset[0x1c - 0x18]; // 0x18
};

// SIZE 0x1c
template <class Class, class T>
class PropDescriptor : public TypedPropertyDescriptor<T>
{
};

// SIZE 0x1c
class EnumPropertyDescriptor : public PropertyDescriptor
{
public:
	undefined m_enumDescriptor[0x1c - 0x18]; // 0x18
};

DECOMP_SIZE_ASSERT(EnumPropertyDescriptor, 0x1c)

// SIZE 0x20
template <class Class, class T>
class EnumPropDescriptor : public EnumPropertyDescriptor
{
private:
	undefined m_getset[0x20 - 0x1c]; // 0x1c
};

} // namespace Reflection
} // namespace RBX

#endif // REFLECTION_PROPERTY_H
