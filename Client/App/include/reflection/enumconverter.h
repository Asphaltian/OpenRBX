#ifndef REFLECTION_ENUMCONVERTER_H
#define REFLECTION_ENUMCONVERTER_H

#include "placeholder.h"
#include "reflection/type.h"

#include <algorithm>
#include <boost/bind.hpp>
#include <boost/checked_delete.hpp>
#include <map>
#include <string>
#include <vector>

namespace RBX {
namespace Reflection {

class EnumDescriptor : public Type
{
public:
	class Item : public Descriptor
	{
	public:
		Item(const char* name, int value, unsigned int index) : Descriptor(name), value(value), index(index) {}

		const int value;
		const unsigned int index;
	};

	static std::vector<const EnumDescriptor*>::const_iterator enumsBegin();
	static std::vector<const EnumDescriptor*>::const_iterator enumsEnd();

	unsigned int getEnumCount() const { return enumCount; }
	unsigned int getEnumCountMSB() const { return enumCountMSB; }

	std::vector<const Item*>::const_iterator begin() const { return allItems.begin(); }
	std::vector<const Item*>::const_iterator end() const { return allItems.end(); }

	bool isValue(int intValue) const
	{
		return std::find_if(allItems.begin(), allItems.end(), boost::bind(&equalValue, _1, intValue)) != allItems.end();
	}

protected:
	EnumDescriptor(const char* name, const std::type_info& type);
	virtual ~EnumDescriptor();

	std::vector<const Item*> allItems;
	unsigned int enumCount;
	unsigned int enumCountMSB;

private:
	static std::vector<const EnumDescriptor*>& allEnums();

	static bool equalValue(const Item* item, int intValue) { return item->value == intValue; }
};

template <class T>
class EnumDesc : public EnumDescriptor
{
public:
	static const EnumDesc<T>& singleton();

	const Name& convertToName(const T& value) const;
	std::string convertToString(const T& value) const;

	bool convertToValue(unsigned int index, T& value) const
	{
		if (index < enumCount) {
			value = indexToEnum[index];
			return true;
		}

		return false;
	}

	bool convertToValue(const std::string& text, T& value) const;
	bool convertToValue(const Name& name, T& value) const;
	unsigned int convertToIndex(T value) const;

private:
	EnumDesc();
	virtual ~EnumDesc();

	void addPair(T value, const char* name);
	void addLegacyName(const char* name, T value);
	void addLegacyName(const Name& name, T value);

	std::map<const Name*, T> nameToEnum;
	std::map<const Name*, T> nameToEnumLegacy;
	std::vector<const Name*> enumToName;
	std::map<std::string, T> stringToEnum;
	std::map<std::string, T> stringToEnumLegacy;
	std::vector<std::string> enumToString;
	std::vector<T> indexToEnum;
	std::vector<unsigned int> enumToIndex;
};

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

template <class T>
const EnumDesc<T>& EnumDesc<T>::singleton()
{
	static EnumDesc<T> s;

	return s;
}

template <class T>
std::string EnumDesc<T>::convertToString(const T& value) const
{
	if (value < 0) {
		return "";
	}

	if (static_cast<unsigned int>(value) >= enumToString.size()) {
		return "";
	}

	return enumToString[value];
}

template <class T>
bool EnumDesc<T>::convertToValue(const Name& name, T& value) const
{
	typename std::map<const Name*, T>::const_iterator iter = nameToEnum.find(&name);

	if (iter != nameToEnum.end()) {
		value = iter->second;
		return true;
	}

	iter = nameToEnumLegacy.find(&name);

	if (iter != nameToEnumLegacy.end()) {
		value = iter->second;
		return true;
	}

	return false;
}

template <class T>
bool EnumDesc<T>::convertToValue(const std::string& text, T& value) const
{
	typename std::map<std::string, T>::const_iterator iter = stringToEnum.find(text);

	if (iter != stringToEnum.end()) {
		value = iter->second;
		return true;
	}

	iter = stringToEnumLegacy.find(text);

	if (iter != stringToEnumLegacy.end()) {
		value = iter->second;
		return true;
	}

	return false;
}

template <class T>
unsigned int EnumDesc<T>::convertToIndex(T value) const
{
	if (static_cast<unsigned int>(value) < enumToIndex.size()) {
		return enumToIndex[value];
	}

	return -1;
}

template <class T>
EnumDesc<T>::~EnumDesc()
{
	std::for_each(allItems.begin(), allItems.end(), boost::checked_deleter<const Item>());
}

} // namespace Reflection
} // namespace RBX

#endif // REFLECTION_ENUMCONVERTER_H
