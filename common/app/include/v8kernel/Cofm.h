#ifndef V8KERNEL_COFM_H
#define V8KERNEL_COFM_H

#include "decomp.h"

#include <G3D/Matrix3.h>
#include <G3D/Vector3.h>

namespace RBX {

class Body;

using G3D::Matrix3;
using G3D::Vector3;

// SIZE 0x3c
class Cofm
{
public:
	Cofm(Body* body);

	bool getIsDirty() const { return dirty; }

	void makeDirty() { dirty = true; }

	const Vector3& getCofmInBody() const;
	const float getMass() const;
	const Matrix3& getMoment() const;

private:
	void updateIfDirty() const;

	Body* body;                 // 0x00
	mutable bool dirty;         // 0x04
	mutable Vector3 cofmInBody; // 0x08
	mutable float mass;         // 0x14
	mutable Matrix3 moment;     // 0x18
};

DECOMP_SIZE_ASSERT(Cofm, 0x3c)

} // namespace RBX

#endif // V8KERNEL_COFM_H
