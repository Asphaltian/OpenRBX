#ifndef REFLECTION_ENUMCONVERTER_H
#define REFLECTION_ENUMCONVERTER_H

#include "decomp.h"
#include "reflection/type.h"

#include <algorithm>
#include <boost/bind.hpp>
#include <boost/checked_delete.hpp>
#include <map>
#include <string>
#include <vector>

namespace RBX {
namespace Reflection {

// SYNTHETIC: WEBSERVICE 0x10222ea0
// SYNTHETIC: ROBLOX 0x007f2eb0
// `RBX::Reflection::EnumDescriptor::allEnums'::`2'::`dynamic atexit destructor for 's''

// SYNTHETIC: WEBSERVICE 0x10094370
// SYNTHETIC: ROBLOX 0x005c0990
// RBX::Reflection::EnumDescriptor::`scalar deleting destructor'

// VTABLE: WEBSERVICE 0x10235de0
// VTABLE: ROBLOX 0x00827630
// SIZE 0x28
class EnumDescriptor : public Type
{
public:
	// VTABLE: WEBSERVICE 0x1022da64
	// VTABLE: ROBLOX 0x00809520
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

	// FUNCTION: WEBSERVICE 0x10081ec0
	// FUNCTION: ROBLOX 0x0044af70
	bool isValue(int intValue) const
	{
		return std::find_if(allItems.begin(), allItems.end(), boost::bind(&equalValue, _1, intValue)) != allItems.end();
	}

protected:
	EnumDescriptor(const char* name, const std::type_info& type);
	virtual ~EnumDescriptor(); // vtable+0x00

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
// VTABLE: ROBLOX 0x008299e8
// class EnumDesc<enum RBX::SoundType>
// VTABLE: WEBSERVICE 0x102366e8
// VTABLE: ROBLOX 0x00832040
// class EnumDesc<enum RBX::Controller::ControllerType>
// VTABLE: WEBSERVICE 0x10236b58
// VTABLE: ROBLOX 0x008255c0
// class EnumDesc<enum RBX::Part::PartType>
// VTABLE: WEBSERVICE 0x1024646c
// VTABLE: ROBLOX 0x0082cd24
// class EnumDesc<enum RBX::Controller::InputType>
// VTABLE: WEBSERVICE 0x10246614
// VTABLE: ROBLOX 0x0082cd1c
// class EnumDesc<enum RBX::SurfaceType>
// SIZE 0x98
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
// TEMPLATE: WEBSERVICE 0x10081410
// TEMPLATE: ROBLOX 0x005c5010
// RBX::Reflection::EnumDesc<enum RBX::Soundscape::ReverbType>::~EnumDesc<enum RBX::Soundscape::ReverbType>
// TEMPLATE: WEBSERVICE 0x10082790
// TEMPLATE: ROBLOX 0x005c6250
// RBX::Reflection::EnumDesc<enum RBX::SoundType>::~EnumDesc<enum RBX::SoundType>
// TEMPLATE: WEBSERVICE 0x100c36b0
// TEMPLATE: ROBLOX 0x005d7250
// RBX::Reflection::EnumDesc<enum RBX::Camera::CameraType>::~EnumDesc<enum RBX::Camera::CameraType>
// TEMPLATE: WEBSERVICE 0x100c8290
// TEMPLATE: ROBLOX 0x005e2140
// RBX::Reflection::EnumDesc<enum RBX::HopperBin::BinType>::~EnumDesc<enum RBX::HopperBin::BinType>
// clang-format on

// SYNTHETIC: WEBSERVICE 0x100818f0
// SYNTHETIC: ROBLOX 0x005c5440
// RBX::Reflection::EnumDesc<enum RBX::Soundscape::ReverbType>::`scalar deleting destructor'
// SYNTHETIC: WEBSERVICE 0x10082a10
// SYNTHETIC: ROBLOX 0x005c64d0
// RBX::Reflection::EnumDesc<enum RBX::SoundType>::`scalar deleting destructor'
// SYNTHETIC: WEBSERVICE 0x100c85e0
// SYNTHETIC: ROBLOX 0x005e26c0
// RBX::Reflection::EnumDesc<enum RBX::HopperBin::BinType>::`scalar deleting destructor'

// clang-format off
// TEMPLATE: WEBSERVICE 0x10099a40
// TEMPLATE: ROBLOX 0x005f7c20
// RBX::Reflection::EnumDesc<enum RBX::Controller::ControllerType>::~EnumDesc<enum RBX::Controller::ControllerType>
// clang-format on

// SYNTHETIC: WEBSERVICE 0x10099bf0
// SYNTHETIC: ROBLOX 0x005f7dd0
// RBX::Reflection::EnumDesc<enum RBX::Controller::ControllerType>::`scalar deleting destructor'

// clang-format off
// TEMPLATE: WEBSERVICE 0x1009dee0
// TEMPLATE: ROBLOX 0x005b09c0
// RBX::Reflection::EnumDesc<enum RBX::Part::PartType>::~EnumDesc<enum RBX::Part::PartType>
// clang-format on

// SYNTHETIC: WEBSERVICE 0x1009e090
// SYNTHETIC: ROBLOX 0x005b0b70
// RBX::Reflection::EnumDesc<enum RBX::Part::PartType>::`scalar deleting destructor'

// SYNTHETIC: WEBSERVICE 0x100c39e0
// SYNTHETIC: ROBLOX 0x005d7580
// RBX::Reflection::EnumDesc<enum RBX::Camera::CameraType>::`scalar deleting destructor'

// clang-format off
// TEMPLATE: WEBSERVICE 0x100dfff0
// STUB: ROBLOX 0x005e22f0
// RBX::Reflection::EnumDesc<enum RBX::HopperBin::BinType>::addPair
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
// TEMPLATE: WEBSERVICE 0x10081c70
// TEMPLATE: ROBLOX 0x005c57c0
// RBX::Reflection::EnumDesc<enum RBX::Soundscape::ReverbType>::singleton
// TEMPLATE: WEBSERVICE 0x10083010
// TEMPLATE: ROBLOX 0x005c6bc0
// RBX::Reflection::EnumDesc<enum RBX::SoundType>::singleton
// TEMPLATE: WEBSERVICE 0x10099d80
// TEMPLATE: ROBLOX 0x005f7f60
// RBX::Reflection::EnumDesc<enum RBX::Controller::ControllerType>::singleton
// TEMPLATE: WEBSERVICE 0x1009dde0
// TEMPLATE: ROBLOX 0x005b08f0
// RBX::Reflection::EnumDesc<enum RBX::PartInstance::FormFactor>::singleton
// TEMPLATE: WEBSERVICE 0x10081e50
// TEMPLATE: ROBLOX 0x0044ae50
// RBX::Reflection::EnumDescriptor::equalValue
// TEMPLATE: WEBSERVICE 0x100c8600
// TEMPLATE: ROBLOX 0x005e26e0
// RBX::Reflection::EnumDesc<enum RBX::HopperBin::BinType>::singleton
// TEMPLATE: WEBSERVICE 0x100e5f90
// TEMPLATE: ROBLOX 0x0057c7f0
// RBX::Reflection::EnumDesc<enum RBX::HopperBin::BinType>::convertToIndex
// TEMPLATE: WEBSERVICE 0x1009ed00
// TEMPLATE: ROBLOX 0x005b1770
// RBX::Reflection::EnumDesc<enum RBX::Part::PartType>::singleton
// TEMPLATE: WEBSERVICE 0x100b4230
// TEMPLATE: ROBLOX 0x0057c230
// RBX::Reflection::EnumDesc<enum RBX::DebugSettings::ErrorReporting>::singleton
// TEMPLATE: WEBSERVICE 0x100b4290
// TEMPLATE: ROBLOX 0x0057c290
// RBX::Reflection::EnumDesc<enum RBX::Debugable::AssertAction>::singleton
// TEMPLATE: WEBSERVICE 0x100c3a00
// TEMPLATE: ROBLOX 0x005d75a0
// RBX::Reflection::EnumDesc<enum RBX::Camera::CameraType>::singleton
// TEMPLATE: WEBSERVICE 0x100e5dd0
// TEMPLATE: ROBLOX 0x00618d50
// RBX::Reflection::EnumDesc<enum RBX::Feature::TopBottom>::singleton
// TEMPLATE: WEBSERVICE 0x100e5e30
// TEMPLATE: ROBLOX 0x00618db0
// RBX::Reflection::EnumDesc<enum RBX::Feature::LeftRight>::singleton
// TEMPLATE: WEBSERVICE 0x100e5e90
// TEMPLATE: ROBLOX 0x00618e10
// RBX::Reflection::EnumDesc<enum RBX::Feature::InOut>::singleton
// TEMPLATE: WEBSERVICE 0x100e6f90
// TEMPLATE: ROBLOX 0x005dff20
// RBX::Reflection::EnumDesc<enum RBX::NormalId>::singleton
// TEMPLATE: WEBSERVICE 0x10101a30
// TEMPLATE: ROBLOX 0x005df010
// RBX::Reflection::EnumDesc<enum RBX::SurfaceType>::singleton
// TEMPLATE: WEBSERVICE 0x10101a90
// TEMPLATE: ROBLOX 0x005df070
// RBX::Reflection::EnumDesc<enum RBX::Controller::InputType>::singleton
// TEMPLATE: WEBSERVICE 0x1012d270
// TEMPLATE: ROBLOX 0x005b3b30
// RBX::Reflection::EnumDesc<enum RBX::SpecialShape::MeshType>::singleton
// SYNTHETIC: WEBSERVICE 0x10222450
// SYNTHETIC: ROBLOX 0x007f3080
// `RBX::Reflection::EnumDesc<enum RBX::Soundscape::ReverbType>::singleton'::`2'::`dynamic atexit destructor for 's''
// SYNTHETIC: WEBSERVICE 0x10222500
// SYNTHETIC: ROBLOX 0x007f30f0
// `RBX::Reflection::EnumDesc<enum RBX::SoundType>::singleton'::`2'::`dynamic atexit destructor for 's''
// SYNTHETIC: WEBSERVICE 0x10223060
// SYNTHETIC: ROBLOX 0x007f45b0
// `RBX::Reflection::EnumDesc<enum RBX::Controller::ControllerType>::singleton'::`2'::`dynamic atexit destructor for 's''
// SYNTHETIC: WEBSERVICE 0x10223320
// SYNTHETIC: ROBLOX 0x007f2b80
// `RBX::Reflection::EnumDesc<enum RBX::PartInstance::FormFactor>::singleton'::`2'::`dynamic atexit destructor for 's''
// SYNTHETIC: WEBSERVICE 0x10223370
// SYNTHETIC: ROBLOX 0x007f2bd0
// `RBX::Reflection::EnumDesc<enum RBX::Part::PartType>::singleton'::`2'::`dynamic atexit destructor for 's''
// SYNTHETIC: WEBSERVICE 0x10223970
// SYNTHETIC: ROBLOX 0x007f2260
// `RBX::Reflection::EnumDesc<enum RBX::Debugable::AssertAction>::singleton'::`2'::`dynamic atexit destructor for 's''
// SYNTHETIC: WEBSERVICE 0x10223980
// SYNTHETIC: ROBLOX 0x007f2270
// `RBX::Reflection::EnumDesc<enum RBX::DebugSettings::ErrorReporting>::singleton'::`2'::`dynamic atexit destructor for 's''
// SYNTHETIC: WEBSERVICE 0x10223b40
// SYNTHETIC: ROBLOX 0x007f3930
// `RBX::Reflection::EnumDesc<enum RBX::Camera::CameraType>::singleton'::`2'::`dynamic atexit destructor for 's''
// SYNTHETIC: WEBSERVICE 0x10223be0
// SYNTHETIC: ROBLOX 0x007f3fd0
// `RBX::Reflection::EnumDesc<enum RBX::HopperBin::BinType>::singleton'::`2'::`dynamic atexit destructor for 's''
// SYNTHETIC: WEBSERVICE 0x10224310
// SYNTHETIC: ROBLOX 0x007f4830
// `RBX::Reflection::EnumDesc<enum RBX::Feature::InOut>::singleton'::`2'::`dynamic atexit destructor for 's''
// SYNTHETIC: WEBSERVICE 0x10224320
// SYNTHETIC: ROBLOX 0x007f4840
// `RBX::Reflection::EnumDesc<enum RBX::Feature::LeftRight>::singleton'::`2'::`dynamic atexit destructor for 's''
// SYNTHETIC: WEBSERVICE 0x10224330
// SYNTHETIC: ROBLOX 0x007f4850
// `RBX::Reflection::EnumDesc<enum RBX::Feature::TopBottom>::singleton'::`2'::`dynamic atexit destructor for 's''
// SYNTHETIC: WEBSERVICE 0x10224340
// SYNTHETIC: ROBLOX 0x007f3f30
// `RBX::Reflection::EnumDesc<enum RBX::NormalId>::singleton'::`2'::`dynamic atexit destructor for 's''
// SYNTHETIC: WEBSERVICE 0x10225110
// SYNTHETIC: ROBLOX 0x007f3ef0
// `RBX::Reflection::EnumDesc<enum RBX::Controller::InputType>::singleton'::`2'::`dynamic atexit destructor for 's''
// SYNTHETIC: WEBSERVICE 0x10225120
// SYNTHETIC: ROBLOX 0x007f3f00
// `RBX::Reflection::EnumDesc<enum RBX::SurfaceType>::singleton'::`2'::`dynamic atexit destructor for 's''
// SYNTHETIC: WEBSERVICE 0x102256e0
// SYNTHETIC: ROBLOX 0x007f2c90
// `RBX::Reflection::EnumDesc<enum RBX::SpecialShape::MeshType>::singleton'::`2'::`dynamic atexit destructor for 's''
// clang-format on
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

// clang-format off
// TEMPLATE: WEBSERVICE 0x10100860
// TEMPLATE: ROBLOX 0x005de890
// RBX::Reflection::EnumDesc<enum RBX::Controller::InputType>::~EnumDesc<enum RBX::Controller::InputType>
// clang-format on
template <class T>
EnumDesc<T>::~EnumDesc()
{
	std::for_each(allItems.begin(), allItems.end(), boost::checked_deleter<const Item>());
}

// SYNTHETIC: WEBSERVICE 0x10100a10
// SYNTHETIC: ROBLOX 0x005dea60
// RBX::Reflection::EnumDesc<enum RBX::Controller::InputType>::`scalar deleting destructor'

// clang-format off
// TEMPLATE: WEBSERVICE 0x101012c0
// TEMPLATE: ROBLOX 0x005de6e0
// RBX::Reflection::EnumDesc<enum RBX::SurfaceType>::~EnumDesc<enum RBX::SurfaceType>
// clang-format on

// SYNTHETIC: WEBSERVICE 0x10101470
// SYNTHETIC: ROBLOX 0x005dea40
// RBX::Reflection::EnumDesc<enum RBX::SurfaceType>::`scalar deleting destructor'

} // namespace Reflection
} // namespace RBX

#endif // REFLECTION_ENUMCONVERTER_H
