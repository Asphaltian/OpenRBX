#ifndef NETWORK_CLIENT_H
#define NETWORK_CLIENT_H

namespace RBX {

class Instance;

namespace Network {

extern char sClient[];

class Client
{
public:
	static bool clientIsPresent(const Instance* context, bool testInDatamodel);
};
} // namespace Network
} // namespace RBX

#endif // NETWORK_CLIENT_H
