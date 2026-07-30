#ifndef UTIL_INDEXBOX_H
#define UTIL_INDEXBOX_H

#include <G3D/Vector3.h>

namespace RBX {

using G3D::Vector3;

class IndexBox
{
public:
	IndexBox(const Vector3& max, const Vector3& min);
	virtual ~IndexBox();

private:
	Vector3 corner[8]; // 0x04
};

} // namespace RBX

#endif // UTIL_INDEXBOX_H
