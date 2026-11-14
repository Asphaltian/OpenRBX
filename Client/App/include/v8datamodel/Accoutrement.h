#ifndef V8DATAMODEL_ACCOUTREMENT_H
#define V8DATAMODEL_ACCOUTREMENT_H

#include "placeholder.h"
#include "util/Handle.h"
#include "util/ICameraSubject.h"
#include "util/IRenderable.h"
#include "util/ISelectable3d.h"
#include "v8datamodel/IEquipable.h"
#include "v8tree/Instance.h"

#include <boost/shared_ptr.hpp>
#include <boost/signals/connection.hpp>

class XmlElement;

namespace RBX {

class ModelInstance;
class PartInstance;

extern const char sHat[];

extern const char sAccoutrement[];

class Accoutrement : public DescribedCreatable<Accoutrement, Instance, sAccoutrement>,
					 public IEquipable,
					 public IRenderable,
					 public virtual ICameraSubject,
					 public virtual ISelectable3d
{
public:
	enum AccoutrementState
	{
		NOTHING = 0,
		HAS_HANDLE = 1,
		IN_WORKSPACE = 2,
		IN_CHARACTER = 3,
		EQUIPPED = 4
	};

protected:
	AccoutrementState backendAccoutrementState;
	CoordinateFrame attachmentPoint;
	boost::signals::scoped_connection handleTouched;
	boost::signals::scoped_connection characterChildAdded;
	boost::signals::scoped_connection characterChildRemoved;

	void onEvent_AddedBackend(boost::shared_ptr<Instance> instance);
	void onEvent_RemovedBackend(boost::shared_ptr<Instance> instance);
	void onEvent_HandleTouched(boost::shared_ptr<Instance> other);
	static AccoutrementState characterCanPickUpAccoutrement(Instance* character);
	static void UnequipThis(boost::shared_ptr<Instance> instance);
	AccoutrementState computeDesiredState(Instance* testParent);
	AccoutrementState computeDesiredState();
	void setDesiredState(AccoutrementState desiredState, const ServiceProvider* serviceProvider);
	void setBackendAccoutrementStateNoReplicate(int value);
	void rebuildBackendState();
	void connectTouchEvent();
	void upTo_Equipped();
	void upTo_InCharacter();
	void upTo_InWorkspace();
	void upTo_HasHandle();
	void downFrom_Equipped();
	void downFrom_InCharacter();
	void downFrom_InWorkspace();
	void downFrom_HasHandle();
	void onChildAdded();
	void onChildRemoved();

	virtual void onAncestorChanged(const AncestorChanged& event);

	virtual bool askSetParent(const Instance* instance) const { return true; }

	virtual bool askAddChild(const Instance* instance) const { return true; }

	virtual void readProperty(const XmlElement* element, IReferenceBinder& binder);
	virtual const CoordinateFrame getLocation() const;
	bool drawSelected() const;
	virtual void render3dSelect(Adorn* adorn, SelectState selectState);

public:
	static bool writeUnlocked;

	Accoutrement();
	virtual ~Accoutrement();

	virtual XmlElement* write();
	static void dropAll(ModelInstance* model);
	static void dropAllOthers(ModelInstance* model, Accoutrement* accoutrement);
	PartInstance* getHandle();
	const PartInstance* getHandleConst() const;
	void setBackendAccoutrementState(int value);

	int getBackendAccoutrementState() const { return backendAccoutrementState; }

	const CoordinateFrame& getAttachmentPoint() const { return attachmentPoint; }

	void setAttachmentPoint(const CoordinateFrame& value);
	virtual void onCameraNear(float distance);
	const G3D::Vector3 getAttachmentPos() const;
	const G3D::Vector3 getAttachmentForward() const;
	const G3D::Vector3 getAttachmentUp() const;
	const G3D::Vector3 getAttachmentRight() const;
	void setAttachmentPos(const G3D::Vector3& v);
	void setAttachmentForward(const G3D::Vector3& v);
	void setAttachmentUp(const G3D::Vector3& v);
	void setAttachmentRight(const G3D::Vector3& v);
};

class Hat : public DescribedCreatable<Hat, Accoutrement, sHat>
{
public:
	Hat();
};

} // namespace RBX

#endif // V8DATAMODEL_ACCOUTREMENT_H
