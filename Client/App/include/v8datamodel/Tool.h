#ifndef V8DATAMODEL_TOOL_H
#define V8DATAMODEL_TOOL_H

#include "reflection/property.h"
#include "placeholder.h"
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
	ToolState backendToolState;
	int frontendActivationState;
	CoordinateFrame grip;
	bool enabled;
	ToolMouseCommand* toolMouseCommand;
	boost::signals::scoped_connection handleTouched;
	boost::signals::scoped_connection characterChildAdded;
	boost::signals::scoped_connection characterChildRemoved;
	boost::signals::scoped_connection torsoChildAdded;
	boost::signals::scoped_connection torsoChildRemoved;

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

	virtual bool askSetParent(const Instance* instance) const { return true; }

	virtual bool askAddChild(const Instance* instance) const { return true; }
	virtual void readProperty(const XmlElement* element, IReferenceBinder& binder);
	virtual const CoordinateFrame getLocation() const;

	virtual bool drawSelected() const { return backendToolState >= EQUIPPED; }

	virtual void onLocalClicked();
	virtual void onLocalOtherClicked();
	virtual void render3dSelect(Adorn* adorn, SelectState selectState);
	static bool characterCanUnequipTool(ModelInstance* character);

public:
	Tool();
	virtual ~Tool();

	static void dropAll(ModelInstance* model);

	virtual bool canUnequip() { return true; }
	virtual bool canBePickedUpByPlayer(Network::Player* player) { return true; }

	PartInstance* getHandle();
	const PartInstance* getHandleConst() const;
	void setFrontendActivationState(int value);

	int getFrontendActivationState() const { return frontendActivationState; }

	void setBackendToolState(int value);

	int getBackendToolState() const { return backendToolState; }

	static Reflection::BoundProp<bool, 1> prop_Enabled;

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

} // namespace RBX

#endif // V8DATAMODEL_TOOL_H
