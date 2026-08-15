#ifndef RBXVIEW_MESHFACTORY_H
#define RBXVIEW_MESHFACTORY_H

#include "decomp.h"
#include "renderlib/Mesh.h"
#include "util/NormalId.h"

#include <G3D/ReferenceCount.h>
#include <G3D/Table.h>
#include <G3D/Vector2.h>
#include <G3D/Vector3.h>
#include <map>

namespace RBX {
namespace View {

// SIZE 0x4
class RenderSurfaceTypes
{
public:
	unsigned int data; // 0x00
};

DECOMP_SIZE_ASSERT(RenderSurfaceTypes, 0x4)

template <class T, int N>
class MeshFactory
{
private:
	struct Variations
	{
		G3D::ReferenceCountedPointer<Render::Mesh> meshes[N]; // 0x00
	};

	typedef G3D::Table<G3D::Vector3, Variations> Cache2;
	typedef G3D::Table<RenderSurfaceTypes, Cache2> Cache1;

	static Cache1 cache1;

	// SIZE 0x10
	class DecalKey
	{
	private:
		G3D::Vector3 size; // 0x00
		NormalId face;     // 0x0c

	public:
		DecalKey(const G3D::Vector3& size, NormalId face) : size(size), face(face) {}

		bool operator<(const DecalKey& other) const
		{
			if (size.x != other.size.x) {
				return size.x < other.size.x;
			}

			if (size.y != other.size.y) {
				return size.y < other.size.y;
			}

			if (size.z != other.size.z) {
				return size.z < other.size.z;
			}

			return face < other.face;
		}
	};

	typedef std::map<DecalKey, G3D::ReferenceCountedPointer<Render::Mesh> > DecalCache;

	static DecalCache decalCache;

	// SIZE 0x18
	class TextureKey : protected DecalKey
	{
	private:
		G3D::Vector2 studsPerTile; // 0x10

	public:
		TextureKey(const G3D::Vector3& size, NormalId face, const G3D::Vector2& studsPerTile)
			: DecalKey(size, face), studsPerTile(studsPerTile)
		{
		}

		bool operator<(const TextureKey& other) const
		{
			if (DecalKey::operator<(other)) {
				return true;
			}

			if (studsPerTile.x != other.studsPerTile.x) {
				return studsPerTile.x < other.studsPerTile.x;
			}

			return studsPerTile.y < other.studsPerTile.y;
		}
	};

	typedef std::map<TextureKey, G3D::ReferenceCountedPointer<Render::Mesh> > TextureCache;

	static TextureCache textureCache;

public:
	static T* create(const G3D::Vector3& size, RenderSurfaceTypes surfaceTypes);

	static T* createDecal(const G3D::Vector3& size, NormalId face);

	static T* createTexture(const G3D::Vector3& size, NormalId face, const G3D::Vector2& studsPerTile);

	static void flushCache();
};

template <class T, int N>
T* MeshFactory<T, N>::createDecal(const G3D::Vector3& size, NormalId face)
{
	DecalKey key(size, face);

	typename DecalCache::iterator iter = decalCache.find(key);

	if (iter == decalCache.end()) {
		T* mesh = new T(size, face);
		decalCache[key] = mesh;
		return mesh;
	}

	return static_cast<T*>(iter->second.pointer());
}

template <class T, int N>
T* MeshFactory<T, N>::createTexture(const G3D::Vector3& size, NormalId face, const G3D::Vector2& studsPerTile)
{
	TextureKey key(size, face, studsPerTile);

	typename TextureCache::iterator iter = textureCache.find(key);

	if (iter == textureCache.end()) {
		T* mesh = new T(size, face, studsPerTile);
		textureCache[key] = mesh;
		return mesh;
	}

	return static_cast<T*>(iter->second.pointer());
}

template <class T, int N>
typename MeshFactory<T, N>::Cache1 MeshFactory<T, N>::cache1;

template <class T, int N>
typename MeshFactory<T, N>::DecalCache MeshFactory<T, N>::decalCache;

template <class T, int N>
typename MeshFactory<T, N>::TextureCache MeshFactory<T, N>::textureCache;

} // namespace View
} // namespace RBX

#endif // RBXVIEW_MESHFACTORY_H
