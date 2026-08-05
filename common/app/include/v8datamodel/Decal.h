#ifndef V8DATAMODEL_DECAL_H
#define V8DATAMODEL_DECAL_H

#include "decomp.h"
#include "v8tree/Instance.h"

#include <G3D/Vector2.h>

namespace RBX {

extern char sTexture[];

extern char sDecal[];

using G3D::Vector2;

// SIZE 0x124
class Decal : public Instance
{
public:
	// FUNCTION: WEBSERVICE 0x100ff500
	float getSpecular() const { return specular; }

	void setSpecular(float value);
	// FUNCTION: WEBSERVICE 0x100ff510
	float getShiny() const { return shiny; }

	void setShiny(float value);

private:
	undefined m_unk0x0f8[0x11c - 0x0f8]; // 0x0f8
	float specular;                      // 0x11c
	float shiny;                         // 0x120
};

DECOMP_SIZE_ASSERT(Decal, 0x124)

// SIZE 0x12c
class Texture : public DescribedCreatable<Texture, Decal, sTexture>
{
public:
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

} // namespace RBX

#endif // V8DATAMODEL_DECAL_H
