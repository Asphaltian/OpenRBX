#ifndef RBXVIEW_QUADVOLUME_H
#define RBXVIEW_QUADVOLUME_H

#include "MeshFactory.h"
#include "decomp.h"
#include "renderlib/Mesh.h"
#include "util/NormalId.h"

#include <G3D/ReferenceCount.h>
#include <G3D/Vector3.h>

namespace RBX {
namespace View {

// SIZE 0x20
class LevelBuilder
{
protected:
	const G3D::Vector3 halfSize;                             // 0x04
	RenderSurfaceTypes surfaceTypes;                         // 0x10
	G3D::ReferenceCountedPointer<Render::Mesh::Level> level; // 0x14

public:
	float textureScale; // 0x18

	undefined m_unk0x1c[0x20 - 0x1c]; // 0x1c

	enum Purpose
	{
		Surface = 0,
		Decal = 1,
		Shadow = 2
	};

protected:
	LevelBuilder(
		G3D::ReferenceCountedPointer<Render::Mesh::Level>& level,
		G3D::Vector3 size,
		RenderSurfaceTypes surfaceTypes
	);

public:
	virtual void build(Purpose purpose); // vtable+0x00

	void buildFace(NormalId normalId, Purpose purpose);

	virtual void buildTop(Purpose purpose) = 0;    // vtable+0x04
	virtual void buildBottom(Purpose purpose) = 0; // vtable+0x08
	virtual void buildLeft(Purpose purpose) = 0;   // vtable+0x0c
	virtual void buildRight(Purpose purpose) = 0;  // vtable+0x10
	virtual void buildFront(Purpose purpose) = 0;  // vtable+0x14
	virtual void buildBack(Purpose purpose) = 0;   // vtable+0x18
};

DECOMP_SIZE_ASSERT(LevelBuilder, 0x20)

} // namespace View
} // namespace RBX

#endif // RBXVIEW_QUADVOLUME_H
