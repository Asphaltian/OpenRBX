#ifndef REFLECTION_OBJECT_H
#define REFLECTION_OBJECT_H

#include "decomp.h"
#include "reflection/descriptor.h"
#include "reflection/member.h"

#include <vector>

namespace RBX {
namespace Reflection {

class FunctionDescriptor;
class PropertyDescriptor;
class SignalDescriptor;

// SYNTHETIC: WEBSERVICE 0x100478a0
// RBX::Reflection::ClassDescriptor::`scalar deleting destructor'

// VTABLE: WEBSERVICE 0x1022e264
// SIZE 0x88
class ClassDescriptor : public Descriptor,
						public MemberDescriptorContainer<PropertyDescriptor>,
						public MemberDescriptorContainer<SignalDescriptor>,
						public MemberDescriptorContainer<FunctionDescriptor>
{
public:
	ClassDescriptor(ClassDescriptor& base, const char* name);

	virtual DECOMP_NOINLINE ~ClassDescriptor(); // vtable+0x00

	const ClassDescriptor* getBase() const { return base; }

	bool operator==(const ClassDescriptor& other) const;
	bool operator!=(const ClassDescriptor& other) const;

	static ClassDescriptor& rootDescriptor()
	{
		static ClassDescriptor d;

		return d;
	}

	static bool lockedDown;

private:
	ClassDescriptor();

	std::vector<ClassDescriptor*> derivedClasses; // 0x74
	ClassDescriptor* base;                        // 0x84
};

DECOMP_SIZE_ASSERT(ClassDescriptor, 0x88)

} // namespace Reflection
} // namespace RBX

#endif // REFLECTION_OBJECT_H
