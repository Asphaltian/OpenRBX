#include "v8datamodel/Feature.h"

#include "reflection/Property.h"

namespace RBX {

using namespace Reflection;

static PropDescriptor<Feature, NormalId> prop_FaceId;
static PropDescriptor<Feature, Feature::TopBottom> prop_TopBottom;
static PropDescriptor<Feature, Feature::LeftRight> prop_LeftRight;
static PropDescriptor<Feature, Feature::InOut> prop_InOut;

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

} // namespace RBX
