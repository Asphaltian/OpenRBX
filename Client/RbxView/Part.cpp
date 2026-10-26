#include "Part.h"

#include "CylinderMesh.h"
#include "HeadMesh.h"
#include "MaterialFactory.h"
#include "MeshFactory.h"
#include "PBBMesh.h"
#include "SphereMesh.h"
#include "TorsoMesh.h"
#include "View.h"
#include "WedgeMesh.h"
#include "renderlib/TextureProxy.h"
#include "util/ContentProvider.h"
#include "v8datamodel/Decal.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/Workspace.h"
#include "v8datamodel/custommesh.h"

#include <boost/bind.hpp>

namespace RBX {
namespace View {

// FUNCTION: WEBSERVICE 0x100e6150 FOLDED
bool PartChunk::castsShadows() const
{
	return true;
}

// FUNCTION: WEBSERVICE 0x100e6150 FOLDED
bool PartChunk::cullable() const
{
	return true;
}

// FUNCTION: WEBSERVICE 0x1016c240
float primaryComponent(const G3D::Vector3& v)
{
	float nx = fabs(v.x);
	float ny = fabs(v.y);
	float nz = fabs(v.z);

	if (nx > ny) {
		if (nx > nz) {
			return nx;
		}

		return nz;
	}

	if (ny > nz) {
		return ny;
	}

	return nz;
}

// FUNCTION: WEBSERVICE 0x1016c6d0
bool Part::usesMegaTexture() const
{
	if (specialShape == NULL && partInstance->getPartType() == RBX::Part::BLOCK_PART) {
		G3D::Vector3 size = partInstance->getPartSizeXml();

		return primaryComponent(size) < 30.0;
	}

	return false;
}

// FUNCTION: WEBSERVICE 0x1016c740
const G3D::CoordinateFrame& PartChunk::cframe()
{
	coordinateFrame = partInstance->getCoordinateFrame();

	return coordinateFrame;
}

// STUB: WEBSERVICE 0x1016cd00
G3D::ReferenceCountedPointer<Render::Material> Part::getMaterial()
{
	STUB(0x1016cd00);
	return material;
}

// FUNCTION: WEBSERVICE 0x1016d060
void Part::onEvent(const PartInstance* source, CanAggregateChanged event)
{
	view->sceneManager->setSleeping(this, event.canClump);
}

// FUNCTION: WEBSERVICE 0x1016d100
void PartChunk::onAncestorChanged(boost::shared_ptr<Instance> ancestor)
{
	Workspace* workspace = Workspace::findWorkspace(partInstance.get());
	Instance* parent;

	if (workspace == NULL ||
		(workspace != (parent = partInstance->getParent()) && (parent == NULL || !parent->isDescendentOf(workspace)))) {
		view->sceneManager->removeModel(this);
	}
}

// FUNCTION: WEBSERVICE 0x1016d210 FOLDED
void Decal::onDecalAncestorChanged(boost::shared_ptr<Instance> ancestor)
{
	Workspace* workspace = Workspace::findWorkspace(decal.get());
	Instance* parent;

	if (workspace == NULL ||
		(workspace != (parent = decal->getParent()) && (parent == NULL || !parent->isDescendentOf(workspace)))) {
		view->sceneManager->removeModel(this);
	}
}

// FUNCTION: WEBSERVICE 0x1016d210 FOLDED
void Texture::onTextureAncestorChanged(boost::shared_ptr<Instance> ancestor)
{
	Workspace* workspace = Workspace::findWorkspace(texture.get());
	Instance* parent;

	if (workspace == NULL ||
		(workspace != (parent = texture->getParent()) && (parent == NULL || !parent->isDescendentOf(workspace)))) {
		view->sceneManager->removeModel(this);
	}
}

// FUNCTION: WEBSERVICE 0x1016d320
void PartChunk::invalidateMesh()
{
	mesh = NULL;

	view->sceneManager->invalidateModel(this, partInstance->getCanAggregate());
}

// FUNCTION: WEBSERVICE 0x1016d410
void PartChunk::invalidateMaterial()
{
	material = NULL;
	materialInvalid = true;

	view->sceneManager->invalidateModel(this, partInstance->getCanAggregate());
}

// FUNCTION: WEBSERVICE 0x1016d500
void PartChunk::onSpecialShapeChanged()
{
	invalidateMesh();
	invalidateMaterial();
}

// FUNCTION: WEBSERVICE 0x1016d510
void PartChunk::onPropertyChanged(const Reflection::PropertyDescriptor* descriptor)
{
	if (descriptor == &PartInstance::prop_Size) {
		invalidateMesh();
		invalidateMaterial();
	}
	else if (descriptor == &PartInstance::prop_shapeXml) {
		invalidateMesh();
	}
	else if (Surfaces::isSurfaceDescriptor(*descriptor)) {
		invalidateMesh();
	}
}

// FUNCTION: WEBSERVICE 0x1016d550
void Part::onPropertyChanged(const Reflection::PropertyDescriptor* descriptor)
{
	PartChunk::onPropertyChanged(descriptor);

	if (descriptor == &PartInstance::prop_Color) {
		if (usesMegaTexture()) {
			invalidateMesh();
		}

		invalidateMaterial();
	}
	else if (descriptor == &PartInstance::prop_Transparency || descriptor == &PartInstance::prop_Reflectance) {
		invalidateMaterial();
	}
}

// FUNCTION: WEBSERVICE 0x1016d5d0
G3D::ReferenceCountedPointer<Render::Mesh> PartChunk::getMesh()
{
	if (mesh.isNull()) {
		updateMesh();
	}

	return mesh;
}

// FUNCTION: WEBSERVICE 0x1016d610
void Decal::onDecalPropertyChanged(const Reflection::PropertyDescriptor* descriptor)
{
	if (descriptor == &FaceInstance::prop_Face) {
		invalidateMesh();
	}
	else if (descriptor == &RBX::Decal::prop_Texture) {
		invalidateMaterial();
	}
	else if (descriptor == &RBX::Decal::prop_Specular) {
		invalidateMaterial();
	}
	else if (descriptor == &RBX::Decal::prop_Shiny) {
		invalidateMaterial();
	}
}

// FUNCTION: WEBSERVICE 0x1016d640
G3D::ReferenceCountedPointer<Render::Material> Decal::getMaterial()
{
	try {
		if (materialInvalid) {
			std::string textureFile;
			if (ContentProvider::singleton().requestContentFile(decal->getTexture(), textureFile)) {
				material = new Render::Material();
				material->appendLevel(
					new Render::TextureProxy(*view->textureManager, textureFile, false),
					G3D::Color3::WHITE,
					decal->getSpecular(),
					decal->getShiny(),
					0.0f,
					0.0f
				);
				materialInvalid = false;
			}
		}
	}
	catch (...) {
	}

	return material;
}

// FUNCTION: WEBSERVICE 0x1016d810
void Texture::onTexturePropertyChanged(const Reflection::PropertyDescriptor* descriptor)
{
	if (descriptor == &FaceInstance::prop_Face) {
		invalidateMesh();
	}
	else if (descriptor == &RBX::Decal::prop_Texture) {
		invalidateMaterial();
	}
	else if (descriptor == &RBX::Decal::prop_Specular) {
		invalidateMaterial();
	}
	else if (descriptor == &RBX::Decal::prop_Shiny) {
		invalidateMaterial();
	}
	else if (descriptor == &RBX::Texture::prop_StudsPerTileU) {
		invalidateMesh();
	}
	else if (descriptor == &RBX::Texture::prop_StudsPerTileV) {
		invalidateMesh();
	}
}

// FUNCTION: WEBSERVICE 0x1016d850
G3D::ReferenceCountedPointer<Render::Material> Texture::getMaterial()
{
	try {
		if (materialInvalid) {
			std::string textureFile;
			if (ContentProvider::singleton().requestContentFile(texture->getTexture(), textureFile)) {
				material = new Render::Material();
				material->appendLevel(
					new Render::TextureProxy(*view->textureManager, textureFile, true),
					G3D::Color3::WHITE,
					texture->getSpecular(),
					texture->getShiny(),
					0.0f,
					0.0f
				);
				materialInvalid = false;
			}
		}
	}
	catch (...) {
	}

	return material;
}

// FUNCTION: WEBSERVICE 0x1016e0e0
void PartChunk::onChildRemoved(boost::shared_ptr<Instance> child)
{
	if (child.get() == specialShape) {
		shapePropertyChangedConnection.disconnect();
		specialShape = NULL;
		invalidateMesh();
	}
}

// FUNCTION: WEBSERVICE 0x1016f210
PartChunk::~PartChunk()
{
}

// FUNCTION: WEBSERVICE 0x1016f360
Decal::~Decal()
{
}

// FUNCTION: WEBSERVICE 0x1016f400
Texture::~Texture()
{
}

// FUNCTION: WEBSERVICE 0x101703a0
Part::~Part()
{
	if (partInstance) {
		partInstance->Notifier<PartInstance, CanAggregateChanged>::removeListener(this);
	}
}

// STUB: WEBSERVICE 0x101722a0
void Part::updateMesh()
{
	STUB(0x101722a0);
}

// FUNCTION: WEBSERVICE 0x10173790
void Decal::updateMesh()
{
	if (partInstance) {
		G3D::Vector3 size = partInstance->getPartSizeXml();

		if (specialShape) {
			size *= specialShape->getScale();
		}

		if (!specialShape) {
			switch (partInstance->getPartType()) {
			case RBX::Part::BALL_PART:
				mesh = MeshFactory<SphereMesh, 1>::createDecal(size, decal->getFace());
				break;
			case RBX::Part::BLOCK_PART:
				mesh = MeshFactory<PBBMesh, 4>::createDecal(size, decal->getFace());
				break;
			case RBX::Part::CYLINDER_PART:
				mesh = MeshFactory<CylinderAlongXMesh, 1>::createDecal(size, decal->getFace());
				break;
			}
		}
		else {
			switch (specialShape->getMeshType()) {
			case SpecialShape::WEDGE_MESH:
				mesh = MeshFactory<WedgeMesh, 1>::createDecal(size, decal->getFace());
				break;
			case SpecialShape::HEAD_MESH:
				mesh = MeshFactory<HeadMesh, 1>::createDecal(size, decal->getFace());
				break;
			case SpecialShape::TORSO_MESH:
				mesh = MeshFactory<TorsoMesh, 1>::createDecal(size, decal->getFace());
				break;
			case SpecialShape::SPHERE_MESH:
				mesh = MeshFactory<SphereMesh, 1>::createDecal(size, decal->getFace());
				break;
			case SpecialShape::CYLINDER_MESH:
				mesh = MeshFactory<CylinderAlongXMesh, 1>::createDecal(size, decal->getFace());
				break;
			case SpecialShape::BRICK_MESH:
				mesh = MeshFactory<PBBMesh, 4>::createDecal(size, decal->getFace());
				break;
			}
		}

		radius = sqrt(2.0f) * 0.5 * primaryComponent(size);
	}
}

// FUNCTION: WEBSERVICE 0x101739a0
void Texture::updateMesh()
{
	if (partInstance) {
		G3D::Vector3 size = partInstance->getPartSizeXml();

		if (specialShape) {
			size *= specialShape->getScale();
		}

		if (!specialShape) {
			switch (partInstance->getPartType()) {
			case RBX::Part::BALL_PART:
				mesh = MeshFactory<SphereMesh, 1>::createTexture(size, texture->getFace(), texture->getStudsPerTile());
				break;
			case RBX::Part::BLOCK_PART:
				mesh = MeshFactory<PBBMesh, 4>::createTexture(size, texture->getFace(), texture->getStudsPerTile());
				break;
			case RBX::Part::CYLINDER_PART:
				mesh = MeshFactory<CylinderAlongXMesh, 1>::createTexture(
					size,
					texture->getFace(),
					texture->getStudsPerTile()
				);
				break;
			}
		}
		else {
			switch (specialShape->getMeshType()) {
			case SpecialShape::WEDGE_MESH:
				mesh = MeshFactory<WedgeMesh, 1>::createTexture(size, texture->getFace(), texture->getStudsPerTile());
				break;
			case SpecialShape::HEAD_MESH:
				mesh = MeshFactory<HeadMesh, 1>::createTexture(size, texture->getFace(), texture->getStudsPerTile());
				break;
			case SpecialShape::TORSO_MESH:
				mesh = MeshFactory<TorsoMesh, 1>::createTexture(size, texture->getFace(), texture->getStudsPerTile());
				break;
			case SpecialShape::SPHERE_MESH:
				mesh = MeshFactory<SphereMesh, 1>::createTexture(size, texture->getFace(), texture->getStudsPerTile());
				break;
			case SpecialShape::CYLINDER_MESH:
				mesh = MeshFactory<CylinderAlongXMesh, 1>::createTexture(
					size,
					texture->getFace(),
					texture->getStudsPerTile()
				);
				break;
			case SpecialShape::BRICK_MESH:
				mesh = MeshFactory<PBBMesh, 4>::createTexture(size, texture->getFace(), texture->getStudsPerTile());
				break;
			}
		}

		radius = sqrt(2.0f) * 0.5 * primaryComponent(size);
	}
}

// FUNCTION: WEBSERVICE 0x10173c90
void PartChunk::onChildAdded(boost::shared_ptr<Instance> child)
{
	SpecialShape* specialShape = dynamic_cast<SpecialShape*>(child.get());

	if (specialShape != NULL) {
		this->specialShape = specialShape;

		shapePropertyChangedConnection = Instance::event_propertyChanged.connect(
			specialShape,
			boost::bind(&PartChunk::onSpecialShapeChanged, this),
			10
		);

		invalidateMesh();
	}
}

// STUB: WEBSERVICE 0x10174160
PartChunk::PartChunk(float polygonOffset, const boost::shared_ptr<PartInstance>& partInstance, View* view)
	: Render::Chunk(polygonOffset), partInstance(partInstance), materialInvalid(true), view(view), specialShape(NULL)
{
	view->sceneManager->addModel(this);
}

// FUNCTION: WEBSERVICE 0x10174480
Decal::Decal(RBX::Decal& decal, PartInstance& parentPart, View* view)
	: PartChunk(Render::Chunk::DECAL_OFFSET, shared_from(&parentPart), view), decal(shared_from(&decal))
{
	decalAncestorChangedConnection =
		Instance::event_ancestryChanged.connect(&decal, boost::bind(&Decal::onDecalAncestorChanged, this, _1), 10);
	decalPropertyChangedConnection =
		Instance::event_propertyChanged.connect(&decal, boost::bind(&Decal::onDecalPropertyChanged, this, _1), 10);
}

// FUNCTION: WEBSERVICE 0x10174640
Texture::Texture(RBX::Texture& texture, PartInstance& parentPart, View* view)
	: PartChunk(Render::Chunk::TEXTURE_OFFSET, shared_from(&parentPart), view), texture(shared_from(&texture))
{
	textureAncestorChangedConnection = Instance::event_ancestryChanged.connect(
		&texture,
		boost::bind(&Texture::onTextureAncestorChanged, this, _1),
		10
	);
	texturePropertyChangedConnection = Instance::event_propertyChanged.connect(
		&texture,
		boost::bind(&Texture::onTexturePropertyChanged, this, _1),
		10
	);
}

// STUB: WEBSERVICE 0x10174800
Part::Part(const boost::shared_ptr<PartInstance>& partInstance, View* view) : PartChunk(0.0f, partInstance, view)
{
	STUB(0x10174800);
}

} // namespace View
} // namespace RBX
