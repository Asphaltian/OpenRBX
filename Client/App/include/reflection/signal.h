#ifndef REFLECTION_SIGNAL_H
#define REFLECTION_SIGNAL_H

#include "decomp.h"
#include "reflection/member.h"
#include "reflection/property.h"
#include "reflection/type.h"
#include "util/standardout.h"

#include <boost/any.hpp>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/signal.hpp>
#include <boost/type_traits/function_traits.hpp>
#include <functional>
#include <map>
#include <memory>
#include <vector>

namespace RBX {
namespace Reflection {

class Signal;
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

private:
	SignalSource* source; // 0x04

public:
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

private:
	friend class SignalDescriptor;

	boost::scoped_ptr<SignalMap> signals; // 0x04
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
	typedef Signal ConstMember;
	typedef Signal Member;

	void (*signalCreatedHook)(SignalSource*); // 0x10

protected:
	friend class SignalInstance;

	SignalInstance* findSignalInstance(const SignalSource* source) const;

private:
	virtual boost::signals::connection connectGeneric(
		SignalInstance* instance,
		GenericSlotWrapper* wrapper,
		boost::signals::connect_position position
	) const = 0; // vtable+0x04

	virtual SignalInstance* newSignalInstance(SignalSource& source) const = 0; // vtable+0x08

public:
	boost::shared_ptr<SignalInstance> getSignalInstance(SignalSource& source) const;

protected:
	SignalDescriptor(ClassDescriptor& classDescriptor, const char* name);

	SignatureDescriptor signature; // 0x14
};

DECOMP_SIZE_ASSERT(SignalDescriptor, 0x24)

// SIZE 0x08
class Signal
{
public:
	Signal(const Signal& other) : descriptor(other.descriptor), instance(other.instance) {}
	Signal(const SignalDescriptor& descriptor, DescribedBase* instance) : descriptor(&descriptor), instance(instance) {}

	const Name& getName() const { return descriptor->name; }

	const SignalDescriptor* getDescriptor() const { return descriptor; }

	boost::shared_ptr<SignalInstance> getSignalInstance() const;

protected:
	const SignalDescriptor* descriptor; // 0x00
	DescribedBase* instance;            // 0x04
};

DECOMP_SIZE_ASSERT(Signal, 0x08)

// clang-format off
// TEMPLATE: WEBSERVICE 0x100b0e00
// RBX::Reflection::SignalInstance::connectGeneric<WaitScriptSlot>
// FUNCTION: WEBSERVICE 0x100447e0
// RBX::Reflection::SignalInstance::`scalar deleting destructor'
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
	typedef boost::function_traits<Signature> traits;

