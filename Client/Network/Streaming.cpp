#include "Streaming.h"

#include "util/Math.h"
#include "util/Quaternion.h"
#include "v8tree/Instance.h"

#include <StringCompressor.h>
#include <algorithm>
#include <boost/bind.hpp>
#include <cmath>

namespace RBX {
namespace Network {

const float brickEpsilon = 0.0005f;

// FUNCTION: WEBSERVICE 0x101ab330
// FUNCTION: ROBLOX 0x004bdec0
void serializeEnum(const Reflection::ConstProperty& property, RakNet::BitStream& bitStream)
{
	const Reflection::EnumPropertyDescriptor& descriptor =
		static_cast<const Reflection::EnumPropertyDescriptor&>(property.getDescriptor());
	unsigned int value = descriptor.getIndexValue(property.getInstance());

	bitStream.WriteBits(reinterpret_cast<unsigned char*>(&value), descriptor.enumDescriptor.getEnumCountMSB() + 1);
}

// FUNCTION: WEBSERVICE 0x101ab370
// FUNCTION: ROBLOX 0x004bdf00
void deserializeEnum(Reflection::Property& property, RakNet::BitStream& bitStream)
{
	const Reflection::EnumPropertyDescriptor& descriptor =
		static_cast<const Reflection::EnumPropertyDescriptor&>(property.getDescriptor());
	unsigned int value = 0;
	bitStream.ReadBits(reinterpret_cast<unsigned char*>(&value), descriptor.enumDescriptor.getEnumCountMSB() + 1);

	descriptor.setIndexValue(property.getInstance(), value);
}

// FUNCTION: WEBSERVICE 0x101ab3b0
// FUNCTION: ROBLOX 0x004bdf40
RakNet::BitStream& operator<<(RakNet::BitStream& stream, bool value)
{
	stream.Write(value);
	return stream;
}

// FUNCTION: WEBSERVICE 0x101ab3d0
// FUNCTION: ROBLOX 0x004bdf60
template <>
RakNet::BitStream& operator>> <bool>(RakNet::BitStream& stream, bool& value)
{
	stream.Read(value);
	return stream;
}

// FUNCTION: WEBSERVICE 0x101ab410
// FUNCTION: ROBLOX 0x004bdfa0
bool brickEq(float a, float b)
{
	return a == b || fabsf(a - b) <= brickEpsilon;
}

// FUNCTION: WEBSERVICE 0x101ab460
// FUNCTION: ROBLOX 0x004bdff0
static bool isBrickLocation(const G3D::Vector3& v, short& x, unsigned short& y, short& z)
{
	if (v.x >= 512.0f) {
		return false;
	}
	if (v.x <= -512.0f) {
		return false;
	}
	if (v.z >= 512.0f) {
		return false;
	}
	if (v.z <= -512.0f) {
		return false;
	}
	if (v.y >= 204.8f) {
		return false;
	}
	if (v.y < 0.0f) {
		return false;
	}

	float dx = v.x * 2.0;
	x = (short) dx;
	if (x != dx) {
		return false;
	}

	float dz = v.z * 2.0;
	z = (short) dz;
	if (z != dz) {
		return false;
	}

	float dy = v.y * 10.0;
	y = (unsigned short) dy;
	return brickEq(y, dy) ? true : false;
}

// FUNCTION: WEBSERVICE 0x101ab600 FOLDED
// FUNCTION: ROBLOX 0x004be610 FOLDED
RakNet::BitStream& operator<<(RakNet::BitStream& stream, int value)
{
	stream.Write(value);
	return stream;
}

// FUNCTION: WEBSERVICE 0x101ab600 FOLDED
// FUNCTION: ROBLOX 0x004be610 FOLDED
RakNet::BitStream& operator<<(RakNet::BitStream& stream, unsigned int value)
{
	stream.Write(value);
	return stream;
}

// FUNCTION: WEBSERVICE 0x101ab630 FOLDED
// FUNCTION: ROBLOX 0x004be640 FOLDED
template <>
RakNet::BitStream& operator>> <int>(RakNet::BitStream& stream, int& value)
{
	stream.Read(value);
	return stream;
}

// FUNCTION: WEBSERVICE 0x101ab630 FOLDED
// FUNCTION: ROBLOX 0x004be640 FOLDED
template <>
RakNet::BitStream& operator>> <long>(RakNet::BitStream& stream, long& value)
{
	stream.Read(value);
	return stream;
}

// FUNCTION: WEBSERVICE 0x101ab630 FOLDED
// FUNCTION: ROBLOX 0x004be640 FOLDED
template <>
RakNet::BitStream& operator>> <unsigned int>(RakNet::BitStream& stream, unsigned int& value)
{
	stream.Read(value);
	return stream;
}

// FUNCTION: WEBSERVICE 0x101ab650
// FUNCTION: ROBLOX 0x004be660
RakNet::BitStream& operator<<(RakNet::BitStream& stream, unsigned char value)
{
	stream.Write(value);
	return stream;
}

// FUNCTION: WEBSERVICE 0x101ab680
// FUNCTION: ROBLOX 0x004be690
template <>
RakNet::BitStream& operator>> <unsigned char>(RakNet::BitStream& stream, unsigned char& value)
{
	stream.Read(value);
	return stream;
}

// FUNCTION: WEBSERVICE 0x101ab6a0
// FUNCTION: ROBLOX 0x004be6b0
RakNet::BitStream& operator<<(RakNet::BitStream& stream, float value)
{
	stream.Write(value);
	return stream;
}

// FUNCTION: WEBSERVICE 0x101ab630 FOLDED
// FUNCTION: ROBLOX 0x004be640 FOLDED
template <>
RakNet::BitStream& operator>> <float>(RakNet::BitStream& stream, float& value)
{
	stream.Read(value);
	return stream;
}

// FUNCTION: WEBSERVICE 0x101ab6d0
// FUNCTION: ROBLOX 0x004be6e0
RakNet::BitStream& operator<<(RakNet::BitStream& stream, double value)
{
	stream.Write(value);
	return stream;
}

// FUNCTION: WEBSERVICE 0x101ab700
// FUNCTION: ROBLOX 0x004be710
template <>
RakNet::BitStream& operator>> <double>(RakNet::BitStream& stream, double& value)
{
	stream.Read(value);
	return stream;
}

// FUNCTION: WEBSERVICE 0x101ab720
// FUNCTION: ROBLOX 0x004be730
RakNet::BitStream& operator<<(RakNet::BitStream& stream, const std::string& value)
{
	stream.Write(value.size());

	stringCompressor->EncodeString(value.c_str(), value.size() + 1, &stream);
	return stream;
}

// FUNCTION: WEBSERVICE 0x101ab770
// FUNCTION: ROBLOX 0x004be780
template <>
RakNet::BitStream& operator>> <std::string>(RakNet::BitStream& stream, std::string& value)
{
	int size;

	stream.Read(size);

	char* buffer = new char[size + 1];
	stringCompressor->DecodeString(buffer, size + 1, &stream);
	value = buffer;
	delete[] buffer;
	return stream;
}

// FUNCTION: WEBSERVICE 0x101ab7d0
// FUNCTION: ROBLOX 0x004be7e0
template <>
RakNet::BitStream& operator>> <ContentId>(RakNet::BitStream& stream, ContentId& value)
{
	std::string text;
	stream >> text;
	value = ContentId(text);
	return stream;
}

// FUNCTION: WEBSERVICE 0x101ab890 FOLDED
// FUNCTION: ROBLOX 0x004bea80 FOLDED
RakNet::BitStream& operator<<(RakNet::BitStream& stream, const G3D::Color3& value)
{
	stream << value.r;
	stream << value.g;
	stream << value.b;

	return stream;
}

// FUNCTION: WEBSERVICE 0x101ab890 FOLDED
// FUNCTION: ROBLOX 0x004bea80 FOLDED
RakNet::BitStream& operator<<(RakNet::BitStream& stream, const G3D::Vector3& value)
{
	stream << value.x;
	stream << value.y;
	stream << value.z;

	return stream;
}

// FUNCTION: WEBSERVICE 0x101ab8f0
// FUNCTION: ROBLOX 0x004be8a0
void writeBrickVector(RakNet::BitStream& stream, const G3D::Vector3& value)
{
	short x;
	unsigned short y;
	short z;

	if (isBrickLocation(value, x, y, z)) {
		stream.Write1();
		stream.WriteBits(reinterpret_cast<unsigned char*>(&x), 11);
		stream.WriteBits(reinterpret_cast<unsigned char*>(&y), 11);
		stream.WriteBits(reinterpret_cast<unsigned char*>(&z), 11);
	}
	else {
		stream.Write0();
		stream << value.x;
		stream << value.y;
		stream << value.z;
	}
}

// FUNCTION: WEBSERVICE 0x101ab9b0
// FUNCTION: ROBLOX 0x004be960
void readBrickVector(RakNet::BitStream& stream, G3D::Vector3& value)
{
	bool isBrickLocation;
	stream >> isBrickLocation;

	if (isBrickLocation) {
		int x = 0;
		int y = 0;
		int z = 0;

		stream.ReadBits(reinterpret_cast<unsigned char*>(&x), 11);
		stream.ReadBits(reinterpret_cast<unsigned char*>(&y), 11);
		stream.ReadBits(reinterpret_cast<unsigned char*>(&z), 11);

		if (x & 0x400) {
			x |= 0xfffffc00;
		}
		if (z & 0x400) {
			z |= 0xfffffc00;
		}

		value.x = (short) x * 0.5;
		value.y = (unsigned short) y / 10.0;
		value.z = (short) z * 0.5;
	}
	else {
		stream >> value.x;
		stream >> value.y;
		stream >> value.z;
	}
}

// FUNCTION: WEBSERVICE 0x101abad0 FOLDED
// FUNCTION: ROBLOX 0x004bed30 FOLDED
template <>
RakNet::BitStream& operator>> <G3D::Color3>(RakNet::BitStream& stream, G3D::Color3& value)
{
	stream >> value.r;
	stream >> value.g;
	stream >> value.b;

	return stream;
}

// FUNCTION: WEBSERVICE 0x101abb10
// FUNCTION: ROBLOX 0x004beae0
void rationalize(G3D::CoordinateFrame& value)
{
	if (!value.translation.isFinite()) {
		value.translation = G3D::Vector3(0.0f, -1e6f, 0.0f);
	}
	else {
		G3D::Vector3 translation =
			G3D::Vector3(-1e6f, -1e6f, -1e6f).clamp(value.translation, G3D::Vector3(1e6f, 1e6f, 1e6f));
		value.translation = translation;
	}
}

// FUNCTION: WEBSERVICE 0x101abbc0
// FUNCTION: ROBLOX 0x004beb90
RakNet::BitStream& operator<<(RakNet::BitStream& stream, const G3D::CoordinateFrame& cf)
{
	G3D::CoordinateFrame value = cf;
	rationalize(value);

	writeBrickVector(stream, value.translation);

	bool isAxisAligned = Math::isAxisAligned(value.rotation);
	stream << isAxisAligned;

	if (isAxisAligned) {
		int orientId = Math::getOrientId(value.rotation);
		stream.WriteBits(reinterpret_cast<unsigned char*>(&orientId), 6);
	}
	else {
		Quaternion q(value.rotation);
		stream.WriteNormQuat(q.w, q.x, q.y, q.z);
	}

	return stream;
}

// FUNCTION: WEBSERVICE 0x101abc90
// FUNCTION: ROBLOX 0x004bec60
template <>
RakNet::BitStream& operator>> <G3D::CoordinateFrame>(RakNet::BitStream& stream, G3D::CoordinateFrame& value)
{
	readBrickVector(stream, value.translation);

	bool isAxisAligned;
	stream >> isAxisAligned;

	if (isAxisAligned) {
		int orientId = 0;
		stream.ReadBits(reinterpret_cast<unsigned char*>(&orientId), 6);
		Math::idToMatrix3(orientId, value.rotation);
	}
	else {
		Quaternion q(0.0f, 0.0f, 0.0f, 1.0f);
		stream.ReadNormQuat(q.w, q.x, q.y, q.z);
		q.toRotationMatrix(value.rotation);
		Math::orthonormalizeIfNecessary(value.rotation);
	}

	return stream;
}

// FUNCTION: WEBSERVICE 0x101abad0 FOLDED
// FUNCTION: ROBLOX 0x004bed30 FOLDED
template <>
RakNet::BitStream& operator>> <G3D::Vector3>(RakNet::BitStream& stream, G3D::Vector3& value)
{
	stream >> value.x;
	stream >> value.y;
	stream >> value.z;

	return stream;
}

// FUNCTION: WEBSERVICE 0x101abd60
// FUNCTION: ROBLOX 0x004bed70
template <>
void serialize<ContentId>(const Reflection::ConstProperty& property, RakNet::BitStream& bitStream)
{
	bitStream << property.getStringValue();
}

// FUNCTION: WEBSERVICE 0x101abe10
// FUNCTION: ROBLOX 0x004bee20
template <>
void deserialize<ContentId>(Reflection::Property& property, RakNet::BitStream& bitStream)
{
	std::string value;
	bitStream >> value;
	property.setStringValue(value);
}

// FUNCTION: WEBSERVICE 0x101abeb0
// FUNCTION: ROBLOX 0x004beec0
void IdSerializer::setRefValue(WaitItem& wi, Instance* instance)
{
	wi.desc->setRefValue(wi.instance.get(), instance);
}

RakNet::BitStream& operator<<(RakNet::BitStream& stream, BrickColor value)
{
	unsigned int i = value.getClosestPaletteIndex();
	stream.WriteBits(reinterpret_cast<unsigned char*>(&i), 5);
	return stream;
}

// FUNCTION: WEBSERVICE 0x101ac010
// FUNCTION: ROBLOX 0x004bf0c0
template <>
RakNet::BitStream& operator>> <BrickColor>(RakNet::BitStream& stream, BrickColor& value)
{
	unsigned int i = 0;
	stream.ReadBits(reinterpret_cast<unsigned char*>(&i), 5);
	value = BrickColor::colorPalette()[i];
	return stream;
}

// FUNCTION: WEBSERVICE 0x101ac720
// FUNCTION: ROBLOX 0x004bf890
template <>
void serialize<BrickColor>(const Reflection::ConstProperty& property, RakNet::BitStream& bitStream)
{
	bitStream << property.getValue<BrickColor>();
}

// FUNCTION: WEBSERVICE 0x101ac760
// FUNCTION: ROBLOX 0x004bf8d0
template <>
void deserialize<BrickColor>(Reflection::Property& property, RakNet::BitStream& bitStream)
{
	BrickColor c;
	bitStream >> c;
	property.setValue(c);
}

// FUNCTION: WEBSERVICE 0x101aca80
// FUNCTION: ROBLOX 0x004bfc40
bool StringSender::trySend(RakNet::BitStream& stream, const std::string& value)
{
	if (value == "") {
		stream << (unsigned char) 0;
		return true;
	}

	Dictionary::const_iterator iter = dictionary.find(value);
	if (iter == dictionary.end()) {
		return false;
	}

	stream << iter->second;
	return true;
}

// FUNCTION: WEBSERVICE 0x101ace20
// FUNCTION: ROBLOX 0x004c0000
void StringReceiver::receive(RakNet::BitStream& stream, std::string& value)
{
	unsigned char id;
	stream >> id;

	if (id == 0) {
		value.clear();
	}
	else if (id & 0x80) {
		stream >> value;
		dictionary[id & 0x7f] = value;
	}
	else {
		value = dictionary[id];
	}
}

// FUNCTION: WEBSERVICE 0x101acf50
// FUNCTION: ROBLOX 0x004c0220
void IdSerializer::deserializeId(RakNet::BitStream& stream, Guid::Data& id)
{
	scopeNames.receive(stream, id.scope);

	if (id.scope != &Name::getNullName()) {
		id.index = 0;
		stream.ReadBits(reinterpret_cast<unsigned char*>(&id.index), 24);
	}
	else {
		id.index = 0;
	}
}

// FUNCTION: WEBSERVICE 0x101acfa0
// FUNCTION: ROBLOX 0x004c0270
void StringSender::send(RakNet::BitStream& stream, const std::string& value)
{
	if (value == "") {
		stream << (unsigned char) 0;
		return;
	}

	std::pair<Dictionary::iterator, bool> pair = dictionary.insert(Dictionary::value_type(value, nextIndex));
	if (!pair.second) {
		stream << pair.first->second;
		return;
	}

	dictionary.erase(strings[nextIndex]);
	strings[nextIndex] = value;

	unsigned char id = nextIndex;
	stream << (unsigned char) (id | 0x80);
	stream << value;

	nextIndex = nextIndex % 127;
	nextIndex++;
}

// STUB: WEBSERVICE 0x101ad130
// STUB: ROBLOX 0x004c0400
void StringReceiver::deserializeString(Reflection::Property& property, RakNet::BitStream& bitStream)
{
	std::string value;
	receive(bitStream, value);

	Instance* instance = static_cast<Instance*>(property.getInstance());

	property.getDescriptor().setStringValue(instance, value);
}

// STUB: WEBSERVICE 0x101ad620
// STUB: ROBLOX 0x004c08f0
void StringSender::serializeString(const Reflection::ConstProperty& property, RakNet::BitStream& bitStream)
{
	const Instance* instance = static_cast<const Instance*>(property.getInstance());

	std::string value = property.getDescriptor().getStringValue(instance);
	send(bitStream, value);
}

// FUNCTION: WEBSERVICE 0x101adac0
// FUNCTION: ROBLOX 0x004c0dd0
void IdSerializer::serializeId(RakNet::BitStream& stream, const Instance* instance)
{
	if (instance) {
		Guid::Data id;
		instance->getGuid().extract(id);
		scopeNames.send(stream, id.scope->toString());

		stream.WriteBits(reinterpret_cast<unsigned char*>(&id.index), 24);
	}
	else {
		scopeNames.send(stream, Name::getNullName().toString());
	}
}

// FUNCTION: WEBSERVICE 0x101adb40
// FUNCTION: ROBLOX 0x004c0e50
bool IdSerializer::deserializeInstanceRef(RakNet::BitStream& stream, Instance*& instance, Guid::Data& id)
{
	deserializeId(stream, id);
	return Instance::lookupByGuid(id, instance);
}

// FUNCTION: WEBSERVICE 0x101adbb0
// FUNCTION: ROBLOX 0x004c0ec0
void IdSerializer::serializeRef(const Reflection::ConstProperty& property, RakNet::BitStream& bitStream)
{
	const Reflection::RefPropertyDescriptor& desc =
		static_cast<const Reflection::RefPropertyDescriptor&>(property.getDescriptor());

	serializeId(bitStream, static_cast<const Instance*>(desc.getRefValue(property.getInstance())));
}

// FUNCTION: WEBSERVICE 0x101adce0
// FUNCTION: ROBLOX 0x004c0ff0
bool IdSerializer::trySerializeId(RakNet::BitStream& stream, const Instance* instance)
{
	if (instance) {
		Guid::Data id;
		instance->getGuid().extract(id);
		if (!scopeNames.trySend(stream, id.scope->toString())) {
			return false;
		}

		stream.WriteBits(reinterpret_cast<unsigned char*>(&id.index), 24);
	}
	else {
		scopeNames.send(stream, Name::getNullName().toString());
	}

	return true;
}

// FUNCTION: WEBSERVICE 0x101adf70
// FUNCTION: ROBLOX 0x004c1570
void IdSerializer::resolvePendingReferences(Instance* instance, Guid::Data id)
{
	WaitItemMap::iterator iter = waitItems.find(id);
	if (iter != waitItems.end()) {
		std::for_each(iter->second.begin(), iter->second.end(), boost::bind(&IdSerializer::setRefValue, _1, instance));
		waitItems.erase(iter);
	}
}

// FUNCTION: WEBSERVICE 0x101ae170
// FUNCTION: ROBLOX 0x004c1770
void IdSerializer::deserializeRef(Reflection::Property& property, RakNet::BitStream& bitStream)
{
	Instance* owner = static_cast<Instance*>(property.getInstance());
	const Reflection::RefPropertyDescriptor& desc =
		static_cast<const Reflection::RefPropertyDescriptor&>(property.getDescriptor());

	Guid::Data id;
	deserializeId(bitStream, id);

	Instance* instance;
	if (Instance::lookupByGuid(id, instance)) {
		desc.setRefValue(owner, instance);
	}
	else {
		WaitItem item = {&desc, shared_from(owner)};
		waitItems[id].push_back(item);
	}
}

} // namespace Network
} // namespace RBX
