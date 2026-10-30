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
	friend class Workspace;

private:
	static float maxSearch() { return 2048.0f; }
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
		const UIEvent& uiEvent, const HitTestFilter* filter = NULL, G3D::Vector3& hitWorld = ignoreVector3
	);
	PartInstance* getUnlockedPart(const UIEvent& uiEvent, G3D::Vector3& hitWorld = ignoreVector3);
	PartInstance* getPartByLocalCharacter(const UIEvent& uiEvent, G3D::Vector3& hitWorld = ignoreVector3);
	PartInstance* getUnlockedPartByLocalCharacter(const UIEvent& uiEvent, G3D::Vector3& hitWorld = ignoreVector3);
	Surface* getSurface(const UIEvent& uiEvent, PartInstance*&, int&);
	Surface* getSurface(const UIEvent& uiEvent);

	// FUNCTION: WEBSERVICE 0x10069fd0 FOLDED
	// FUNCTION: ROBLOX 0x005c0390 FOLDED
	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent) // vtable+0x04
	{
		return this;
	}

protected:
	virtual void onMouseIdle(const UIEvent& uiEvent) {}  // vtable+0x08
	virtual void onMouseHover(const UIEvent& uiEvent) {} // vtable+0x0c

	// FUNCTION: WEBSERVICE 0x10069fd0 FOLDED
	// FUNCTION: ROBLOX 0x005c0390 FOLDED
	virtual MouseCommand* onKeyDown(const UIEvent& uiEvent) // vtable+0x10
	{
		return this;
	}

	virtual MouseCommand* onPeekKeyDown(const UIEvent& uiEvent) { return NULL; } // vtable+0x14
	virtual void onMouseMove(const UIEvent& uiEvent) {}                          // vtable+0x18
	virtual void onMouseDelta(const UIEvent& uiEvent) {}                         // vtable+0x1c

	// FUNCTION: WEBSERVICE 0x10069fe0
	// FUNCTION: ROBLOX 0x005b4660
	virtual MouseCommand* onMouseUp(const UIEvent& uiEvent) // vtable+0x20
	{
		releaseCapture();
		return NULL;
	}

	virtual void capture(); // vtable+0x24

	// FUNCTION: WEBSERVICE 0x10069ff0
	// FUNCTION: ROBLOX 0x005b4670
	virtual void releaseCapture() { capturedMouse = false; } // vtable+0x28

	// FUNCTION: WEBSERVICE 0x1006a700
	// FUNCTION: ROBLOX 0x005b4920
	virtual void cancel() // vtable+0x2c
	{
		if (capturedMouse) {
			releaseCapture();
		}
	}

	void snapshotSelectionPositions(XmlState*);
	bool characterCanReach(const G3D::Vector3& hitPoint) const;

	MouseCommand(Workspace* workspace);

public:
	virtual ~MouseCommand(); // vtable+0x30

	// SYNTHETIC: WEBSERVICE 0x1006a790
	// SYNTHETIC: ROBLOX 0x005d25f0
	// ??_GMouseCommand@RBX@@UAEPAXI@Z

	bool captured() const { return capturedMouse; }

	virtual MouseCommand* isSticky() const { return NULL; } // vtable+0x34
	virtual TextureId getCursorId() const;                  // vtable+0x38

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
	// FUNCTION: WEBSERVICE 0x100beda0
	// FUNCTION: ROBLOX 0x005c03a0
	virtual const std::string getCursorName() const { return "ArrowCursor"; } // vtable+0x3c
};

DECOMP_SIZE_ASSERT(MouseCommand, 0x20)

} // namespace RBX

#endif // V8DATAMODEL_MOUSECOMMAND_H
