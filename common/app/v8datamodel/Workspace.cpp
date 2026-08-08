#include "v8datamodel/Workspace.h"

#include "tool/ToolsArrow.h"
#include "util/Name.h"
#include "util/standardout.h"
#include "v8datamodel/Camera.h"
#include "v8datamodel/Flag.h"
#include "v8datamodel/Hopper.h"
#include "v8datamodel/Tool.h"
#include "v8datamodel/UserController.h"

namespace RBX {

char sWorkspace[] = "Workspace";

bool Workspace::showWorldCoord;

// FUNCTION: WEBSERVICE 0x1006a0f0
const G3D::GCamera& Workspace::getGCamera() const
{
	return getCamera()->getGCamera();
}

// STUB: WEBSERVICE 0x1006a960
IScriptOwner* Workspace::scriptShouldRun(Script* script)
{
	STUB(0x1006a960);
	return NULL;
}

// STUB: WEBSERVICE 0x1006bfd0
Camera* Workspace::getCamera() const
{
	STUB(0x1006bfd0);
	return NULL;
}

// STUB: WEBSERVICE 0x1006ccb0
void Workspace::runScript(Script* script, ScriptContext* context)
{
	STUB(0x1006ccb0);
}

// STUB: WEBSERVICE 0x1006cd70
Workspace::~Workspace()
{
	STUB(0x1006cd70);
}

// STUB: WEBSERVICE 0x1006d0d0
Extents Workspace::computeCameraOwnerExtents()
{
	STUB(0x1006d0d0);
	return Extents();
}

// STUB: WEBSERVICE 0x1006d230
void Workspace::cameraMoved()
{
	Notifier<Workspace, DrawChanged>::raise(DrawChanged());
}

// STUB: WEBSERVICE 0x1006d440
void Workspace::releaseScript(Script* script)
{
	STUB(0x1006d440);
}

// STUB: WEBSERVICE 0x1006ebc0
Workspace* Workspace::findWorkspace(const Instance* context)
{
	STUB(0x1006ebc0);
	return NULL;
}

// STUB: WEBSERVICE 0x1006ec00
World* Workspace::getWorldIfInWorkspace(const Instance* context)
{
	STUB(0x1006ec00);
	return NULL;
}

// STUB: WEBSERVICE 0x1006ec40
bool Workspace::contextInWorkspace(const Instance* context)
{
	STUB(0x1006ec40);
	return false;
}

// STUB: WEBSERVICE 0x1006ec80
void Workspace::onEvent(const RunService* source, Heartbeat event)
{
	STUB(0x1006ec80);
}

// STUB: WEBSERVICE 0x100703e0
Workspace::Workspace(IDataState* dataState) : VerbContainer(NULL)
{
	STUB(0x100703e0);
}

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sControllerService>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sCamera>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sHopperBin>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sStarterPackService>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sLegacyHopperService>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sTool>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sFlag>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sArrowTool>();
template void RBX::Name::callDoDeclare<RBX::sControllerService>();
template void RBX::Name::callDoDeclare<RBX::sCamera>();
template void RBX::Name::callDoDeclare<RBX::sHopperBin>();
template void RBX::Name::callDoDeclare<RBX::sStarterPackService>();
template void RBX::Name::callDoDeclare<RBX::sLegacyHopperService>();
template void RBX::Name::callDoDeclare<RBX::sTool>();
template void RBX::Name::callDoDeclare<RBX::sFlag>();
template void RBX::Name::callDoDeclare<RBX::sArrowTool>();
