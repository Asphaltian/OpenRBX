#include "v8datamodel/JointInstance.h"

#include "rbxgraphics/Adorn.h"
#include "reflection/property.h"
#include "util/Math.h"
#include "util/Name.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/Workspace.h"
#include "v8world/World.h"

namespace RBX {

const char sAutoJoint[] = "AutoJoint";

const char sJointInstance[] = "JointInstance";

const char sMotor[] = "Motor";

const char sRotateV[] = "RotateV";

const char sRotateP[] = "RotateP";

const char sRotate[] = "Rotate";

const char sGlue[] = "Glue";

const char sWeld[] = "Weld";

const char sSnap[] = "Snap";

} // namespace RBX

using namespace RBX;

static Reflection::RefPropDescriptor<AutoJoint, PartInstance> prop_Part0(
	"Part0", "Data", &RBX::AutoJoint::getPart0, &RBX::AutoJoint::setPart0
);

static Reflection::RefPropDescriptor<AutoJoint, PartInstance> prop_Part1(
	"Part1", "Data", &RBX::AutoJoint::getPart1, &RBX::AutoJoint::setPart1
);

static Reflection::PropDescriptor<AutoJoint, CoordinateFrame> prop_C1(
	"C1", "Data", &RBX::AutoJoint::getC1, &RBX::AutoJoint::setC1
);

static Reflection::PropDescriptor<AutoJoint, CoordinateFrame> prop_C0(
	"C0", "Data", &RBX::AutoJoint::getC0, &RBX::AutoJoint::setC0
);

static Reflection::PropDescriptor<Motor, float> prop_MaxVelocity(
	"MaxVelocity", "Data", &RBX::Motor::getMaxVelocity, &RBX::Motor::setMaxVelocity
);

static Reflection::PropDescriptor<Motor, float> prop_DesiredAngle(
	"DesiredAngle", "Data", &RBX::Motor::getDesiredAngle, &RBX::Motor::setDesiredAngle
);

static Reflection::PropDescriptor<Motor, float> prop_CurrentAngle(
	"CurrentAngle", "Data", &RBX::Motor::getCurrentAngle, &RBX::Motor::setCurrentAngle
);

