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

// GLOBAL: WEBSERVICE 0x102f9ff4
// GLOBAL: ROBLOX 0x0095fd08
G3D::Vector3 MouseCommand::ignoreVector3;

// FUNCTION: WEBSERVICE 0x100be7d0
// FUNCTION: ROBLOX 0x00620410
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

// FUNCTION: WEBSERVICE 0x100be850
// FUNCTION: ROBLOX 0x00620490
void MouseCommand::capture()
{
	capturedMouse = true;
}

// FUNCTION: WEBSERVICE 0x100be860
// FUNCTION: ROBLOX 0x006204a0
bool MouseCommand::characterCanReach(const G3D::Vector3& hitPoint) const
{
	if (PartInstance* head = Humanoid::getLocalHeadFromContext(workspace)) {
		G3D::Vector3 delta = head->getCoordinateFrame().translation - hitPoint;
		return delta.magnitude() < 60.0;
	}

	return false;
}

// FUNCTION: WEBSERVICE 0x100be8c0
// FUNCTION: ROBLOX 0x00620500
G3D::Ray MouseCommand::getSearchRay(const G3D::Ray& unitRay)
{
	return G3D::Ray::fromOriginAndDirection(unitRay.origin, unitRay.direction * maxSearch());
}

// FUNCTION: WEBSERVICE 0x100be910
// FUNCTION: ROBLOX 0x00620550
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

// FUNCTION: WEBSERVICE 0x100be9c0
// FUNCTION: ROBLOX 0x00620600
TextureId MouseCommand::getCursorId() const
{
	return ContentId::fromAssets("Textures\\" + getCursorName() + ".png");
}

// FUNCTION: WEBSERVICE 0x100bec20
// FUNCTION: ROBLOX 0x006207e0
G3D::Ray MouseCommand::getUnitMouseRay(const UIEvent& uiEvent, ICameraOwner* _workspace)
{
	return _workspace->getGCamera().worldRay(
		uiEvent.mousePosition.x,
		uiEvent.mousePosition.y,
		G3D::Rect2D::xyxy(0, 0, uiEvent.windowSize.x, uiEvent.windowSize.y)
	);
}

// FUNCTION: WEBSERVICE 0x100becb0
// FUNCTION: ROBLOX 0x00620870
G3D::Ray MouseCommand::getSearchRay(const UIEvent& uiEvent, ICameraOwner* _workspace)
{
	G3D::Ray unitRay = getUnitMouseRay(uiEvent, _workspace);

	return getSearchRay(unitRay);
}

// FUNCTION: WEBSERVICE 0x100becf0
// FUNCTION: ROBLOX 0x006208b0
G3D::Ray MouseCommand::getUnitMouseRay(const UIEvent& uiEvent) const
{
	return getUnitMouseRay(uiEvent, workspace);
}

// FUNCTION: WEBSERVICE 0x100bed30
// FUNCTION: ROBLOX 0x006208f0
G3D::Ray MouseCommand::getSearchRay(const UIEvent& uiEvent) const
{
	return getSearchRay(uiEvent, workspace);
}

// FUNCTION: WEBSERVICE 0x100bed70
// FUNCTION: ROBLOX 0x00620940
MouseCommand::MouseCommand(Workspace* workspace) : capturedMouse(false), workspace(workspace)
{
}

// FUNCTION: WEBSERVICE 0x100bedd0
// STUB: ROBLOX 0x00620970
MouseCommand::~MouseCommand()
{
}

// FUNCTION: WEBSERVICE 0x100bee40
// FUNCTION: ROBLOX 0x006209e0
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

// FUNCTION: WEBSERVICE 0x100bef80
// FUNCTION: ROBLOX 0x00620ba0
PartInstance* MouseCommand::getPartByLocalCharacter(const UIEvent& uiEvent, G3D::Vector3& hitWorld)
{
	PartByLocalCharacter filter(workspace);
	return getPart(uiEvent, &filter, hitWorld);
}

// FUNCTION: WEBSERVICE 0x100beff0
// FUNCTION: ROBLOX 0x00620c90
PartInstance* MouseCommand::getUnlockedPart(const UIEvent& uiEvent, G3D::Vector3& hitWorld)
{
	Unlocked filter;
	return getPart(uiEvent, &filter, hitWorld);
}

// FUNCTION: WEBSERVICE 0x100bf020
// STUB: ROBLOX 0x00620cc0
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
