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

	const Vector3& getCofmInBody();
	float getMass();
	const Matrix3& getMoment();

private:
	void updateIfDirty();

	Body* body;         // 0x00
	bool dirty;         // 0x04
	Vector3 cofmInBody; // 0x08
	float mass;         // 0x14
	Matrix3 moment;     // 0x18
};

DECOMP_SIZE_ASSERT(Cofm, 0x3c)

} // namespace RBX

#endif // V8KERNEL_COFM_H