namespace RBX {

bool JointInstance::shouldRender3dAdorn() const
{
	return PartInstance::showSpanningTree;
}

const CoordinateFrame& AutoJoint::getC0() const
{
	return joint->getJointCoord(0);
}

const CoordinateFrame& AutoJoint::getC1() const
{
	return joint->getJointCoord(1);
}

void JointInstance::render3dAdorn(Adorn* adorn)
{
	if (Joint::isAssemblyJoint(joint)) {
		Primitive* p0 = joint->getPrimitive(0);
		Primitive* p1 = joint->getPrimitive(1);

		Vector3 pt0 = p0 ? p0->getCoordinateFrame().translation : Vector3::zero();
		Vector3 pt1 = p1 ? p1->getCoordinateFrame().translation : Vector3::zero();

		Color3 color = joint->getActive() ? Color3::green() : Color3::red();

		adorn->setObjectToWorldMatrix(CoordinateFrame());

		adorn->lineSegment(G3D::LineSegment::fromTwoPoints(pt0, pt1), color, 20.0f);
	}
}

float Motor::getMaxVelocity() const
{
	return getMotorJoint()->maxVelocity;
}

float Motor::getDesiredAngle() const
{
	return getMotorJoint()->desiredAngle;
}

JointInstance::~JointInstance()
{
	joint->setJointOwner(NULL);
	delete joint;
	joint = NULL;
}

void AutoJoint::setPart(int i, PartInstance* value)
{
	if (part[i].get() == value) {
		return;
	}

	part[i] = shared_from(value);
	joint->setPrimitive(i, value ? value->getPrimitive() : NULL);
}

AutoJoint::~AutoJoint()
{
	setPart(0, NULL);
	setPart(1, NULL);
}

bool AutoJoint::askSetParent(const Instance* instance) const
{
	return instance->getDescriptor() == Instance::classDescriptor() ||
		   instance->getDescriptor() == PartInstance::classDescriptor();
}

JointInstance::JointInstance(Joint* joint) : joint(joint)
{
	joint->setJointOwner(this);
}

World* AutoJoint::computeWorld()
{
	if (getParent() != NULL && getParent()->getDescriptor() == Instance::classDescriptor()) {
		Workspace* workspace = ServiceProvider::find<Workspace>(this);
		return workspace ? workspace->getWorld() : NULL;
	}

	return Workspace::getWorldIfInWorkspace(this);
}

void AutoJoint::onAncestorChanged(const AncestorChanged& event)
{
	Instance::onAncestorChanged(event);

	World* oldWorld = joint->getWorld();
	World* newWorld = computeWorld();

	if (oldWorld != newWorld) {
		if (oldWorld) {
			oldWorld->removeJoint(joint);
		}

		if (newWorld) {
			newWorld->insertJoint(joint);
		}
	}
}

AutoJoint::AutoJoint(Joint* _joint) : DescribedNonCreatable<AutoJoint, JointInstance, sAutoJoint>(_joint)
{
	PartInstance* p0 = const_cast<PartInstance*>(PartInstance::fromPrimitiveConst(joint->getPrimitive(0)));
	PartInstance* p1 = const_cast<PartInstance*>(PartInstance::fromPrimitiveConst(joint->getPrimitive(1)));
	part[0] = shared_from(p0);
	part[1] = shared_from(p1);
}

void AutoJoint::setPart0(PartInstance* value)
{
	setPart(0, value);
	raisePropertyChanged(prop_Part0);
}

void AutoJoint::setPart1(PartInstance* value)
{
	setPart(1, value);
	raisePropertyChanged(prop_Part1);
}

void AutoJoint::setC0(const CoordinateFrame& value)
{
	joint->setJointCoord(0, value);
	raisePropertyChanged(prop_C0);
}

void AutoJoint::setC1(const CoordinateFrame& value)
{
	joint->setJointCoord(1, value);
	raisePropertyChanged(prop_C1);
}

void Motor::setMaxVelocity(float value)
{
	if (value != getMotorJoint()->maxVelocity) {
		getMotorJoint()->maxVelocity = value;
		raisePropertyChanged(prop_MaxVelocity);
	}
}

void Motor::setDesiredAngle(float value)
{
	if (!Math::fuzzyEq(value, getMotorJoint()->desiredAngle, 0.034906585f)) {
		getMotorJoint()->desiredAngle = value;
		raisePropertyChanged(prop_DesiredAngle);
	}
}

void Motor::setCurrentAngle(float value)
{
	if (value != getMotorJoint()->getCurrentAngle()) {
		getMotorJoint()->setCurrentAngle(value);
		raisePropertyChanged(prop_CurrentAngle);
	}
}

Snap::Snap(Joint* joint) : DescribedCreatable<Snap, AutoJoint, sSnap>(joint)
{
	setName("Snap");
}

Snap::Snap() : DescribedCreatable<Snap, AutoJoint, sSnap>(new SnapJoint())
{
	setName("Snap");
}

Weld::Weld(Joint* joint) : DescribedCreatable<Weld, AutoJoint, sWeld>(joint)
{
}

Weld::Weld() : DescribedCreatable<Weld, AutoJoint, sWeld>(new WeldJoint())
{
}

Glue::Glue(Joint* joint) : DescribedCreatable<Glue, AutoJoint, sGlue>(joint)
{
}

Glue::Glue() : DescribedCreatable<Glue, AutoJoint, sGlue>(new GlueJoint())
{
}

Rotate::Rotate(Joint* joint) : DescribedCreatable<Rotate, AutoJoint, sRotate>(joint)
{
}

Rotate::Rotate() : DescribedCreatable<Rotate, AutoJoint, sRotate>(new RotateJoint())
{
}

RotateP::RotateP(Joint* joint) : DescribedCreatable<RotateP, AutoJoint, sRotateP>(joint)
{
}

RotateP::RotateP() : DescribedCreatable<RotateP, AutoJoint, sRotateP>(new RotatePJoint())
{
}

RotateV::RotateV(Joint* joint) : DescribedCreatable<RotateV, AutoJoint, sRotateV>(joint)
{
}

RotateV::RotateV() : DescribedCreatable<RotateV, AutoJoint, sRotateV>(new RotateVJoint())
{
}

Motor::Motor(Joint* joint) : DescribedCreatable<Motor, AutoJoint, sMotor>(joint)
{
}

Motor::Motor() : DescribedCreatable<Motor, AutoJoint, sMotor>(new MotorJoint())
{
	setName("Motor");
}

PartInstance* AutoJoint::getPart1() const
{
	return part[1].get();
}

float Motor::getCurrentAngle() const
{
	return getMotorJoint()->getCurrentAngle();
}

PartInstance* AutoJoint::getPart0() const
{
	return part[0].get();
}

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sJointInstance>();
template void RBX::Name::callDoDeclare<RBX::sJointInstance>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sAutoJoint>();
template void RBX::Name::callDoDeclare<RBX::sAutoJoint>();

template class RBX::FactoryProduct<RBX::Glue, RBX::AutoJoint, RBX::sGlue>;
template class RBX::Reflection::
	Described<RBX::Glue, RBX::sGlue, RBX::FactoryProduct<RBX::Glue, RBX::AutoJoint, RBX::sGlue> >;
template class RBX::DescribedCreatable<RBX::Glue, RBX::AutoJoint, RBX::sGlue>;

template class RBX::FactoryProduct<RBX::Rotate, RBX::AutoJoint, RBX::sRotate>;
template class RBX::Reflection::
	Described<RBX::Rotate, RBX::sRotate, RBX::FactoryProduct<RBX::Rotate, RBX::AutoJoint, RBX::sRotate> >;
template class RBX::DescribedCreatable<RBX::Rotate, RBX::AutoJoint, RBX::sRotate>;

template class RBX::FactoryProduct<RBX::RotateP, RBX::AutoJoint, RBX::sRotateP>;
template class RBX::Reflection::
	Described<RBX::RotateP, RBX::sRotateP, RBX::FactoryProduct<RBX::RotateP, RBX::AutoJoint, RBX::sRotateP> >;
template class RBX::DescribedCreatable<RBX::RotateP, RBX::AutoJoint, RBX::sRotateP>;

template class RBX::FactoryProduct<RBX::RotateV, RBX::AutoJoint, RBX::sRotateV>;
template class RBX::Reflection::
	Described<RBX::RotateV, RBX::sRotateV, RBX::FactoryProduct<RBX::RotateV, RBX::AutoJoint, RBX::sRotateV> >;
template class RBX::DescribedCreatable<RBX::RotateV, RBX::AutoJoint, RBX::sRotateV>;

template class RBX::FactoryProduct<RBX::Snap, RBX::AutoJoint, RBX::sSnap>;
template class RBX::Reflection::
	Described<RBX::Snap, RBX::sSnap, RBX::FactoryProduct<RBX::Snap, RBX::AutoJoint, RBX::sSnap> >;
template class RBX::DescribedCreatable<RBX::Snap, RBX::AutoJoint, RBX::sSnap>;

template class RBX::FactoryProduct<RBX::Weld, RBX::AutoJoint, RBX::sWeld>;
template class RBX::Reflection::
	Described<RBX::Weld, RBX::sWeld, RBX::FactoryProduct<RBX::Weld, RBX::AutoJoint, RBX::sWeld> >;
template class RBX::DescribedCreatable<RBX::Weld, RBX::AutoJoint, RBX::sWeld>;
