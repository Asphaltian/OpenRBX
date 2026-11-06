#ifndef RENDERLIBNEW_MESH_H
#define RENDERLIBNEW_MESH_H

#include "decomp.h"

#include <G3D/Array.h>
#include <G3D/ReferenceCount.h>
#include <G3D/Vector2.h>
#include <G3D/Vector3.h>
#include <GLG3D/RenderDevice.h>

namespace RBX {
namespace RenderNew {

class Mesh : public G3D::ReferenceCountedObject
{
public:
	typedef unsigned int IndexType;
	typedef G3D::Array<int> VertexList;

	class Level : public G3D::ReferenceCountedObject
	{
	public:
		G3D::Array<IndexType> indexArray;       // 0x0c
		G3D::RenderDevice::Primitive primitive; // 0x18

		Level(G3D::RenderDevice::Primitive primitive) : primitive(primitive) {}

		virtual ~Level();
	};

	typedef G3D::ReferenceCountedPointer<Level> LevelRef;

private:
	static VertexList vertexRefCounts;

public:
	static void freeVertex(unsigned int index);

	static unsigned int allocVertex(unsigned int i, unsigned int count);

	static unsigned int allocVertex(
		const G3D::Vector3& vertex, const G3D::Vector3& normal, const G3D::Vector2& tex, bool share
	);
};

} // namespace RenderNew
} // namespace RBX

#endif // RENDERLIBNEW_MESH_H
