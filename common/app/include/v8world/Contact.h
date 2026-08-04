#ifndef V8WORLD_CONTACT_H
#define V8WORLD_CONTACT_H

#include "decomp.h"
#include "v8world/Primitive.h"

namespace RBX {

// SIZE 0x34
class Contact : public Edge
{
public:
	int& steppingIndexFunc() { return steppingIndex; }

	bool step(int uiStepId);

protected:
	virtual void deleteAllConnectors() = 0; // vtable+0x14
	virtual bool stepContact() = 0;         // vtable+0x18

public:

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
