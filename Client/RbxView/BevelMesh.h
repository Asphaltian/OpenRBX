#ifndef RBXVIEW_BEVELMESH_H
#define RBXVIEW_BEVELMESH_H

#include "QuadVolume.h"
#include "decomp.h"
#include "renderlib/Mesh.h"

namespace RBX {
namespace View {

// SIZE 0x50
class BevelMesh : public Render::Mesh
{
public:
	// SIZE 0x24
	class Builder : public LevelBuilder
	{
	protected:
		const float bevel; // 0x20

		Builder(
			G3D::ReferenceCountedPointer<Render::Mesh::Level>& level,
			G3D::Vector3 size,
			RenderSurfaceTypes surfaceTypes,
			float bevel
		);

	public:
		virtual void build(Purpose purpose); // vtable+0x00
	};
};

DECOMP_SIZE_ASSERT(BevelMesh::Builder, 0x24)

} // namespace View
} // namespace RBX

#endif // RBXVIEW_BEVELMESH_H
