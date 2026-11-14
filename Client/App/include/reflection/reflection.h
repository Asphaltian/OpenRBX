#ifndef REFLECTION_REFLECTION_H
#define REFLECTION_REFLECTION_H

#include "placeholder.h"
#include "reflection/object.h"
#include "reflection/property.h"
#include "reflection/signal.h"
#include "util/Handle.h"
#include "v8xml/Serializer.h"
#include "v8xml/XmlElement.h"

#include <boost/cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/type_traits/function_traits.hpp>

namespace RBX {
namespace Reflection {

template <class T, const char* sName, class Base>
class __declspec(novtable) Described : public Base
{
public:
	Described() { this->descriptor = &classDescriptor(); }

	template <class U>
	Described(U arg0) : Base(arg0)
	{
		this->descriptor = &classDescriptor();
	}

	static ClassDescriptor& classDescriptor()
	{
		static ClassDescriptor foo(Base::classDescriptor(), sName);

		return foo;
	}
};

template <class Class, class T>
class EnumPropDescriptor : public EnumPropertyDescriptor
{
	std::auto_ptr<typename TypedPropertyDescriptor<T>::GetSet> getset;

public:
	template <class GetFunc, class SetFunc>
	EnumPropDescriptor(
		const char* name,
		const char* category,
		GetFunc get,
		SetFunc set,
		typename PropertyDescriptor::Functionality flags = PropertyDescriptor::STANDARD
	)
		: EnumPropertyDescriptor(Class::classDescriptor(), EnumDesc<T>::singleton(), name, category, flags),
		  getset(PropDescriptor<Class, T>::getset(get, set))
	{
	}

	virtual bool isReadOnly() const { return getset->isReadOnly(); }

	T getValue(const DescribedBase* instance) const { return getset->getValue(instance); }

	void setValue(DescribedBase* instance, T value) const { getset->setValue(instance, value); }

	virtual bool equalValues(const DescribedBase* a, const DescribedBase* b) const
	{
		return getValue(a) == getValue(b);
	}

	virtual int getEnumValue(const DescribedBase* instance) const { return getValue(instance); }

	virtual bool setEnumValue(DescribedBase* instance, int intValue) const
	{
		if (EnumDesc<T>::singleton().isValue(intValue)) {
			setValue(instance, static_cast<T>(intValue));
			return true;
		}
		else {
			return false;
		}
	}

	virtual unsigned int getIndexValue(const DescribedBase* instance) const
	{
		return EnumDesc<T>::singleton().convertToIndex(getValue(instance));
	}

	virtual bool setIndexValue(DescribedBase* instance, unsigned int index) const
	{
		T value;

		if (EnumDesc<T>::singleton().convertToValue(index, value)) {
			setValue(instance, value);
			return true;
		}
		else {
			return false;
		}
	}

	virtual bool hasStringValue() const { return true; }

	virtual std::string getStringValue(const DescribedBase* instance) const
	{
		return EnumDesc<T>::singleton().convertToString(getValue(instance));
	}

	virtual bool setStringValue(DescribedBase* instance, const std::string& text) const
	{
		T value;

		if (EnumDesc<T>::singleton().convertToValue(text, value)) {
			setValue(instance, value);
			return true;
		}
		else {
			return false;
		}
	}

	virtual bool setStringValue(DescribedBase* instance, const Name& name) const
	{
		T value;

		if (EnumDesc<T>::singleton().convertToValue(name, value)) {
			setValue(instance, value);
			return true;
		}
		else {
			return false;
		}
	}

	virtual void readValue(DescribedBase* instance, const XmlElement* element, IReferenceBinder& binder) const
	{
		if (!element->isXsiNil()) {
			if (element->isValueType<std::string>()) {
				std::string sValue;

				if (element->getValue(sValue)) {
					T e;

					if (EnumDesc<T>::singleton().convertToValue(sValue, e)) {
						setValue(instance, e);
						return;
					}
					else if (sValue.size() == 0) {
						if (setIndexValue(instance, 0)) {
							return;
						}
					}
				}
			}

			int value;

			if (element->getValue(value)) {
				setValue(instance, static_cast<T>(value));
			}
		}
	}

