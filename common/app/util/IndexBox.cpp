#include "util/IndexBox.h"

namespace RBX {

IndexBox::~IndexBox()
{
}

// FUNCTION: WEBSERVICE 0x1008ec50
IndexBox::IndexBox(const Vector3& max, const Vector3& min)
{
	corner[0].x = corner[1].x = corner[2].x = corner[3].x = min.x;
	corner[4].x = corner[5].x = corner[6].x = corner[7].x = max.x;

	corner[0].y = corner[1].y = corner[4].y = corner[5].y = min.y;
	corner[2].y = corner[3].y = corner[6].y = corner[7].y = max.y;

	corner[0].z = corner[2].z = corner[4].z = corner[6].z = min.z;
	corner[1].z = corner[3].z = corner[5].z = corner[7].z = max.z;
}

} // namespace RBX
