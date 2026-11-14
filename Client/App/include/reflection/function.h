#ifndef REFLECTION_FUNCTION_H
#define REFLECTION_FUNCTION_H

#include "reflection/member.h"
#include "reflection/type.h"
#include "placeholder.h"

namespace RBX {
namespace Reflection {

class DescribedBase;
class Function;

class __declspec(novtable) FunctionDescriptor : public MemberDescriptor
{
public:
	class __declspec(novtable) Arguments
	{
	public:
		virtual unsigned int size() const = 0;
		virtual void get(int, Value&) const = 0;

		Value returnValue;
	};

	typedef Function ConstMember;
	typedef Function Member;

	enum Security
	{
		NeedTrustedCaller = 0,
		AnyCaller = 1,
	};

	virtual void execute(DescribedBase* instance, Arguments& arguments) const = 0;

	const SignatureDescriptor& getSignature() const;

	const Security security;

protected:
	FunctionDescriptor(ClassDescriptor& classDescriptor, const char* name, Security security);

	SignatureDescriptor signature;
};

class Function
{
public:
	Function(const Function& other) : descriptor(other.descriptor), instance(other.instance) {}
	Function(const FunctionDescriptor& descriptor, const DescribedBase* instance)
		: descriptor(&descriptor), instance(instance)
	{
	}

	const Name& getName() const { return descriptor->name; }

	const FunctionDescriptor* getDescriptor() const { return descriptor; }

	void execute(FunctionDescriptor::Arguments& arguments) const;

protected:
	const FunctionDescriptor* descriptor;
	const DescribedBase* instance;
};

} // namespace Reflection
} // namespace RBX

#endif // REFLECTION_FUNCTION_H
