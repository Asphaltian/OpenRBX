#ifndef RBXVIEW_MESHFACTORY_H
#define RBXVIEW_MESHFACTORY_H

#include "decomp.h"
#include "renderlib/Mesh.h"
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

// SIZE 0x4
class RenderSurfaceTypes
{
public:
	unsigned int data; // 0x00

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

DECOMP_SIZE_ASSERT(RenderSurfaceTypes, 0x4)

template <class T, int N>
class MeshFactory
{
private:
	struct Variations
	{
		G3D::ReferenceCountedPointer<Render::Mesh> meshes[N]; // 0x00

		// clang-format off
		// SYNTHETIC: WEBSERVICE 0x1016a4e0
		// RBX::View::MeshFactory<RBX::View::PBBMesh,4>::Variations::~Variations
		// SYNTHETIC: WEBSERVICE 0x1016c7a0
		// RBX::View::MeshFactory<RBX::View::CylinderAlongXMesh,1>::Variations::~Variations
		// SYNTHETIC: WEBSERVICE 0x1016c7f0
		// SYNTHETIC: ROBLOX 0x00477d80
		// RBX::View::MeshFactory<RBX::View::PBBMesh,4>::Variations::operator=
		// SYNTHETIC: WEBSERVICE 0x1016c890
		// SYNTHETIC: ROBLOX 0x005150d0
		// RBX::View::MeshFactory<RBX::View::CylinderAlongXMesh,1>::Variations::operator=
		// clang-format on
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

