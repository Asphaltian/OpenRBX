#include "v8datamodel/PVInstance.h"

#include "reflection/property.h"

namespace RBX {

static Reflection::PropDescriptor<PVInstance, bool> prop_ControllerFlagShown(
	"ControllerFlagShown",
	"Appearance",
	&RBX::PVInstance::getShowControllerFlag,
	&RBX::PVInstance::setShowControllerFlag
);

unsigned int PVInstance::topHashCode() const
{
	STUB(0x10059d20);
	return 0;
}

unsigned int PVInstance::childHashCode() const
{
	STUB(0x10059d21);
	return 0;
}

// STUB: WEBSERVICE 0x1005a7f0
bool PVInstance::isControllable() const
{
	STUB(0x1005a7f0);
	return false;
}

// STUB: WEBSERVICE 0x10099100
void PVInstance::readProperty(const XmlElement* element, IReferenceBinder& binder)
{
	STUB(0x10099100);
}

// STUB: WEBSERVICE 0x100991c0
void PVInstance::onChildAdded(Instance* child)
{
	STUB(0x100991c0);
}

void PVInstance::onChildRemoving(Instance* child)
{
	STUB(0x100991c1);
}

// STUB: WEBSERVICE 0x100992c0
void PVInstance::onChildControllerChanged()
{
	STUB(0x100992c0);
}

// STUB: WEBSERVICE 0x10099300
void PVInstance::onDescendentAdded(Instance* instance)
{
	STUB(0x10099300);
}

// STUB: WEBSERVICE 0x10099330
void PVInstance::onDescendentRemoving(const shared_ptr<Instance>& instance)
{
	STUB(0x10099330);
}

// STUB: WEBSERVICE 0x10099350
void PVInstance::onExtentsChanged() const
{
	STUB(0x10099350);
}

// STUB: WEBSERVICE 0x10099450
void PVInstance::onParentControllerChanged()
{
	STUB(0x10099450);
}

// STUB: WEBSERVICE 0x100994d0
bool PVInstance::computeIsControllable() const
{
	STUB(0x100994d0);
	return false;
}

// FUNCTION: WEBSERVICE 0x10099780
Controller* PVInstance::getTopPVController() const
{
	Controller* controller = TopPVController.getValue().getPointer();

	if (controller != NULL) {
		return controller;
	}

	return NullController::getStaticNullController();
}

// STUB: WEBSERVICE 0x10099870
PVInstance::~PVInstance()
{
	STUB(0x10099870);
}

// STUB: WEBSERVICE 0x10099910
bool PVInstance::computeIsTopFlag() const
{
	STUB(0x10099910);
	return false;
}

// STUB: WEBSERVICE 0x10099ea0
G3D::ReferenceCountedPointer<Controller> PVInstance::computeTopPVController() const
{
	STUB(0x10099ea0);
	return G3D::ReferenceCountedPointer<Controller>();
}

// STUB: WEBSERVICE 0x1009a3b0
PVInstance::PVInstance(const char* name)
	: Instance(name), IsControllable(this, &PVInstance::computeIsControllable),
	  IsTopFlag(this, &PVInstance::computeIsTopFlag), TopPVController(this, &PVInstance::computeTopPVController),
	  controllerType(Controller::NO_CONTROLLER), showControllerFlag(true)
{
}

// FUNCTION: WEBSERVICE 0x1009a4e0
void PVInstance::setShowControllerFlag(bool _showControllerFlag)
{
	if (showControllerFlag != _showControllerFlag) {
		showControllerFlag = _showControllerFlag;
		raisePropertyChanged(prop_ControllerFlagShown);
	}
}

} // namespace RBX
