#ifndef REFLECTION_SIGNAL_H
#define REFLECTION_SIGNAL_H

#include "placeholder.h"
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

class SignalInstance : public boost::noncopyable
{
public:
	SignalSource* getSource() { return source; }

	template <class Slot>
	boost::signals::connection connectGeneric(Slot slot, boost::signals::connect_position position);

	virtual ~SignalInstance();

protected:
	SignalInstance(SignalSource* source, const SignalDescriptor& descriptor) : source(source), descriptor(descriptor) {}

private:
	SignalSource* source;

public:
	const SignalDescriptor& descriptor;
};

class __declspec(novtable) SignalSource
{
public:
	typedef std::map<const SignalDescriptor*, boost::shared_ptr<SignalInstance> > SignalMap;

	virtual ~SignalSource();

	void disconnect_all_slots();

private:
	friend class SignalDescriptor;

	boost::scoped_ptr<SignalMap> signals;
};

class __declspec(novtable) GenericSlotWrapper
{
public:
	virtual ~GenericSlotWrapper();

	virtual void execute(const std::vector<boost::any>& arguments) = 0;

	template <class Slot>
	static GenericSlotWrapper* create(Slot slot);
};

template <class Slot>
class TGenericSlotWrapper : public GenericSlotWrapper
{
	friend class GenericSlotWrapper;

public:
	virtual void execute(const std::vector<boost::any>& arguments);

private:
	TGenericSlotWrapper(const Slot& slot) : slot(slot) {}

	Slot slot;
};

template <class Slot>
void TGenericSlotWrapper<Slot>::execute(const std::vector<boost::any>& arguments)
{
	try {
		slot(arguments);
	}
	catch (const std::exception& e) {
		StandardOut::singleton()->print(MESSAGE_ERROR, "Exception caught in TGenericSlotWrapper. %s", e.what());
	}
}

template <class Slot>
GenericSlotWrapper* GenericSlotWrapper::create(Slot slot)
{
	return new TGenericSlotWrapper<Slot>(slot);
}

class __declspec(novtable) SignalDescriptor : public MemberDescriptor
{
public:
	typedef Signal ConstMember;
	typedef Signal Member;

	void (*signalCreatedHook)(SignalSource*);

protected:
	friend class SignalInstance;

	SignalInstance* findSignalInstance(const SignalSource* source) const;

private:
	virtual boost::signals::connection connectGeneric(
		SignalInstance* instance, GenericSlotWrapper* wrapper, boost::signals::connect_position position
	) const = 0;

	virtual SignalInstance* newSignalInstance(SignalSource& source) const = 0;

public:
	boost::shared_ptr<SignalInstance> getSignalInstance(SignalSource& source) const;
	const SignatureDescriptor& getSignature() const;

protected:
	SignalDescriptor(ClassDescriptor& classDescriptor, const char* name);

	SignatureDescriptor signature;
};

class Signal
{
public:
	Signal(const Signal& other) : descriptor(other.descriptor), instance(other.instance) {}
	Signal(const SignalDescriptor& descriptor, DescribedBase* instance) : descriptor(&descriptor), instance(instance) {}

	const Name& getName() const { return descriptor->name; }

	const SignalDescriptor* getDescriptor() const { return descriptor; }

	boost::shared_ptr<SignalInstance> getSignalInstance() const;

protected:
	const SignalDescriptor* descriptor;
	DescribedBase* instance;
};

template <class Slot>
boost::signals::connection SignalInstance::connectGeneric(Slot slot, boost::signals::connect_position pos)
{
	return descriptor.connectGeneric(this, GenericSlotWrapper::create(slot), pos);
}

template <class Signature>
class __declspec(novtable) TSignalDesc : public SignalDescriptor
{
public:
	typedef boost::function_traits<Signature> traits;

	class TSignalInstance
		: public SignalInstance,
		  public boost::signal<Signature, boost::last_value<void>, int, std::less<int>, boost::function<Signature> >
	{
	public:
		TSignalInstance(SignalSource* source, const SignalDescriptor& descriptor) : SignalInstance(source, descriptor)
		{
		}
	};

protected:
	TSignalDesc(ClassDescriptor& classDescriptor, const char* name) : SignalDescriptor(classDescriptor, name) {}

private:
	virtual SignalInstance* newSignalInstance(SignalSource& source) const
	{
		return new TSignalInstance(&source, *this);
	}

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
};

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
		boost::shared_ptr<GenericSlotWrapper> wrapper;
	};

	virtual boost::signals::connection connectGeneric(
		SignalInstance* si, GenericSlotWrapper* wrapper, boost::signals::connect_position pos
	) const
	{
		return static_cast<typename TSignalDesc<Signature>::TSignalInstance*>(si)->connect(
			GenericSlotAdapter(wrapper), pos
		);
	}

	SignalDescImpl(ClassDescriptor& classDescriptor, const char* name) : TSignalDesc<Signature>(classDescriptor, name)
	{
	}

public:
	void fire(SignalSource* source)
	{
		typename TSignalDesc<Signature>::TSignalInstance* instance = this->findSig(source);

		if (instance != NULL) {
			(*instance)();
		}
	}
};

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
			std::vector<boost::any> args(1);

			args[0] = arg1;

			wrapper->execute(args);
		}

	private:
		boost::shared_ptr<GenericSlotWrapper> wrapper;
	};

	virtual boost::signals::connection connectGeneric(
		SignalInstance* si, GenericSlotWrapper* wrapper, boost::signals::connect_position pos
	) const
	{
		return static_cast<typename TSignalDesc<Signature>::TSignalInstance*>(si)->connect(
			GenericSlotAdapter(wrapper), pos
		);
	}

	SignalDescImpl(ClassDescriptor& classDescriptor, const char* name) : TSignalDesc<Signature>(classDescriptor, name)
	{
	}
};

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
			std::vector<boost::any> args(2);

			args[0] = arg1;
			args[1] = arg2;

			wrapper->execute(args);
		}

	private:
		boost::shared_ptr<GenericSlotWrapper> wrapper;
	};

	virtual boost::signals::connection connectGeneric(
		SignalInstance* si, GenericSlotWrapper* wrapper, boost::signals::connect_position pos
	) const
	{
		return static_cast<typename TSignalDesc<Signature>::TSignalInstance*>(si)->connect(
			GenericSlotAdapter(wrapper), pos
		);
	}

	SignalDescImpl(ClassDescriptor& classDescriptor, const char* name) : TSignalDesc<Signature>(classDescriptor, name)
	{
	}
};

template <class Class, class Signature>
class SignalDesc : public SignalDescImpl<boost::function_traits<Signature>::arity, Signature>
{
public:
	SignalDesc(const char* name)
		: SignalDescImpl<boost::function_traits<Signature>::arity, Signature>(Class::classDescriptor(), name)
	{
	}

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
