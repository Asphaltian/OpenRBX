#ifndef V8KERNEL_POINT_H
#define V8KERNEL_POINT_H

#include "decomp.h"
#include "util/IndexArray.h"
#include "v8kernel/KernelIndex.h"

#include <G3D/Vector3.h>

namespace RBX {

class Body;
class Kernel;
class KernelData;

using G3D::Vector3;

// VTABLE: WEBSERVICE 0x10247f9c
// SIZE 0x34
class Point : public KernelIndex
{
private:
	friend class Kernel;
	friend class KernelData;

	// FUNCTION: WEBSERVICE 0x100d4ec0 FOLDED
	int& getKernelIndex() const { return kernelIndex; }

	int numOwners; // 0x08

protected:
	Body* body;       // 0x0c
	Vector3 localPos; // 0x10
	Vector3 worldPos; // 0x1c
	Vector3 force;    // 0x28

	Point(Body* _body);

	virtual ~Point() {} // vtable+0x00

	// SYNTHETIC: WEBSERVICE 0x1011e6b0
	// RBX::Point::`scalar deleting destructor'

public:
	static bool sameBodyAndOffset(const Point* p0, const Point* p1)
	{
		return p0->body == p1->body && p0->localPos == p1->localPos;
	}

	void step();
	void accumulateForce(const Vector3& value) { force += value; }
	void forceToBody();
	void setLocalPos(const Vector3& value);
	void setWorldPos(const Vector3& _worldPos);

	void setBody(Body* body);

	Body* getBody() { return body; }

	const Vector3& getWorldPos() { return worldPos; }
};

DECOMP_SIZE_ASSERT(Point, 0x34)

} // namespace RBX

#endif // V8KERNEL_POINT_H
