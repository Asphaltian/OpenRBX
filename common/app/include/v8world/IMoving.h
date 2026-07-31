#ifndef V8WORLD_IMOVING_H
#define V8WORLD_IMOVING_H

#include "decomp.h"

namespace RBX {

class IMovingManager;

// SIZE 0x0c
class __declspec(novtable) IMoving
{
protected:
	virtual void onCanAggregateChanged() = 0; // vtable+0x00

public:
	virtual bool reportTouches() const = 0; // vtable+0x04

	void notifyMoved();

private:
	IMovingManager* iMovingManager; // 0x04
	int stepsToSleep;               // 0x08
};

DECOMP_SIZE_ASSERT(IMoving, 0x0c)

} // namespace RBX

#endif // V8WORLD_IMOVING_H
