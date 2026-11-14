#ifndef V8DATAMODEL_DECAL_H
#define V8DATAMODEL_DECAL_H

#include "reflection/property.h"
#include "placeholder.h"
#include "util/TextureId.h"
#include "v8datamodel/FaceInstance.h"
#include "v8tree/Instance.h"

#include <G3D/Vector2.h>

namespace RBX {

extern char sTexture[];

extern char sDecal[];

using G3D::Vector2;

class Decal : public DescribedCreatable<Decal, FaceInstance, sDecal>
{
public:
	Decal();

	static const Reflection::PropDescriptor<Decal, TextureId> prop_Texture;
	static const Reflection::PropDescriptor<Decal, float> prop_Specular;
	static const Reflection::PropDescriptor<Decal, float> prop_Shiny;

	TextureId getTexture() const { return texture; }

	void setTexture(TextureId value);

	float getSpecular() const { return specular; }

	void setSpecular(float value);
	float getShiny() const { return shiny; }

	void setShiny(float value);

private:
	TextureId texture;
	float specular;
	float shiny;
};

class Texture : public DescribedCreatable<Texture, Decal, sTexture>
{
public:
	Texture();

	const Vector2& getStudsPerTile() { return studsPerTile; }

	static const Reflection::PropDescriptor<Texture, float> prop_StudsPerTileU;

	float getStudsPerTileU() const { return studsPerTile.x; }

	void setStudsPerTileU(float value);

	static const Reflection::PropDescriptor<Texture, float> prop_StudsPerTileV;

	float getStudsPerTileV() const { return studsPerTile.y; }

	void setStudsPerTileV(float value);

private:
	Vector2 studsPerTile;
};

} // namespace RBX

#endif // V8DATAMODEL_DECAL_H
