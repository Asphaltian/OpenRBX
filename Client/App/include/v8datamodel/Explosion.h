#ifndef V8DATAMODEL_EXPLOSION_H
#define V8DATAMODEL_EXPLOSION_H

#include "decomp.h"
#include "reflection/property.h"
#include "util/Handle.h"
#include "util/IRenderable.h"
#include "util/RunStateOwner.h"
#include "v8tree/Instance.h"

#include <G3D/Array.h>
#include <G3D/Vector3.h>

namespace RBX {

class Primitive;

extern char sExplosion[];

// SIZE 0x124
class Explosion : public DescribedCreatable<Explosion, Instance, sExplosion>,
				  public Listener<RunService, Stepped>,
				  public IRenderable
{
private:
	float age;             // 0x10c
	G3D::Vector3 position; // 0x110
	float blastRadius;     // 0x11c
	float blastPressure;   // 0x120

	float renderTime() const { return 0.1f; }
	float visualRadius() const;
	float killRadius() const;
	float blastMaxObjectRadius() const { return 2.0f * blastRadius; }
	float killMaxObjectRadius() const;

	void doKill();
	void doBlast(const G3D::Array<Primitive*>& primitives);
	void signalBlast(const G3D::Array<Primitive*>& primitives);

	virtual void onEvent(const RunService* source, Stepped event);

	virtual bool askSetParent(const Instance* instance) const;

	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);

	// FUNCTION: WEBSERVICE 0x100e6150 FOLDED
	virtual bool shouldRender3dAdorn() const { return true; }

	virtual void render3dAdorn(Adorn* adorn);

public:
	Explosion();

	// FUNCTION: WEBSERVICE 0x100e3050
	// FUNCTION: ROBLOX 0x00626160
	virtual ~Explosion() {}

	static Reflection::BoundProp<G3D::Vector3, 1> propPosition;
	static Reflection::BoundProp<float, 1> propBlastPressure;

	void setVisualOnly();
	void setBlastRadius(float _blastRadius);

	// FUNCTION: WEBSERVICE 0x100ff500 FOLDED
	float getBlastRadius() const { return blastRadius; }
};

DECOMP_SIZE_ASSERT(Explosion, 0x124)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x1021cbd0
// RBX::`dynamic initializer for 'propBlastRadius''
// SYNTHETIC: WEBSERVICE 0x1021cc10
// SYNTHETIC: ROBLOX 0x007e86b0
// `dynamic initializer for 'RBX::Explosion::propBlastPressure''
// SYNTHETIC: WEBSERVICE 0x1021cc40
// `dynamic initializer for 'RBX::Explosion::propPosition''
// SYNTHETIC: WEBSERVICE 0x1021cc70
// RBX::`dynamic initializer for 'signal_Hit''
// SYNTHETIC: WEBSERVICE 0x10224180
// `dynamic atexit destructor for 'RBX::Explosion::propPosition''
// SYNTHETIC: WEBSERVICE 0x102241a0
// SYNTHETIC: ROBLOX 0x007f1db0
// `dynamic atexit destructor for 'RBX::Explosion::propBlastPressure''
// SYNTHETIC: WEBSERVICE 0x102241c0
// RBX::`dynamic atexit destructor for 'propBlastRadius''
// SYNTHETIC: WEBSERVICE 0x102241e0
// RBX::`dynamic atexit destructor for 'signal_Hit''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_EXPLOSION_H
