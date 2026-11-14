#include "v8world/IMoving.h"

#include "placeholder.h"

#include <cstddef>

namespace RBX {

IMoving::IMoving() : iMovingManager(NULL), stepsToSleep(0)
{
}

bool IMoving::checkSleep()
{
	if (stepsToSleep > 1) {
		stepsToSleep--;

		return false;
	}

	stepsToSleep = 0;
	onCanAggregateChanged(true);

	return true;
}

void IMoving::makeMoving()
{
	if (iMovingManager != NULL) {
		iMovingManager->moved(this);
	}
}

IMovingManager::~IMovingManager()
{
}

void IMovingManager::remove(IMoving* iMoving)
{
	if (current != moving.end() && *current == iMoving) {
		current = moving.erase(current);
	}
	else {
		moving.erase(iMoving);
	}
}

void IMovingManager::onMovingHeartbeat()
{
	for (current = moving.begin(); current != moving.end();) {
		IMoving* m = *current;

		if (m->checkSleep()) {
			remove(m);
		}
		else {
			++current;
		}
	}
}

void IMoving::setMovingManager(IMovingManager* _iMovingManager)
{
	if (iMovingManager != NULL) {
		iMovingManager->remove(this);
	}

	iMovingManager = _iMovingManager;

	if (_iMovingManager != NULL && stepsToSleep > 0) {
		onCanAggregateChanged(false);
		makeMoving();
	}
}

void IMoving::notifyMoved()
{
	if (stepsToSleep == 0) {
		stepsToSleep = 30;
		onCanAggregateChanged(false);
		makeMoving();
	}
}

IMovingManager::IMovingManager()
{
	current = moving.end();
}

} // namespace RBX
