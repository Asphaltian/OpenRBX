#include "v8datamodel/PVInstance.h"

#include "rbxgraphics/Adorn.h"
#include "reflection/property.h"
#include "v8datamodel/UserController.h"
#include "v8datamodel/Workspace.h"
#include "v8xml/XmlElement.h"

namespace RBX {

static const Reflection::PropDescriptor<PVInstance, CoordinateFrame> desc_CoordFrame(
	"CoordinateFrame", "Data", NULL, &PVInstance::setPVGridOffsetLegacy, Reflection::PropertyDescriptor::LEGACY
);

static Reflection::PropDescriptor<PVInstance, bool> prop_ControllerFlagShown(
	"ControllerFlagShown", "Appearance", &PVInstance::getShowControllerFlag, &PVInstance::setShowControllerFlag
);

const char sPVInstance[] = "PVInstance";

const Reflection::EnumPropDescriptor<PVInstance, Controller::ControllerType> PVInstance::prop_ControllerType(
	"Controller", "Behavior", &PVInstance::getControllerType, &PVInstance::setControllerType
);

unsigned int PVInstance::topHashCode() const
{
	return 0;
}

unsigned int PVInstance::childHashCode() const
{
	return 0;
}

bool PVInstance::isControllable() const
{
	return IsControllable;
}

void PVInstance::moveToPoint(G3D::Vector3 point)
{
	if (Workspace* workspace = Workspace::findWorkspace(this)) {
		workspace->moveToPoint(this, point);
	}
}

void PVInstance::readProperty(const XmlElement* propertyElement, IReferenceBinder& binder)
{
	if (propertyElement->getTag().name.compare("Feature") == 0) {
		const RBX::Name* name = NULL;

		const XmlAttribute* attribute = propertyElement->findAttribute(name_name);

		if (attribute != NULL) {
			attribute->getValue(name);
		}

		if (name->name.compare("Part") == 0) {
			readProperties(propertyElement, binder);
			return;
		}

		if (name->name.compare("Item") == 0) {
			readProperties(propertyElement, binder);
			return;
		}
	}

	Instance::readProperty(propertyElement, binder);
}

void PVInstance::onChildAdded(Instance* instance)
{
	PVInstance* pvInstance = dynamic_cast<PVInstance*>(instance);

	if (pvInstance != NULL) {
		pvInstance->onChildControllerChanged();
		pvInstance->onParentControllerChanged();
	}
}

void PVInstance::onChildRemoving(Instance* instance)
{
	PVInstance* pvInstance = dynamic_cast<PVInstance*>(instance);

	if (pvInstance != NULL) {
		pvInstance->onChildControllerChanged();
		pvInstance->onParentControllerChanged();
	}
}

void PVInstance::renderCoordinateFrame(Adorn* adorn)
{
	adorn->setObjectToWorldMatrix(getLocation());

	adorn->axes(
		G3D::Color4(G3D::Color3::red(), 1.0f),
		G3D::Color4(G3D::Color3::green(), 1.0f),
		G3D::Color4(G3D::Color3::blue(), 1.0f),
		10.0f
	);
}

void PVInstance::onChildControllerChanged()
{
	TopPVController.setDirty();
	IsControllable.setDirty();
	IsTopFlag.setDirty();

	PVInstance* pvInstance = dynamic_cast<PVInstance*>(getParent());

	if (pvInstance != NULL) {
		pvInstance->onChildControllerChanged();
	}
}

void PVInstance::onDescendentAdded(Instance* instance)
{
	Instance::onDescendentAdded(instance);

	TopPVController.setDirty();
	IsControllable.setDirty();
	IsTopFlag.setDirty();
}

void PVInstance::onDescendentRemoving(const shared_ptr<Instance>& instance)
{
	TopPVController.setDirty();
	IsControllable.setDirty();
	IsTopFlag.setDirty();

	Instance::onDescendentRemoving(instance);
}

void PVInstance::onExtentsChanged() const
{
	const PVInstance* pvInstance = dynamic_cast<const PVInstance*>(getParent());

	if (pvInstance != NULL) {
		pvInstance->onExtentsChanged();
	}
}

void PVInstance::onParentControllerChanged()
{
	TopPVController.setDirty();
	IsControllable.setDirty();
	IsTopFlag.setDirty();

	for (unsigned int i = 0; i < numChildren(); ++i) {
		Instance* child = (*getChildren())[i].get();
		PVInstance* pvInstance = dynamic_cast<PVInstance*>(child);

		if (pvInstance != NULL) {
			pvInstance->onParentControllerChanged();
		}
	}
}

bool PVInstance::computeIsControllable() const
{
	for (unsigned int i = 0; i < numChildren(); ++i) {
		Instance* child = (*getChildren())[i].get();
		IControllable* controllable = dynamic_cast<IControllable*>(child);

		if (controllable != NULL && controllable->isControllable()) {
			return true;
		}
	}

	return false;
}

Controller* PVInstance::getTopPVController() const
{
	Controller* controller = TopPVController.getValue().getPointer();

	if (controller != NULL) {
		return controller;
	}

	return NullController::getStaticNullController();
}

PVInstance* PVInstance::getTopLevelPVParent()
{
	if (isTopLevelPVInstance()) {
		return this;
	}

	return static_cast<PVInstance*>(getParent())->getTopLevelPVParent();
}

void PVInstance::clearLegacyOffset()
{
	legacyOffset.reset();

	for (unsigned int i = 0; i < numChildren(); i++) {
		if (PVInstance* pv = dynamic_cast<PVInstance*>(getChild(i))) {
			pv->clearLegacyOffset();
		}
	}
}

PVInstance::~PVInstance()
{
}

bool PVInstance::computeIsTopFlag() const
{
	if (IsControllable && controllerType != Controller::NO_CONTROLLER) {
		if (isTopLevelPVInstance()) {
			return true;
		}

		const PVInstance* parentPV = static_cast<const PVInstance*>(getParent());

		return parentPV->getTopPVController()->getControllerType() == Controller::NO_CONTROLLER;
	}

	return false;
}

void PVInstance::setPVGridOffsetLegacy(const CoordinateFrame& _offset)
{
	if (!legacyOffset) {
		legacyOffset.reset(new CoordinateFrame());
		*legacyOffset = _offset;
	}
}

G3D::ReferenceCountedPointer<Controller> PVInstance::computeTopPVController() const
{
	if (!isTopLevelPVInstance()) {
		G3D::ReferenceCountedPointer<Controller> parentController =
			static_cast<const PVInstance*>(getParent())->TopPVController;

		if (parentController.notNull() && parentController->getControllerType() != Controller::NO_CONTROLLER) {
			if (!parentController->isUserController()) {
				return parentController.getPointer();
			}
		}
	}

	ControllerService* service = ServiceProvider::create<ControllerService>(this);
	return service ? service->getController(getControllerType(), this) : G3D::ReferenceCountedPointer<Controller>();
}

PVInstance::PVInstance(const char* name)
	: Reflection::Described<PVInstance, sPVInstance, Instance>(name),
	  IsControllable(this, &PVInstance::computeIsControllable), IsTopFlag(this, &PVInstance::computeIsTopFlag),
	  TopPVController(this, &PVInstance::computeTopPVController), controllerType(Controller::NO_CONTROLLER),
	  showControllerFlag(true)
{
}

void PVInstance::setControllerType(Controller::ControllerType _control)
{
	if (controllerType != _control) {
		controllerType = _control;

		onChildControllerChanged();
		onParentControllerChanged();

		raisePropertyChanged(prop_ControllerType);
	}
}

void PVInstance::setShowControllerFlag(bool _showControllerFlag)
{
	if (showControllerFlag != _showControllerFlag) {
		showControllerFlag = _showControllerFlag;
		raisePropertyChanged(prop_ControllerFlagShown);
	}
}

} // namespace RBX
