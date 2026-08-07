#ifndef V8DATAMODEL_DECAL_H
#define V8DATAMODEL_DECAL_H

#include "decomp.h"
#include "reflection/property.h"
#include "v8datamodel/FaceInstance.h"
#include "v8tree/Instance.h"

#include <G3D/Vector2.h>

namespace RBX {

extern char sTexture[];

extern char sDecal[];

using G3D::Vector2;

// SIZE 0x124
class Decal : public FaceInstance
{
public:
	static const Reflection::PropDescriptor<Decal, float> prop_Specular;
	static const Reflection::PropDescriptor<Decal, float> prop_Shiny;

	// FUNCTION: WEBSERVICE 0x100ff500
	float getSpecular() const { return specular; }

	void setSpecular(float value);
	// FUNCTION: WEBSERVICE 0x100ff510
	float getShiny() const { return shiny; }

	void setShiny(float value);

private:
	undefined m_unk0x0fc[0x11c - 0x0fc]; // 0x0fc
	float specular;                      // 0x11c
	float shiny;                         // 0x120
};

DECOMP_SIZE_ASSERT(Decal, 0x124)

// SIZE 0x12c
class Texture : public DescribedCreatable<Texture, Decal, sTexture>
{
public:
	static const Reflection::PropDescriptor<Texture, float> prop_StudsPerTileU;
	static const Reflection::PropDescriptor<Texture, float> prop_StudsPerTileV;

	// FUNCTION: WEBSERVICE 0x100ff520
	float getStudsPerTileU() const { return studsPerTile.x; }

	void setStudsPerTileU(float value);
	// FUNCTION: WEBSERVICE 0x1007cde0
	float getStudsPerTileV() const { return studsPerTile.y; }

	void setStudsPerTileV(float value);

private:
	Vector2 studsPerTile; // 0x124
};

DECOMP_SIZE_ASSERT(Texture, 0x12c)

// clang-format off
// STUB: WEBSERVICE 0x100ffa30
// RBX::Decal::~Decal
// clang-format on

// clang-format off
// SYNTHETIC: WEBSERVICE 0x1021dce0
// `dynamic initializer for 'RBX::Decal::prop_Specular''
// SYNTHETIC: WEBSERVICE 0x1021dd20
// `dynamic initializer for 'RBX::Decal::prop_Shiny''
// SYNTHETIC: WEBSERVICE 0x1021dd60
// `dynamic initializer for 'RBX::Texture::prop_StudsPerTileU''
// SYNTHETIC: WEBSERVICE 0x1021dda0
// `dynamic initializer for 'RBX::Texture::prop_StudsPerTileV''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_DECAL_H
