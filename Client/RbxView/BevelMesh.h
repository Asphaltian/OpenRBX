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

	// SYNTHETIC: WEBSERVICE 0x1017df90
	// ??1BevelMesh@View@RBX@@UAE@XZ
};

DECOMP_SIZE_ASSERT(BevelMesh::Builder, 0x24)

} // namespace View
} // namespace RBX

#endif // RBXVIEW_BEVELMESH_H
