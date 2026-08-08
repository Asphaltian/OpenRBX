#ifndef V8DATAMODEL_PVINSTANCE_H
#define V8DATAMODEL_PVINSTANCE_H

#include "decomp.h"
#include "util/ComputeProp.h"
#include "util/Extents.h"
#include "util/IControllable.h"
#include "util/ILocation.h"
#include "v8datamodel/IPrimaryPart.h"
#include "v8tree/Instance.h"
#include "v8world/Controller.h"

#include <G3D/CoordinateFrame.h>
#include <G3D/Ray.h>
#include <G3D/ReferenceCount.h>
#include <G3D/Vector3.h>
#include <boost/scoped_ptr.hpp>

namespace RBX {

class Adorn;
class Primitive;
class XmlState;

extern const char sPVInstance[];

// SIZE 0x170
class __declspec(novtable) PVInstance : public Reflection::Described<PVInstance, sPVInstance, Instance>,
										public IControllable,
										public virtual IPrimaryPart,
										public virtual ILocation
{
protected:
	ComputeProp<bool, PVInstance> IsControllable;                                      // 0x108
	ComputeProp<bool, PVInstance> IsTopFlag;                                           // 0x120
	ComputeProp<G3D::ReferenceCountedPointer<Controller>, PVInstance> TopPVController; // 0x138

private:
	bool computeIsControllable() const;
	bool computeIsTopFlag() const;
	G3D::ReferenceCountedPointer<Controller> computeTopPVController() const;
	void dirtyAll();

	bool canSelect;                            // 0x158
	Controller::ControllerType controllerType; // 0x15c
	bool showControllerFlag;                   // 0x160

protected:
	boost::scoped_ptr<CoordinateFrame> legacyOffset; // 0x164

	PVInstance(const char* name);

public:
	virtual ~PVInstance();

	Controller::ControllerType getControllerType() const;
	void setControllerType(Controller::ControllerType _control);

	// FUNCTION: WEBSERVICE 0x100990b0
	bool getShowControllerFlag() const { return showControllerFlag; }

	void setShowControllerFlag(bool value);

	static const Reflection::EnumPropDescriptor<PVInstance, Controller::ControllerType> prop_ControllerType;

protected:
	virtual unsigned int topHashCode() const;   // vtable+0x40
	virtual unsigned int childHashCode() const; // vtable+0x44

	virtual void onChildAdded(Instance* child);
	virtual void onChildRemoving(Instance* child);
	virtual void onDescendentAdded(Instance* instance);
	virtual void onDescendentRemoving(const shared_ptr<Instance>& instance);
	virtual void readProperty(const XmlElement* propertyElement, IReferenceBinder& binder);

public:
	virtual bool isControllable() const;

protected:
	void renderCoordinateFrame(Adorn* adorn);
	void onControllerChanged();

	virtual void onChildControllerChanged();  // vtable+0x48
	virtual void onParentControllerChanged(); // vtable+0x4c
	virtual void onExtentsChanged() const;    // vtable+0x50

public:
	void moveToPoint(G3D::Vector3 point);
	Controller* getTopPVController() const;
	bool isChaseable() const;

	virtual Extents getExtentsWorld() const = 0;                           // vtable+0x54
	virtual Extents getExtentsLocal() const = 0;                           // vtable+0x58
	virtual const Primitive* getBiggestPrimitive() const = 0;              // vtable+0x5c
	virtual bool hitTest(const G3D::Ray& ray, G3D::Vector3& hitPoint) = 0; // vtable+0x60

	void writeCoordinateFrameData(XmlState* state);
	void writeVelocityData(XmlState* state);

	const PVInstance* getTopLevelPVParent() const;
	PVInstance* getTopLevelPVParent();

	bool isTopLevelPVInstance() const;

	void setPVGridOffsetLegacy(const CoordinateFrame& value);
	CoordinateFrame* getLegacyOffset();
	void clearLegacyOffset();

	virtual void legacyTraverseState(const CoordinateFrame& state) = 0; // vtable+0x64
};

DECOMP_SIZE_ASSERT(PVInstance, 0x170)

// clang-format off
// STUB: WEBSERVICE 0x1021a6f0
// RBX::`dynamic initializer for 'prop_ControllerFlagShown''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_PVINSTANCE_H
