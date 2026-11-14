#ifndef V8DATAMODEL_MOUSECOMMAND_H
#define V8DATAMODEL_MOUSECOMMAND_H

#include "placeholder.h"
#include "util/IRenderable.h"
#include "util/Name.h"
#include "util/TextureId.h"

#include <G3D/Ray.h>
#include <G3D/Vector3.h>
#include <memory>
#include <string>
#include <vector>

namespace RBX {

class ContactManager;
class HitTestFilter;
class ICameraOwner;
class Instance;
class PartInstance;
class Primitive;
class Surface;
class UIEvent;
class Workspace;
class XmlState;

class MouseCommand : public INamed, public IRenderable
{
	friend class Workspace;

private:
	static float maxSearch() { return 2048.0f; }
	static G3D::Vector3 ignoreVector3;

	bool capturedMouse;

protected:
	Workspace* workspace;
	std::auto_ptr<XmlState> undoState;

public:
	G3D::Ray getUnitMouseRay(const UIEvent& uiEvent) const;
	static G3D::Ray getUnitMouseRay(const UIEvent& uiEvent, ICameraOwner* _workspace);
	G3D::Ray getSearchRay(const UIEvent& uiEvent) const;
	static G3D::Ray getSearchRay(const G3D::Ray& unitRay);
	static G3D::Ray getSearchRay(const UIEvent& uiEvent, ICameraOwner* _workspace);
	static Instance* getTopSelectable3d(PartInstance* part);
	PartInstance* getPart(
		const UIEvent& uiEvent, const HitTestFilter* filter = NULL, G3D::Vector3& hitWorld = ignoreVector3
	);
	PartInstance* getUnlockedPart(const UIEvent& uiEvent, G3D::Vector3& hitWorld = ignoreVector3);
	PartInstance* getPartByLocalCharacter(const UIEvent& uiEvent, G3D::Vector3& hitWorld = ignoreVector3);
	PartInstance* getUnlockedPartByLocalCharacter(const UIEvent& uiEvent, G3D::Vector3& hitWorld = ignoreVector3);
	Surface* getSurface(const UIEvent& uiEvent, PartInstance*&, int&);
	Surface* getSurface(const UIEvent& uiEvent);

	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent) { return this; }

protected:
	virtual void onMouseIdle(const UIEvent& uiEvent) {}
	virtual void onMouseHover(const UIEvent& uiEvent) {}

	virtual MouseCommand* onKeyDown(const UIEvent& uiEvent) { return this; }

	virtual MouseCommand* onPeekKeyDown(const UIEvent& uiEvent) { return NULL; }
	virtual void onMouseMove(const UIEvent& uiEvent) {}
	virtual void onMouseDelta(const UIEvent& uiEvent) {}

	virtual MouseCommand* onMouseUp(const UIEvent& uiEvent)
	{
		releaseCapture();
		return NULL;
	}

	virtual void capture();

	virtual void releaseCapture() { capturedMouse = false; }

	virtual void cancel()
	{
		if (capturedMouse) {
			releaseCapture();
		}
	}

	void snapshotSelectionPositions(XmlState*);
	bool characterCanReach(const G3D::Vector3& hitPoint) const;

	MouseCommand(Workspace* workspace);

public:
	virtual ~MouseCommand();

	bool captured() const { return capturedMouse; }

	virtual MouseCommand* isSticky() const { return NULL; }
	virtual TextureId getCursorId() const;

	static PartInstance* getMousePart(
		const G3D::Ray& unitRay,
		const ContactManager& contactManager,
		const std::vector<const Primitive*>& ignore,
		const HitTestFilter* filter,
		G3D::Vector3& hitPoint,
		float maxSearchGrid
	);
	static PartInstance* getMousePart(
		const G3D::Ray& unitRay,
		const ContactManager& contactManager,
		const Primitive* ignore,
		const HitTestFilter* filter,
		G3D::Vector3& hitPoint,
		float maxSearchGrid
	);

private:
	virtual const std::string getCursorName() const { return "ArrowCursor"; }
};

} // namespace RBX

#endif // V8DATAMODEL_MOUSECOMMAND_H
