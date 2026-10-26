#include "ThumbnailGenerator.h"

#include "rbxgraphics/TextureProxyBase.h"
#include "script/Script.h"
#include "script/ScriptContext.h"
#include "util/Name.h"
#include "util/RunStateOwner.h"
#include "v8datamodel/Lighting.h"
#include "v8datamodel/ModelInstance.h"
#include "v8datamodel/Selection.h"
#include "v8datamodel/Workspace.h"

const char sThumbnailGenerator[] = "ThumbnailGenerator";

static RBX::Reflection::BoundFuncDesc<ThumbnailGenerator, std::string(std::string, int, int, bool), 4> clickFunction(
	&ThumbnailGenerator::click,
	"Click",
	"fileType",
	"width",
	"height",
	"hideSky",
	RBX::Reflection::FunctionDescriptor::NeedTrustedCaller
);
static RBX::Reflection::BoundFuncDesc<ThumbnailGenerator, std::string(std::string, std::string, int, int), 4>
	clickTextureFunction(
		&ThumbnailGenerator::clickTexture,
		"ClickTexture",
		"textureId",
		"fileType",
		"width",
		"height",
		RBX::Reflection::FunctionDescriptor::NeedTrustedCaller
	);

// STUB: WEBSERVICE 0x10006470
ThumbnailGenerator::ThumbnailGenerator()
{
}

// STUB: WEBSERVICE 0x10006af0
std::string ThumbnailGenerator::clickTexture(std::string textureId, std::string fileType, int cx, int cy)
{
	STUB(0x10006af0);
	return std::string();
}

// STUB: WEBSERVICE 0x10007170
std::string ThumbnailGenerator::click(std::string fileType, int cx, int cy, bool hideSky)
{
	STUB(0x10007170);
	return std::string();
}

// STUB: WEBSERVICE 0x10007470
void ThumbnailGenerator::doClick(std::string fileType, int cx, int cy, bool hideSky, G3D::BinaryOutput* binaryOutput)
{
	STUB(0x10007470);
}

template void RBX::Name::callDoDeclare<sThumbnailGenerator>();
template const RBX::Name& RBX::Name::doDeclare<sThumbnailGenerator>();
template void RBX::Name::callDoDeclare<RBX::sLighting>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sLighting>();
template void RBX::Name::callDoDeclare<RBX::sRunService>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sRunService>();
template void RBX::Name::callDoDeclare<RBX::sModel>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sModel>();
template void RBX::Name::callDoDeclare<RBX::sSelection>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sSelection>();
template void RBX::Name::callDoDeclare<RBX::sScriptContext>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sScriptContext>();
template void RBX::Name::callDoDeclare<RBX::sScript>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sScript>();
template void RBX::Name::callDoDeclare<RBX::sLocalScript>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sLocalScript>();
template void RBX::Name::callDoDeclare<RBX::sWorkspace>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sWorkspace>();

template const RBX::FactoryProduct<RBX::Lighting, RBX::Instance, RBX::sLighting>::Creator
	RBX::FactoryProduct<RBX::Lighting, RBX::Instance, RBX::sLighting>::creator;
template const RBX::FactoryProduct<RBX::RunService, RBX::Instance, RBX::sRunService>::Creator
	RBX::FactoryProduct<RBX::RunService, RBX::Instance, RBX::sRunService>::creator;
template const RBX::FactoryProduct<RBX::ModelInstance, RBX::PVInstance, RBX::sModel>::Creator
	RBX::FactoryProduct<RBX::ModelInstance, RBX::PVInstance, RBX::sModel>::creator;
template const RBX::FactoryProduct<RBX::Selection, RBX::Instance, RBX::sSelection>::Creator
	RBX::FactoryProduct<RBX::Selection, RBX::Instance, RBX::sSelection>::creator;
template const RBX::FactoryProduct<RBX::ScriptContext, RBX::Instance, RBX::sScriptContext>::Creator
	RBX::FactoryProduct<RBX::ScriptContext, RBX::Instance, RBX::sScriptContext>::creator;
template const RBX::FactoryProduct<RBX::Script, RBX::Instance, RBX::sScript>::Creator
	RBX::FactoryProduct<RBX::Script, RBX::Instance, RBX::sScript>::creator;
template const RBX::FactoryProduct<RBX::LocalScript, RBX::Script, RBX::sLocalScript>::Creator
	RBX::FactoryProduct<RBX::LocalScript, RBX::Script, RBX::sLocalScript>::creator;

typedef std::vector<boost::shared_ptr<RBX::Instance> > InstanceVector;
template InstanceVector::size_type InstanceVector::size() const;
template InstanceVector::iterator InstanceVector::begin();
template InstanceVector::iterator InstanceVector::end();
template InstanceVector::iterator InstanceVector::erase(InstanceVector::iterator);
template void InstanceVector::resize(InstanceVector::size_type);

template boost::shared_ptr<RBX::Instance>::~shared_ptr();
template boost::shared_ptr<RBX::ModelInstance>::~shared_ptr();
template boost::shared_ptr<RBX::Selection>::~shared_ptr();
template boost::shared_ptr<RBX::RunService>::~shared_ptr();
template boost::shared_ptr<RBX::Script>::~shared_ptr();
template boost::shared_ptr<RBX::ScriptContext>::~shared_ptr();
template boost::shared_ptr<RBX::Lighting>::~shared_ptr();

template G3D::ReferenceCountedPointer<G3D::Texture>::~ReferenceCountedPointer();
template G3D::ReferenceCountedPointer<RBX::TextureProxyBase>::~ReferenceCountedPointer();
