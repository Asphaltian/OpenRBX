#ifndef SECURITY_SECURITYCONTEXT_H
#define SECURITY_SECURITYCONTEXT_H

#include "decomp.h"

#include <G3D/format.h>
#include <boost/thread/tss.hpp>
#include <stdexcept>

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
	const Identities identity; // 0x00

public:
	// FUNCTION: WEBSERVICE 0x10066c80
	// FUNCTION: ROBLOX 0x004ae120
	static Context& current()
	{
		Context* context = ptr().get();

		if (context == NULL) {
			context = new Context(Anonymous);
			ptr().reset(context);
		}

		return *context;
	}

	// FUNCTION: WEBSERVICE 0x1005e9c0
	// STUB: ROBLOX 0x004a4dd0
	void requirePermission(Permissions permission, const char* operation) const
	{
		if (!isInRole(identity, permission)) {
			if (operation != NULL) {
				throw std::runtime_error(G3D::format("The current security context cannot %s", operation));
			}
			else {
				throw std::runtime_error(
					G3D::format("The current security context cannot perform the requested operation")
				);
			}
		}
	}

	bool hasPermission(Permissions permission) { return isInRole(identity, permission); }

	static bool isInRole(Identities identity, Permissions permission);

private:
	friend class Impersonator;

	Context(Identities identity) : identity(identity) {}

	// FUNCTION: WEBSERVICE 0x10066850
	// STUB: ROBLOX 0x00452830
	static boost::thread_specific_ptr<Context>& ptr()
	{
		static boost::thread_specific_ptr<Context> value;

		return value;
	}
};

DECOMP_SIZE_ASSERT(Context, 0x4)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x10221a60
// `RBX::Security::Context::ptr'::`2'::`dynamic atexit destructor for 'value''
// clang-format on

// SIZE 0x4
class Impersonator
{
	Context* previous; // 0x00

public:
	// FUNCTION: WEBSERVICE 0x100668b0
	// FUNCTION: ROBLOX 0x004528a0
	Impersonator(Identities identity)
	{
		Context* context = new Context(identity);
		previous = Context::ptr().release();
		Context::ptr().reset(context);
	}

	// FUNCTION: WEBSERVICE 0x10066930
	~Impersonator() { Context::ptr().reset(previous); }
};

DECOMP_SIZE_ASSERT(Impersonator, 0x4)

} // namespace Security
} // namespace RBX

#endif // SECURITY_SECURITYCONTEXT_H
