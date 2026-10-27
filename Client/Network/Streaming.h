#ifndef NETWORK_STREAMING_H
#define NETWORK_STREAMING_H

#include "decomp.h"
#include "reflection/property.h"
#include "util/ContentProvider.h"
#include "util/Guid.h"
#include "v8datamodel/BrickColor.h"
#include "v8tree/Instance.h"

#include <BitStream.h>
#include <G3D/Color3.h>
#include <G3D/CoordinateFrame.h>
#include <G3D/Vector3.h>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <map>
#include <string>
#include <vector>

namespace RBX {
namespace Network {

void serializeEnum(const Reflection::ConstProperty& property, RakNet::BitStream& bitStream);
void deserializeEnum(Reflection::Property& property, RakNet::BitStream& bitStream);

RakNet::BitStream& operator<<(RakNet::BitStream& stream, int value);
RakNet::BitStream& operator<<(RakNet::BitStream& stream, unsigned int value);
RakNet::BitStream& operator<<(RakNet::BitStream& stream, unsigned char value);
RakNet::BitStream& operator<<(RakNet::BitStream& stream, bool value);
RakNet::BitStream& operator<<(RakNet::BitStream& stream, float value);
RakNet::BitStream& operator<<(RakNet::BitStream& stream, double value);
RakNet::BitStream& operator<<(RakNet::BitStream& stream, const std::string& value);
RakNet::BitStream& operator<<(RakNet::BitStream& stream, const G3D::Color3& value);
RakNet::BitStream& operator<<(RakNet::BitStream& stream, const G3D::Vector3& value);
RakNet::BitStream& operator<<(RakNet::BitStream& stream, BrickColor value);
RakNet::BitStream& operator<<(RakNet::BitStream& stream, const G3D::CoordinateFrame& cf);

template <class T>
RakNet::BitStream& operator>>(RakNet::BitStream& stream, T& value);

template <>
RakNet::BitStream& operator>> <int>(RakNet::BitStream& stream, int& value);
template <>
RakNet::BitStream& operator>> <long>(RakNet::BitStream& stream, long& value);
template <>
RakNet::BitStream& operator>> <unsigned int>(RakNet::BitStream& stream, unsigned int& value);
template <>
RakNet::BitStream& operator>> <unsigned char>(RakNet::BitStream& stream, unsigned char& value);
template <>
RakNet::BitStream& operator>> <bool>(RakNet::BitStream& stream, bool& value);
template <>
RakNet::BitStream& operator>> <float>(RakNet::BitStream& stream, float& value);
template <>
RakNet::BitStream& operator>> <double>(RakNet::BitStream& stream, double& value);
template <>
RakNet::BitStream& operator>> <std::string>(RakNet::BitStream& stream, std::string& value);
template <>
RakNet::BitStream& operator>> <ContentId>(RakNet::BitStream& stream, ContentId& value);
template <>
RakNet::BitStream& operator>> <G3D::Color3>(RakNet::BitStream& stream, G3D::Color3& value);
template <>
RakNet::BitStream& operator>> <BrickColor>(RakNet::BitStream& stream, BrickColor& value);
template <>
RakNet::BitStream& operator>> <G3D::Vector3>(RakNet::BitStream& stream, G3D::Vector3& value);
template <>
RakNet::BitStream& operator>> <G3D::CoordinateFrame>(RakNet::BitStream& stream, G3D::CoordinateFrame& value);

bool brickEq(float a, float b);
void writeBrickVector(RakNet::BitStream& stream, const G3D::Vector3& value);
void readBrickVector(RakNet::BitStream& stream, G3D::Vector3& value);

template <class T>
void serialize(const Reflection::ConstProperty& property, RakNet::BitStream& bitStream)
{
	bitStream << property.getValue<T>();
}

template <class T>
void deserialize(Reflection::Property& property, RakNet::BitStream& bitStream)
{
	T value;
	bitStream >> value;
	property.setValue(value);
}

void rationalize(G3D::CoordinateFrame& value);

template <>
void serialize<ContentId>(const Reflection::ConstProperty& property, RakNet::BitStream& bitStream);
template <>
void serialize<BrickColor>(const Reflection::ConstProperty& property, RakNet::BitStream& bitStream);
template <>
void deserialize<BrickColor>(Reflection::Property& property, RakNet::BitStream& bitStream);
template <>
void deserialize<ContentId>(Reflection::Property& property, RakNet::BitStream& bitStream);

// SIZE 0xe10
class StringSender
{
	typedef std::map<std::string, unsigned char> Dictionary;

	Dictionary dictionary;    // 0x000
	std::string strings[128]; // 0x00c
	int nextIndex;            // 0xe0c

public:
	StringSender() : nextIndex(1) {}

	void serializeString(const Reflection::ConstProperty& property, RakNet::BitStream& bitStream);
	void send(RakNet::BitStream& stream, const std::string& value);
	bool trySend(RakNet::BitStream& stream, const std::string& value);
};

DECOMP_SIZE_ASSERT(StringSender, 0xe10)

// SIZE 0x0c
class StringReceiver
{
	typedef std::map<unsigned char, std::string> Dictionary;

	Dictionary dictionary; // 0x00

public:
	void deserializeString(Reflection::Property& property, RakNet::BitStream& bitStream);
	void receive(RakNet::BitStream& stream, std::string& value);

	// FUNCTION: WEBSERVICE 0x1019bc60
	void receive(RakNet::BitStream& stream, const Name*& value)
	{
		std::string s;
		receive(stream, s);
		value = &Name::declare(s.c_str(), -1);
	}
};

DECOMP_SIZE_ASSERT(StringReceiver, 0x0c)

// SIZE 0xe20
class SharedStringDictionary : public StringSender, public StringReceiver, private boost::noncopyable
{
};

DECOMP_SIZE_ASSERT(SharedStringDictionary, 0xe20)

// SIZE 0xf24
class IdSerializer : public Instance
{
	SharedStringDictionary scopeNames; // 0x0f8

protected:
	// SIZE 0x0c
	struct WaitItem
	{
		const Reflection::RefPropertyDescriptor* desc; // 0x00
		boost::shared_ptr<Instance> instance;          // 0x04
	};

	typedef std::map<Guid::Data, std::vector<WaitItem> > WaitItemMap;

	WaitItemMap waitItems; // 0xf18

	static void setRefValue(WaitItem& wi, Instance* instance);

public:
	void serializeId(RakNet::BitStream& stream, const Instance* instance);
	bool trySerializeId(RakNet::BitStream& stream, const Instance* instance);
	void deserializeId(RakNet::BitStream& stream, Guid::Data& id);
	void resolvePendingReferences(Instance* instance, Guid::Data id);
	bool deserializeInstanceRef(RakNet::BitStream& stream, Instance*& instance, Guid::Data& id);
	unsigned int numWaitingRefs() const { return waitItems.size(); }
	void serializeRef(const Reflection::ConstProperty& property, RakNet::BitStream& bitStream);
	void deserializeRef(Reflection::Property& property, RakNet::BitStream& bitStream);
};

} // namespace Network
} // namespace RBX

#endif // NETWORK_STREAMING_H
