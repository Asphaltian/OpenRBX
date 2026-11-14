#ifndef UTIL_OBJECT_H
#define UTIL_OBJECT_H

#include "placeholder.h"
#include "util/Name.h"

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <map>
#include <stdlib.h>

namespace RBX {

class Instance;

class Object
{
public:
	virtual ~Object() {}
};

template <class T>
boost::shared_ptr<T> shared_from(T* r)
{
	return r != NULL ? boost::shared_static_cast<T>(r->shared_from_this()) : boost::shared_ptr<T>();
}

template <class T, class Base>
boost::shared_ptr<T> shared_from_dynamic_cast(boost::enable_shared_from_this<Base>* r)
{
	return r != NULL ? boost::shared_dynamic_cast<T>(r->shared_from_this()) : boost::shared_ptr<T>();
}

template <class T, class Base>
boost::shared_ptr<T> shared_from_polymorphic_downcast(boost::enable_shared_from_this<Base>* r)
{
	return r != NULL ? boost::shared_polymorphic_downcast<T>(r->shared_from_this()) : boost::shared_ptr<T>();
}

template <class T>
class Creatable : public Object
{
public:
	class Deleter
	{
	public:
		void operator()(T* instance)
		{
			T::predelete(instance);
			delete instance;
		}
	};

	template <class U>
	static boost::shared_ptr<U> create()
	{
		return boost::shared_ptr<U>(new U(), Deleter());
	}

	template <class U, class A>
	static boost::shared_ptr<U> create(A p1)
	{
		return boost::shared_ptr<U>(new U(p1), Deleter());
	}

	template <class U, class A, class B>
	static boost::shared_ptr<U> create(A p1, B p2)
	{
		return boost::shared_ptr<U>(new U(p1, p2), Deleter());
	}

	template <class U, class A, class B, class C>
	static boost::shared_ptr<U> create(A p1, B p2, C p3)
	{
		return boost::shared_ptr<U>(new U(p1, p2, p3), Deleter());
	}

	template <class U, class A, class B, class C, class D>
	static boost::shared_ptr<U> create(A p1, B p2, C p3, D p4)
	{
		return boost::shared_ptr<U>(new U(p1, p2, p3, p4), Deleter());
	}

	template <class U, class A, class B, class C, class D, class E>
	static boost::shared_ptr<U> create(A p1, B p2, C p3, D p4, E p5)
	{
		return boost::shared_ptr<U>(new U(p1, p2, p3, p4, p5), Deleter());
	}

	template <class U, class A, class B, class C, class D, class E, class F>
	static boost::shared_ptr<U> create(A p1, B p2, C p3, D p4, E p5, F p6)
	{
		return boost::shared_ptr<U>(new U(p1, p2, p3, p4, p5, p6), Deleter());
	}

protected:
	Creatable() {}
	virtual ~Creatable() {}

	static void* operator new(size_t size) { return malloc(size); }

	static void operator delete(void* p) { free(p); }
};

class __declspec(novtable) ICreator
{
public:
	virtual boost::shared_ptr<Object> create() const = 0;
};

template <class T>
class AbstractFactoryProduct : public Creatable<T>
{
protected:
	AbstractFactoryProduct() {}

	static std::map<const Name*, const ICreator*>& getCreators()
	{
		static std::map<const Name*, const ICreator*> creators;
		return creators;
	}

public:
	virtual const Name& getClassName() const = 0;

	static boost::shared_ptr<T> create(const Name& name)
	{
		std::map<const Name*, const ICreator*>::iterator iter = getCreators().find(&name);
		if (iter == getCreators().end()) {
			return boost::shared_ptr<T>();
		}

		return boost::shared_dynamic_cast<T>(iter->second->create());
	}
};

template <class T, class Base, const char* sName>
class FactoryProduct : public Base
{
public:
	class Creator : public ICreator
	{
	public:
		virtual boost::shared_ptr<Object> create() const { return Creatable<Instance>::create<T>(); }

		const Name& getClassName() const { return Name::declare<sName>(); }

		Creator() { FactoryProduct::getCreators()[&getClassName()] = this; }

		~Creator() { FactoryProduct::getCreators().erase(&getClassName()); }
	};

	const Creator& getCreator() const { return creator; }

	static const Name& className() { return Name::declare<sName>(); }

	virtual const Name& getClassName() const { return className(); }

protected:
	FactoryProduct() {}

	template <class U>
	FactoryProduct(U arg0) : Base(arg0)
	{
	}

	virtual ~FactoryProduct() {}

private:
	static const Creator creator;
};

template <class T, class Base, const char* sName>
const typename FactoryProduct<T, Base, sName>::Creator FactoryProduct<T, Base, sName>::creator;

template <class Base, const char* sName>
class NonFactoryProduct : public Base
{
public:
	NonFactoryProduct() {}

	template <class U>
	NonFactoryProduct(U arg0) : Base(arg0)
	{
	}

	static const Name& className() { return Name::declare<sName>(); }

	const Name& getClassName() const { return className(); }
};

} // namespace RBX

#endif // UTIL_OBJECT_H
