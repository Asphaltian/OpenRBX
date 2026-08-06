#ifndef REFLECTION_ENUMCONVERTER_H
#define REFLECTION_ENUMCONVERTER_H

#include "decomp.h"
#include "reflection/type.h"

#include <algorithm>
#include <boost/checked_delete.hpp>
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
		Item(const char* name, int value, unsigned int index) : Descriptor(name), value(value), index(index) {}

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

	static bool equalValue(const Item* item, int intValue) { return item->value == intValue; }
};

DECOMP_SIZE_ASSERT(EnumDescriptor, 0x28)
DECOMP_SIZE_ASSERT(EnumDescriptor::Item, 0x10)

// VTABLE: WEBSERVICE 0x102342d8
// class EnumDesc<enum RBX::SoundType>
// VTABLE: WEBSERVICE 0x102366e8
// class EnumDesc<enum RBX::Controller::ControllerType>
// VTABLE: WEBSERVICE 0x10236b58
// class EnumDesc<enum RBX::Part::PartType>
// VTABLE: WEBSERVICE 0x1024646c
// class EnumDesc<enum RBX::Controller::InputType>
// VTABLE: WEBSERVICE 0x10246614
// class EnumDesc<enum RBX::SurfaceType>
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
	void addLegacyName(const char* name, T value);
	void addLegacyName(const Name& name, T value);

	std::map<const Name*, T> nameToEnum;         // 0x28
	std::map<const Name*, T> nameToEnumLegacy;   // 0x34
	std::vector<const Name*> enumToName;         // 0x40
	std::map<std::string, T> stringToEnum;       // 0x50
	std::map<std::string, T> stringToEnumLegacy; // 0x5c
	std::vector<std::string> enumToString;       // 0x68
	std::vector<T> indexToEnum;                  // 0x78
	std::vector<unsigned int> enumToIndex;       // 0x88
};

// clang-format off
// TEMPLATE: WEBSERVICE 0x10082790
// RBX::Reflection::EnumDesc<enum RBX::SoundType>::~EnumDesc<enum RBX::SoundType>
// clang-format on

// SYNTHETIC: WEBSERVICE 0x10082a10
// RBX::Reflection::EnumDesc<enum RBX::SoundType>::`scalar deleting destructor'

// clang-format off
// TEMPLATE: WEBSERVICE 0x10099a40
// RBX::Reflection::EnumDesc<enum RBX::Controller::ControllerType>::~EnumDesc<enum RBX::Controller::ControllerType>
// clang-format on

// SYNTHETIC: WEBSERVICE 0x10099bf0
// RBX::Reflection::EnumDesc<enum RBX::Controller::ControllerType>::`scalar deleting destructor'

// clang-format off
// TEMPLATE: WEBSERVICE 0x1009dee0
// RBX::Reflection::EnumDesc<enum RBX::Part::PartType>::~EnumDesc<enum RBX::Part::PartType>
// clang-format on

// SYNTHETIC: WEBSERVICE 0x1009e090
// RBX::Reflection::EnumDesc<enum RBX::Part::PartType>::`scalar deleting destructor'

// clang-format off
// TEMPLATE: WEBSERVICE 0x100dfff0
// RBX::Reflection::EnumDesc<enum RBX::NormalId>::addPair
// clang-format on
template <class T>
void EnumDesc<T>::addPair(T value, const char* name)
{
	const Item* item = new Item(name, value, enumCount);

	allItems.push_back(item);

	if (enumToIndex.size() <= (unsigned int) value) {
		enumToIndex.resize(value + 1, -1);
	}
	enumToIndex[value] = enumCount;
	indexToEnum.push_back(value);

	if (enumToName.size() <= (unsigned int) value) {
		enumToName.resize(value + 1, &Name::getNullName());
	}
	enumToName[value] = &item->name;

	if (enumToString.size() <= (unsigned int) value) {
		enumToString.resize(value + 1);
	}
	enumToString[value] = name;

	nameToEnum[&item->name] = value;
	stringToEnum[name] = value;

	enumCount++;

	int msb = -1;
	for (unsigned int i = enumCount; i > 0; msb++) {
		i >>= 1;
	}
	enumCountMSB = msb;
}

template <class T>
void EnumDesc<T>::addLegacyName(const char* name, T value)
{
	addLegacyName(Name::declare(name, -1), value);
}

template <class T>
void EnumDesc<T>::addLegacyName(const Name& name, T value)
{
	nameToEnumLegacy[&name] = value;
	stringToEnumLegacy[name.name] = value;
}

// clang-format off
// STUB: WEBSERVICE 0x100e5dd0
// RBX::Reflection::EnumDesc<enum RBX::Feature::TopBottom>::singleton
// clang-format on
template <class T>
const EnumDesc<T>& EnumDesc<T>::singleton()
{
	static EnumDesc<T> s;

	return s;
}

// clang-format off
// TEMPLATE: WEBSERVICE 0x10100860
// RBX::Reflection::EnumDesc<enum RBX::Controller::InputType>::~EnumDesc<enum RBX::Controller::InputType>
// clang-format on
template <class T>
EnumDesc<T>::~EnumDesc()
{
	std::for_each(allItems.begin(), allItems.end(), boost::checked_deleter<const Item>());
}

// SYNTHETIC: WEBSERVICE 0x10100a10
// RBX::Reflection::EnumDesc<enum RBX::Controller::InputType>::`scalar deleting destructor'

// clang-format off
// TEMPLATE: WEBSERVICE 0x101012c0
// RBX::Reflection::EnumDesc<enum RBX::SurfaceType>::~EnumDesc<enum RBX::SurfaceType>
// clang-format on

// SYNTHETIC: WEBSERVICE 0x10101470
// RBX::Reflection::EnumDesc<enum RBX::SurfaceType>::`scalar deleting destructor'

} // namespace Reflection
} // namespace RBX

#endif // REFLECTION_ENUMCONVERTER_H
