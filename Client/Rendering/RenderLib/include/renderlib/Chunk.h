#ifndef RENDERLIB_CHUNK_H
#define RENDERLIB_CHUNK_H

#include "decomp.h"
#include "renderlib/Material.h"
#include "renderlib/Mesh.h"

#include <G3D/Array.h>
#include <G3D/CoordinateFrame.h>
#include <G3D/GLight.h>
#include <G3D/ReferenceCount.h>
#include <GLG3D/RenderDevice.h>
#include <GLG3D/VAR.h>
#include <vector>

namespace RBX {
namespace Render {

class AggregateChunk;

// SIZE 0x1c
class Chunk : public G3D::ReferenceCountedObject
{
public:
	static const float DECAL_OFFSET;
	static const float TEXTURE_OFFSET;

	float polygonOffset; // 0x0c
	float radius;        // 0x10

	Chunk() {}

	virtual ~Chunk() {}

	// FUNCTION: WEBSERVICE 0x100eb790 FOLDED
	virtual bool cachesShadows() const { return false; }

	// FUNCTION: WEBSERVICE 0x101aaa90 FOLDED
	virtual void renderShadows(
		G3D::RenderDevice* renderDevice,
		const G3D::GLight& light,
		bool shadowsEnabled,
		float shadowAlpha
	)
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

	// FUNCTION: WEBSERVICE 0x100e6150 FOLDED
	virtual bool cachesShadows() const { return true; }

	// FUNCTION: WEBSERVICE 0x101f9ce0
	virtual bool castsShadows() const { return _castsShadows; }

	// FUNCTION: WEBSERVICE 0x101f9cf0
	virtual bool cullable() const { return _cullable; }

	// FUNCTION: WEBSERVICE 0x101f9d00
	virtual const G3D::CoordinateFrame& cframe() { return _cframe; }

	// FUNCTION: WEBSERVICE 0x101f9d10
	virtual G3D::ReferenceCountedPointer<Material> getMaterial() { return material; }

	// FUNCTION: WEBSERVICE 0x101f9d40
	virtual G3D::ReferenceCountedPointer<Mesh> getMesh() { return mesh; }

	virtual void renderShadows(
		G3D::RenderDevice* renderDevice,
		const G3D::GLight& light,
		bool shadowsEnabled,
		float shadowAlpha
	);

	typedef std::vector<G3D::ReferenceCountedPointer<Chunk> > Components;

	Components components; // 0xe0

	AggregateChunk(const G3D::ReferenceCountedPointer<Chunk>& chunk);
	virtual ~AggregateChunk();

	void makeMesh();
};

DECOMP_SIZE_ASSERT(AggregateChunk, 0xf0)

} // namespace Render
} // namespace RBX

#endif // RENDERLIB_CHUNK_H
