#include "Part.h"

#include "View.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/custommesh.h"

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

// STUB: WEBSERVICE 0x1016c240
float primaryComponent(const G3D::Vector3& size)
{
	STUB(0x1016c240);
	return 0.0f;
}

// STUB: WEBSERVICE 0x1016c6d0
bool Part::usesMegaTexture() const
{
	STUB(0x1016c6d0);
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

// STUB: WEBSERVICE 0x1016d060
void Part::onEvent(const PartInstance*, CanAggregateChanged)
{
	STUB(0x1016d060);
}

// STUB: WEBSERVICE 0x1016d100
void PartChunk::onAncestorChanged(boost::shared_ptr<Instance>)
{
	STUB(0x1016d100);
}

// STUB: WEBSERVICE 0x1016d320
void PartChunk::invalidateMesh()
{
	mesh = NULL;
}

// STUB: WEBSERVICE 0x1016d410
void PartChunk::invalidateMaterial()
{
	material = NULL;
}

// STUB: WEBSERVICE 0x1016d500
void PartChunk::onSpecialShapeChanged()
{
	invalidateMesh();
	invalidateMaterial();
}

// STUB: WEBSERVICE 0x1016d510
void PartChunk::onPropertyChanged(const Reflection::PropertyDescriptor*)
{
	STUB(0x1016d510);
}

// STUB: WEBSERVICE 0x1016d550
void Part::onPropertyChanged(const Reflection::PropertyDescriptor* descriptor)
{
	STUB(0x1016d550);
}

// FUNCTION: WEBSERVICE 0x1016d5d0
G3D::ReferenceCountedPointer<Render::Mesh> PartChunk::getMesh()
{
	if (mesh.isNull()) {
		updateMesh();
	}

	return mesh;
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

// STUB: WEBSERVICE 0x10173c90
void PartChunk::onChildAdded(boost::shared_ptr<Instance> child)
{
	STUB(0x10173c90);
}

// STUB: WEBSERVICE 0x10174160
PartChunk::PartChunk(float polygonOffset, const boost::shared_ptr<PartInstance>& partInstance, View* view)
	: Render::Chunk(polygonOffset), partInstance(partInstance), materialInvalid(true), view(view), specialShape(NULL)
{
	view->sceneManager->addModel(this);
}

// STUB: WEBSERVICE 0x10174800
Part::Part(const boost::shared_ptr<PartInstance>& partInstance, View* view) : PartChunk(0.0f, partInstance, view)
{
	STUB(0x10174800);
}

} // namespace View
} // namespace RBX