	virtual void writeValue(const DescribedBase* instance, XmlElement* element) const
	{
		element->setValue(static_cast<int>(getValue(instance)));
	}
};

template <class Class, class T>
class RefPropDescriptor : public RefPropertyDescriptor, public IIDREF
{
public:
	template <class Getter, class Setter>
	RefPropDescriptor(
		const char* name,
		const char* category,
		Getter get,
		Setter set,
		typename PropertyDescriptor::Functionality flags = PropertyDescriptor::STANDARD
	)
		: RefPropertyDescriptor(Class::classDescriptor(), RefType::singleton<T*>(), name, category, flags),
		  getset(PropDescriptor<Class, T*>::template getset<Getter, Setter>(get, set))
	{
	}

	virtual bool isReadOnly() const { return getset->isReadOnly(); }

	T* getValue(const DescribedBase* instance) const { return getset->getValue(instance); }

	void setValue(DescribedBase* instance, T* value) const { getset->setValue(instance, value); }

	virtual bool equalValues(const DescribedBase* a, const DescribedBase* b) const
	{
		return getValue(a) == getValue(b);
	}

	virtual DescribedBase* getRefValue(const DescribedBase* instance) const { return getValue(instance); }

	virtual void setRefValue(DescribedBase* instance, DescribedBase* value) const
	{
		T* t = value != NULL ? boost::polymorphic_cast<T*>(value) : NULL;

		setValue(instance, t);
	}

	virtual void readValue(DescribedBase* instance, const XmlElement* element, IReferenceBinder& binder) const
	{
		binder.announceIDREF(element, instance, this);
	}

	virtual void writeValue(const DescribedBase* instance, XmlElement* element) const
	{
		element->setValue(InstanceHandle(getValue(instance)));
	}

	virtual void assignIDREF(DescribedBase* propertyOwner, const InstanceHandle& handle) const
	{
		boost::shared_ptr<Instance> t = handle.getTarget();

		setValue(propertyOwner, static_cast<T*>(t.get()));
	}

private:
	std::auto_ptr<typename TypedPropertyDescriptor<T*>::GetSet> getset;
};

template <class Class>
class FuncDesc : public FunctionDescriptor
{
protected:
	FuncDesc(const char* name, Security security) : FunctionDescriptor(Class::classDescriptor(), name, security) {}
};

template <class Class, class Signature, int arity>
class BoundFuncDesc;

template <class Class, class Signature>
class BoundFuncDesc<Class, Signature, 0> : public FuncDesc<Class>
{
public:
	typedef typename boost::function_traits<Signature>::result_type result_type;
	typedef result_type (Class::*Function)();

private:
	Function function;

	template <class R>
	void call(Class* o, Value& returnValue) const
	{
		returnValue = (o->*function)();
	}

	template <>
	void call<void>(Class* o, Value& returnValue) const
	{
		(o->*function)();
	}

	void declareSignature() { this->signature.resultType = &Type::singleton<result_type>(); }

public:
	BoundFuncDesc(Function function, const char* name, typename FunctionDescriptor::Security security)
		: FuncDesc<Class>(name, security), function(function)
	{
		declareSignature();
	}

	virtual void execute(DescribedBase* instance, typename FunctionDescriptor::Arguments& arguments) const
	{
		call<result_type>(boost::polymorphic_cast<Class*>(instance), arguments.returnValue);
	}
};

template <class Class, class Signature>
class BoundFuncDesc<Class, Signature, 1> : public FuncDesc<Class>
{
public:
	typedef typename boost::function_traits<Signature>::result_type result_type;
	typedef typename boost::function_traits<Signature>::arg1_type Arg1;
	typedef result_type (Class::*Function)(Arg1);

private:
	Function function;
	Value default1;

