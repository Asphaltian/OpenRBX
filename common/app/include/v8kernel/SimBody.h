#ifndef V8KERNEL_SIMBODY_H
#define V8KERNEL_SIMBODY_H

#include "decomp.h"
#include "util/PV.h"
#include "util/Quaternion.h"

#include <G3D/Vector3.h>

namespace RBX {

class Body;

using G3D::Vector3;

// SIZE 0x98
class SimBody
{
public:
	void makeDirty() { dirty = true; }

	bool getDirty() const { return dirty; }

private:
	Body* body;              // 0x00
	bool dirty;              // 0x04
	PV pv;                   // 0x08
	Quaternion qOrientation; // 0x50
	Vector3 angMomentum;     // 0x60
	Vector3 momentRecip;     // 0x6c
	float massRecip;         // 0x78
	float constantForceY;    // 0x7c
	Vector3 force;           // 0x80
	Vector3 torque;          // 0x8c
};

DECOMP_SIZE_ASSERT(SimBody, 0x98)

} // namespace RBX

#endif // V8KERNEL_SIMBODY_H
