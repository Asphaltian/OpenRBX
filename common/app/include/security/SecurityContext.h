#ifndef SECURITY_SECURITYCONTEXT_H
#define SECURITY_SECURITYCONTEXT_H

namespace RBX {
namespace Security {

enum Identities
{
	Anonymous = 0,
	LocalGUI = 1,
	GameScript = 2,
	CmdLine = 3,
	TrustedCOM = 4,
	TrustedWebService = 5,
	Replicator = 6,
};

enum Permissions
{
	None = 0,
	Plugin = 1,
	LocalUser = 2,
	WebService = 3,
};

class Context
{
public:
	static bool isInRole(Identities identity, Permissions permission);
};

} // namespace Security
} // namespace RBX

#endif // SECURITY_SECURITYCONTEXT_H
