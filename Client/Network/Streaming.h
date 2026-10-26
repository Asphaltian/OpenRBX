#ifndef NETWORK_STREAMING_H
#define NETWORK_STREAMING_H

#include "decomp.h"

#include <BitStream.h>
#include <string>

namespace RBX {
namespace Network {

RakNet::BitStream& operator<<(RakNet::BitStream& stream, int value);
RakNet::BitStream& operator<<(RakNet::BitStream& stream, unsigned int value);
RakNet::BitStream& operator<<(RakNet::BitStream& stream, unsigned char value);
RakNet::BitStream& operator<<(RakNet::BitStream& stream, bool value);
RakNet::BitStream& operator<<(RakNet::BitStream& stream, float value);
RakNet::BitStream& operator<<(RakNet::BitStream& stream, double value);
RakNet::BitStream& operator<<(RakNet::BitStream& stream, const std::string& value);

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

} // namespace Network
} // namespace RBX

#endif // NETWORK_STREAMING_H
