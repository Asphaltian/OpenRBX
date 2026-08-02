#include "v8datamodel/custommesh.h"

#include "reflection/property.h"

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

template class RBX::FactoryProduct<RBX::SpecialShape, RBX::Instance, RBX::sSpecialShape>;
template class RBX::Reflection::Described<
	RBX::SpecialShape,
	RBX::sSpecialShape,
	RBX::FactoryProduct<RBX::SpecialShape, RBX::Instance, RBX::sSpecialShape> >;
template class RBX::DescribedCreatable<RBX::SpecialShape, RBX::Instance, RBX::sSpecialShape>;
