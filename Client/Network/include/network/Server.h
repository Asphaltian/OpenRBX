#ifndef NETWORK_SERVER_H
#define NETWORK_SERVER_H

namespace RBX {

class Instance;

namespace Network {

extern char sServer[];

class Server
{
public:
	static bool serverIsPresent(const Instance* context, bool testInDatamodel);
};
} // namespace Network
} // namespace RBX

#endif // NETWORK_SERVER_H
