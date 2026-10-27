#ifndef V8DATAMODEL_ACCOUTREMENT_H
#define V8DATAMODEL_ACCOUTREMENT_H

#include "decomp.h"
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

// SIZE 0x1a0
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
	AccoutrementState backendAccoutrementState;              // 0x11c
	CoordinateFrame attachmentPoint;                         // 0x120
	boost::signals::scoped_connection handleTouched;         // 0x150
	boost::signals::scoped_connection characterChildAdded;   // 0x164
	boost::signals::scoped_connection characterChildRemoved; // 0x178

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

	// FUNCTION: WEBSERVICE 0x100e44b0 FOLDED
	virtual bool askSetParent(const Instance* instance) const { return true; }

	// FUNCTION: WEBSERVICE 0x100e44b0 FOLDED
	virtual bool askAddChild(const Instance* instance) const { return true; }

	virtual void readProperty(const XmlElement* element, IReferenceBinder& binder);
	virtual const CoordinateFrame getLocation() const;
	bool drawSelected() const;
	virtual void render3dSelect(Adorn* adorn, SelectState selectState);

public:
	static bool writeUnlocked;

	Accoutrement();
	virtual ~Accoutrement();

	// SYNTHETIC: WEBSERVICE 0x100dc260
	// RBX::Accoutrement::`scalar deleting destructor'

	virtual XmlElement* write();
	static void dropAll(ModelInstance* model);
	static void dropAllOthers(ModelInstance* model, Accoutrement* accoutrement);
	PartInstance* getHandle();
	const PartInstance* getHandleConst() const;
	void setBackendAccoutrementState(int value);

	// FUNCTION: WEBSERVICE 0x100db430
	int getBackendAccoutrementState() const { return backendAccoutrementState; }

	// FUNCTION: WEBSERVICE 0x100db440
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

DECOMP_SIZE_ASSERT(Accoutrement, 0x1a0)

// SIZE 0x1a0
class Hat : public DescribedCreatable<Hat, Accoutrement, sHat>
{
public:
	Hat();
};

DECOMP_SIZE_ASSERT(Hat, 0x1a0)

// clang-format off
// FUNCTION: WEBSERVICE 0x1021c690
// RBX::`dynamic initializer for 'prop_AttachmentPoint''
// FUNCTION: WEBSERVICE 0x1021c6f0
// RBX::`dynamic initializer for 'prop_AttachmentPos''
// FUNCTION: WEBSERVICE 0x1021c750
// RBX::`dynamic initializer for 'prop_AttachmentForward''
// FUNCTION: WEBSERVICE 0x1021c7b0
// RBX::`dynamic initializer for 'prop_AttachmentUp''
// FUNCTION: WEBSERVICE 0x1021c810
// RBX::`dynamic initializer for 'prop_AttachmentRight''
// SYNTHETIC: WEBSERVICE 0x1021c870
// RBX::`dynamic initializer for 'prop_BackendAccoutrementState''
// SYNTHETIC: WEBSERVICE 0x10223ea0
// RBX::`dynamic atexit destructor for 'prop_AttachmentUp''
// SYNTHETIC: WEBSERVICE 0x10223ec0
// RBX::`dynamic atexit destructor for 'prop_AttachmentRight''
// SYNTHETIC: WEBSERVICE 0x10223ee0
// RBX::`dynamic atexit destructor for 'prop_AttachmentForward''
// SYNTHETIC: WEBSERVICE 0x10223f00
// RBX::`dynamic atexit destructor for 'prop_AttachmentPos''
// SYNTHETIC: WEBSERVICE 0x10223f20
// RBX::`dynamic atexit destructor for 'prop_AttachmentPoint''
// SYNTHETIC: WEBSERVICE 0x10223f40
// RBX::`dynamic atexit destructor for 'prop_BackendAccoutrementState''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_ACCOUTREMENT_H
