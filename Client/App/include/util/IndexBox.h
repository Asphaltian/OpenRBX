#ifndef UTIL_INDEXBOX_H
#define UTIL_INDEXBOX_H

#include <G3D/Vector3.h>

namespace RBX {

using G3D::Vector3;

// VTABLE: WEBSERVICE 0x1022cf1c
class IndexBox
{
public:
	IndexBox(const Vector3& min, const Vector3& max);
	virtual ~IndexBox(); // vtable+0x00

	// SYNTHETIC: WEBSERVICE 0x100314f0
	// RBX::IndexBox::`scalar deleting destructor'

	void getFaceCorners(int f, Vector3& v0, Vector3& v1, Vector3& v2, Vector3& v3) const;

	static const int INDEXBOX_FACE_TO_VERTEX[6][4];

private:
	Vector3 corner[8]; // 0x04
};

} // namespace RBX

#endif // UTIL_INDEXBOX_H
