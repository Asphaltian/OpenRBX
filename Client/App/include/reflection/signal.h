#ifndef REFLECTION_SIGNAL_H
#define REFLECTION_SIGNAL_H

#include "decomp.h"
#include "reflection/member.h"
#include "reflection/property.h"
#include "reflection/type.h"
#include "util/standardout.h"

#include <boost/any.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/signal.hpp>
#include <boost/type_traits/function_traits.hpp>
#include <functional>
#include <map>
#include <memory>
#include <vector>

namespace RBX {
namespace Reflection {

class SignalDescriptor;
class SignalSource;

// SIZE 0x0c
class SignalInstance : public boost::noncopyable
{
public:
	SignalSource* getSource() { return source; }

	template <class Slot>
	boost::signals::connection connectGeneric(Slot slot, boost::signals::connect_position position);

	virtual ~SignalInstance(); // vtable+0x00

protected:
	SignalInstance(SignalSource* source, const SignalDescriptor& descriptor) : source(source), descriptor(descriptor) {}

	SignalSource* source;               // 0x04
	const SignalDescriptor& descriptor; // 0x08
};

DECOMP_SIZE_ASSERT(SignalInstance, 0x0c)

// SIZE 0x08
class __declspec(novtable) SignalSource
{
public:
	typedef std::map<const SignalDescriptor*, boost::shared_ptr<SignalInstance> > SignalMap;

	virtual ~SignalSource(); // vtable+0x00

	void disconnect_all_slots();

protected:
	friend class SignalDescriptor;

	std::auto_ptr<SignalMap> signals; // 0x04
};

DECOMP_SIZE_ASSERT(SignalSource, 0x08)

// SIZE 0x04
class __declspec(novtable) GenericSlotWrapper
{
public:
	virtual ~GenericSlotWrapper(); // vtable+0x00

	virtual void execute(const std::vector<boost::any>& arguments) = 0; // vtable+0x04

	template <class Slot>
	static GenericSlotWrapper* create(Slot slot);
};

DECOMP_SIZE_ASSERT(GenericSlotWrapper, 0x04)

template <class Slot>
class TGenericSlotWrapper : public GenericSlotWrapper
{
	friend class GenericSlotWrapper;

public:
	virtual void execute(const std::vector<boost::any>& arguments); // vtable+0x04

private:
	// clang-format off
	// TEMPLATE: WEBSERVICE 0x100b0060
	// RBX::Reflection::TGenericSlotWrapper<WaitScriptSlot>::TGenericSlotWrapper<WaitScriptSlot>
	// TEMPLATE: WEBSERVICE 0x100b0870
	// RBX::Reflection::TGenericSlotWrapper<FunctionScriptSlot>::TGenericSlotWrapper<FunctionScriptSlot>
	// clang-format on
	TGenericSlotWrapper(const Slot& slot) : slot(slot) {}

	Slot slot; // 0x04
};

// clang-format off
// TEMPLATE: WEBSERVICE 0x100b0b50
// RBX::Reflection::TGenericSlotWrapper<FunctionScriptSlot>::execute
// TEMPLATE: WEBSERVICE 0x100b0be0
// RBX::Reflection::TGenericSlotWrapper<WaitScriptSlot>::execute
// clang-format on
template <class Slot>
void TGenericSlotWrapper<Slot>::execute(const std::vector<boost::any>& arguments)
{
	try {
		slot(arguments);
	}
	catch (const std::exception& error) {
		StandardOut::singleton()->print(MESSAGE_ERROR, "Exception caught in TGenericSlotWrapper. %s", error.what());
	}
}

// clang-format off
// TEMPLATE: WEBSERVICE 0x100b0800
// RBX::Reflection::GenericSlotWrapper::create<WaitScriptSlot>
// TEMPLATE: WEBSERVICE 0x100b0e90
// RBX::Reflection::GenericSlotWrapper::create<FunctionScriptSlot>
// clang-format on
template <class Slot>
GenericSlotWrapper* GenericSlotWrapper::create(Slot slot)
{
	return new TGenericSlotWrapper<Slot>(slot);
}

// SIZE 0x24
class __declspec(novtable) SignalDescriptor : public MemberDescriptor
{
public:
	void (*signalCreatedHook)(SignalSource*); // 0x10

protected:
	friend class SignalInstance;

