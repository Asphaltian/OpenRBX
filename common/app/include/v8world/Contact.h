#ifndef V8WORLD_CONTACT_H
#define V8WORLD_CONTACT_H

#include "decomp.h"
#include "v8world/Primitive.h"

namespace RBX {

// SIZE 0x34
class Contact : public Edge
{
private:
	static bool ignoreBool;

	int lastContactStep; // 0x20
	int steppingIndex;   // 0x24
	float jointK;        // 0x28
	float elasticJointK; // 0x2c
	float kFriction;     // 0x30
};

DECOMP_SIZE_ASSERT(Contact, 0x34)

} // namespace RBX

#endif // V8WORLD_CONTACT_H
