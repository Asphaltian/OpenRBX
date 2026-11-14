#include "v8datamodel/MouseCommand.h"

#include "humanoid/Humanoid.h"
#include "util/ContentProvider.h"
#include "util/ISelectable3d.h"
#include "util/UIEvent.h"
#include "v8datamodel/Camera.h"
#include "v8datamodel/Filters.h"
#include "v8datamodel/ICharacterSubject.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/Workspace.h"
#include "v8world/SpatialHash.h"
#include "v8world/World.h"

#include <G3D/GCamera.h>
#include <G3D/Rect2D.h>

namespace RBX {

G3D::Vector3 MouseCommand::ignoreVector3;

Instance* MouseCommand::getTopSelectable3d(PartInstance* part)
{
	Instance* answer = part;
	Instance* test = part;
	while (!dynamic_cast<Workspace*>(test->getParent())) {
		test = test->getParent();
		if (dynamic_cast<ISelectable3d*>(test)) {
			answer = test;
		}
	}

	return answer;
}

void MouseCommand::capture()
{
	capturedMouse = true;
}

bool MouseCommand::characterCanReach(const G3D::Vector3& hitPoint) const
{
	if (PartInstance* head = Humanoid::getLocalHeadFromContext(workspace)) {
		G3D::Vector3 delta = head->getCoordinateFrame().translation - hitPoint;
		return delta.magnitude() < 60.0;
	}

	return false;
}

G3D::Ray MouseCommand::getSearchRay(const G3D::Ray& unitRay)
{
	return G3D::Ray::fromOriginAndDirection(unitRay.origin, unitRay.direction * maxSearch());
}

PartInstance* MouseCommand::getMousePart(
	const G3D::Ray& unitRay,
	const ContactManager& contactManager,
	const std::vector<const Primitive*>& ignore,
	const HitTestFilter* filter,
	G3D::Vector3& hitPoint,
	float maxSearchGrid
)
{
	G3D::Ray searchRay = G3D::Ray::fromOriginAndDirection(unitRay.origin, unitRay.direction * maxSearch());

	Primitive* hit = contactManager.getHit(searchRay, &ignore, filter, hitPoint);

	return hit ? const_cast<PartInstance*>(PartInstance::fromPrimitiveConst(hit)) : NULL;
}

TextureId MouseCommand::getCursorId() const
{
	return ContentId::fromAssets("Textures\\" + getCursorName() + ".png");
}

G3D::Ray MouseCommand::getUnitMouseRay(const UIEvent& uiEvent, ICameraOwner* _workspace)
{
	return _workspace->getGCamera().worldRay(
		uiEvent.mousePosition.x,
		uiEvent.mousePosition.y,
		G3D::Rect2D::xyxy(0, 0, uiEvent.windowSize.x, uiEvent.windowSize.y)
	);
}

G3D::Ray MouseCommand::getSearchRay(const UIEvent& uiEvent, ICameraOwner* _workspace)
{
	G3D::Ray unitRay = getUnitMouseRay(uiEvent, _workspace);

	return getSearchRay(unitRay);
}

G3D::Ray MouseCommand::getUnitMouseRay(const UIEvent& uiEvent) const
{
	return getUnitMouseRay(uiEvent, workspace);
}

G3D::Ray MouseCommand::getSearchRay(const UIEvent& uiEvent) const
{
	return getSearchRay(uiEvent, workspace);
}

MouseCommand::MouseCommand(Workspace* workspace) : capturedMouse(false), workspace(workspace)
{
}

MouseCommand::~MouseCommand()
{
}

PartInstance* MouseCommand::getPart(const UIEvent& uiEvent, const HitTestFilter* filter, G3D::Vector3& hitWorld)
{
	G3D::Ray unitRay = getUnitMouseRay(uiEvent);
	ContactManager& contactManager = workspace->getWorld()->getContactManager();

	std::vector<const Primitive*> ignorePrims;
	static_cast<ICameraOwner*>(workspace)->getCameraIgnorePrimitives(ignorePrims);
	if (Camera* camera = workspace->getCamera()) {
		if (ICharacterSubject* subject = dynamic_cast<ICharacterSubject*>(camera->getCameraSubject())) {
			subject->getCameraIgnorePrimitives(ignorePrims);
		}
	}

	PartInstance* part = getMousePart(unitRay, contactManager, ignorePrims, filter, hitWorld, maxSearch());

	if (part == NULL) {
		hitWorld = unitRay.origin + unitRay.direction * 10000.0f;
	}

	return part;
}

Surface* MouseCommand::getSurface(const UIEvent& uiEvent, PartInstance*& part, int& surfaceId)
{
	part = getPart(uiEvent);
	if (part) {
		G3D::Ray ray = getUnitMouseRay(uiEvent);
		Surface* surface = part->getSurface(ray, surfaceId);
		if (surface) {
			return surface;
		}
	}
	return NULL;
}

PartInstance* MouseCommand::getPartByLocalCharacter(const UIEvent& uiEvent, G3D::Vector3& hitWorld)
{
	PartByLocalCharacter filter(workspace);
	return getPart(uiEvent, &filter, hitWorld);
}

PartInstance* MouseCommand::getUnlockedPartByLocalCharacter(const UIEvent& uiEvent, G3D::Vector3& hitWorld)
{
	UnlockedPartByLocalCharacter filter(workspace);
	return getPart(uiEvent, &filter, hitWorld);
}

PartInstance* MouseCommand::getUnlockedPart(const UIEvent& uiEvent, G3D::Vector3& hitWorld)
{
	Unlocked filter;
	return getPart(uiEvent, &filter, hitWorld);
}

PartInstance* MouseCommand::getMousePart(
	const G3D::Ray& unitRay,
	const ContactManager& contactManager,
	const Primitive* ignore,
	const HitTestFilter* filter,
	G3D::Vector3& hitPoint,
	float maxSearchGrid
)
{
	std::vector<const Primitive*> ignoreVector;
	ignoreVector.push_back(ignore);
	return getMousePart(unitRay, contactManager, ignoreVector, filter, hitPoint, maxSearchGrid);
}

} // namespace RBX
