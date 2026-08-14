#ifndef SCRIPT_SCRIPT_H
#define SCRIPT_SCRIPT_H

#include "decomp.h"
#include "util/ContentProvider.h"
#include "v8tree/Instance.h"

#include <boost/shared_ptr.hpp>
#include <boost/signals/connection.hpp>
#include <string>

namespace RBX {

class IScriptOwner;

extern char sScript[];

extern char sLocalScript[];
// SIZE 0x128
class Script : public DescribedCreatable<Script, Instance, sScript>
{
public:
	// SIZE 0x8
	class Slot
	{
	public:
		boost::shared_ptr<boost::signals::connection> cnction; // 0x00

	protected:
		Slot() : cnction(new boost::signals::connection()) {}
	};

	// FUNCTION: WEBSERVICE 0x10068960
	const ContentId& getScriptId() const { return scriptId; }

	boost::shared_ptr<const std::string> requestCode();

	// FUNCTION: WEBSERVICE 0x10068b70 FOLDED
	const std::string* getEmbeddedCode() const { return embeddedSource.get(); }

private:
	boost::shared_ptr<std::string> embeddedSource; // 0x0f8
	ContentId scriptId;                            // 0x100
	bool disabled;                                 // 0x120
	IScriptOwner* owner;                           // 0x124
};

DECOMP_SIZE_ASSERT(Script, 0x128)

// SIZE 0x128
class LocalScript : public DescribedCreatable<LocalScript, Script, sLocalScript>
{
};

DECOMP_SIZE_ASSERT(LocalScript, 0x128)

class ScriptContext;

// SIZE 0x4
class __declspec(novtable) IScriptOwner
{
protected:
	virtual IScriptOwner* scriptShouldRun(Script* script) = 0;            // vtable+0x00
	virtual void runScript(Script* script, ScriptContext* scriptContext); // vtable+0x04
	virtual void releaseScript(Script* script);                           // vtable+0x08
};

DECOMP_SIZE_ASSERT(IScriptOwner, 0x4)

// clang-format off
// STUB: WEBSERVICE 0x10068bf0
// RBX::Script::~Script
// clang-format on

} // namespace RBX

#endif // SCRIPT_SCRIPT_H
