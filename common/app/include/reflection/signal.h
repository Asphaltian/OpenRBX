#ifndef REFLECTION_SIGNAL_H
#define REFLECTION_SIGNAL_H

#include "decomp.h"
#include "reflection/member.h"
#include "reflection/property.h"
#include "reflection/type.h"

#include <boost/type_traits/function_traits.hpp>

namespace RBX {
namespace Reflection {

// SIZE 0x08
class SignalSource
{
public:
	virtual ~SignalSource() {} // vtable+0x00

private:
	undefined m_unk0x04[0x08 - 0x04]; // 0x04
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
class __declspec(novtable) SignalDescriptor : public MemberDescriptor
{
public:
	SignalInstance* findSignalInstance(const SignalSource* source) const;

	void (*signalCreatedHook)(SignalSource*); // 0x10

protected:
	SignalDescriptor(ClassDescriptor& classDescriptor, const char* name);

	SignatureDescriptor signature; // 0x14
};

DECOMP_SIZE_ASSERT(SignalDescriptor, 0x24)

// SIZE 0x24
template <class Signature>
class __declspec(novtable) TSignalDesc : public SignalDescriptor
{
protected:
	TSignalDesc(ClassDescriptor& classDescriptor, const char* name) : SignalDescriptor(classDescriptor, name) {}
};

// SIZE 0x24
template <int arity, class Signature>
class __declspec(novtable) SignalDescImpl : public TSignalDesc<Signature>
{
protected:
	SignalDescImpl(ClassDescriptor& classDescriptor, const char* name) : TSignalDesc<Signature>(classDescriptor, name)
	{
	}
};

// SIZE 0x24
template <class Class, class Signature>
class SignalDesc : public SignalDescImpl<boost::function_traits<Signature>::arity, Signature>
{
public:
	SignalDesc(const char* name)
		: SignalDescImpl<boost::function_traits<Signature>::arity, Signature>(Class::classDescriptor(), name)
	{
	}

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x1004c110
	// RBX::Reflection::SignalDesc<RBX::Instance,void __cdecl(RBX::Reflection::PropertyDescriptor const *)>::SignalDesc<RBX::Instance,void __cdecl(RBX::Reflection::PropertyDescriptor const *)>
	// clang-format on
	SignalDesc(const char* name, const char* arg1name)
		: SignalDescImpl<boost::function_traits<Signature>::arity, Signature>(Class::classDescriptor(), name)
	{
		SignatureDescriptor::Item arg1 = {
			&Name::declare(arg1name, -1),
			&Type::singleton<typename boost::function_traits<Signature>::arg1_type>(),
			Value()
		};

		this->signature.arguments.push_back(arg1);
	}
};

} // namespace Reflection
} // namespace RBX

#endif // REFLECTION_SIGNAL_H
