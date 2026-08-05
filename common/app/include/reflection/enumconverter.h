#ifndef REFLECTION_ENUMCONVERTER_H
#define REFLECTION_ENUMCONVERTER_H

#include "decomp.h"
#include "reflection/type.h"

#include <map>
#include <string>
#include <vector>

namespace RBX {
namespace Reflection {

// SYNTHETIC: WEBSERVICE 0x10222ea0
// `RBX::Reflection::EnumDescriptor::allEnums'::`2'::`dynamic atexit destructor for 's''

// SYNTHETIC: WEBSERVICE 0x10094370
// RBX::Reflection::EnumDescriptor::`scalar deleting destructor'

// VTABLE: WEBSERVICE 0x10235de0
// SIZE 0x28
class EnumDescriptor : public Type
{
public:
	// VTABLE: WEBSERVICE 0x1022da64
	// SIZE 0x10
	class Item : public Descriptor
	{
	public:
		Item(const char* name, int value, unsigned int index);

		const int value;          // 0x08
		const unsigned int index; // 0x0c
	};

	static std::vector<const EnumDescriptor*>::const_iterator enumsBegin();
	static std::vector<const EnumDescriptor*>::const_iterator enumsEnd();

	unsigned int getEnumCount() const { return enumCount; }
	unsigned int getEnumCountMSB() const { return enumCountMSB; }

	std::vector<const Item*>::const_iterator begin() const { return allItems.begin(); }
	std::vector<const Item*>::const_iterator end() const { return allItems.end(); }

	bool isValue(int value) const;

protected:
	EnumDescriptor(const char* name, const std::type_info& type);
	virtual DECOMP_NOINLINE ~EnumDescriptor(); // vtable+0x00

	std::vector<const Item*> allItems; // 0x10
	unsigned int enumCount;            // 0x20
	unsigned int enumCountMSB;         // 0x24

private:
	static std::vector<const EnumDescriptor*>& allEnums();

	// FUNCTION: WEBSERVICE 0x10081e50
	static bool equalValue(const Item* item, int intValue) { return item->value == intValue; }
};

DECOMP_SIZE_ASSERT(EnumDescriptor, 0x28)
DECOMP_SIZE_ASSERT(EnumDescriptor::Item, 0x10)

// SIZE 0x98
template <class T>
class EnumDesc : public EnumDescriptor
{
public:
	static const EnumDesc<T>& singleton();

	const Name& convertToName(T value) const;
	const std::string& convertToString(T value) const;
	unsigned int convertToIndex(T value) const;

private:
	EnumDesc();
	virtual ~EnumDesc(); // vtable+0x00

	void addPair(T value, const char* name);

	std::map<const Name*, T> nameToEnum;         // 0x28
	std::map<const Name*, T> nameToEnumLegacy;   // 0x34
	std::vector<const Name*> enumToName;         // 0x40
	std::map<std::string, T> stringToEnum;       // 0x50
	std::map<std::string, T> stringToEnumLegacy; // 0x5c
	std::vector<std::string> enumToString;       // 0x68
	std::vector<T> indexToEnum;                  // 0x78
	std::vector<unsigned int> enumToIndex;       // 0x88
};

} // namespace Reflection
} // namespace RBX

#endif // REFLECTION_ENUMCONVERTER_H
