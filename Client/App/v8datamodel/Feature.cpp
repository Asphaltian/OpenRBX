#include "v8datamodel/Feature.h"

#include "appdraw/Draw.h"
#include "appdraw/DrawAdorn.h"
#include "reflection/property.h"
#include "util/Extents.h"
#include "util/Math.h"
#include "util/Name.h"
#include "util/NormalId.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/Workspace.h"
#include "v8world/Primitive.h"
#include "v8world/World.h"

#include <boost/bind.hpp>

namespace RBX {

const char sVelocityMotor[] = "VelocityMotor";

const char sHole[] = "Hole";

const char sMotorFeature[] = "MotorFeature";

const char sFeature[] = "Feature";

using namespace Reflection;

static RefPropDescriptor<VelocityMotor, Hole> prop_Hole(
	"Hole", "Data", &VelocityMotor::getHole, &VelocityMotor::setHole
);

static PropDescriptor<VelocityMotor, float> prop_MaxVelocity(
	"MaxVelocity", "Data", &VelocityMotor::getMaxVelocity, &VelocityMotor::setMaxVelocity
);

static PropDescriptor<VelocityMotor, float> prop_DesiredAngle(
	"DesiredAngle", "Data", &VelocityMotor::getDesiredAngle, &VelocityMotor::setDesiredAngle
);

static PropDescriptor<VelocityMotor, float> prop_CurrentAngle(
	"CurrentAngle", "Data", &VelocityMotor::getCurrentAngle, &VelocityMotor::setCurrentAngle
);

static EnumPropDescriptor<Feature, NormalId> prop_FaceId("FaceId", "Data", &Feature::getFaceId, &Feature::setFaceId);

static EnumPropDescriptor<Feature, Feature::TopBottom> prop_TopBottom(
	"TopBottom", "Data", &Feature::getTopBottom, &Feature::setTopBottom
);

static EnumPropDescriptor<Feature, Feature::LeftRight> prop_LeftRight(
	"LeftRight", "Data", &Feature::getLeftRight, &Feature::setLeftRight
);

static EnumPropDescriptor<Feature, Feature::InOut> prop_InOut("InOut", "Data", &Feature::getInOut, &Feature::setInOut);

float VelocityMotor::getMaxVelocity() const
{
	return static_cast<MotorJoint*>(joint)->maxVelocity;
}

float VelocityMotor::getDesiredAngle() const
{
	return static_cast<MotorJoint*>(joint)->desiredAngle;
}

float VelocityMotor::getCurrentAngle() const
{
	return static_cast<MotorJoint*>(joint)->getCurrentAngle();
}

bool FUN_00617200(Instance* first, Instance* second)
{
	return (dynamic_cast<Hole*>(first) && dynamic_cast<MotorFeature*>(second)) ||
		   (dynamic_cast<Hole*>(second) && dynamic_cast<MotorFeature*>(first));
}

CoordinateFrame Feature::computeLocalCoordinateFrame() const
{
	PartInstance* part = dynamic_cast<PartInstance*>(getParent());

	if (!part) {
		return CoordinateFrame();
	}

	Extents extents = part->getPrimitive()->getExtentsLocal();
	Vector3 corner = extents.max();

	Vector3 uv = Math::vector3Abs(objectToUvw(corner, faceId));
	uv.z = 0.0f;

	switch (leftRight) {
	case LEFT:
		uv.x = -uv.x;
		break;
	case CENTER_LR:
		uv.x = 0.0f;
		break;
	}

	switch (topBottom) {
	case CENTER_TB:
		uv.y = 0.0f;
		break;
	case BOTTOM:
		uv.y = -uv.y;
		break;
	}

	switch (inOut) {
	case INSET:
		uv.x = Math::sign(uv.x) * std::max(fabs(uv.x) - 1.0f, 0.0f);
		uv.y = Math::sign(uv.y) * std::max(fabs(uv.y) - 1.0f, 0.0f);
		break;
	case CENTER_IO:
		uv.x = 0.0f;
		uv.y = 0.0f;
		break;
	}

	Vector3 uvInObject = uvwToObject(uv, faceId);
	Vector3 ptInObject = extents.faceCenter(faceId) + uvInObject;

	NormalId normalId = getCoordOrientation() == Z_OUT ? faceId : intToNormalId((faceId + 3) % 6);

	return CoordinateFrame(normalIdToMatrix3(normalId), ptInObject);
}

void VelocityMotor::setPart(int i, Feature* feature)
{
	PartInstance* part = feature ? dynamic_cast<PartInstance*>(feature->getParent()) : NULL;
	Primitive* primitive = part ? part->getPrimitive() : NULL;
	CoordinateFrame c = feature ? feature->computeLocalCoordinateFrame() : CoordinateFrame();

	if (motorJoint()->getPrimitive(i) != primitive) {
		motorJoint()->setPrimitive(i, primitive);
		if (primitive) {
			motorJoint()->setJointCoord(i, c);
		}
	}
}

void VelocityMotor::onAncestorChanged(const AncestorChanged& event)
{
	Instance::onAncestorChanged(event);

	setPart(0, dynamic_cast<Feature*>(getParent()));

	World* oldWorld = joint->getWorld();
	World* newWorld = Workspace::getWorldIfInWorkspace(this);

	if (oldWorld != newWorld) {
		if (oldWorld) {
			oldWorld->removeJoint(joint);
			setPart(0, NULL);
			setPart(1, NULL);
		}

		if (newWorld) {
			newWorld->insertJoint(joint);
		}
	}
}

void VelocityMotor::onEvent_HoleAncestorChanged()
{
	setPart(1, hole.get());
}

Feature::~Feature()
{
}

bool Feature::getRenderCoord(CoordinateFrame& c) const
{
	if (PartInstance* part = dynamic_cast<PartInstance*>(getParent())) {
		CoordinateFrame local = computeLocalCoordinateFrame();
		c = part->getCoordinateFrame() * local;
		return true;
	}
	else {
		return false;
	}
}

void Feature::render3dSelect(Adorn* adorn, SelectState selectState)
{
	CoordinateFrame worldCoord;
	if (getRenderCoord(worldCoord)) {
		DrawAdorn::cylinder(adorn, worldCoord, 2, 0.3f, 0.6f, Draw::selectColor());
	}
}

void Hole::render3dAdorn(Adorn* adorn)
{
	CoordinateFrame worldCoord;
	if (getRenderCoord(worldCoord)) {
		DrawAdorn::cylinder(adorn, worldCoord, 2, 0.2f, 0.3f, G3D::Color3::black());
	}
}

void MotorFeature::render3dAdorn(Adorn* adorn)
{
	CoordinateFrame worldCoord;
	if (getRenderCoord(worldCoord)) {
		DrawAdorn::cylinder(adorn, worldCoord, 2, 1.0f, 0.3f, G3D::Color3::yellow());
	}
}

VelocityMotor::~VelocityMotor()
{
}

namespace Reflection {

template <>
EnumDesc<Feature::TopBottom>::EnumDesc() : EnumDescriptor("TopBottom", typeid(Feature::TopBottom))
{
	addPair(Feature::TOP, "Top");
	addPair(Feature::CENTER_TB, "Center");
	addPair(Feature::BOTTOM, "Bottom");
}

template <>
EnumDesc<Feature::LeftRight>::EnumDesc() : EnumDescriptor("LeftRight", typeid(Feature::LeftRight))
{
	addPair(Feature::LEFT, "Left");
	addPair(Feature::CENTER_LR, "Center");
	addPair(Feature::RIGHT, "Right");
}

template <>
EnumDesc<Feature::InOut>::EnumDesc() : EnumDescriptor("InOut", typeid(Feature::InOut))
{
	addPair(Feature::EDGE, "Edge");
	addPair(Feature::INSET, "Inset");
	addPair(Feature::CENTER_IO, "Center");
}

} // namespace Reflection

VelocityMotor::VelocityMotor() : DescribedCreatable<VelocityMotor, JointInstance, sVelocityMotor>(new MotorJoint())
{
	setName("VelocityMotor");
}

Feature::Feature() : faceId(NORM_X), topBottom(CENTER_TB), leftRight(CENTER_LR), inOut(CENTER_IO)
{
	setName("Feature");
}

void Feature::setFaceId(NormalId value)
{
	if (faceId != value) {
		faceId = value;
		raisePropertyChanged(prop_FaceId);
	}
}

void Feature::setTopBottom(TopBottom value)
{
	if (topBottom != value) {
		topBottom = value;
		raisePropertyChanged(prop_TopBottom);
	}
}

void Feature::setLeftRight(LeftRight value)
{
	if (leftRight != value) {
		leftRight = value;
		raisePropertyChanged(prop_LeftRight);
	}
}

void Feature::setInOut(InOut value)
{
	if (inOut != value) {
		inOut = value;
		raisePropertyChanged(prop_InOut);
	}
}

void VelocityMotor::setHole(Hole* value)
{
	if (value != hole.get()) {
		hole = shared_from(value);
		raisePropertyChanged(prop_Hole);
		setPart(1, hole.get());
		if (hole) {
			holeAncestorChanged = Instance::event_ancestryChanged.connect(
				hole.get(), boost::bind(&VelocityMotor::onEvent_HoleAncestorChanged, this), 10
			);
		}
		else {
			holeAncestorChanged.disconnect();
		}
	}
}

void VelocityMotor::setMaxVelocity(float value)
{
	if (value != static_cast<MotorJoint*>(joint)->maxVelocity) {
		static_cast<MotorJoint*>(joint)->maxVelocity = value;
		raisePropertyChanged(prop_MaxVelocity);
	}
}

void VelocityMotor::setDesiredAngle(float value)
{
	if (value != static_cast<MotorJoint*>(joint)->desiredAngle) {
		static_cast<MotorJoint*>(joint)->desiredAngle = value;
		raisePropertyChanged(prop_DesiredAngle);
	}
}

void VelocityMotor::setCurrentAngle(float value)
{
	if (value != static_cast<MotorJoint*>(joint)->getCurrentAngle()) {
		static_cast<MotorJoint*>(joint)->setCurrentAngle(value);
		raisePropertyChanged(prop_CurrentAngle);
	}
}
Hole::Hole()
{
	setName("Hole");
}

MotorFeature::MotorFeature()
{
	setName("MotorFeature");
}

void FUN_0061a840(Instance* first, Instance* second)
{
	if (FUN_00617200(first, second)) {
		Instance* hole = dynamic_cast<Hole*>(first) ? first : second;
		Instance* feature = hole == first ? second : first;

		boost::shared_ptr<VelocityMotor> motor = Creatable<Instance>::create<VelocityMotor>();
		motor->setHole(static_cast<Hole*>(hole));
		motor->setParent(feature);
	}
}

Hole* VelocityMotor::getHole() const
{
	return hole.get();
}

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sFeature>();
template void RBX::Name::callDoDeclare<RBX::sFeature>();

template class RBX::FactoryProduct<RBX::Hole, RBX::Feature, RBX::sHole>;
template class RBX::Reflection::
	Described<RBX::Hole, RBX::sHole, RBX::FactoryProduct<RBX::Hole, RBX::Feature, RBX::sHole> >;
template class RBX::DescribedCreatable<RBX::Hole, RBX::Feature, RBX::sHole>;

template class RBX::FactoryProduct<RBX::MotorFeature, RBX::Feature, RBX::sMotorFeature>;
template class RBX::Reflection::Described<
	RBX::MotorFeature,
	RBX::sMotorFeature,
	RBX::FactoryProduct<RBX::MotorFeature, RBX::Feature, RBX::sMotorFeature> >;
template class RBX::DescribedCreatable<RBX::MotorFeature, RBX::Feature, RBX::sMotorFeature>;

template class RBX::NonFactoryProduct<RBX::Instance, RBX::sFeature>;
template class RBX::Reflection::
	Described<RBX::Feature, RBX::sFeature, RBX::NonFactoryProduct<RBX::Instance, RBX::sFeature> >;
template class RBX::DescribedNonCreatable<RBX::Feature, RBX::Instance, RBX::sFeature>;

template class RBX::FactoryProduct<RBX::VelocityMotor, RBX::JointInstance, RBX::sVelocityMotor>;
template class RBX::Reflection::Described<
	RBX::VelocityMotor,
	RBX::sVelocityMotor,
	RBX::FactoryProduct<RBX::VelocityMotor, RBX::JointInstance, RBX::sVelocityMotor> >;
template class RBX::DescribedCreatable<RBX::VelocityMotor, RBX::JointInstance, RBX::sVelocityMotor>;
