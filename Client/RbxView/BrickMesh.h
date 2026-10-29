#ifndef RBXVIEW_BRICKMESH_H
#define RBXVIEW_BRICKMESH_H

#include "MeshFactory.h"
#include "decomp.h"
#include "renderlib/Mesh.h"

#include <G3D/ReferenceCount.h>
#include <G3D/Vector3.h>

namespace RBX {

class BrickColor;

namespace View {

// SYNTHETIC: WEBSERVICE 0x10175df0
// Variations::~Variations
// STUB: WEBSERVICE 0x10176170
// STUB: ROBLOX 0x00480730
// meshFace<0>
// STUB: WEBSERVICE 0x10176a60
// STUB: ROBLOX 0x00481020
// meshFace<1>
// STUB: WEBSERVICE 0x10177350
// STUB: ROBLOX 0x00481910
// meshFace<2>
// STUB: WEBSERVICE 0x10177c40
// STUB: ROBLOX 0x00482200
// meshFace<3>
// STUB: WEBSERVICE 0x10178530
// STUB: ROBLOX 0x00482af0
// meshFace<4>
// STUB: WEBSERVICE 0x10178e20
// STUB: ROBLOX 0x004833e0
// meshFace<5>

// SIZE 0x50
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

	// SYNTHETIC: WEBSERVICE 0x1018a8d0
	// RBX::View::BrickMesh::`scalar deleting destructor'
};

// SYNTHETIC: WEBSERVICE 0x1021f100
// `dynamic initializer for 'cache''
// SYNTHETIC: WEBSERVICE 0x10225cd0
// `dynamic atexit destructor for 'cache''

DECOMP_SIZE_ASSERT(BrickMesh, 0x50)

} // namespace View
} // namespace RBX

#endif // RBXVIEW_BRICKMESH_H
