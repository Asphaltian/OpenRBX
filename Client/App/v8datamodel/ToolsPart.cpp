#include "v8datamodel/ToolsPart.h"

#include "util/Name.h"
#include "util/Sound.h"
#include "util/SoundWorld.h"
#include "v8datamodel/Commands.h"
#include "v8datamodel/ModelInstance.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/Workspace.h"
#include "v8datamodel/custommesh.h"
#include "v8tree/Service.h"

template const RBX::Name& RBX::Name::doDeclare<RBX::sSpecialShape>();
template void RBX::Name::callDoDeclare<RBX::sSpecialShape>();

namespace RBX {

// GLOBAL: ROBLOX 0x0093657c
char sFillTool[] = "Fill";

// GLOBAL: ROBLOX 0x00936584
char sDropperTool[] = "Dropper";

// GLOBAL: ROBLOX 0x0093658c
char sModelSetPrimaryPartTool[] = "ModelSetPrimaryPart";

// GLOBAL: ROBLOX 0x009365a0
FillToolColor FillTool::color;

// FUNCTION: ROBLOX 0x005c02f0
MouseCommand* ModelSetPrimaryPartTool::onMouseDown(const UIEvent& uiEvent)
{
	PartInstance* part = getPart(uiEvent);
	if (part) {
		ModelInstance* model = dynamic_cast<ModelInstance*>(part->getTopLevelPVParent());
		if (model) {
			model->setPrimaryPart(part);
		}
	}
	return NULL;
}

// FUNCTION: ROBLOX 0x005c0340
void PartTool::render3dAdorn(Adorn* adorn)
{
	if (partInstance) {
		partInstance->render3dSelect(adorn, SELECT_LIMIT);
	}
}

// FUNCTION: ROBLOX 0x005c0360
PartTool::PartTool(Workspace* workspace) : MouseCommand(workspace)
{
}

// FUNCTION: ROBLOX 0x005c0450
void PartTool::onMouseHover(const UIEvent& uiEvent)
{
	partInstance = shared_from(getPart(uiEvent));
}

// FUNCTION: ROBLOX 0x005c0530
MouseCommand* FillTool::onMouseDown(const UIEvent& uiEvent)
{
	PartInstance* part = getPart(uiEvent);
	if (part) {
		UndoState* undoState = new UndoState();
		undoState->addProperty(Reflection::Property(PartInstance::prop_BrickColor, part));
		part->setColor(color.get());
		ServiceProvider::create<Soundscape::SoundService>(workspace)->playSound(SPLAT_SOUND);

		RedoState* redoState = new RedoState();
		redoState->addProperty(Reflection::Property(PartInstance::prop_BrickColor, part));
		IDataState& dataState = workspace->getDataState();
		const Name& name = getName();
		dataState.pushState(name.toString(), undoState, redoState);
		workspace->getDataState().setDirty(true);
	}
	return NULL;
}

// FUNCTION: ROBLOX 0x005c0690
MouseCommand* DropperTool::onMouseDown(const UIEvent& uiEvent)
{
	PartInstance* part = getPart(uiEvent);
	if (part) {
		FillTool::color.set(part->getColor());
		ServiceProvider::create<Soundscape::SoundService>(workspace)->playSound(CLICK_SOUND);
	}
	return NULL;
}

} // namespace RBX
