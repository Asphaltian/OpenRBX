#ifndef REFLECTION_SIGNAL_H
#define REFLECTION_SIGNAL_H

#include "decomp.h"
#include "reflection/Property.h"

namespace RBX {
namespace Reflection {

// SIZE 0x08
class SignalSource
{
private:
	undefined m_unk0x00[0x08 - 0x00]; // 0x00
};

DECOMP_SIZE_ASSERT(SignalSource, 0x08)

// SIZE 0x0c
class SignalInstance
{
private:
	undefined m_unk0x00[0x0c - 0x00]; // 0x00
};

DECOMP_SIZE_ASSERT(SignalInstance, 0x0c)

// SIZE 0x24
class SignalDescriptor : public MemberDescriptor
{
public:
	SignalDescriptor() {}

	SignalInstance* findSignalInstance(const SignalSource* source) const;

private:
	undefined m_unk0x10[0x24 - 0x10]; // 0x10
};

DECOMP_SIZE_ASSERT(SignalDescriptor, 0x24)

template <class Class, class Signature>
class SignalDesc : public SignalDescriptor
{
public:
	SignalDesc() {}
};

} // namespace Reflection
} // namespace RBX

#endif // REFLECTION_SIGNAL_H
