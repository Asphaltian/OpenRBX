#ifndef RBXVIEW_BRICKMESH_H
#define RBXVIEW_BRICKMESH_H

#include "MeshFactory.h"
#include "placeholder.h"
#include "renderlib/Mesh.h"

#include <G3D/ReferenceCount.h>
#include <G3D/Vector3.h>

namespace RBX {

class BrickColor;

namespace View {

class BrickMesh : public Render::Mesh
{
private:
	BrickMesh(const G3D::Vector3& size, RenderSurfaceTypes surfaceTypes, BrickColor color);

public:
	static const float widthInset;
	static const float normalPerturbation;
	static const float bevel;

	static G3D::ReferenceCountedPointer<Render::Mesh> create(
		const G3D::Vector3& size, RenderSurfaceTypes surfaceTypes, BrickColor color
	);
	static void flushCache();
};

} // namespace View
} // namespace RBX

#endif // RBXVIEW_BRICKMESH_H
