#ifndef SCRIPT_SCRIPT_H
#define SCRIPT_SCRIPT_H

#include "placeholder.h"
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
class Script : public DescribedCreatable<Script, Instance, sScript>
{
public:
	Script();
	virtual ~Script();

	class Slot
	{
	public:
		boost::shared_ptr<boost::signals::connection> cnction;

	protected:
		Slot() : cnction(new boost::signals::connection()) {}
	};

	const ContentId& getScriptId() const { return scriptId; }

	boost::shared_ptr<const std::string> requestCode();
	void setEmbeddedCode(const std::string& value);

	const std::string& getEmbeddedCode() const { return *embeddedSource; }

	void setScriptId(const ContentId& value);

	bool isDisabled() const { return disabled; }

	static const Reflection::PropDescriptor<Script, std::string> prop_EmbeddedSourceCode;
	static const Reflection::PropDescriptor<Script, ContentId> prop_SourceCodeId;
	static Reflection::BoundProp<bool, 1> prop_Disabled;

	virtual bool askSetParent(const Instance* instance) const { return true; }
	bool isCodeEmbedded() const;

protected:
	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);
	virtual void onAncestorChanged(const AncestorChanged& event);

private:
	boost::shared_ptr<const std::string> embeddedSource;
	ContentId scriptId;
	bool disabled;
	IScriptOwner* owner;
};

class LocalScript : public DescribedCreatable<LocalScript, Script, sLocalScript>
{
public:
	LocalScript();

	virtual ~LocalScript() {}
};

class ScriptContext;

class __declspec(novtable) IScriptOwner
{
	friend class Script;

protected:
	virtual IScriptOwner* scriptShouldRun(Script* script) = 0;

	virtual void runScript(Script* script, ScriptContext* scriptContext) {}

	virtual void releaseScript(Script* script) {}
};

} // namespace RBX

#endif // SCRIPT_SCRIPT_H