	void declareSignature(const char* arg1Name)
	{
		this->signature.resultType = &Type::singleton<result_type>();
		this->signature.addArgument(Name::declare(arg1Name, -1), Type::singleton<Arg1>(), default1);
	}

	template <class R>
	void call(Class* o, Value& returnValue, Value& arg1) const
	{
		returnValue = (o->*function)(arg1.convert<Arg1>());
	}

	template <>
	void call<void>(Class* o, Value& returnValue, Value& arg1) const
	{
		(o->*function)(arg1.convert<Arg1>());
	}

public:
	BoundFuncDesc(
		Function function, const char* name, const char* arg1Name, typename FunctionDescriptor::Security security
	)
		: FuncDesc<Class>(name, security), function(function)
	{
		declareSignature(arg1Name);
	}

	BoundFuncDesc(
		Function function,
		const char* name,
		const char* arg1Name,
		Arg1 default1,
		typename FunctionDescriptor::Security security
	)
		: FuncDesc<Class>(name, security), function(function), default1(default1)
	{
		declareSignature(arg1Name);
	}

	virtual void execute(DescribedBase* instance, typename FunctionDescriptor::Arguments& arguments) const
	{
		Value arg1(default1);
		arguments.get(1, arg1);
		call<result_type>(boost::polymorphic_cast<Class*>(instance), arguments.returnValue, arg1);
	}
};

template <class Class, class Signature>
class BoundFuncDesc<Class, Signature, 2> : public FuncDesc<Class>
{
public:
	typedef typename boost::function_traits<Signature>::result_type result_type;
	typedef typename boost::function_traits<Signature>::arg1_type Arg1;
	typedef typename boost::function_traits<Signature>::arg2_type Arg2;
	typedef result_type (Class::*Function)(Arg1, Arg2);

private:
	Function function;
	Value default1;
	Value default2;

	void declareSignature(const char* arg1Name, const char* arg2Name)
	{
		this->signature.resultType = &Type::singleton<result_type>();
		this->signature.addArgument(Name::declare(arg1Name, -1), Type::singleton<Arg1>(), default1);
		this->signature.addArgument(Name::declare(arg2Name, -1), Type::singleton<Arg2>(), default2);
	}

	template <class R>
	void call(Class* o, Value& returnValue, Value& arg1, Value& arg2) const
	{
		returnValue = (o->*function)(arg1.convert<Arg1>(), arg2.convert<Arg2>());
	}

	template <>
	void call<void>(Class* o, Value& returnValue, Value& arg1, Value& arg2) const
	{
		(o->*function)(arg1.convert<Arg1>(), arg2.convert<Arg2>());
	}

public:
	BoundFuncDesc(
		Function function,
		const char* name,
		const char* arg1Name,
		const char* arg2Name,
		typename FunctionDescriptor::Security security
	)
		: FuncDesc<Class>(name, security), function(function)
	{
		declareSignature(arg1Name, arg2Name);
	}

	BoundFuncDesc(
		Function function,
		const char* name,
		const char* arg1Name,
		const char* arg2Name,
		Arg2 default2,
		typename FunctionDescriptor::Security security
	)
		: FuncDesc<Class>(name, security), function(function), default2(default2)
	{
		declareSignature(arg1Name, arg2Name);
	}

	BoundFuncDesc(
		Function function,
		const char* name,
		const char* arg1Name,
		Arg1 default1,
		const char* arg2Name,
		Arg2 default2,
		typename FunctionDescriptor::Security security
	)
		: FuncDesc<Class>(name, security), function(function), default1(default1), default2(default2)
	{
		declareSignature(arg1Name, arg2Name);
	}

