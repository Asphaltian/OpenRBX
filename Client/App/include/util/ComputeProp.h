#ifndef UTIL_COMPUTEPROP_H
#define UTIL_COMPUTEPROP_H

namespace RBX {

template <class T, class Class>
class ComputeProp
{
public:
	typedef T (Class::*GetFunc)() const;

	ComputeProp(Class* object, GetFunc getFunc) : dirty(true), object(object), getFunc(getFunc) {}

	T getValue() const
	{
		if (dirty) {
			val = (object->*getFunc)();
			dirty = false;
		}

		return val;
	}

	operator T() const { return getValue(); }

	const T* getValuePointer() const
	{
		getValue();
		return &val;
	}

	const T& getValueRef() const
	{
		getValue();
		return val;
	}

	bool setDirty() const
	{
		dirty = true;
		return dirty;
	}

private:
	mutable T val;
	mutable bool dirty;
	Class* object;
	GetFunc getFunc;
};

} // namespace RBX

#endif // UTIL_COMPUTEPROP_H
