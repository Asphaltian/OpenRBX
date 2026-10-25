#ifndef REFLECTION_FUNCTION_H
#define REFLECTION_FUNCTION_H

#include "decomp.h"
#include "reflection/member.h"
#include "reflection/type.h"

namespace RBX {
namespace Reflection {

class DescribedBase;
class Function;

// SIZE 0x24
class __declspec(novtable) FunctionDescriptor : public MemberDescriptor
{
public:
	// SIZE 0x0c
	class __declspec(novtable) Arguments
	{
	public:
		virtual unsigned int size() const = 0;   // vtable+0x00
		virtual void get(int, Value&) const = 0; // vtable+0x04

		Value returnValue; // 0x04
	};

	typedef Function ConstMember;
	typedef Function Member;

	enum Security
	{
		NeedTrustedCaller = 0,
		AnyCaller = 1,
	};

	virtual void execute(DescribedBase* instance, Arguments& arguments) const = 0; // vtable+0x04

	const SignatureDescriptor& getSignature() const;

	const Security security; // 0x10

protected:
	FunctionDescriptor(ClassDescriptor& classDescriptor, const char* name, Security security);

	SignatureDescriptor signature; // 0x14
};

DECOMP_SIZE_ASSERT(FunctionDescriptor::Arguments, 0x0c)
DECOMP_SIZE_ASSERT(FunctionDescriptor, 0x24)

} // namespace Reflection
} // namespace RBX

#endif // REFLECTION_FUNCTION_H
