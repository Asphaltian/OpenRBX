#include "v8datamodel/FaceInstance.h"

#include "reflection/property.h"
#include "v8datamodel/PartInstance.h"

namespace RBX {

const char sFaceInstance[] = "FaceInstance";

FaceInstance::FaceInstance() : face(NORM_Z_NEG)
{
}

const Reflection::EnumPropDescriptor<FaceInstance, NormalId> FaceInstance::prop_Face(
	"Face", "Data", &FaceInstance::getFace, &FaceInstance::setFace
);

bool FaceInstance::askSetParent(const Instance* instance) const
{
	return dynamic_cast<const PartInstance*>(instance) != NULL;
}

void FaceInstance::setFace(NormalId value)
{
	if (face != value) {
		face = value;
		raisePropertyChanged(prop_Face);
	}
}

} // namespace RBX