	// SIZE 0x18
	class TextureKey : protected DecalKey
	{
	private:
		G3D::Vector2 studsPerTile; // 0x10

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

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x1016b4b0
	// TEMPLATE: ROBLOX 0x004768e0
	// RBX::View::MeshFactory<RBX::View::PBBMesh,4>::flushCache
	// TEMPLATE: WEBSERVICE 0x1016b530
	// TEMPLATE: ROBLOX 0x00476960
	// RBX::View::MeshFactory<RBX::View::SphereMesh,1>::flushCache
	// TEMPLATE: WEBSERVICE 0x1016b5b0
	// TEMPLATE: ROBLOX 0x004769e0
	// RBX::View::MeshFactory<RBX::View::CylinderAlongXMesh,1>::flushCache
	// TEMPLATE: WEBSERVICE 0x101712a0
	// TEMPLATE: ROBLOX 0x0047b850
	// RBX::View::MeshFactory<RBX::View::PBBMesh,4>::create
	// TEMPLATE: WEBSERVICE 0x10171440
	// TEMPLATE: ROBLOX 0x0047b9f0
	// RBX::View::MeshFactory<RBX::View::WedgeMesh,1>::create
	// TEMPLATE: WEBSERVICE 0x101715d0
	// TEMPLATE: ROBLOX 0x0047bb80
	// RBX::View::MeshFactory<RBX::View::TorsoMesh,1>::create
	// TEMPLATE: WEBSERVICE 0x10171760
	// TEMPLATE: ROBLOX 0x0047bd10
	// RBX::View::MeshFactory<RBX::View::SphereMesh,1>::create
	// TEMPLATE: WEBSERVICE 0x101718f0
	// TEMPLATE: ROBLOX 0x0047bea0
	// RBX::View::MeshFactory<RBX::View::CylinderAlongXMesh,1>::create
	// TEMPLATE: WEBSERVICE 0x10171a80
	// TEMPLATE: ROBLOX 0x0047c030
	// RBX::View::MeshFactory<RBX::View::HeadMesh,1>::create
	// TEMPLATE: WEBSERVICE 0x101727e0
	// TEMPLATE: ROBLOX 0x0047cba0
	// RBX::View::MeshFactory<RBX::View::PBBMesh,4>::createDecal
	// TEMPLATE: WEBSERVICE 0x101728f0
	// TEMPLATE: ROBLOX 0x0047ccb0
	// RBX::View::MeshFactory<RBX::View::PBBMesh,4>::createTexture
	// TEMPLATE: WEBSERVICE 0x10172a10
	// TEMPLATE: ROBLOX 0x0047cdd0
	// RBX::View::MeshFactory<RBX::View::WedgeMesh,1>::createDecal
	// TEMPLATE: WEBSERVICE 0x10172b20
	// TEMPLATE: ROBLOX 0x0047cee0
	// RBX::View::MeshFactory<RBX::View::WedgeMesh,1>::createTexture
	// TEMPLATE: WEBSERVICE 0x10172c40
	// TEMPLATE: ROBLOX 0x0047d000
	// RBX::View::MeshFactory<RBX::View::TorsoMesh,1>::createDecal
	// TEMPLATE: WEBSERVICE 0x10172d50
	// TEMPLATE: ROBLOX 0x0047d110
	// RBX::View::MeshFactory<RBX::View::TorsoMesh,1>::createTexture
	// TEMPLATE: WEBSERVICE 0x10172e70
	// TEMPLATE: ROBLOX 0x0047d230
	// RBX::View::MeshFactory<RBX::View::SphereMesh,1>::createDecal
	// TEMPLATE: WEBSERVICE 0x10172f80
	// TEMPLATE: ROBLOX 0x0047d340
	// RBX::View::MeshFactory<RBX::View::SphereMesh,1>::createTexture
	// TEMPLATE: WEBSERVICE 0x101730a0
	// TEMPLATE: ROBLOX 0x0047d460
	// RBX::View::MeshFactory<RBX::View::CylinderAlongXMesh,1>::createDecal
	// TEMPLATE: WEBSERVICE 0x101731b0
	// TEMPLATE: ROBLOX 0x0047d570
	// RBX::View::MeshFactory<RBX::View::CylinderAlongXMesh,1>::createTexture
	// TEMPLATE: WEBSERVICE 0x101732d0
	// TEMPLATE: ROBLOX 0x0047d690
	// RBX::View::MeshFactory<RBX::View::HeadMesh,1>::createDecal
	// TEMPLATE: WEBSERVICE 0x101733e0
	// TEMPLATE: ROBLOX 0x0047d7a0
	// RBX::View::MeshFactory<RBX::View::HeadMesh,1>::createTexture
	// clang-format on
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

// clang-format off
// SYNTHETIC: WEBSERVICE 0x1021ec80
// `dynamic initializer for 'RBX::View::MeshFactory<RBX::View::PBBMesh,4>::cache1''
// SYNTHETIC: WEBSERVICE 0x1021ecc0
// `dynamic initializer for 'RBX::View::MeshFactory<RBX::View::PBBMesh,4>::decalCache''
// SYNTHETIC: WEBSERVICE 0x1021ed00
// `dynamic initializer for 'RBX::View::MeshFactory<RBX::View::SphereMesh,1>::cache1''
// SYNTHETIC: WEBSERVICE 0x1021ed40
// `dynamic initializer for 'RBX::View::MeshFactory<RBX::View::SphereMesh,1>::decalCache''
// SYNTHETIC: WEBSERVICE 0x1021ed80
// `dynamic initializer for 'RBX::View::MeshFactory<RBX::View::CylinderAlongXMesh,1>::cache1''
// SYNTHETIC: WEBSERVICE 0x1021edc0
// `dynamic initializer for 'RBX::View::MeshFactory<RBX::View::CylinderAlongXMesh,1>::decalCache''
// SYNTHETIC: WEBSERVICE 0x1021ee00
// `dynamic initializer for 'RBX::View::MeshFactory<RBX::View::PBBMesh,4>::textureCache''
// SYNTHETIC: WEBSERVICE 0x1021ee40
// `dynamic initializer for 'RBX::View::MeshFactory<RBX::View::WedgeMesh,1>::cache1''
// SYNTHETIC: WEBSERVICE 0x1021ee80
// `dynamic initializer for 'RBX::View::MeshFactory<RBX::View::WedgeMesh,1>::decalCache''
// SYNTHETIC: WEBSERVICE 0x1021eec0
// `dynamic initializer for 'RBX::View::MeshFactory<RBX::View::WedgeMesh,1>::textureCache''
// SYNTHETIC: WEBSERVICE 0x1021ef00
// `dynamic initializer for 'RBX::View::MeshFactory<RBX::View::TorsoMesh,1>::cache1''
// SYNTHETIC: WEBSERVICE 0x1021ef40
// `dynamic initializer for 'RBX::View::MeshFactory<RBX::View::TorsoMesh,1>::decalCache''
// SYNTHETIC: WEBSERVICE 0x1021ef80
// `dynamic initializer for 'RBX::View::MeshFactory<RBX::View::TorsoMesh,1>::textureCache''
// SYNTHETIC: WEBSERVICE 0x1021efc0
// `dynamic initializer for 'RBX::View::MeshFactory<RBX::View::SphereMesh,1>::textureCache''
// SYNTHETIC: WEBSERVICE 0x1021f000
// `dynamic initializer for 'RBX::View::MeshFactory<RBX::View::CylinderAlongXMesh,1>::textureCache''
// SYNTHETIC: WEBSERVICE 0x1021f040
// `dynamic initializer for 'RBX::View::MeshFactory<RBX::View::HeadMesh,1>::cache1''
// SYNTHETIC: WEBSERVICE 0x1021f080
// `dynamic initializer for 'RBX::View::MeshFactory<RBX::View::HeadMesh,1>::decalCache''
// SYNTHETIC: WEBSERVICE 0x1021f0c0
// `dynamic initializer for 'RBX::View::MeshFactory<RBX::View::HeadMesh,1>::textureCache''
// SYNTHETIC: WEBSERVICE 0x10225910
// `dynamic atexit destructor for 'RBX::View::MeshFactory<RBX::View::CylinderAlongXMesh,1>::decalCache''
// SYNTHETIC: WEBSERVICE 0x10225950
// `dynamic atexit destructor for 'RBX::View::MeshFactory<RBX::View::CylinderAlongXMesh,1>::cache1''
// SYNTHETIC: WEBSERVICE 0x10225970
// `dynamic atexit destructor for 'RBX::View::MeshFactory<RBX::View::SphereMesh,1>::decalCache''
// SYNTHETIC: WEBSERVICE 0x102259b0
// `dynamic atexit destructor for 'RBX::View::MeshFactory<RBX::View::SphereMesh,1>::cache1''
// SYNTHETIC: WEBSERVICE 0x102259d0
// `dynamic atexit destructor for 'RBX::View::MeshFactory<RBX::View::PBBMesh,4>::decalCache''
// SYNTHETIC: WEBSERVICE 0x10225a10
// `dynamic atexit destructor for 'RBX::View::MeshFactory<RBX::View::PBBMesh,4>::cache1''
// SYNTHETIC: WEBSERVICE 0x10225a30
// `dynamic atexit destructor for 'RBX::View::MeshFactory<RBX::View::HeadMesh,1>::cache1''
// SYNTHETIC: WEBSERVICE 0x10225a50
// `dynamic atexit destructor for 'RBX::View::MeshFactory<RBX::View::TorsoMesh,1>::cache1''
// SYNTHETIC: WEBSERVICE 0x10225a70
// `dynamic atexit destructor for 'RBX::View::MeshFactory<RBX::View::WedgeMesh,1>::cache1''
// SYNTHETIC: WEBSERVICE 0x10225a90
// `dynamic atexit destructor for 'RBX::View::MeshFactory<RBX::View::HeadMesh,1>::textureCache''
// SYNTHETIC: WEBSERVICE 0x10225ad0
// `dynamic atexit destructor for 'RBX::View::MeshFactory<RBX::View::HeadMesh,1>::decalCache''
// SYNTHETIC: WEBSERVICE 0x10225b10
// `dynamic atexit destructor for 'RBX::View::MeshFactory<RBX::View::CylinderAlongXMesh,1>::textureCache''
// SYNTHETIC: WEBSERVICE 0x10225b50
// `dynamic atexit destructor for 'RBX::View::MeshFactory<RBX::View::SphereMesh,1>::textureCache''
// SYNTHETIC: WEBSERVICE 0x10225b90
// `dynamic atexit destructor for 'RBX::View::MeshFactory<RBX::View::TorsoMesh,1>::textureCache''
// SYNTHETIC: WEBSERVICE 0x10225bd0
// `dynamic atexit destructor for 'RBX::View::MeshFactory<RBX::View::TorsoMesh,1>::decalCache''
// SYNTHETIC: WEBSERVICE 0x10225c10
// `dynamic atexit destructor for 'RBX::View::MeshFactory<RBX::View::WedgeMesh,1>::textureCache''
// SYNTHETIC: WEBSERVICE 0x10225c50
// `dynamic atexit destructor for 'RBX::View::MeshFactory<RBX::View::WedgeMesh,1>::decalCache''
// SYNTHETIC: WEBSERVICE 0x10225c90
// `dynamic atexit destructor for 'RBX::View::MeshFactory<RBX::View::PBBMesh,4>::textureCache''
// clang-format on

} // namespace View
} // namespace RBX

inline unsigned int hashCode(const RBX::View::RenderSurfaceTypes& surfaceTypes)
{
	return surfaceTypes.hashCode();
}

#endif // RBXVIEW_MESHFACTORY_H
