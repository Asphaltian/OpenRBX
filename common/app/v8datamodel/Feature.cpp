#include "v8datamodel/Feature.h"

#include "reflection/property.h"
#include "util/Name.h"

namespace RBX {

const char sVelocityMotor[] = "VelocityMotor";

const char sHole[] = "Hole";

const char sMotorFeature[] = "MotorFeature";

const char sFeature[] = "Feature";

using namespace Reflection;

static PropDescriptor<Feature, NormalId> prop_FaceId;
static PropDescriptor<Feature, Feature::TopBottom> prop_TopBottom;
static PropDescriptor<Feature, Feature::LeftRight> prop_LeftRight;
static PropDescriptor<Feature, Feature::InOut> prop_InOut;
static PropDescriptor<VelocityMotor, float> prop_MaxVelocity;
static PropDescriptor<VelocityMotor, float> prop_DesiredAngle;
static PropDescriptor<VelocityMotor, float> prop_CurrentAngle;

// FUNCTION: WEBSERVICE 0x100e7430
void Feature::setFaceId(NormalId value)
{
	if (faceId != value) {
		faceId = value;
		raisePropertyChanged(prop_FaceId);
	}
}

// FUNCTION: WEBSERVICE 0x100e7460
void Feature::setTopBottom(TopBottom value)
{
	if (topBottom != value) {
		topBottom = value;
		raisePropertyChanged(prop_TopBottom);
	}
}

// FUNCTION: WEBSERVICE 0x100e7490
void Feature::setLeftRight(LeftRight value)
{
	if (leftRight != value) {
		leftRight = value;
		raisePropertyChanged(prop_LeftRight);
	}
}

// FUNCTION: WEBSERVICE 0x100e74c0
void Feature::setInOut(InOut value)
{
	if (inOut != value) {
		inOut = value;
		raisePropertyChanged(prop_InOut);
	}
}

// STUB: WEBSERVICE 0x100e7670
void VelocityMotor::setMaxVelocity(float value)
{
	if (value != joint->getMaxVelocity()) {
		joint->setMaxVelocity(value);
		raisePropertyChanged(prop_MaxVelocity);
	}
}

// STUB: WEBSERVICE 0x100e76b0
void VelocityMotor::setDesiredAngle(float value)
{
	if (value != joint->getDesiredAngle()) {
		joint->setDesiredAngle(value);
		raisePropertyChanged(prop_DesiredAngle);
	}
}

// STUB: WEBSERVICE 0x100e76f0
void VelocityMotor::setCurrentAngle(float value)
{
	if (value != joint->getCurrentAngle()) {
		joint->setCurrentAngle(value);
		raisePropertyChanged(prop_CurrentAngle);
	}
}

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sFeature>();
template void RBX::Name::callDoDeclare<RBX::sFeature>();
