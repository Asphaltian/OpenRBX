#include "Part.h"

#include "v8datamodel/PartInstance.h"

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

// FUNCTION: WEBSERVICE 0x1016c740
const G3D::CoordinateFrame& PartChunk::cframe()
{
	coordinateFrame = partInstance->getCoordinateFrame();

	return coordinateFrame;
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

// FUNCTION: WEBSERVICE 0x1016d5d0
G3D::ReferenceCountedPointer<Render::Mesh> PartChunk::getMesh()
{
	if (mesh.isNull()) {
		updateMesh();
	}

	return mesh;
}

// STUB: WEBSERVICE 0x1016e0e0
void PartChunk::onChildRemoved(boost::shared_ptr<Instance>)
{
	STUB(0x1016e0e0);
}

// FUNCTION: WEBSERVICE 0x1016f210
PartChunk::~PartChunk()
{
}

// STUB: WEBSERVICE 0x10173c90
void PartChunk::onChildAdded(boost::shared_ptr<Instance>)
{
	STUB(0x10173c90);
}

// STUB: WEBSERVICE 0x10174160
PartChunk::PartChunk(float polygonOffset, const boost::shared_ptr<PartInstance>& partInstance, View* view)
	: Render::Chunk(polygonOffset), partInstance(partInstance), materialInvalid(true), view(view), specialShape(NULL)
{
	STUB(0x10174160);
}

} // namespace View
} // namespace RBX
