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
#include <vector>

namespace RBX {
namespace RenderNew {

class AggregateChunk;

// SIZE 0x28
class Chunk : public G3D::ReferenceCountedObject
{
public:
	static const float DECAL_OFFSET;
	static const float TEXTURE_OFFSET;

	undefined4* m_unk0x0c; // 0x0c
	undefined4* m_unk0x10; // 0x10
	float m_unk0x14;       // 0x14
	float polygonOffset;   // 0x18
	float radius;          // 0x1c

	Chunk(float polygonOffset);

	virtual ~Chunk();

	virtual bool cachesShadows() const { return false; }

	virtual void renderShadows(G3D::RenderDevice* rd, const G3D::GLight& source, bool caps, float shadowVertexDistance)
	{
	}
	virtual bool castsShadows() const = 0;
	virtual bool cullable() const = 0;
	virtual G3D::ReferenceCountedPointer<Material> getMaterial() = 0;
	virtual G3D::ReferenceCountedPointer<Mesh> getMesh() = 0;
	virtual const G3D::CoordinateFrame& cframe() = 0;
	virtual void VTable0x20(); // vtable+0x20

	// FUNCTION: ROBLOX 0x00515850
	virtual void VTable0x24(float p_unk0x14) // vtable+0x24
	{
		m_unk0x14 = p_unk0x14;
	}

	virtual float VTable0x28() { return m_unk0x14; } // vtable+0x28

	G3D::WeakReferenceCountedPointer<AggregateChunk> aggregate; // 0x20

	// SYNTHETIC: ROBLOX 0x005369e0
	// RBX::RenderNew::Chunk::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(Chunk, 0x28)

// SIZE 0x74
class AggregateChunk : public Chunk
{
private:
	G3D::ReferenceCountedPointer<Material> material; // 0x28
	G3D::ReferenceCountedPointer<Mesh> mesh;         // 0x2c
	G3D::CoordinateFrame _cframe;                    // 0x30
	bool _castsShadows;                              // 0x60
	bool _cullable;                                  // 0x61

public:
	static bool randomColors;
	static const G3D::CoordinateFrame identity;

	virtual bool cachesShadows() const { return true; }

	// FUNCTION: ROBLOX 0x00536bd0
	virtual bool castsShadows() const { return _castsShadows; }

	// FUNCTION: ROBLOX 0x00536be0
	virtual bool cullable() const { return _cullable; }

	virtual const G3D::CoordinateFrame& cframe() { return _cframe; }

	virtual G3D::ReferenceCountedPointer<Material> getMaterial() { return material; }

	// FUNCTION: ROBLOX 0x00536bf0
	virtual G3D::ReferenceCountedPointer<Mesh> getMesh() { return mesh; }

	typedef std::vector<G3D::ReferenceCountedPointer<Chunk> > Components;

	Components components; // 0x64

	AggregateChunk(const G3D::ReferenceCountedPointer<Chunk>& firstChunk);
	virtual ~AggregateChunk();

	void makeMesh();

	// SYNTHETIC: ROBLOX 0x00536d40
	// RBX::RenderNew::AggregateChunk::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(AggregateChunk, 0x74)

} // namespace RenderNew
} // namespace RBX

#endif // RENDERLIBNEW_CHUNK_H
