#ifndef V8WORLD_JOINTBUILDER_H
#define V8WORLD_JOINTBUILDER_H

#include "decomp.h"

namespace RBX {

class Joint;
class Primitive;

// SIZE 0x01
class JointBuilder
{
public:
	static Joint* canJoin(Primitive* prim0, Primitive* prim1);
};

} // namespace RBX

#endif // V8WORLD_JOINTBUILDER_H
