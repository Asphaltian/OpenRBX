#ifndef V8WORLD_ANCHOR_H
#define V8WORLD_ANCHOR_H

#include "placeholder.h"

namespace RBX {

class Primitive;

class Anchor
{
public:
	Anchor(Primitive* primitive) : primitive(primitive) {}

	~Anchor() {}

	Primitive* getPrimitive() { return primitive; }

private:
	Primitive* primitive;
};

} // namespace RBX

#endif // V8WORLD_ANCHOR_H
