#ifndef V8WORLD_ANCHOR_H
#define V8WORLD_ANCHOR_H

#include "decomp.h"

namespace RBX {

class Primitive;

// SIZE 0x04
class Anchor
{
public:
	Anchor(Primitive* primitive) : primitive(primitive) {}

	~Anchor() {}

	Primitive* getPrimitive() const { return primitive; }

private:
	Primitive* primitive; // 0x00
};

DECOMP_SIZE_ASSERT(Anchor, 0x04)

} // namespace RBX

#endif // V8WORLD_ANCHOR_H
