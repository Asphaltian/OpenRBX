#ifndef V8DATAMODEL_SKY_H
#define V8DATAMODEL_SKY_H

#include "decomp.h"
#include "reflection/property.h"
#include "util/TextureId.h"
#include "v8tree/Instance.h"

#include <GLG3D/Sky.h>

namespace RBX {

extern char sSky[];

// SIZE 0x1c0
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

	TextureId skyUp;          // 0x0f8
	TextureId skyLf;          // 0x118
	TextureId skyRt;          // 0x138
	TextureId skyBk;          // 0x158
	TextureId skyFt;          // 0x178
	TextureId skyDn;          // 0x198
	bool drawCelestialBodies; // 0x1b8
	int numStars;             // 0x1bc
};

DECOMP_SIZE_ASSERT(Sky, 0x1c0)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x100956c0
// RBX::Sky::~Sky
// FUNCTION: WEBSERVICE 0x10095770
// RBX::Sky::`scalar deleting destructor'
// SYNTHETIC: WEBSERVICE 0x1021a530
// `dynamic initializer for 'RBX::Sky::prop_SkyUp''
// SYNTHETIC: WEBSERVICE 0x1021a560
// `dynamic initializer for 'RBX::Sky::prop_SkyLf''
// SYNTHETIC: WEBSERVICE 0x1021a590
// `dynamic initializer for 'RBX::Sky::prop_SkyRt''
// SYNTHETIC: WEBSERVICE 0x1021a5c0
// `dynamic initializer for 'RBX::Sky::prop_SkyBk''
// SYNTHETIC: WEBSERVICE 0x1021a5f0
// `dynamic initializer for 'RBX::Sky::prop_SkyFt''
// SYNTHETIC: WEBSERVICE 0x1021a620
// `dynamic initializer for 'RBX::Sky::prop_SkyDn''
// SYNTHETIC: WEBSERVICE 0x1021a650
// `dynamic initializer for 'RBX::Sky::prop_StarCount''
// SYNTHETIC: WEBSERVICE 0x1021a680
// `dynamic initializer for 'RBX::Sky::prop_CelestialBodiesShown''
// SYNTHETIC: WEBSERVICE 0x10222ee0
// `dynamic atexit destructor for 'RBX::Sky::prop_SkyRt''
// SYNTHETIC: WEBSERVICE 0x10222f00
// `dynamic atexit destructor for 'RBX::Sky::prop_SkyBk''
// SYNTHETIC: WEBSERVICE 0x10222f20
// `dynamic atexit destructor for 'RBX::Sky::prop_SkyFt''
// SYNTHETIC: WEBSERVICE 0x10222f40
// `dynamic atexit destructor for 'RBX::Sky::prop_SkyDn''
// SYNTHETIC: WEBSERVICE 0x10222f60
// `dynamic atexit destructor for 'RBX::Sky::prop_SkyLf''
// SYNTHETIC: WEBSERVICE 0x10222f80
// `dynamic atexit destructor for 'RBX::Sky::prop_SkyUp''
// SYNTHETIC: WEBSERVICE 0x10222fa0
// `dynamic atexit destructor for 'RBX::Sky::prop_CelestialBodiesShown''
// SYNTHETIC: WEBSERVICE 0x10222fc0
// `dynamic atexit destructor for 'RBX::Sky::prop_StarCount''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_SKY_H