	SignalInstance* findSignalInstance(const SignalSource* source) const;

	virtual boost::signals::connection connectGeneric(
		SignalInstance* instance,
		GenericSlotWrapper* wrapper,
		boost::signals::connect_position position
	) const = 0; // vtable+0x04

	SignalDescriptor(ClassDescriptor& classDescriptor, const char* name);

	SignatureDescriptor signature; // 0x14
};

DECOMP_SIZE_ASSERT(SignalDescriptor, 0x24)

// clang-format off
// TEMPLATE: WEBSERVICE 0x100b0e00
// RBX::Reflection::SignalInstance::connectGeneric<WaitScriptSlot>
// TEMPLATE: WEBSERVICE 0x100b1020
// RBX::Reflection::SignalInstance::connectGeneric<FunctionScriptSlot>
// clang-format on
template <class Slot>
boost::signals::connection SignalInstance::connectGeneric(Slot slot, boost::signals::connect_position position)
{
	return descriptor.connectGeneric(this, GenericSlotWrapper::create(slot), position);
}

// SIZE 0x24
template <class Signature>
class __declspec(novtable) TSignalDesc : public SignalDescriptor
{
public:
	// SIZE 0x28
	class TSignalInstance
		: public SignalInstance,
		  public boost::signal<Signature, boost::last_value<void>, int, std::less<int>, boost::function<Signature> >
	{
	public:
		TSignalInstance(SignalSource* source, const SignalDescriptor& descriptor) : SignalInstance(source, descriptor)
		{
		}
	};

	bool empty(const SignalSource* source)
	{
		const TSignalInstance* instance = findSig(source);

		return instance == NULL || instance->empty();
	}

protected:
	TSignalInstance* findSig(const SignalSource* source) const
	{
		return static_cast<TSignalInstance*>(findSignalInstance(source));
	}

	TSignalDesc(ClassDescriptor& classDescriptor, const char* name) : SignalDescriptor(classDescriptor, name) {}
};

DECOMP_SIZE_ASSERT(TSignalDesc<void(bool)>::TSignalInstance, 0x28)

// SIZE 0x24
template <int arity, class Signature>
class __declspec(novtable) SignalDescImpl : public TSignalDesc<Signature>
{
protected:
	class GenericSlotAdapter
	{
	public:
		GenericSlotAdapter(GenericSlotWrapper* wrapper) : wrapper(wrapper) {}

		void operator()() const
		{
			std::vector<boost::any> arguments;

			wrapper->execute(arguments);
		}

	private:
		boost::shared_ptr<GenericSlotWrapper> wrapper; // 0x00
	};

	virtual boost::signals::connection connectGeneric(
		SignalInstance* instance,
		GenericSlotWrapper* wrapper,
		boost::signals::connect_position position
	) const // vtable+0x04
	{
		return static_cast<typename TSignalDesc<Signature>::TSignalInstance*>(instance)->connect(
			GenericSlotAdapter(wrapper),
			position
		);
	}

	SignalDescImpl(ClassDescriptor& classDescriptor, const char* name) : TSignalDesc<Signature>(classDescriptor, name)
	{
	}
};

// SIZE 0x24
template <class Signature>
class __declspec(novtable) SignalDescImpl<1, Signature> : public TSignalDesc<Signature>
{
public:
	void fire(SignalSource* source, typename boost::function_traits<Signature>::arg1_type arg1)
	{
		typename TSignalDesc<Signature>::TSignalInstance* instance = this->findSig(source);

		if (instance != NULL) {
			(*instance)(arg1);
		}
	}

protected:
	class GenericSlotAdapter
	{
	public:
		GenericSlotAdapter(GenericSlotWrapper* wrapper) : wrapper(wrapper) {}