	virtual void execute(DescribedBase* instance, typename FunctionDescriptor::Arguments& arguments) const
	{
		Value arg1(default1);
		arguments.get(1, arg1);
		Value arg2(default2);
		arguments.get(2, arg2);
		call<result_type>(boost::polymorphic_cast<Class*>(instance), arguments.returnValue, arg1, arg2);
	}
};

template <class Class, class Signature>
class BoundFuncDesc<Class, Signature, 3> : public FuncDesc<Class>
{
public:
	typedef typename boost::function_traits<Signature>::result_type result_type;
	typedef typename boost::function_traits<Signature>::arg1_type Arg1;
	typedef typename boost::function_traits<Signature>::arg2_type Arg2;
	typedef typename boost::function_traits<Signature>::arg3_type Arg3;
	typedef result_type (Class::*Function)(Arg1, Arg2, Arg3);

private:
	Function function;
	Value default1;
	Value default2;
	Value default3;

	void declareSignature(const char* arg1Name, const char* arg2Name, const char* arg3Name)
	{
		this->signature.resultType = &Type::singleton<result_type>();
		this->signature.addArgument(Name::declare(arg1Name, -1), Type::singleton<Arg1>(), default1);
		this->signature.addArgument(Name::declare(arg2Name, -1), Type::singleton<Arg2>(), default2);
		this->signature.addArgument(Name::declare(arg3Name, -1), Type::singleton<Arg3>(), default3);
	}

	template <class R>
	void call(Class* o, Value& returnValue, Value& arg1, Value& arg2, Value& arg3) const
	{
		returnValue = (o->*function)(arg1.convert<Arg1>(), arg2.convert<Arg2>(), arg3.convert<Arg3>());
	}

	template <>
	void call<void>(Class* o, Value& returnValue, Value& arg1, Value& arg2, Value& arg3) const
	{
		(o->*function)(arg1.convert<Arg1>(), arg2.convert<Arg2>(), arg3.convert<Arg3>());
	}

public:
	BoundFuncDesc(
		Function function,
		const char* name,
		const char* arg1Name,
		const char* arg2Name,
		const char* arg3Name,
		typename FunctionDescriptor::Security security
	)
		: FuncDesc<Class>(name, security), function(function)
	{
		declareSignature(arg1Name, arg2Name, arg3Name);
	}

	BoundFuncDesc(
		Function function,
		const char* name,
		const char* arg1Name,
		const char* arg2Name,
		const char* arg3Name,
		Arg3 default3,
		typename FunctionDescriptor::Security security
	)
		: FuncDesc<Class>(name, security), function(function), default3(default3)
	{
		declareSignature(arg1Name, arg2Name, arg3Name);
	}

	virtual void execute(DescribedBase* instance, typename FunctionDescriptor::Arguments& arguments) const
	{
		Value arg1(default1);
		arguments.get(1, arg1);
		Value arg2(default2);
		arguments.get(2, arg2);
		Value arg3(default3);
		arguments.get(3, arg3);
		call<result_type>(boost::polymorphic_cast<Class*>(instance), arguments.returnValue, arg1, arg2, arg3);
	}
};

template <class Class, class Signature>
class BoundFuncDesc<Class, Signature, 4> : public FuncDesc<Class>
{
public:
	typedef typename boost::function_traits<Signature>::result_type result_type;
	typedef typename boost::function_traits<Signature>::arg1_type Arg1;
	typedef typename boost::function_traits<Signature>::arg2_type Arg2;
	typedef typename boost::function_traits<Signature>::arg3_type Arg3;
	typedef typename boost::function_traits<Signature>::arg4_type Arg4;
	typedef result_type (Class::*Function)(Arg1, Arg2, Arg3, Arg4);

private:
	Function function;
	Value default1;
	Value default2;
	Value default3;
	Value default4;

