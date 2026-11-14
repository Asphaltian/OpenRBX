#ifndef V8KERNEL_POINT_H
#define V8KERNEL_POINT_H

#include "placeholder.h"
#include "util/IndexArray.h"
#include "v8kernel/KernelIndex.h"

#include <G3D/Vector3.h>

namespace RBX {

class Body;
class Kernel;
class KernelData;

using G3D::Vector3;

class Point : public KernelIndex
{
private:
	friend class Kernel;
	friend class KernelData;

	int& getKernelIndex() { return kernelIndex; }

	int numOwners;

protected:
	Body* body;
	Vector3 localPos;
	Vector3 worldPos;
	Vector3 force;

	Point(Body* _body);

	virtual ~Point() {}

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

} // namespace RBX

#endif // V8KERNEL_POINT_H
