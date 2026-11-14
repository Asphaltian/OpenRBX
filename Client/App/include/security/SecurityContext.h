#ifndef SECURITY_SECURITYCONTEXT_H
#define SECURITY_SECURITYCONTEXT_H

#include "placeholder.h"

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

class Context
{
	const Identities identity;

public:
	static Context& current()
	{
		Context* context = ptr().get();

		if (context == NULL) {
			context = new Context(Anonymous);
			ptr().reset(context);
		}

		return *context;
	}

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

	static boost::thread_specific_ptr<Context>& ptr()
	{
		static boost::thread_specific_ptr<Context> value;

		return value;
	}
};

class Impersonator
{
	Context* previous;

public:
	Impersonator(Identities identity)
	{
		Context* context = new Context(identity);
		previous = Context::ptr().release();
		Context::ptr().reset(context);
	}

	~Impersonator() { Context::ptr().reset(previous); }
};

} // namespace Security
} // namespace RBX

#endif // SECURITY_SECURITYCONTEXT_H
