#ifndef RENDERLIBNEW_BEVELMESH_H
#define RENDERLIBNEW_BEVELMESH_H

#include "QuadVolume.h"
#include "decomp.h"
#include "renderlibnew/Mesh.h"

namespace RBX {
namespace ViewNew {

// SIZE 0x50
class BevelMesh : public RenderNew::Mesh
{
public:
	// SIZE 0x24
	class Builder : public LevelBuilder
	{
	protected:
		const float bevel; // 0x20

		Builder(
			G3D::ReferenceCountedPointer<RenderNew::Mesh::Level>& level,
			G3D::Vector3 size,
			RenderSurfaceTypes surfaceTypes,
			float bevel
		)
			: LevelBuilder(level, size, surfaceTypes), bevel(bevel)
		{
		}

	public:
		virtual void build(Purpose purpose); // vtable+0x00

		void operator()(
			G3D::Vector3& vertex,
			G3D::Vector3& normal,
			G3D::Vector2& tex,
			const G3D::Vector3& radius,
			const G3D::Vector2int16& ivertex
		)
		{
			if (vertex.x > 0) {
				vertex.x -= bevel;
			}
			else {
				vertex.x += bevel;
			}
			if (vertex.y > 0) {
				vertex.y -= bevel;
			}
			else {
				vertex.y += bevel;
			}
		}
	};
};

DECOMP_SIZE_ASSERT(BevelMesh::Builder, 0x24)

} // namespace ViewNew
} // namespace RBX

#endif // RENDERLIBNEW_BEVELMESH_H
