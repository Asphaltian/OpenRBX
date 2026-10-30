#ifndef V8DATAMODEL_TOOL_H
#define V8DATAMODEL_TOOL_H

#include "decomp.h"
#include "reflection/property.h"
#include "util/ILocation.h"
#include "util/IRenderable.h"
#include "util/ISelectable3d.h"
#include "v8datamodel/Hopper.h"
#include "v8datamodel/IEquipable.h"
#include "v8tree/Instance.h"

#include <boost/shared_ptr.hpp>
#include <boost/signals/connection.hpp>

class XmlElement;

namespace RBX {

class ModelInstance;
class Mouse;
class PartInstance;
class ToolMouseCommand;

namespace Network {
class Player;
} // namespace Network

extern const char sTool[];

// SIZE 0x238
class Tool : public DescribedCreatable<Tool, BackpackItem, sTool>,
			 public IEquipable,
			 public IRenderable,
			 virtual public ILocation,
			 virtual public ISelectable3d
{
private:
	int getNumToolsInCharacter();

public:
	enum ToolState
	{
		NOTHING = 0,
		HAS_HANDLE = 1,
		IN_WORKSPACE = 2,
		IN_CHARACTER = 3,
		HAS_TORSO = 4,
		EQUIPPED = 5,
		ACTIVATED = 6
	};

private:
	ToolState backendToolState;                              // 0x18c
	int frontendActivationState;                             // 0x190
	CoordinateFrame grip;                                    // 0x194
	bool enabled;                                            // 0x1c4
	ToolMouseCommand* toolMouseCommand;                      // 0x1c8
	boost::signals::scoped_connection handleTouched;         // 0x1cc
	boost::signals::scoped_connection characterChildAdded;   // 0x1e0
	boost::signals::scoped_connection characterChildRemoved; // 0x1f4
	boost::signals::scoped_connection torsoChildAdded;       // 0x208
	boost::signals::scoped_connection torsoChildRemoved;     // 0x21c

	void onEvent_AddedBackend(boost::shared_ptr<Instance> instance);
	void onEvent_RemovedBackend(boost::shared_ptr<Instance> instance);
	void onEvent_HandleTouched(boost::shared_ptr<Instance> other);
	static void moveAllToolsToBackpack(Network::Player* player);
	static ToolState characterCanPickUpTool(Instance* character);
	boost::shared_ptr<Mouse> getMouse();
	ToolState computeDesiredState(Instance* testParent);
	ToolState computeDesiredState();
	void setDesiredState(ToolState desiredState, const ServiceProvider* serviceProvider);
	void setBackendToolStateNoReplicate(int value);
	void rebuildBackendState();
	void connectTouchEvent();
	void upTo_Activated();
	void upTo_Equipped();
	void upTo_HasTorso();
	void upTo_InCharacter();
	void upTo_InWorkspace();
	void upTo_HasHandle();
	void downFrom_Activated();
	void downFrom_Equipped();
	void downFrom_HasTorso();
	void downFrom_InCharacter();
	void downFrom_InWorkspace();
	void downFrom_HasHandle();
	void onEquipping();
	void onUnequipped();
	void onChildAdded();
	void onChildRemoved();

	virtual void onAncestorChanged(const AncestorChanged& event);

	// FUNCTION: WEBSERVICE 0x100e44b0 FOLDED
	// FUNCTION: ROBLOX 0x00633110 FOLDED
	virtual bool askSetParent(const Instance* instance) const { return true; }

	virtual bool askAddChild(const Instance* instance) const { return true; }
	virtual void readProperty(const XmlElement* element, IReferenceBinder& binder);
	virtual const CoordinateFrame getLocation() const;

	// FUNCTION: WEBSERVICE 0x100cb120
	// FUNCTION: ROBLOX 0x0060fcf0
	virtual bool drawSelected() const { return backendToolState >= EQUIPPED; }

	virtual void onLocalClicked();
	virtual void onLocalOtherClicked();
	virtual void render3dSelect(Adorn* adorn, SelectState selectState);
	static bool characterCanUnequipTool(ModelInstance* character);

public:
	Tool();
	virtual ~Tool();

	// SYNTHETIC: WEBSERVICE 0x100cb1d0
	// SYNTHETIC: ROBLOX 0x0060fda0
	// RBX::Tool::`scalar deleting destructor'

	static void dropAll(ModelInstance* model);

	virtual bool canUnequip() { return true; }                                   // vtable+0x8c
	virtual bool canBePickedUpByPlayer(Network::Player* player) { return true; } // vtable+0x90

	PartInstance* getHandle();
	const PartInstance* getHandleConst() const;
	void setFrontendActivationState(int value);

	// FUNCTION: WEBSERVICE 0x100c9da0
	int getFrontendActivationState() const { return frontendActivationState; }

	void setBackendToolState(int value);

	// FUNCTION: WEBSERVICE 0x100c9db0
	int getBackendToolState() const { return backendToolState; }

	static Reflection::BoundProp<bool, 1> prop_Enabled;

	// FUNCTION: WEBSERVICE 0x100c9dc0
	const CoordinateFrame& getGrip() const { return grip; }

	void setGrip(const CoordinateFrame& value);
	void activate();
	void deactivate();
	const G3D::Vector3 getGripPos() const;
	const G3D::Vector3 getGripForward() const;
	const G3D::Vector3 getGripUp() const;
	const G3D::Vector3 getGripRight() const;
	void setGripPos(const G3D::Vector3& v);
	void setGripForward(const G3D::Vector3& v);
	void setGripUp(const G3D::Vector3& v);
	void setGripRight(const G3D::Vector3& v);
};

DECOMP_SIZE_ASSERT(Tool, 0x238)

// clang-format off
// FUNCTION: WEBSERVICE 0x1021c060
// RBX::`dynamic initializer for 'prop_Grip''
// FUNCTION: WEBSERVICE 0x1021c0c0
// RBX::`dynamic initializer for 'prop_GripPos''
// FUNCTION: WEBSERVICE 0x1021c120
// RBX::`dynamic initializer for 'prop_GripForward''
// FUNCTION: WEBSERVICE 0x1021c180
// RBX::`dynamic initializer for 'prop_GripUp''
// FUNCTION: WEBSERVICE 0x1021c1e0
// RBX::`dynamic initializer for 'prop_GripRight''
// FUNCTION: WEBSERVICE 0x1021c240
// RBX::`dynamic initializer for 'prop_BackendToolState''
// FUNCTION: WEBSERVICE 0x1021c2a0
// RBX::`dynamic initializer for 'prop_FrontendActivationState''
// FUNCTION: WEBSERVICE 0x1021c300
// FUNCTION: ROBLOX 0x007ece20
// `dynamic initializer for 'RBX::Tool::prop_Enabled''
// FUNCTION: WEBSERVICE 0x1021c330
// RBX::`dynamic initializer for 'event_Equipped''
// FUNCTION: WEBSERVICE 0x1021c350
// RBX::`dynamic initializer for 'event_Unequipped''
// FUNCTION: WEBSERVICE 0x1021c380
// RBX::`dynamic initializer for 'event_Activated''
// FUNCTION: WEBSERVICE 0x1021c3b0
// RBX::`dynamic initializer for 'event_Deactivated''
// SYNTHETIC: WEBSERVICE 0x10223c10
// RBX::`dynamic atexit destructor for 'prop_GripUp''
// SYNTHETIC: WEBSERVICE 0x10223c30
// RBX::`dynamic atexit destructor for 'prop_GripRight''
// SYNTHETIC: WEBSERVICE 0x10223c50
// RBX::`dynamic atexit destructor for 'prop_GripForward''
// SYNTHETIC: WEBSERVICE 0x10223c70
// RBX::`dynamic atexit destructor for 'prop_GripPos''
// SYNTHETIC: WEBSERVICE 0x10223c90
// RBX::`dynamic atexit destructor for 'prop_Grip''
// SYNTHETIC: WEBSERVICE 0x10223cb0
// RBX::`dynamic atexit destructor for 'prop_FrontendActivationState''
// SYNTHETIC: WEBSERVICE 0x10223cd0
// RBX::`dynamic atexit destructor for 'prop_BackendToolState''
// SYNTHETIC: WEBSERVICE 0x10223cf0
// SYNTHETIC: ROBLOX 0x007f46a0
// `dynamic atexit destructor for 'RBX::Tool::prop_Enabled''
// SYNTHETIC: WEBSERVICE 0x10223d20
// RBX::`dynamic atexit destructor for 'event_Activated''
// SYNTHETIC: WEBSERVICE 0x10223d30
// RBX::`dynamic atexit destructor for 'event_Deactivated''
// SYNTHETIC: WEBSERVICE 0x10223d40
// RBX::`dynamic atexit destructor for 'event_Unequipped''
// SYNTHETIC: WEBSERVICE 0x10223d50
// RBX::`dynamic atexit destructor for 'event_Equipped''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_TOOL_H
