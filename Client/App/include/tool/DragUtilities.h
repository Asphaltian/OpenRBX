#ifndef TOOL_DRAGUTILITIES_H
#define TOOL_DRAGUTILITIES_H

#include <G3D/Vector3.h>

namespace RBX {

class PartInstance;

class DragUtilities
{
public:
	static void moveAndClean(PartInstance* part, const G3D::Vector3& delta);
};

} // namespace RBX

#endif // TOOL_DRAGUTILITIES_H
