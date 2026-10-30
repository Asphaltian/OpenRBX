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
	// TEMPLATE: WEBSERVICE 0x1005a5b0
	// TEMPLATE: ROBLOX 0x00565380
	// RBX::ComputeProp<RBX::Extents,RBX::ModelInstance>::ComputeProp<RBX::Extents,RBX::ModelInstance>
	// clang-format on

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x10059d30
	// TEMPLATE: ROBLOX 0x00564c20
	// RBX::ComputeProp<RBX::Extents,RBX::ModelInstance>::getValue
	// TEMPLATE: WEBSERVICE 0x10099560
	// TEMPLATE: ROBLOX 0x005ae7c0
	// RBX::ComputeProp<G3D::ReferenceCountedPointer<RBX::Controller>,RBX::PVInstance>::getValue
	// FUNCTION: WEBSERVICE 0x1009b290
	// FUNCTION: ROBLOX 0x005adef0
	// RBX::ComputeProp<RBX::Part,RBX::PartInstance>::getValue
	// TEMPLATE: ROBLOX 0x0065f950
	// RBX::ComputeProp<RBX::ControllerTypeArray,RBX::RootInstance>::getValue
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
	mutable T val;      // 0x00
	mutable bool dirty; // 0x04
	Class* object;      // 0x08
	GetFunc getFunc;    // 0x0c
};

} // namespace RBX

#endif // UTIL_COMPUTEPROP_H