	// SIZE 0x28
	class TSignalInstance
		: public SignalInstance,
		  public boost::signal<Signature, boost::last_value<void>, int, std::less<int>, boost::function<Signature> >
	{
	public:
		TSignalInstance(SignalSource* source, const SignalDescriptor& descriptor) : SignalInstance(source, descriptor)
		{
		}

		// clang-format off
		// TEMPLATE: WEBSERVICE 0x100455d0
		// RBX::Reflection::TSignalDesc<void __cdecl(bool)>::TSignalInstance::TSignalInstance
		// TEMPLATE: WEBSERVICE 0x1004cc60
		// RBX::Reflection::TSignalDesc<void __cdecl(boost::shared_ptr<RBX::Instance>)>::TSignalInstance::TSignalInstance
		// TEMPLATE: WEBSERVICE 0x1004ccd0
		// RBX::Reflection::TSignalDesc<void __cdecl(RBX::Reflection::PropertyDescriptor const *)>::TSignalInstance::TSignalInstance
		// TEMPLATE: WEBSERVICE 0x10056210
		// RBX::Reflection::TSignalDesc<void __cdecl(float,float)>::TSignalInstance::TSignalInstance
		// TEMPLATE: WEBSERVICE 0x10056280
		// RBX::Reflection::TSignalDesc<void __cdecl(float)>::TSignalInstance::TSignalInstance
		// TEMPLATE: WEBSERVICE 0x100789a0
		// RBX::Reflection::TSignalDesc<void __cdecl(void)>::TSignalInstance::TSignalInstance
		// TEMPLATE: WEBSERVICE 0x100f2ef0
		// RBX::Reflection::TSignalDesc<void __cdecl(int)>::TSignalInstance::TSignalInstance
		// TEMPLATE: WEBSERVICE 0x100f2f60
		// RBX::Reflection::TSignalDesc<void __cdecl(double)>::TSignalInstance::TSignalInstance
		// TEMPLATE: WEBSERVICE 0x100f2fd0
		// RBX::Reflection::TSignalDesc<void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >)>::TSignalInstance::TSignalInstance
		// TEMPLATE: WEBSERVICE 0x100f3040
		// RBX::Reflection::TSignalDesc<void __cdecl(G3D::Vector3)>::TSignalInstance::TSignalInstance
		// TEMPLATE: WEBSERVICE 0x100f30b0
		// RBX::Reflection::TSignalDesc<void __cdecl(G3D::CoordinateFrame)>::TSignalInstance::TSignalInstance
		// SYNTHETIC: WEBSERVICE 0x100f3120
		// RBX::Reflection::TSignalDesc<void __cdecl(float)>::TSignalInstance::~TSignalInstance
		// SYNTHETIC: WEBSERVICE 0x100f3170
		// RBX::Reflection::TSignalDesc<void __cdecl(float)>::TSignalInstance::`scalar deleting destructor'
		// TEMPLATE: WEBSERVICE 0x100f3190
		// RBX::Reflection::TSignalDesc<void __cdecl(G3D::Color3)>::TSignalInstance::TSignalInstance
		// TEMPLATE: WEBSERVICE 0x100f3200
		// RBX::Reflection::TSignalDesc<void __cdecl(RBX::BrickColor)>::TSignalInstance::TSignalInstance
		// clang-format on
	};

protected:
	TSignalDesc(ClassDescriptor& classDescriptor, const char* name) : SignalDescriptor(classDescriptor, name) {}

private:
	virtual SignalInstance* newSignalInstance(SignalSource& source) const // vtable+0x08
	{
		return new TSignalInstance(&source, *this);
	}

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x10045b70
	// RBX::Reflection::TSignalDesc<void __cdecl(bool)>::newSignalInstance
	// TEMPLATE: WEBSERVICE 0x1004cfd0
	// RBX::Reflection::TSignalDesc<void __cdecl(boost::shared_ptr<RBX::Instance>)>::newSignalInstance
	// TEMPLATE: WEBSERVICE 0x1004d040
	// RBX::Reflection::TSignalDesc<void __cdecl(RBX::Reflection::PropertyDescriptor const *)>::newSignalInstance
	// TEMPLATE: WEBSERVICE 0x10056620
	// RBX::Reflection::TSignalDesc<void __cdecl(float,float)>::newSignalInstance
	// TEMPLATE: WEBSERVICE 0x10056760
	// RBX::Reflection::TSignalDesc<void __cdecl(float)>::newSignalInstance
	// TEMPLATE: WEBSERVICE 0x10079440
	// RBX::Reflection::TSignalDesc<void __cdecl(void)>::newSignalInstance
	// TEMPLATE: WEBSERVICE 0x100e2e30
	// RBX::Reflection::TSignalDesc<void __cdecl(boost::shared_ptr<RBX::Instance>,float)>::newSignalInstance
	// TEMPLATE: WEBSERVICE 0x100f3b90
	// RBX::Reflection::TSignalDesc<void __cdecl(int)>::newSignalInstance
	// TEMPLATE: WEBSERVICE 0x100f3c00
	// RBX::Reflection::TSignalDesc<void __cdecl(double)>::newSignalInstance
	// TEMPLATE: WEBSERVICE 0x100f3c70
	// RBX::Reflection::TSignalDesc<void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >)>::newSignalInstance
	// TEMPLATE: WEBSERVICE 0x100f3ce0
	// RBX::Reflection::TSignalDesc<void __cdecl(G3D::Vector3)>::newSignalInstance
	// TEMPLATE: WEBSERVICE 0x100f3d50
	// RBX::Reflection::TSignalDesc<void __cdecl(G3D::CoordinateFrame)>::newSignalInstance
	// TEMPLATE: WEBSERVICE 0x100f3dc0
	// RBX::Reflection::TSignalDesc<void __cdecl(G3D::Color3)>::newSignalInstance
	// TEMPLATE: WEBSERVICE 0x100f3e30
	// RBX::Reflection::TSignalDesc<void __cdecl(RBX::BrickColor)>::newSignalInstance
	// clang-format on

protected:
	TSignalInstance* findSig(const SignalSource* source) const
	{
		return static_cast<TSignalInstance*>(findSignalInstance(source));
	}

	TSignalInstance& sig(SignalSource& source) const
	{
		return *static_cast<TSignalInstance*>(getSignalInstance(source).get());
	}

public:
	boost::signals::connection connect(SignalSource* source, const typename TSignalInstance::slot_type& slot, int group)
	{
		if (source != NULL) {
			return sig(*source).connect(group, slot, boost::signals::at_back);
		}

		return boost::signals::connection();
	}

