#ifndef V8KERNEL_COFM_H
#define V8KERNEL_COFM_H

#include "placeholder.h"

#include <G3D/Matrix3.h>
#include <G3D/Vector3.h>

namespace RBX {

class Body;

using G3D::Matrix3;
using G3D::Vector3;

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

	Body* body;
	mutable bool dirty;
	mutable Vector3 cofmInBody;
	mutable float mass;
	mutable Matrix3 moment;
};

} // namespace RBX

#endif // V8KERNEL_COFM_H
