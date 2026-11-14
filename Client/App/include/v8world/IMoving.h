#ifndef V8WORLD_IMOVING_H
#define V8WORLD_IMOVING_H

#include "placeholder.h"

#include <set>

namespace RBX {

class IMoving;

class __declspec(novtable) IMovingManager
{
public:
	typedef std::set<IMoving*> MovingSet;

	IMovingManager();

	virtual ~IMovingManager();

	void onMovingHeartbeat();

protected:
	void moved(IMoving* m) { moving.insert(m); }

	void remove(IMoving* iMoving);

private:
	MovingSet moving;
	MovingSet::iterator current;

	friend class IMoving;
};

struct CanAggregateChanged
{
	const bool canClump;

	CanAggregateChanged(const CanAggregateChanged& other) : canClump(other.canClump) {}
	CanAggregateChanged(bool canClump) : canClump(canClump) {}
};

class __declspec(novtable) IMoving
{
protected:
	virtual void onCanAggregateChanged(bool value) = 0;

public:
	IMoving();

	virtual bool reportTouches() const = 0;

	void notifyMoved();

	bool getCanAggregate() const { return stepsToSleep == 0; }

protected:
	bool checkSleep();

	void setMovingManager(IMovingManager* value);

private:
	void makeMoving();

	IMovingManager* iMovingManager;
	int stepsToSleep;

	friend class IMovingManager;
};

} // namespace RBX

#endif // V8WORLD_IMOVING_H
