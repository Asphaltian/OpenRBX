#ifndef REFLECTIONMETADATA_H
#define REFLECTIONMETADATA_H

#include "reflection/function.h"
#include "reflection/property.h"
#include "reflection/signal.h"
#include "placeholder.h"
#include "v8tree/Instance.h"

#include <algorithm>
#include <string>

namespace RBX {
namespace Reflection {
namespace Metadata {

extern char sReflection[];
extern char sClasses[];
extern char sItem[];
extern char sClass[];
extern char sProperties[];
extern char sFunctions[];
extern char sEvents[];
extern char sMember[];

class Class;

class Members : public Instance
{
public:
	Members() {}

protected:
	virtual bool askAddChild(const Instance* instance) const;
};

class Classes : public DescribedCreatable<Classes, Instance, sClasses>
{
public:
	Classes() {}

	Class* FUN_00446130(const ClassDescriptor* descriptor, bool recurse);
	void FUN_00447d00(const ClassDescriptor& descriptor);

protected:
	virtual bool askAddChild(const Instance* instance) const;
};

class Properties : public DescribedCreatable<Properties, Members, sProperties>
{
public:
	Properties() {}
};

class Functions : public DescribedCreatable<Functions, Members, sFunctions>
{
public:
	Functions() {}
};

class Events : public DescribedCreatable<Events, Members, sEvents>
{
public:
	Events() {}
};

class Item : public DescribedNonCreatable<Item, Instance, sItem>
{
public:
	Item() : browsable(true), deprecated(false), preliminary(false) {}

	static RBX::Reflection::BoundProp<bool, 1> prop_Browsable;
	static RBX::Reflection::BoundProp<bool, 1> prop_Deprecated;
	static RBX::Reflection::BoundProp<bool, 1> prop_Preliminary;
	static RBX::Reflection::BoundProp<std::string, 1> prop_summary;

	bool browsable;
	bool deprecated;
	bool preliminary;
	std::string summary;
};

class Class : public DescribedCreatable<Class, Item, sClass>
{
public:
	Class() : explorerOrder(-1), explorerImageIndex(0) {}

	int FUN_00422100() const
	{
		if (explorerOrder) {
			return explorerOrder;
		}
		return dynamic_cast<Class*>(getParent()) && dynamic_cast<Class*>(getParent())->FUN_00422100();
	}

	int FUN_004251c0() const
	{
		if (explorerImageIndex == -1) {
			return dynamic_cast<Class*>(getParent()) && dynamic_cast<Class*>(getParent())->FUN_004251c0();
		}
		return std::max<int>(explorerImageIndex, 0);
	}

	static RBX::Reflection::BoundProp<int, 1> prop_ExplorerOrder;
	static RBX::Reflection::BoundProp<int, 1> prop_ExplorerImageIndex;

protected:
	virtual bool askAddChild(const Instance* instance) const;

private:
	int explorerOrder;
	int explorerImageIndex;
};

class Member : public DescribedCreatable<Member, Item, sMember>
{
public:
	Member() {}
};

class Reflection : public DescribedCreatable<Reflection, Instance, sReflection>
{
public:
	Reflection();

	Class* FUN_00446220(const ClassDescriptor* descriptor, bool recurse);
	Member* FUN_00446920(const PropertyDescriptor* descriptor);
	Member* FUN_00446970(const FunctionDescriptor* descriptor);
	Member* FUN_004469c0(const SignalDescriptor* descriptor);

	void FUN_00447e50(std::string fileName);

	static boost::shared_ptr<Reflection> FUN_004480c0();

protected:
	virtual bool askAddChild(const Instance* instance) const;

private:
	Classes* m_unk0xf8;
};

void FUN_00447680();

} // namespace Metadata
} // namespace Reflection
} // namespace RBX

#endif // REFLECTIONMETADATA_H
