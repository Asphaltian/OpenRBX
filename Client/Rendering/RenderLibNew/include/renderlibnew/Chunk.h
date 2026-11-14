#ifndef RENDERLIBNEW_CHUNK_H
#define RENDERLIBNEW_CHUNK_H

#include "placeholder.h"
#include "renderlib/RenderStats.h"
#include "renderlibnew/Material.h"
#include "renderlibnew/Mesh.h"

#include <G3D/Array.h>
#include <G3D/CoordinateFrame.h>
#include <G3D/GLight.h>
#include <G3D/ReferenceCount.h>
#include <GLG3D/RenderDevice.h>
#include <vector>

namespace Ogre {
class RbxEntity;
class RbxSubEntity;
} // namespace Ogre

namespace RBX {
namespace RenderNew {

class AggregateChunk;

class Chunk : public G3D::ReferenceCountedObject
{
public:
	static const float DECAL_OFFSET;
	static const float TEXTURE_OFFSET;

	Ogre::RbxEntity* m_unk0x0c;
	Ogre::RbxSubEntity* m_unk0x10;
	float m_unk0x14;
	float polygonOffset;
	float radius;

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
	virtual void VTable0x20();

	virtual void VTable0x24(float p_unk0x14) { m_unk0x14 = p_unk0x14; }

	virtual float VTable0x28() { return m_unk0x14; }

	G3D::WeakReferenceCountedPointer<AggregateChunk> aggregate;
};

class AggregateChunk : public Chunk
{
private:
	G3D::ReferenceCountedPointer<Material> material;
	G3D::ReferenceCountedPointer<Mesh> mesh;
	G3D::CoordinateFrame _cframe;
	bool _castsShadows;
	bool _cullable;

public:
	static bool randomColors;
	static const G3D::CoordinateFrame identity;

	virtual bool cachesShadows() const { return true; }

	virtual bool castsShadows() const { return _castsShadows; }

	virtual bool cullable() const { return _cullable; }

	virtual const G3D::CoordinateFrame& cframe() { return _cframe; }

	virtual G3D::ReferenceCountedPointer<Material> getMaterial() { return material; }

	virtual G3D::ReferenceCountedPointer<Mesh> getMesh() { return mesh; }

	typedef std::vector<G3D::ReferenceCountedPointer<Chunk> > Components;

	Components components;

	AggregateChunk(const G3D::ReferenceCountedPointer<Chunk>& firstChunk);
	virtual ~AggregateChunk();

	void makeMesh();
};

} // namespace RenderNew
} // namespace RBX

#endif // RENDERLIBNEW_CHUNK_H
