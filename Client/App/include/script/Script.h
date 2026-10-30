#ifndef SCRIPT_SCRIPT_H
#define SCRIPT_SCRIPT_H

#include "decomp.h"
#include "reflection/property.h"
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
	Script();
	virtual ~Script();

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
	void setEmbeddedCode(const std::string& value);

	// FUNCTION: WEBSERVICE 0x10068b70 FOLDED
	const std::string& getEmbeddedCode() const { return *embeddedSource; }

	void setScriptId(const ContentId& value);

	bool isDisabled() const { return disabled; }

	static const Reflection::PropDescriptor<Script, std::string> prop_EmbeddedSourceCode;
	static const Reflection::PropDescriptor<Script, ContentId> prop_SourceCodeId;
	static Reflection::BoundProp<bool, 1> prop_Disabled;

	// FUNCTION: WEBSERVICE 0x100e44b0 FOLDED
	// FUNCTION: ROBLOX 0x00633110 FOLDED
	virtual bool askSetParent(const Instance* instance) const { return true; }
	bool isCodeEmbedded() const;

protected:
	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);
	virtual void onAncestorChanged(const AncestorChanged& event);

private:
	boost::shared_ptr<const std::string> embeddedSource; // 0x0f8
	ContentId scriptId;                                  // 0x100
	bool disabled;                                       // 0x120
	IScriptOwner* owner;                                 // 0x124
};

DECOMP_SIZE_ASSERT(Script, 0x128)

// SIZE 0x128
class LocalScript : public DescribedCreatable<LocalScript, Script, sLocalScript>
{
public:
	LocalScript();

	// FUNCTION: WEBSERVICE 0x10069e10
	virtual ~LocalScript() {}
};

DECOMP_SIZE_ASSERT(LocalScript, 0x128)

class ScriptContext;

// SIZE 0x4
class __declspec(novtable) IScriptOwner
{
	friend class Script;

protected:
	virtual IScriptOwner* scriptShouldRun(Script* script) = 0; // vtable+0x00

	// FUNCTION: WEBSERVICE 0x101019f0 FOLDED
	// FUNCTION: ROBLOX 0x007859e0 FOLDED
	virtual void runScript(Script* script, ScriptContext* scriptContext) // vtable+0x04
	{
	}

	// FUNCTION: WEBSERVICE 0x100d15c0 FOLDED
	// FUNCTION: ROBLOX 0x0045e450 FOLDED
	virtual void releaseScript(Script* script) // vtable+0x08
	{
	}
};

DECOMP_SIZE_ASSERT(IScriptOwner, 0x4)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x10068fd0
// RBX::Script::`scalar deleting destructor'
// SYNTHETIC: WEBSERVICE 0x10069e60
// RBX::LocalScript::`scalar deleting destructor'
// SYNTHETIC: WEBSERVICE 0x10218c00
// `dynamic initializer for 'RBX::Script::prop_EmbeddedSourceCode''
// SYNTHETIC: WEBSERVICE 0x10218c40
// `dynamic initializer for 'RBX::Script::prop_SourceCodeId''
// SYNTHETIC: WEBSERVICE 0x10218c80
// `dynamic initializer for 'RBX::Script::prop_Disabled''
// SYNTHETIC: WEBSERVICE 0x10218cb0
// `dynamic initializer for 'helloWorld''
// SYNTHETIC: WEBSERVICE 0x10221b00
// `dynamic atexit destructor for 'helloWorld''
// SYNTHETIC: WEBSERVICE 0x10221b40
// `dynamic atexit destructor for 'RBX::Script::prop_Disabled''
// SYNTHETIC: WEBSERVICE 0x10221b60
// `dynamic atexit destructor for 'RBX::Script::prop_SourceCodeId''
// SYNTHETIC: WEBSERVICE 0x10221b80
// `dynamic atexit destructor for 'RBX::Script::prop_EmbeddedSourceCode''
// clang-format on

} // namespace RBX

#endif // SCRIPT_SCRIPT_H
