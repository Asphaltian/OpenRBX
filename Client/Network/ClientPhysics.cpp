#include "ClientPhysics.h"

#include "network/NetworkSettings.h"
#include "v8datamodel/PartInstance.h"

#include <G3D/System.h>
#include <math.h>

using G3D::CoordinateFrame;

bool areClose(const CoordinateFrame& c1, const CoordinateFrame& c2)
{
	if (fabs(c1.translation.x - c2.translation.x) > 0.2f) {
		return false;
	}
	if (fabs(c1.translation.y - c2.translation.y) > 0.2f) {
		return false;
	}
	if (fabs(c1.translation.z - c2.translation.z) > 0.2f) {
		return false;
	}

	G3D::Vector3 v1;
	G3D::Vector3 v2;

	for (int i = 0; i < 3; i++) {
		v1.x = c1.rotation[0][i];
		v1.y = c1.rotation[1][i];
		v1.z = c1.rotation[2][i];
		v2.x = c2.rotation[0][i];
		v2.y = c2.rotation[1][i];
		v2.z = c2.rotation[2][i];

		if ((v1 - v2).squaredLength() > 0.1f * 0.1f) {
			return false;
		}
	}

	return true;
}

namespace RBX {
namespace Network {

ClientPhysics::ClientPhysics(PartInstance* part) : part(part)
{
}

ClientPhysics::~ClientPhysics()
{
	stopListening(runService.lock(), this);
}

void ClientPhysics::sleep()
{
	stopListening(runService.lock(), this);

	runService.reset();
	lastPos = part->getCoordinateFrame();
}

void ClientPhysics::onEvent(const RunService* source, Heartbeat event)
{
	if (G3D::System::getLocalTime() > lastUpdateTime + NetworkSettings::singleton().clientPhysicsLifetime) {
		sleep();
	}
}

void ClientPhysics::update(const boost::shared_ptr<RunService>& runService)
{
	lastUpdateTime = G3D::System::getLocalTime();

	const CoordinateFrame& cf = part->getCoordinateFrame();

	if (areClose(lastPos, cf)) {
		sleep();
	}
	else {
		lastPos = cf;

		if (this->runService.lock() != runService) {
			stopListening(this->runService.lock(), this);

			this->runService = runService;

			startListening(this->runService.lock(), this);
		}
	}
}

} // namespace Network
} // namespace RBX
