#ifndef RBXVIEW_MESHFACTORY_H
#define RBXVIEW_MESHFACTORY_H

#include "renderlib/Mesh.h"
#include "placeholder.h"
#include "util/NormalId.h"

#include <G3D/ReferenceCount.h>
#include <G3D/Table.h>
#include <G3D/Vector2.h>
#include <G3D/Vector3.h>
#include <G3D/g3dmath.h>
#include <map>

namespace RBX {
namespace View {

enum RenderSurfaceType
{
	FLAT = 0,
	STUDS = 1,
	INLET = 2,
	WELD = 3,
	GLUE = 4
};

class RenderSurfaceTypes
{
public:
	unsigned int data;

	RenderSurfaceTypes() : data(0) {}

	void setSurfaceType(NormalId face, RenderSurfaceType surfaceType)
	{
		data = (data & (unsigned short) ~(7 << (face * 3))) | (surfaceType << (face * 3));
	}

	RenderSurfaceType getSurfaceType(NormalId face) const { return (RenderSurfaceType) ((data >> (face * 3)) & 7); }

	unsigned int hashCode() const { return data; }

	bool operator==(const RenderSurfaceTypes& other) const { return data == other.data; }
	bool operator<(const RenderSurfaceTypes& other) const { return data < other.data; }
	bool operator>(const RenderSurfaceTypes& other) const { return data > other.data; }
};

template <class T, int N>
class MeshFactory
{
private:
	struct Variations
	{
		G3D::ReferenceCountedPointer<Render::Mesh> meshes[N];
	};

	typedef G3D::Table<G3D::Vector3, Variations> Cache2;
	typedef G3D::Table<RenderSurfaceTypes, Cache2> Cache1;

	static Cache1 cache1;

	class DecalKey
	{
	private:
		G3D::Vector3 size;
		NormalId face;

	public:
		DecalKey(NormalId face, const G3D::Vector3& size) : size(size), face(face) {}

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

	class TextureKey : protected DecalKey
	{
	private:
		G3D::Vector2 studsPerTile;

	public:
		TextureKey(NormalId face, const G3D::Vector3& size, const G3D::Vector2& studsPerTile)
			: DecalKey(face, size), studsPerTile(studsPerTile)
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
T* MeshFactory<T, N>::create(const G3D::Vector3& size, RenderSurfaceTypes surfaceTypes)
{
	if (!cache1.containsKey(surfaceTypes)) {
		cache1.set(surfaceTypes, Cache2());
	}

	Cache2& cache2 = cache1[surfaceTypes];
	if (!cache2.containsKey(size)) {
		cache2.set(size, Variations());
	}

	Variations& variations = cache2.get(size);
	int index = N > 1 ? G3D::iRandom(0, N - 1) : 0;

	G3D::ReferenceCountedPointer<Render::Mesh>& mesh = variations.meshes[index];
	if (mesh.isNull()) {
		T* result = new T(size, surfaceTypes);
		mesh = result;
		return result;
	}

	return static_cast<T*>(mesh.pointer());
}

template <class T, int N>
T* MeshFactory<T, N>::createDecal(const G3D::Vector3& size, NormalId face)
{
	DecalKey key(face, size);

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
	TextureKey key(face, size, studsPerTile);

	typename TextureCache::iterator iter = textureCache.find(key);

	if (iter == textureCache.end()) {
		T* mesh = new T(size, face, studsPerTile);
		textureCache[key] = mesh;
		return mesh;
	}

	return static_cast<T*>(iter->second.pointer());
}

template <class T, int N>
void MeshFactory<T, N>::flushCache()
{
	cache1.clear();
	decalCache.clear();
}

template <class T, int N>
typename MeshFactory<T, N>::Cache1 MeshFactory<T, N>::cache1;

template <class T, int N>
typename MeshFactory<T, N>::DecalCache MeshFactory<T, N>::decalCache;

template <class T, int N>
typename MeshFactory<T, N>::TextureCache MeshFactory<T, N>::textureCache;

} // namespace View
} // namespace RBX

inline unsigned int hashCode(const RBX::View::RenderSurfaceTypes& surfaceTypes)
{
	return surfaceTypes.hashCode();
}

#endif // RBXVIEW_MESHFACTORY_H