	bool empty(const SignalSource* source)
	{
		const TSignalInstance* instance = findSig(source);

		return instance == NULL || instance->empty();
	}

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x100caf70
	// RBX::Reflection::TSignalDesc<void __cdecl(boost::shared_ptr<RBX::Instance>)>::connect
	// TEMPLATE: WEBSERVICE 0x100e28b0
	// RBX::Reflection::TSignalDesc<void __cdecl(boost::shared_ptr<RBX::Instance>,float)>::TSignalInstance::TSignalInstance
	// TEMPLATE: WEBSERVICE 0x100fca60
	// RBX::Reflection::TSignalDesc<void __cdecl(bool)>::connect
	// TEMPLATE: WEBSERVICE 0x10170460
	// RBX::Reflection::TSignalDesc<void __cdecl(RBX::Reflection::PropertyDescriptor const *)>::connect
	// clang-format on
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

		void operator()() const { wrapper->execute(std::vector<boost::any>()); }

	private:
		boost::shared_ptr<GenericSlotWrapper> wrapper; // 0x00

		// clang-format off
		// TEMPLATE: WEBSERVICE 0x10045470
		// RBX::Reflection::SignalDescImpl<1,void __cdecl(bool)>::GenericSlotAdapter::operator()
		// TEMPLATE: WEBSERVICE 0x1004bce0
		// RBX::Reflection::SignalDescImpl<1,void __cdecl(boost::shared_ptr<RBX::Instance>)>::GenericSlotAdapter::operator()
		// TEMPLATE: WEBSERVICE 0x1004be30
		// RBX::Reflection::SignalDescImpl<1,void __cdecl(RBX::Reflection::PropertyDescriptor const *)>::GenericSlotAdapter::operator()
		// TEMPLATE: WEBSERVICE 0x10055f60
		// RBX::Reflection::SignalDescImpl<2,void __cdecl(float,float)>::GenericSlotAdapter::operator()
		// TEMPLATE: WEBSERVICE 0x100560b0
		// RBX::Reflection::SignalDescImpl<1,void __cdecl(float)>::GenericSlotAdapter::operator()
		// TEMPLATE: WEBSERVICE 0x10076130
		// RBX::Reflection::SignalDescImpl<0,void __cdecl(void)>::GenericSlotAdapter::operator()
		// TEMPLATE: WEBSERVICE 0x100e2710
		// RBX::Reflection::SignalDescImpl<2,void __cdecl(boost::shared_ptr<RBX::Instance>,float)>::GenericSlotAdapter::operator()
		// TEMPLATE: WEBSERVICE 0x100f1d60
		// RBX::Reflection::SignalDescImpl<1,void __cdecl(int)>::GenericSlotAdapter::operator()
		// TEMPLATE: WEBSERVICE 0x100f1e50
		// RBX::Reflection::SignalDescImpl<1,void __cdecl(double)>::GenericSlotAdapter::operator()
		// TEMPLATE: WEBSERVICE 0x100f1f40
		// RBX::Reflection::SignalDescImpl<1,void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >)>::GenericSlotAdapter::operator()
		// TEMPLATE: WEBSERVICE 0x100f2040
		// RBX::Reflection::SignalDescImpl<1,void __cdecl(G3D::Vector3)>::GenericSlotAdapter::operator()
		// TEMPLATE: WEBSERVICE 0x100f2140
		// RBX::Reflection::SignalDescImpl<1,void __cdecl(G3D::CoordinateFrame)>::GenericSlotAdapter::operator()
		// TEMPLATE: WEBSERVICE 0x100f2230
		// RBX::Reflection::SignalDescImpl<1,void __cdecl(G3D::Color3)>::GenericSlotAdapter::operator()
		// TEMPLATE: WEBSERVICE 0x100f2330
		// RBX::Reflection::SignalDescImpl<1,void __cdecl(RBX::BrickColor)>::GenericSlotAdapter::operator()
		// clang-format on
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

public:
	// TEMPLATE: WEBSERVICE 0x1007b390
	// RBX::Reflection::SignalDescImpl<0,void __cdecl(void)>::fire
	void fire(SignalSource* source)
	{
		typename TSignalDesc<Signature>::TSignalInstance* instance = this->findSig(source);

		if (instance != NULL) {
			(*instance)();
		}
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

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x1004edd0
	// RBX::Reflection::SignalDescImpl<1,void __cdecl(boost::shared_ptr<RBX::Instance>)>::connectGeneric
	// TEMPLATE: WEBSERVICE 0x1004eeb0
	// RBX::Reflection::SignalDescImpl<1,void __cdecl(RBX::Reflection::PropertyDescriptor const *)>::connectGeneric
	// TEMPLATE: WEBSERVICE 0x10057d90
	// RBX::Reflection::SignalDescImpl<1,void __cdecl(float)>::connectGeneric
	// TEMPLATE: WEBSERVICE 0x100f84f0
	// RBX::Reflection::SignalDescImpl<1,void __cdecl(int)>::connectGeneric
	// TEMPLATE: WEBSERVICE 0x100f85d0
	// RBX::Reflection::SignalDescImpl<1,void __cdecl(double)>::connectGeneric
	// TEMPLATE: WEBSERVICE 0x100f8790
	// RBX::Reflection::SignalDescImpl<1,void __cdecl(G3D::Vector3)>::connectGeneric
	// TEMPLATE: WEBSERVICE 0x100f8870
	// RBX::Reflection::SignalDescImpl<1,void __cdecl(G3D::CoordinateFrame)>::connectGeneric
	// TEMPLATE: WEBSERVICE 0x100f8950
	// RBX::Reflection::SignalDescImpl<1,void __cdecl(G3D::Color3)>::connectGeneric
	// TEMPLATE: WEBSERVICE 0x100f8a30
	// RBX::Reflection::SignalDescImpl<1,void __cdecl(RBX::BrickColor)>::connectGeneric
	// clang-format on

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

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x10057cb0
	// RBX::Reflection::SignalDescImpl<2,void __cdecl(float,float)>::connectGeneric
	// TEMPLATE: WEBSERVICE 0x100e3b40
	// RBX::Reflection::SignalDescImpl<2,void __cdecl(boost::shared_ptr<RBX::Instance>,float)>::connectGeneric
	// clang-format on

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
	// TEMPLATE: WEBSERVICE 0x10045aa0
	// RBX::Reflection::SignalDesc<RBX::Lighting,void __cdecl(bool)>::SignalDesc<RBX::Lighting,void __cdecl(bool)>
	// TEMPLATE: WEBSERVICE 0x1004c110
	// RBX::Reflection::SignalDesc<RBX::Instance,void __cdecl(RBX::Reflection::PropertyDescriptor const *)>::SignalDesc<RBX::Instance,void __cdecl(RBX::Reflection::PropertyDescriptor const *)>
	// TEMPLATE: WEBSERVICE 0x100564f0
	// RBX::Reflection::SignalDesc<RBX::RunService,void __cdecl(float,float)>::SignalDesc<RBX::RunService,void __cdecl(float,float)>
	// TEMPLATE: WEBSERVICE 0x10056690
	// RBX::Reflection::SignalDesc<RBX::RunService,void __cdecl(float)>::SignalDesc<RBX::RunService,void __cdecl(float)>
	// TEMPLATE: WEBSERVICE 0x100c92c0
	// RBX::Reflection::SignalDesc<RBX::HopperBin,void __cdecl(boost::shared_ptr<RBX::Instance>)>::SignalDesc<RBX::HopperBin,void __cdecl(boost::shared_ptr<RBX::Instance>)>
	// SYNTHETIC: WEBSERVICE 0x100c9390
	// RBX::Reflection::SignalDesc<RBX::Humanoid,void __cdecl(void)>::`scalar deleting destructor'
	// TEMPLATE: WEBSERVICE 0x1009e9f0
	// RBX::Reflection::SignalDesc<RBX::PartInstance,void __cdecl(boost::shared_ptr<RBX::Instance>)>::SignalDesc<RBX::PartInstance,void __cdecl(boost::shared_ptr<RBX::Instance>)>
	// TEMPLATE: WEBSERVICE 0x100a3c20
	// RBX::Reflection::SignalDesc<RBX::Humanoid,void __cdecl(float)>::SignalDesc<RBX::Humanoid,void __cdecl(float)>
	// TEMPLATE: WEBSERVICE 0x100a3cf0
	// RBX::Reflection::SignalDesc<RBX::Humanoid,void __cdecl(bool)>::SignalDesc<RBX::Humanoid,void __cdecl(bool)>
	// TEMPLATE: WEBSERVICE 0x100cbe90
	// RBX::Reflection::SignalDesc<RBX::Tool,void __cdecl(boost::shared_ptr<RBX::Instance>)>::SignalDesc<RBX::Tool,void __cdecl(boost::shared_ptr<RBX::Instance>)>
	// TEMPLATE: WEBSERVICE 0x100e8ac0
	// RBX::Reflection::SignalDesc<RBX::FlagStand,void __cdecl(boost::shared_ptr<RBX::Instance>)>::SignalDesc<RBX::FlagStand,void __cdecl(boost::shared_ptr<RBX::Instance>)>
	// TEMPLATE: WEBSERVICE 0x100f2420
	// RBX::Reflection::SignalDesc<RBX::ObjectValue,void __cdecl(boost::shared_ptr<RBX::Instance>)>::SignalDesc<RBX::ObjectValue,void __cdecl(boost::shared_ptr<RBX::Instance>)>
	// TEMPLATE: WEBSERVICE 0x100f2560
	// RBX::Reflection::SignalDesc<RBX::Value<int,&RBX::sIntValue>,void __cdecl(int)>::SignalDesc<RBX::Value<int,&RBX::sIntValue>,void __cdecl(int)>
	// TEMPLATE: WEBSERVICE 0x100f26a0
	// RBX::Reflection::SignalDesc<RBX::Value<bool,&RBX::sBoolValue>,void __cdecl(bool)>::SignalDesc<RBX::Value<bool,&RBX::sBoolValue>,void __cdecl(bool)>
	// TEMPLATE: WEBSERVICE 0x100f27e0
	// RBX::Reflection::SignalDesc<RBX::Value<double,&RBX::sDoubleValue>,void __cdecl(double)>::SignalDesc<RBX::Value<double,&RBX::sDoubleValue>,void __cdecl(double)>
	// TEMPLATE: WEBSERVICE 0x100f2a60
	// RBX::Reflection::SignalDesc<RBX::Value<G3D::Vector3,&RBX::sVector3Value>,void __cdecl(G3D::Vector3)>::SignalDesc<RBX::Value<G3D::Vector3,&RBX::sVector3Value>,void __cdecl(G3D::Vector3)>
	// TEMPLATE: WEBSERVICE 0x100f2ba0
	// RBX::Reflection::SignalDesc<RBX::Value<G3D::CoordinateFrame,&RBX::sCFrameValue>,void __cdecl(G3D::CoordinateFrame)>::SignalDesc<RBX::Value<G3D::CoordinateFrame,&RBX::sCFrameValue>,void __cdecl(G3D::CoordinateFrame)>
	// TEMPLATE: WEBSERVICE 0x100f2ce0
	// RBX::Reflection::SignalDesc<RBX::Value<G3D::Color3,&RBX::sColor3Value>,void __cdecl(G3D::Color3)>::SignalDesc<RBX::Value<G3D::Color3,&RBX::sColor3Value>,void __cdecl(G3D::Color3)>
	// TEMPLATE: WEBSERVICE 0x100f2e20
	// RBX::Reflection::SignalDesc<RBX::Value<RBX::BrickColor,&RBX::sBrickColorValue>,void __cdecl(RBX::BrickColor)>::SignalDesc<RBX::Value<RBX::BrickColor,&RBX::sBrickColorValue>,void __cdecl(RBX::BrickColor)>
	// TEMPLATE: WEBSERVICE 0x10117950
	// RBX::Reflection::SignalDesc<RBX::Mouse,void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >)>::SignalDesc<RBX::Mouse,void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >)>
	// TEMPLATE: WEBSERVICE 0x1018f2e0
	// RBX::Reflection::SignalDesc<RBX::Network::Player,void __cdecl(float)>::SignalDesc<RBX::Network::Player,void __cdecl(float)>
	// TEMPLATE: WEBSERVICE 0x10194160
	// RBX::Reflection::SignalDesc<RBX::Network::Players,void __cdecl(boost::shared_ptr<RBX::Instance>)>::SignalDesc<RBX::Network::Players,void __cdecl(boost::shared_ptr<RBX::Instance>)>
	// TEMPLATE: WEBSERVICE 0x10197d60
	// RBX::Reflection::SignalDesc<RBX::Network::Client,void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >,boost::shared_ptr<RBX::Instance>)>::SignalDesc<RBX::Network::Client,void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >,boost::shared_ptr<RBX::Instance>)>
	// TEMPLATE: WEBSERVICE 0x10197ea0
	// RBX::Reflection::SignalDesc<RBX::Network::Client,void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >,int)>::SignalDesc<RBX::Network::Client,void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >,int)>
	// TEMPLATE: WEBSERVICE 0x10197fe0
	// RBX::Reflection::SignalDesc<RBX::Network::Client,void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >)>::SignalDesc<RBX::Network::Client,void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >)>
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

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x100e2d00
	// RBX::Reflection::SignalDesc<RBX::Explosion,void __cdecl(boost::shared_ptr<RBX::Instance>,float)>::SignalDesc<RBX::Explosion,void __cdecl(boost::shared_ptr<RBX::Instance>,float)>
	// clang-format on
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
