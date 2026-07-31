#ifndef UTIL_INDEXBOX_H
#define UTIL_INDEXBOX_H

#include <G3D/Vector3.h>

namespace RBX {

using G3D::Vector3;

// VTABLE: WEBSERVICE 0x1022cf1c
class IndexBox
{
public:
	IndexBox(const Vector3& max, const Vector3& min);
	virtual ~IndexBox(); // vtable+0x00

	// SYNTHETIC: WEBSERVICE 0x100314f0
	// RBX::IndexBox::`scalar deleting destructor'

private:
	Vector3 corner[8]; // 0x04
};

} // namespace RBX

#endif // UTIL_INDEXBOX_H
