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

char sFillTool[] = "Fill";

char sDropperTool[] = "Dropper";

char sModelSetPrimaryPartTool[] = "ModelSetPrimaryPart";

FillToolColor FillTool::color;

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

void PartTool::render3dAdorn(Adorn* adorn)
{
	if (partInstance) {
		partInstance->render3dSelect(adorn, SELECT_LIMIT);
	}
}

PartTool::PartTool(Workspace* workspace) : MouseCommand(workspace)
{
}

void PartTool::onMouseHover(const UIEvent& uiEvent)
{
	partInstance = shared_from(getPart(uiEvent));
}

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
