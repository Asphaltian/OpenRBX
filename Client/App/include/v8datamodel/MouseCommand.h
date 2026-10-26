#ifndef V8DATAMODEL_MOUSECOMMAND_H
#define V8DATAMODEL_MOUSECOMMAND_H

#include "decomp.h"
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

// SIZE 0x20
class MouseCommand : public INamed, public IRenderable
{
private:
	static float maxSearch();
	static G3D::Vector3 ignoreVector3;

	bool capturedMouse; // 0x14

protected:
	Workspace* workspace;              // 0x18
	std::auto_ptr<XmlState> undoState; // 0x1c

public:
	G3D::Ray getUnitMouseRay(const UIEvent& uiEvent) const;
	static G3D::Ray getUnitMouseRay(const UIEvent& uiEvent, ICameraOwner* _workspace);
	G3D::Ray getSearchRay(const UIEvent& uiEvent) const;
	static G3D::Ray getSearchRay(const G3D::Ray& unitRay);
	static G3D::Ray getSearchRay(const UIEvent& uiEvent, ICameraOwner* _workspace);
	static Instance* getTopSelectable3d(PartInstance* part);
	PartInstance* getPart(
		const UIEvent& uiEvent,
		const HitTestFilter* filter = NULL,
		G3D::Vector3& hitWorld = ignoreVector3
	);
	PartInstance* getUnlockedPart(const UIEvent& uiEvent, G3D::Vector3& hitWorld = ignoreVector3);
	PartInstance* getPartByLocalCharacter(const UIEvent& uiEvent, G3D::Vector3& hitWorld = ignoreVector3);
	PartInstance* getUnlockedPartByLocalCharacter(const UIEvent& uiEvent, G3D::Vector3& hitWorld = ignoreVector3);
	Surface* getSurface(const UIEvent& uiEvent, PartInstance*&, int&);
	Surface* getSurface(const UIEvent& uiEvent);

	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent); // vtable+0x04

protected:
	virtual void onMouseIdle(const UIEvent& uiEvent);            // vtable+0x08
	virtual void onMouseHover(const UIEvent& uiEvent);           // vtable+0x0c
	virtual MouseCommand* onKeyDown(const UIEvent& uiEvent);     // vtable+0x10
	virtual MouseCommand* onPeekKeyDown(const UIEvent& uiEvent); // vtable+0x14
	virtual void onMouseMove(const UIEvent& uiEvent);            // vtable+0x18
	virtual void onMouseDelta(const UIEvent& uiEvent);           // vtable+0x1c
	virtual MouseCommand* onMouseUp(const UIEvent& uiEvent);     // vtable+0x20
	virtual void capture();                                      // vtable+0x24
	virtual void releaseCapture();                               // vtable+0x28
	virtual void cancel();                                       // vtable+0x2c

	void snapshotSelectionPositions(XmlState*);
	bool characterCanReach(const G3D::Vector3& hitPoint) const;

	MouseCommand(Workspace* workspace);

public:
	virtual ~MouseCommand(); // vtable+0x30

	bool captured() const;

	virtual MouseCommand* isSticky() const; // vtable+0x34
	virtual TextureId getCursorId() const;  // vtable+0x38

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
	virtual const std::string getCursorName() const; // vtable+0x3c
};

DECOMP_SIZE_ASSERT(MouseCommand, 0x20)

} // namespace RBX

#endif // V8DATAMODEL_MOUSECOMMAND_H
