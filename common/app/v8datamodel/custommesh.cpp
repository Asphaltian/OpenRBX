#include "v8datamodel/custommesh.h"

#include "reflection/Property.h"

namespace RBX {

char sSpecialShape[] = "SpecialMesh";

static Reflection::PropDescriptor<SpecialShape, SpecialShape::MeshType> desc_meshType;

// FUNCTION: WEBSERVICE 0x1012dbb0
void SpecialShape::setMeshType(MeshType value)
{
	if (meshType != value) {
		meshType = value;
		raisePropertyChanged(desc_meshType);
	}
}

} // namespace RBX
