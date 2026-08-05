#include "v8datamodel/custommesh.h"

#include "reflection/property.h"

namespace RBX {

char sSpecialShape[] = "SpecialMesh";

static Reflection::EnumPropDescriptor<SpecialShape, SpecialShape::MeshType> desc_meshType(
	"MeshType",
	"Data",
	&SpecialShape::getMeshType,
	&SpecialShape::setMeshType
);

namespace Reflection {

// FUNCTION: WEBSERVICE 0x1012d0c0
template <>
EnumDesc<SpecialShape::MeshType>::EnumDesc() : EnumDescriptor("MeshType", typeid(SpecialShape::MeshType))
{
	addPair(SpecialShape::HEAD_MESH, "Head");
	addPair(SpecialShape::TORSO_MESH, "Torso");
	addPair(SpecialShape::WEDGE_MESH, "Wedge");
	addPair(SpecialShape::BRICK_MESH, "Brick");
	addPair(SpecialShape::SPHERE_MESH, "Sphere");
	addPair(SpecialShape::CYLINDER_MESH, "Cylinder");
	addPair(SpecialShape::FILE_MESH, "FileMesh");
}

} // namespace Reflection

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
