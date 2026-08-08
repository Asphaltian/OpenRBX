#ifndef UTIL_COMPUTEPROP_H
#define UTIL_COMPUTEPROP_H

namespace RBX {

template <class T, class Class>
class ComputeProp
{
public:
	typedef T (Class::*GetFunc)() const;

	ComputeProp(Class* object, GetFunc getFunc) : dirty(true), object(object), getFunc(getFunc) {}

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x10059d30
	// RBX::ComputeProp<RBX::Extents,RBX::ModelInstance>::getValue
	// clang-format on
	T getValue() const
	{
		if (dirty) {
			val = (object->*getFunc)();
			dirty = false;
		}

		return val;
	}

	operator T() const { return getValue(); }

	T* getValuePointer() const
	{
		getValue();
		return &val;
	}

	T& getValueRef() const
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
	mutable T val;      // 0x00
	mutable bool dirty; // 0x04
	Class* object;      // 0x08
	GetFunc getFunc;    // 0x0c
};

} // namespace RBX

#endif // UTIL_COMPUTEPROP_H
