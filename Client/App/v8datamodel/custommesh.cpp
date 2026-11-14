#include "v8datamodel/custommesh.h"

#include "reflection/property.h"
#include "v8datamodel/PartInstance.h"

namespace RBX {

char sSpecialShape[] = "SpecialMesh";

static Reflection::EnumPropDescriptor<SpecialShape, SpecialShape::MeshType> desc_meshType(
	"MeshType", "Data", &SpecialShape::getMeshType, &SpecialShape::setMeshType
);

static Reflection::PropDescriptor<SpecialShape, G3D::Vector3> desc_scale(
	"Scale", "Data", &SpecialShape::getScale, &SpecialShape::setScale
);

static Reflection::PropDescriptor<SpecialShape, MeshId> desc_meshId(
	"MeshId", "Data", &SpecialShape::getMeshId, &SpecialShape::setMeshId
);

static Reflection::PropDescriptor<SpecialShape, TextureId> desc_textureId(
	"TextureId", "Data", &SpecialShape::getTextureId, &SpecialShape::setTextureId
);

static Reflection::PropDescriptor<SpecialShape, G3D::Vector3> desc_vertColor(
	"VertexColor", "Data", &SpecialShape::getVertColor, &SpecialShape::setVertColor
);

bool SpecialShape::askSetParent(const Instance* instance) const
{
	return dynamic_cast<const PartInstance*>(instance) != NULL;
}

const float SpecialShape::getAlpha() const
{
	if (getParent()) {
		PartInstance* part = dynamic_cast<PartInstance*>(getParent());
		if (part) {
			return part->getTransparencyUi();
		}
	}

	return 0;
}

namespace Reflection {

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

SpecialShape::SpecialShape() : meshType(HEAD_MESH), scale(1.0f, 1.0f, 1.0f), vertColor(1.0f, 1.0f, 1.0f)
{
	setName("Mesh");
}

void SpecialShape::setMeshType(MeshType value)
{
	if (meshType != value) {
		meshType = value;
		raisePropertyChanged(desc_meshType);
	}
}

void SpecialShape::setScale(const G3D::Vector3& value)
{
	if (scale != value) {
		scale = value;
		raisePropertyChanged(desc_scale);
	}
}

void SpecialShape::setVertColor(const G3D::Vector3& value)
{
	if (vertColor != value) {
		vertColor = value;
		raisePropertyChanged(desc_vertColor);
	}
}

void SpecialShape::setMeshId(const MeshId& value)
{
	if (meshId != value) {
		meshId = value;
		raisePropertyChanged(desc_meshId);
		setMeshType(FILE_MESH);
	}
}

void SpecialShape::setTextureId(const TextureId& value)
{
	if (textureId != value) {
		textureId = value;
		raisePropertyChanged(desc_textureId);
		setMeshType(FILE_MESH);
	}
}
} // namespace RBX

template class RBX::FactoryProduct<RBX::SpecialShape, RBX::Instance, RBX::sSpecialShape>;
template class RBX::Reflection::Described<
	RBX::SpecialShape,
	RBX::sSpecialShape,
	RBX::FactoryProduct<RBX::SpecialShape, RBX::Instance, RBX::sSpecialShape> >;
template class RBX::DescribedCreatable<RBX::SpecialShape, RBX::Instance, RBX::sSpecialShape>;