	void declareSignature(const char* arg1Name, const char* arg2Name, const char* arg3Name, const char* arg4Name)
	{
		this->signature.resultType = &Type::singleton<result_type>();
		this->signature.addArgument(Name::declare(arg1Name, -1), Type::singleton<Arg1>(), default1);
		this->signature.addArgument(Name::declare(arg2Name, -1), Type::singleton<Arg2>(), default2);
		this->signature.addArgument(Name::declare(arg3Name, -1), Type::singleton<Arg3>(), default3);
		this->signature.addArgument(Name::declare(arg4Name, -1), Type::singleton<Arg4>(), default4);
	}

	template <class R>
	void call(Class* o, Value& returnValue, Value& arg1, Value& arg2, Value& arg3, Value& arg4) const
	{
		returnValue =
			(o->*function)(arg1.convert<Arg1>(), arg2.convert<Arg2>(), arg3.convert<Arg3>(), arg4.convert<Arg4>());
	}

	template <>
	void call<void>(Class* o, Value& returnValue, Value& arg1, Value& arg2, Value& arg3, Value& arg4) const
	{
		(o->*function)(arg1.convert<Arg1>(), arg2.convert<Arg2>(), arg3.convert<Arg3>(), arg4.convert<Arg4>());
	}

public:
	BoundFuncDesc(
		Function function,
		const char* name,
		const char* arg1Name,
		Arg1 default1,
		const char* arg2Name,
		Arg2 default2,
		const char* arg3Name,
		Arg3 default3,
		const char* arg4Name,
		Arg4 default4,
		typename FunctionDescriptor::Security security
	)
		: FuncDesc<Class>(name, security), function(function), default1(default1), default2(default2),
		  default3(default3), default4(default4)
	{
		declareSignature(arg1Name, arg2Name, arg3Name, arg4Name);
	}

	BoundFuncDesc(
		Function function,
		const char* name,
		const char* arg1Name,
		const char* arg2Name,
		Arg2 default2,
		const char* arg3Name,
		Arg3 default3,
		const char* arg4Name,
		Arg4 default4,
		typename FunctionDescriptor::Security security
	)
		: FuncDesc<Class>(name, security), function(function), default2(default2), default3(default3),
		  default4(default4)
	{
		declareSignature(arg1Name, arg2Name, arg3Name, arg4Name);
	}

	BoundFuncDesc(
		Function function,
		const char* name,
		const char* arg1Name,
		const char* arg2Name,
		const char* arg3Name,
		Arg3 default3,
		const char* arg4Name,
		Arg4 default4,
		typename FunctionDescriptor::Security security
	)
		: FuncDesc<Class>(name, security), function(function), default3(default3), default4(default4)
	{
		declareSignature(arg1Name, arg2Name, arg3Name, arg4Name);
	}

	BoundFuncDesc(
		Function function,
		const char* name,
		const char* arg1Name,
		const char* arg2Name,
		const char* arg3Name,
		const char* arg4Name,
		Arg4 default4,
		typename FunctionDescriptor::Security security
	)
		: FuncDesc<Class>(name, security), function(function), default4(default4)
	{
		declareSignature(arg1Name, arg2Name, arg3Name, arg4Name);
	}

	BoundFuncDesc(
		Function function,
		const char* name,
		const char* arg1Name,
		const char* arg2Name,
		const char* arg3Name,
		const char* arg4Name,
		typename FunctionDescriptor::Security security
	)
		: FuncDesc<Class>(name, security), function(function)
	{
		declareSignature(arg1Name, arg2Name, arg3Name, arg4Name);
	}

	virtual void execute(DescribedBase* instance, typename FunctionDescriptor::Arguments& arguments) const
	{
		Value arg1(default1);
		arguments.get(1, arg1);
		Value arg2(default2);
		arguments.get(2, arg2);
		Value arg3(default3);
		arguments.get(3, arg3);
		Value arg4(default4);
		arguments.get(4, arg4);
		call<result_type>(boost::polymorphic_cast<Class*>(instance), arguments.returnValue, arg1, arg2, arg3, arg4);
	}
};

} // namespace Reflection
} // namespace RBX

#endif // REFLECTION_REFLECTION_H
