#include "Streaming.h"

#include <StringCompressor.h>

namespace RBX {
namespace Network {

// FUNCTION: WEBSERVICE 0x101ab3b0
RakNet::BitStream& operator<<(RakNet::BitStream& stream, bool value)
{
	stream.Write(value);
	return stream;
}

// FUNCTION: WEBSERVICE 0x101ab3d0
template <>
RakNet::BitStream& operator>> <bool>(RakNet::BitStream& stream, bool& value)
{
	stream.Read(value);
	return stream;
}

// FUNCTION: WEBSERVICE 0x101ab600 FOLDED
RakNet::BitStream& operator<<(RakNet::BitStream& stream, int value)
{
	stream.Write(value);
	return stream;
}

// FUNCTION: WEBSERVICE 0x101ab600 FOLDED
RakNet::BitStream& operator<<(RakNet::BitStream& stream, unsigned int value)
{
	stream.Write(value);
	return stream;
}

// FUNCTION: WEBSERVICE 0x101ab630 FOLDED
template <>
RakNet::BitStream& operator>> <int>(RakNet::BitStream& stream, int& value)
{
	stream.Read(value);
	return stream;
}

// FUNCTION: WEBSERVICE 0x101ab630 FOLDED
template <>
RakNet::BitStream& operator>> <long>(RakNet::BitStream& stream, long& value)
{
	stream.Read(value);
	return stream;
}

// FUNCTION: WEBSERVICE 0x101ab630 FOLDED
template <>
RakNet::BitStream& operator>> <unsigned int>(RakNet::BitStream& stream, unsigned int& value)
{
	stream.Read(value);
	return stream;
}

// FUNCTION: WEBSERVICE 0x101ab650
RakNet::BitStream& operator<<(RakNet::BitStream& stream, unsigned char value)
{
	stream.Write(value);
	return stream;
}

// FUNCTION: WEBSERVICE 0x101ab680
template <>
RakNet::BitStream& operator>> <unsigned char>(RakNet::BitStream& stream, unsigned char& value)
{
	stream.Read(value);
	return stream;
}

// FUNCTION: WEBSERVICE 0x101ab6a0
RakNet::BitStream& operator<<(RakNet::BitStream& stream, float value)
{
	stream.Write(value);
	return stream;
}

// FUNCTION: WEBSERVICE 0x101ab630 FOLDED
template <>
RakNet::BitStream& operator>> <float>(RakNet::BitStream& stream, float& value)
{
	stream.Read(value);
	return stream;
}

// FUNCTION: WEBSERVICE 0x101ab6d0
RakNet::BitStream& operator<<(RakNet::BitStream& stream, double value)
{
	stream.Write(value);
	return stream;
}

// FUNCTION: WEBSERVICE 0x101ab700
template <>
RakNet::BitStream& operator>> <double>(RakNet::BitStream& stream, double& value)
{
	stream.Read(value);
	return stream;
}

// FUNCTION: WEBSERVICE 0x101ab720
RakNet::BitStream& operator<<(RakNet::BitStream& stream, const std::string& value)
{
	stream.Write(value.size());

	stringCompressor->EncodeString(value.c_str(), value.size() + 1, &stream);
	return stream;
}

// FUNCTION: WEBSERVICE 0x101ab770
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

} // namespace Network
} // namespace RBX
