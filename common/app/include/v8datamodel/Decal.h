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
	void setSpecular(float value);
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
	void setStudsPerTileU(float value);
	void setStudsPerTileV(float value);

private:
	Vector2 studsPerTile; // 0x124
};

DECOMP_SIZE_ASSERT(Texture, 0x12c)

} // namespace RBX

#endif // V8DATAMODEL_DECAL_H
