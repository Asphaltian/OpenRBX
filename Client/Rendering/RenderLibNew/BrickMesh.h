#ifndef RENDERLIBNEW_BRICKMESH_H
#define RENDERLIBNEW_BRICKMESH_H

#include "MeshFactory.h"
#include "decomp.h"
#include "renderlibnew/Mesh.h"

#include <G3D/ReferenceCount.h>
#include <G3D/Vector3.h>

namespace RBX {

class BrickColor;

namespace ViewNew {

// Variations::~Variations
// meshFace<0>
// meshFace<1>
// meshFace<2>
// meshFace<3>
// meshFace<4>
// meshFace<5>

// SIZE 0x50
class BrickMesh : public RenderNew::Mesh
{
private:
	BrickMesh(const G3D::Vector3& size, RenderSurfaceTypes surfaceTypes, BrickColor color);

public:
	static const float widthInset;
	static const float normalPerturbation;
	static const float bevel;

	static G3D::ReferenceCountedPointer<RenderNew::Mesh> create(
		const G3D::Vector3& size, RenderSurfaceTypes surfaceTypes, BrickColor color
	);
	static void flushCache();
};

DECOMP_SIZE_ASSERT(BrickMesh, 0x50)

} // namespace ViewNew
} // namespace RBX

#endif // RENDERLIBNEW_BRICKMESH_H
