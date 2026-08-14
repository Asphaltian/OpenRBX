#ifndef V8WORLD_IMOVING_H
#define V8WORLD_IMOVING_H

#include "decomp.h"

#include <set>

namespace RBX {

class IMoving;

// SIZE 0x18
class __declspec(novtable) IMovingManager
{
public:
	typedef std::set<IMoving*> MovingSet;

	IMovingManager();

	virtual ~IMovingManager(); // vtable+0x00

	void onMovingHeartbeat();

protected:
	void moved(IMoving* m) { moving.insert(m); }

	void remove(IMoving* iMoving);

private:
	MovingSet moving;            // 0x04
	MovingSet::iterator current; // 0x10

	friend class IMoving;
};

DECOMP_SIZE_ASSERT(IMovingManager, 0x18)

// SIZE 0x1
struct CanAggregateChanged
{
	const bool canClump; // 0x00

	CanAggregateChanged(bool canClump) : canClump(canClump) {}
};

DECOMP_SIZE_ASSERT(CanAggregateChanged, 0x1)

// SIZE 0x0c
class __declspec(novtable) IMoving
{
protected:
	virtual void onCanAggregateChanged(bool value) = 0; // vtable+0x00

public:
	IMoving();

	virtual bool reportTouches() const = 0; // vtable+0x04

	void notifyMoved();

protected:
	bool checkSleep();

	void setMovingManager(IMovingManager* value);

private:
	void makeMoving();

	IMovingManager* iMovingManager; // 0x04
	int stepsToSleep;               // 0x08

	friend class IMovingManager;
};

DECOMP_SIZE_ASSERT(IMoving, 0x0c)

} // namespace RBX

#endif // V8WORLD_IMOVING_H
