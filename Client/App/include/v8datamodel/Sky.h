#ifndef V8DATAMODEL_SKY_H
#define V8DATAMODEL_SKY_H

#include "reflection/property.h"
#include "placeholder.h"
#include "util/TextureId.h"
#include "v8tree/Instance.h"

#include <GLG3D/Sky.h>

namespace RBX {

extern char sSky[];

class Sky : public DescribedCreatable<Sky, Instance, sSky>
{
public:
	static Reflection::BoundProp<TextureId, 1> prop_SkyUp;
	static Reflection::BoundProp<TextureId, 1> prop_SkyLf;
	static Reflection::BoundProp<TextureId, 1> prop_SkyRt;
	static Reflection::BoundProp<TextureId, 1> prop_SkyBk;
	static Reflection::BoundProp<TextureId, 1> prop_SkyFt;
	static Reflection::BoundProp<TextureId, 1> prop_SkyDn;
	static Reflection::BoundProp<bool, 1> prop_CelestialBodiesShown;
	static Reflection::BoundProp<int, 1> prop_StarCount;

	Sky();

	static G3D::SkyRef getG3DSky(Sky* sky);

	TextureId skyUp;
	TextureId skyLf;
	TextureId skyRt;
	TextureId skyBk;
	TextureId skyFt;
	TextureId skyDn;
	bool drawCelestialBodies;
	int numStars;
};

} // namespace RBX

#endif // V8DATAMODEL_SKY_H
