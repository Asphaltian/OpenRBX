#ifndef SECURITY_SECURITYCONTEXT_H
#define SECURITY_SECURITYCONTEXT_H

#include "decomp.h"

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
	Administrator = 1,
};

// SIZE 0x4
class Context
{
public:
	static bool isInRole(Identities identity, Permissions permission);

private:
	const Identities identity; // 0x00
};

DECOMP_SIZE_ASSERT(Context, 0x4)

} // namespace Security
} // namespace RBX

#endif // SECURITY_SECURITYCONTEXT_H
