#ifndef TOOL_DRAGGER_H
#define TOOL_DRAGGER_H

#include "v8world/Primitive.h"

#include <G3D/Array.h>
#include <G3D/Vector3.h>

namespace RBX {

class ContactManager;

class Dragger
{
public:
	static G3D::Vector3 safeMoveNoDrop(const G3D::Array<Primitive*>&, const G3D::Vector3&, ContactManager&);
};

} // namespace RBX

#endif // TOOL_DRAGGER_H