		void operator()(typename boost::function_traits<Signature>::arg1_type arg1) const
		{
			std::vector<boost::any> arguments(1);

			arguments[0] = arg1;

			wrapper->execute(arguments);
		}

	private:
		boost::shared_ptr<GenericSlotWrapper> wrapper; // 0x00
	};

	virtual boost::signals::connection connectGeneric(
		SignalInstance* instance,
		GenericSlotWrapper* wrapper,
		boost::signals::connect_position position
	) const // vtable+0x04
	{
		return static_cast<typename TSignalDesc<Signature>::TSignalInstance*>(instance)->connect(
			GenericSlotAdapter(wrapper),
			position
		);
	}

	SignalDescImpl(ClassDescriptor& classDescriptor, const char* name) : TSignalDesc<Signature>(classDescriptor, name)
	{
	}
};

// SIZE 0x24
template <class Signature>
class __declspec(novtable) SignalDescImpl<2, Signature> : public TSignalDesc<Signature>
{
public:
	void fire(
		SignalSource* source,
		typename boost::function_traits<Signature>::arg1_type arg1,
		typename boost::function_traits<Signature>::arg2_type arg2
	)
	{
		typename TSignalDesc<Signature>::TSignalInstance* instance = this->findSig(source);

		if (instance != NULL) {
			(*instance)(arg1, arg2);
		}
	}

protected:
	class GenericSlotAdapter
	{
	public:
		GenericSlotAdapter(GenericSlotWrapper* wrapper) : wrapper(wrapper) {}

		void operator()(
			typename boost::function_traits<Signature>::arg1_type arg1,
			typename boost::function_traits<Signature>::arg2_type arg2
		) const
		{
			std::vector<boost::any> arguments(2);

			arguments[0] = arg1;
			arguments[1] = arg2;

			wrapper->execute(arguments);
		}

	private:
		boost::shared_ptr<GenericSlotWrapper> wrapper; // 0x00
	};

	virtual boost::signals::connection connectGeneric(
		SignalInstance* instance,
		GenericSlotWrapper* wrapper,
		boost::signals::connect_position position
	) const // vtable+0x04
	{
		return static_cast<typename TSignalDesc<Signature>::TSignalInstance*>(instance)->connect(
			GenericSlotAdapter(wrapper),
			position
		);
	}

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
	// TEMPLATE: WEBSERVICE 0x100a3c20
	// RBX::Reflection::SignalDesc<RBX::Humanoid,void __cdecl(float)>::SignalDesc<RBX::Humanoid,void __cdecl(float)>
	// TEMPLATE: WEBSERVICE 0x100a3cf0
	// RBX::Reflection::SignalDesc<RBX::Humanoid,void __cdecl(bool)>::SignalDesc<RBX::Humanoid,void __cdecl(bool)>
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

	SignalDesc(const char* name, const char* arg1name, const char* arg2name)
		: SignalDescImpl<boost::function_traits<Signature>::arity, Signature>(Class::classDescriptor(), name)
	{
		SignatureDescriptor::Item arg1 = {
			&Name::declare(arg1name, -1),
			&Type::singleton<typename boost::function_traits<Signature>::arg1_type>(),
			Value()
		};

		this->signature.arguments.push_back(arg1);

		SignatureDescriptor::Item arg2 = {
			&Name::declare(arg2name, -1),
			&Type::singleton<typename boost::function_traits<Signature>::arg2_type>(),
			Value()
		};

		this->signature.arguments.push_back(arg2);
	}
};

} // namespace Reflection
} // namespace RBX

#endif // REFLECTION_SIGNAL_H
