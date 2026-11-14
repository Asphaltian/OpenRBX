#ifndef RENDERLIBNEW_BRICKMESH_H
#define RENDERLIBNEW_BRICKMESH_H

#include "MeshFactory.h"
#include "renderlibnew/Mesh.h"
#include "placeholder.h"

#include <G3D/ReferenceCount.h>
#include <G3D/Vector3.h>
#include <vector>

namespace RBX {

class BrickColor;

namespace ViewNew {

// Variations::~Variations

class BrickMesh : public RenderNew::Mesh
{
private:
	BrickMesh(const G3D::Vector3& size, RenderSurfaceTypes surfaceTypes, BrickColor color);

	void FUN_00514070(const G3D::Vector3& size, RenderSurfaceTypes surfaceTypes, BrickColor color);

public:
	static void FUN_00514960(
		const G3D::Vector3& halfSize, std::vector<G3D::Vector3>& vertices, std::vector<unsigned short>& indices
	);

	static const float widthInset;
	static const float normalPerturbation;
	static const float bevel;
	static float g_unk0x00958544;

	static G3D::ReferenceCountedPointer<RenderNew::Mesh> create(
		const G3D::Vector3& size, RenderSurfaceTypes surfaceTypes, BrickColor color
	);
	static void flushCache();
};

extern bool g_unk0x0092a908;

} // namespace ViewNew
} // namespace RBX

#endif // RENDERLIBNEW_BRICKMESH_H
