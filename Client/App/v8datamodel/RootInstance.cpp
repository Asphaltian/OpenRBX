#include "v8datamodel/RootInstance.h"

#define NOMINMAX

#include "network/Player.h"
#include "network/Players.h"
#include "tool/DragUtilities.h"
#include "tool/Dragger.h"
#include "tool/MegaDragger.h"
#include "v8datamodel/Backpack.h"
#include "v8datamodel/Camera.h"
#include "v8datamodel/Hopper.h"
#include "v8datamodel/PVInstance.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/SpawnLocation.h"
#include "v8datamodel/Team.h"
#include "v8datamodel/Teams.h"
#include "v8datamodel/Tool.h"
#include "v8world/World.h"

#include <G3D/GCamera.h>
#include <algorithm>
#include <boost/bind.hpp>
#include <windows.h>

namespace RBX {

char sRootInstance[] = "RootInstance";

// FUNCTION: WEBSERVICE 0x100c0d50
void RootInstance::setInsertPoint(const G3D::Vector3& topCenter)
{
	insertPoint = Dragger::toGrid(topCenter);
}

// FUNCTION: WEBSERVICE 0x100c0d90
void RootInstance::moveSafe(MegaDragger& megaDragger, G3D::Vector3 move, MoveType moveType)
{
	megaDragger.startDragging();

	megaDragger.alignAndCleanParts();

	if (moveType == MOVE_DROP) {
		megaDragger.safeMoveYDrop(move);
	}
	else {
		megaDragger.safeMoveNoDrop(move);
	}

	megaDragger.finishDragging();
}

// FUNCTION: WEBSERVICE 0x100c0df0
void RootInstance::moveSafe(
	std::vector<boost::weak_ptr<PartInstance> >& partArray,
	G3D::Vector3 move,
	MoveType moveType
)
{
	MegaDragger megaDragger(NULL, partArray, this);

	moveSafe(megaDragger, move, moveType);
}

// FUNCTION: WEBSERVICE 0x100c0e80
void RootInstance::onDescendentAdded(Instance* instance)
{
	ModelInstance::onDescendentAdded(instance);

	ControllersUsed.setDirty();
}

// FUNCTION: WEBSERVICE 0x100c0ea0
void RootInstance::onDescendentRemoving(const boost::shared_ptr<Instance>& instance)
{
	ControllersUsed.setDirty();
	ModelInstance::onDescendentRemoving(instance);
}

// FUNCTION: WEBSERVICE 0x100c0eb0
G3D::Vector3 RootInstance::computeIdeInsertPoint() const
{
	float tempY = insertPoint.y = std::max(0.0f, insertPoint.y);

	insertPoint = getExtentsWorld().clip(insertPoint);

	insertPoint.y = tempY;
	insertPoint = Dragger::toGrid(insertPoint);
	return insertPoint;
}

// FUNCTION: WEBSERVICE 0x100c0f60
void RootInstance::moveToIdeInsertPoint(std::vector<boost::weak_ptr<PartInstance> >& partArray)
{
	if (!partArray.empty()) {
		Extents extents = DragUtilities::computeExtents(partArray);

		G3D::Vector3 insertPoint = computeIdeInsertPoint();

		insertPoint = insertPoint - extents.bottomCenter();

		G3D::Vector3 moveToInsert = Dragger::toGrid(insertPoint);

		moveSafe(partArray, moveToInsert, MOVE_DROP);
	}
}

// FUNCTION: WEBSERVICE 0x100c1020
ControllerTypeArray RootInstance::computeControllersUsed() const
{
	ControllerTypeArray result;

	for (unsigned int i = 0; i < numChildren(); i++) {
		const PVInstance* pv = dynamic_cast<const PVInstance*>(getChild(i));

		if (pv != NULL && pv->isControllable()) {
			result.setController(pv->getControllerType(), true);
		}
	}

	return result;
}

// STUB: WEBSERVICE 0x100c1110
void RootInstance::insertRaw(
	const std::vector<boost::shared_ptr<Instance> >& instances,
	Instance* requestedParent,
	std::vector<boost::weak_ptr<PartInstance> >& partArray
)
{
	std::for_each(instances.begin(), instances.end(), boost::bind(&Instance::setParent, _1, requestedParent));

	for (unsigned int i = 0; i < instances.size(); ++i) {
		PartInstance::findParts(instances[i].get(), partArray);
	}

	DragUtilities::join(partArray);
}

// FUNCTION: WEBSERVICE 0x100c1250
RootInstance::~RootInstance()
{
}

// FUNCTION: WEBSERVICE 0x100c1430
G3D::Vector3 RootInstance::computeCharacterInsertPoint(const Extents& extents) const
{
	ModelInstance* character = Network::Players::findLocalCharacter(this);

	if (character) {
		G3D::Vector3 cameraLook = getGCamera().getCoordinateFrame().lookVector();

		cameraLook.y = 0.0f;
		cameraLook.unitize();

		G3D::Vector3 halfSize = extents.size() * 0.5f;
		halfSize.y = 0.0f;

		std::vector<boost::weak_ptr<PartInstance> > partArray;
		PartInstance::findParts(character, partArray);
		Extents modelExtents = DragUtilities::computeExtents(partArray);

		G3D::Vector3 insertPoint = modelExtents.bottomCenter() + cameraLook * 7.0f + halfSize * cameraLook;

		insertPoint = Dragger::toGrid(insertPoint);

		return insertPoint;
	}

	return computeIdeInsertPoint();
}

// FUNCTION: WEBSERVICE 0x100c1640
void RootInstance::moveToPoint(PVInstance* pv, G3D::Vector3 point)
{
	if (contains(pv)) {
		std::vector<PVInstance*> pvInstances;
		pvInstances.push_back(pv);

		G3D::Vector3 move = Dragger::toGrid(point - pv->getLocation().translation);

		MegaDragger megaDragger(pv->getPrimaryPart(), pvInstances, this);

		moveSafe(megaDragger, move, MOVE_NO_DROP);
	}
}

// FUNCTION: WEBSERVICE 0x100c17a0
void RootInstance::moveToCharacterInsertPoint(std::vector<boost::weak_ptr<PartInstance> >& partArray)
{
	if (!partArray.empty()) {
		Extents extents = DragUtilities::computeExtents(partArray);

		G3D::Vector3 characterInsertPoint = computeCharacterInsertPoint(extents);

		characterInsertPoint = characterInsertPoint - extents.bottomCenter();

		G3D::Vector3 moveToInsert = Dragger::toGrid(characterInsertPoint);

		moveSafe(partArray, moveToInsert, MOVE_DROP);
	}
}

// FUNCTION: WEBSERVICE 0x100c1860
void RootInstance::insertToTree(const std::vector<boost::shared_ptr<Instance> >& instances, Instance* requestedParent)
{
	std::vector<boost::weak_ptr<PartInstance> > partArray;
	insertRaw(instances, requestedParent, partArray);

	if (!partArray.empty()) {
		moveSafe(partArray, G3D::Vector3::zero(), MOVE_NO_DROP);
	}
}

// FUNCTION: WEBSERVICE 0x100c1960
void RootInstance::insertCharacterView(
	const std::vector<boost::shared_ptr<Instance> >& instances,
	std::vector<boost::weak_ptr<PartInstance> >& partArray
)
{
	if (instances.size() == 1) {
		Instance* instance = instances[0].get();

		if (dynamic_cast<HopperBin*>(instance)) {
			Network::Player* player = Network::Players::findLocalPlayer(this);

			if (player) {
				instance->setParent(player->getPlayerBackpack());
				return;
			}
		}
	}

	insertRaw(instances, this, partArray);

	moveToCharacterInsertPoint(partArray);
}

// FUNCTION: WEBSERVICE 0x100c1b50
void RootInstance::insertIdeView(
	const std::vector<boost::shared_ptr<Instance> >& instances,
	std::vector<boost::weak_ptr<PartInstance> >& partArray,
	PromptMode promptMode
)
{
	if (instances.size() == 1) {
		Instance* instance = instances[0].get();

		if (dynamic_cast<HopperBin*>(instance)) {
			instance->setParent(ServiceProvider::create<StarterPackService>(this));
			return;
		}

		if (dynamic_cast<Tool*>(instance) && promptMode == ALLOW_PROMPTS) {
			if (MessageBox(
					NULL,
					"Put this tool into the starter pack (otherwise drop into the 3d view)?",
					"Inserting Tool",
					MB_YESNO
				) == IDYES) {
				instance->setParent(ServiceProvider::create<StarterPackService>(this));
				return;
			}
		}
	}

	insertRaw(instances, this, partArray);

	if (!partArray.empty()) {
		moveToIdeInsertPoint(partArray);

		Extents extents = DragUtilities::computeExtents(partArray);

		getCamera()->lookAt(extents.center());
	}
}

// FUNCTION: WEBSERVICE 0x100c1c90
void RootInstance::insert3dView(const std::vector<boost::shared_ptr<Instance> >& instances, PromptMode promptMode)
{
	std::vector<boost::weak_ptr<PartInstance> > partArray;

	if (Network::Players::findLocalCharacter(this)) {
		insertCharacterView(instances, partArray);
	}
	else {
		insertIdeView(instances, partArray, promptMode);
	}

	if (!partArray.empty()) {
		Extents extents = DragUtilities::computeExtents(partArray);

		getCamera()->zoomExtents(extents, viewPort, Camera::ZOOM_OUT_ONLY);
	}
}

// FUNCTION: WEBSERVICE 0x100c1db0
void RootInstance::insertSpawnLocation(SpawnLocation* s)
{
	if (!s->neutral) {
		Teams* teams = ServiceProvider::create<Teams>(this);

		if (!teams->teamExists(s->getTeamColor())) {
			boost::shared_ptr<Team> t = Creatable<Instance>::create<Team>();
			t->setParent(teams);
			t->setTeamColor(s->getTeamColor());
			t->setName(s->getTeamColor().name() + " Team");
		}
	}
}

// STUB: WEBSERVICE 0x100c1ee0
void RootInstance::insertInstances(
	const std::vector<boost::shared_ptr<Instance> >& instances,
	Instance* requestedParent,
	InsertMode insertMode,
	PromptMode promptMode
)
{
	STUB(0x100c1ee0);
}

// FUNCTION: WEBSERVICE 0x100c22b0
RootInstance::RootInstance()
	: ControllersUsed(this, &RootInstance::computeControllersUsed), insertPoint(G3D::Vector3::zero()),
	  viewPort(G3D::Vector2(800.0f, 600.0f)), world(new World())
{
}

} // namespace RBX
