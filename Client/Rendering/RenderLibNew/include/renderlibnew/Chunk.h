#ifndef RENDERLIBNEW_CHUNK_H
#define RENDERLIBNEW_CHUNK_H

#include "decomp.h"
#include "renderlib/RenderStats.h"
#include "renderlibnew/Material.h"
#include "renderlibnew/Mesh.h"

#include <G3D/Array.h>
#include <G3D/CoordinateFrame.h>
#include <G3D/GLight.h>
#include <G3D/ReferenceCount.h>
#include <GLG3D/RenderDevice.h>
#include <GLG3D/VAR.h>
#include <vector>

namespace RBX {
namespace RenderNew {

class AggregateChunk;

// SIZE 0x1c
class __declspec(novtable) Chunk : public G3D::ReferenceCountedObject
{
public:
	static const float DECAL_OFFSET;
	static const float TEXTURE_OFFSET;

	float polygonOffset; // 0x0c
	float radius;        // 0x10

	Chunk(float polygonOffset);

	virtual ~Chunk() { RenderStats::chunkCount--; }

	virtual bool cachesShadows() const { return false; }

	virtual void renderShadows(G3D::RenderDevice* rd, const G3D::GLight& source, bool caps, float shadowVertexDistance)
	{
	}
	virtual bool castsShadows() const = 0;
	virtual bool cullable() const = 0;
	virtual G3D::ReferenceCountedPointer<Material> getMaterial() = 0;
	virtual G3D::ReferenceCountedPointer<Mesh> getMesh() = 0;
	virtual const G3D::CoordinateFrame& cframe() = 0;

	G3D::WeakReferenceCountedPointer<AggregateChunk> aggregate; // 0x14
};

DECOMP_SIZE_ASSERT(Chunk, 0x1c)

// SIZE 0xf0
class AggregateChunk : public Chunk
{
private:
	G3D::ReferenceCountedPointer<Material> material; // 0x1c
	G3D::ReferenceCountedPointer<Mesh> mesh;         // 0x20
	G3D::CoordinateFrame _cframe;                    // 0x24
	bool _castsShadows;                              // 0x54
	bool _cullable;                                  // 0x55
	G3D::GLight shadowSource;                        // 0x58
	G3D::VAR shadowVAR;                              // 0xa8
	G3D::Array<unsigned int> shadowIndexArray;       // 0xc8
	G3D::Array<unsigned short> shadowIndexArray16;   // 0xd4

public:
	static bool randomColors;
	static const G3D::CoordinateFrame identity;

	virtual bool cachesShadows() const { return true; }

	virtual bool castsShadows() const { return _castsShadows; }

	virtual bool cullable() const { return _cullable; }

	virtual const G3D::CoordinateFrame& cframe() { return _cframe; }

	virtual G3D::ReferenceCountedPointer<Material> getMaterial() { return material; }

	virtual G3D::ReferenceCountedPointer<Mesh> getMesh() { return mesh; }

	virtual void renderShadows(G3D::RenderDevice* rd, const G3D::GLight& light, bool caps, float shadowVertexDistance);

	typedef std::vector<G3D::ReferenceCountedPointer<Chunk> > Components;

	Components components; // 0xe0

	AggregateChunk(const G3D::ReferenceCountedPointer<Chunk>& firstChunk);
	virtual ~AggregateChunk();

	// clang-format off
	// clang-format on

	void makeMesh();

	// clang-format off
	// clang-format on
};

DECOMP_SIZE_ASSERT(AggregateChunk, 0xf0)

} // namespace RenderNew
} // namespace RBX

#endif // RENDERLIBNEW_CHUNK_H
