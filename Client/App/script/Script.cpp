#include "script/Script.h"

#include "network/Player.h"
#include "script/ScriptContext.h"
#include "util/ContentProvider.h"
#include "util/Name.h"
#include "v8datamodel/Team.h"

#include <boost/thread/once.hpp>

namespace RBX {

char sScript[] = "Script";

char sLocalScript[] = "LocalScript";

const Reflection::PropDescriptor<Script, std::string> Script::prop_EmbeddedSourceCode(
	"Source", "Data", &Script::getEmbeddedCode, &Script::setEmbeddedCode
);

const Reflection::PropDescriptor<Script, ContentId> Script::prop_SourceCodeId(
	"LinkedSource", "Data", &Script::getScriptId, &Script::setScriptId
);

Reflection::BoundProp<bool, 1> Script::prop_Disabled("Disabled", "Behavior", &Script::disabled);

} // namespace RBX

static boost::shared_ptr<const std::string> helloWorld;
static boost::once_flag flagInitScriptCpp = BOOST_ONCE_INIT;

namespace RBX {

Script::~Script()
{
}

boost::shared_ptr<const std::string> Script::requestCode()
{
	if (scriptId.isNull()) {
		return embeddedSource;
	}

	return ContentProvider::singleton().requestContentString(scriptId);
}

} // namespace RBX

static void initScriptCpp()
{
	helloWorld.reset(new std::string("print(\"Hello world!\")\r\n"));
}

namespace RBX {

Script::Script() : DescribedCreatable<Script, Instance, sScript>("Script"), disabled(false), owner(NULL)
{
	boost::call_once(initScriptCpp, flagInitScriptCpp);

	embeddedSource = helloWorld;
}

void Script::onAncestorChanged(const AncestorChanged& event)
{
	Instance::onAncestorChanged(event);

	IScriptOwner* newOwner = NULL;

	for (Instance* i = getParent(); i != NULL; i = i->getParent()) {
		IScriptOwner* scriptOwner = dynamic_cast<IScriptOwner*>(i);

		if (scriptOwner && (newOwner = scriptOwner->scriptShouldRun(this)) != NULL) {
			break;
		}
	}

	if (newOwner != owner) {
		if (owner) {
			owner->releaseScript(this);
		}

		owner = newOwner;

		if (ScriptContext* scriptContext = ServiceProvider::find<ScriptContext>(this)) {
			scriptContext->removeScript(this);

			if (owner) {
				owner->runScript(this, scriptContext);
			}
		}
	}
}

void Script::onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider)
{
	if (ScriptContext* scriptContext = oldProvider ? oldProvider->find<ScriptContext>() : NULL) {
		scriptContext->removeScript(this);
	}

	Instance::onServiceProvider(oldProvider, newProvider);
}

LocalScript::LocalScript()
{
	setName("LocalScript");
}

void Script::setEmbeddedCode(const std::string& value)
{
	if (*embeddedSource != value) {
		embeddedSource.reset(new std::string(value));
		raisePropertyChanged(prop_EmbeddedSourceCode);
	}
}

void Script::setScriptId(const ContentId& value)
{
	if (scriptId != value) {
		scriptId = value;
		requestCode();
		raisePropertyChanged(prop_SourceCodeId);
	}
}

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sTeam>();
template const RBX::Name& RBX::Name::doDeclare<RBX::Network::sPlayer>();
template void RBX::Name::callDoDeclare<RBX::sTeam>();
template void RBX::Name::callDoDeclare<RBX::Network::sPlayer>();

template const RBX::FactoryProduct<RBX::Network::Player, RBX::Instance, RBX::Network::sPlayer>::Creator
	RBX::FactoryProduct<RBX::Network::Player, RBX::Instance, RBX::Network::sPlayer>::creator;

template class RBX::FactoryProduct<RBX::Script, RBX::Instance, RBX::sScript>;
template class RBX::Reflection::
	Described<RBX::Script, RBX::sScript, RBX::FactoryProduct<RBX::Script, RBX::Instance, RBX::sScript> >;
template class RBX::DescribedCreatable<RBX::Script, RBX::Instance, RBX::sScript>;

template class RBX::FactoryProduct<RBX::LocalScript, RBX::Script, RBX::sLocalScript>;
template class RBX::Reflection::Described<
	RBX::LocalScript,
	RBX::sLocalScript,
	RBX::FactoryProduct<RBX::LocalScript, RBX::Script, RBX::sLocalScript> >;
template class RBX::DescribedCreatable<RBX::LocalScript, RBX::Script, RBX::sLocalScript>;
