#include "v8datamodel/FaceInstance.h"

#include "reflection/property.h"

namespace RBX {

static Reflection::EnumPropDescriptor<FaceInstance, NormalId> prop_Face(
	"Face",
	"Data",
	&RBX::FaceInstance::getFace,
	&RBX::FaceInstance::setFace
);

// STUB: WEBSERVICE 0x10121d10
void FaceInstance::setFace(NormalId value)
{
	if (face != value) {
		face = value;
		raisePropertyChanged(prop_Face);
	}
}

} // namespace RBX
