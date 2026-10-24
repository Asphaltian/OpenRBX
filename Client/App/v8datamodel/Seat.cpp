#include "v8datamodel/Seat.h"

#include "humanoid/Humanoid.h"
#include "v8datamodel/JointInstance.h"
#include "v8datamodel/Workspace.h"
#include "v8world/Assembly2.h"

#include <G3D/System.h>
#include <boost/bind.hpp>

namespace RBX {

char sSeat[] = "Seat";

bool Seat::charSeated()
{
	return weld && weld->getPart0() && weld->getPart1() && Workspace::getWorkspaceIfInWorkspace(weld.get());
}

// FUNCTION: WEBSERVICE 0x100fc4e0
void Seat::unseatCharacter()
{
	sleepTime = G3D::System::getLocalTime();

	if (weld) {
		weld->setParent(NULL);
		weld.reset();
	}
}

// FUNCTION: WEBSERVICE 0x100fc550
void Seat::onEvent_humanoidJumped(bool active)
{
	unseatCharacter();
	humanoidJumped.disconnect();
}

// FUNCTION: WEBSERVICE 0x100fc5a0
void Seat::seatCharacter(Humanoid* h)
{
	CoordinateFrame seatCoord;
	seatCoord.lookAt(Vector3(0.0f, -1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f));
	seatCoord.translation = Vector3(0.0f, 3.0f, 0.0f);

	CoordinateFrame torsoCoord;
	torsoCoord.lookAt(Vector3(0.0f, -1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f));
	torsoCoord.translation = Vector3(0.0f, 0.6f, 0.0f);

	CoordinateFrame seatCoordWorld = getCoordinateFrame() * seatCoord;
	CoordinateFrame desiredTorsoCoordWorld = seatCoordWorld * torsoCoord.inverse();

	PartInstance* torso = h->getTorso();

	torso->getCoordinateFrame();

	Primitive* root = torso->getPrimitive()->getAssembly()->getAssemblyPrimitive();

	CoordinateFrame rootInTorso = root->getCoordinateFrame().toObjectSpace(torso->getCoordinateFrame());
	CoordinateFrame desiredRootCoordWorld = desiredTorsoCoordWorld * rootInTorso;
	root->setCoordinateFrame(desiredRootCoordWorld);

	weld = Creatable<Instance>::create<Weld>();

	weld->setName("SeatWeld");
	weld->setPart0(this);
	weld->setPart1(torso);

	weld->setC0(seatCoord);
	weld->setC1(torsoCoord);
	weld->setParent(this);
}

// STUB: WEBSERVICE 0x100fcd50
void Seat::onEvent_seatTouched(boost::shared_ptr<Instance> other)
{
	if (charSeated() || G3D::System::getLocalTime() - sleepTime < 3.0) {
		return;
	}

	Instance* parent = other->getParent();

	if (parent != NULL) {
		Humanoid* humanoid = parent->findFirstChildOfType<Humanoid>();

		if (humanoid != NULL && humanoid->getTorso() != NULL && humanoid->canSit()) {
			humanoid->setSit(true);
			seatCharacter(humanoid);
			humanoidJumped =
				Humanoid::event_Jumping.connect(humanoid, boost::bind(&Seat::onEvent_humanoidJumped, this, _1), 10);
		}
	}
}

// FUNCTION: WEBSERVICE 0x100fcef0
void Seat::onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider)
{
	Instance::onServiceProvider(oldProvider, newProvider);

	if (oldProvider == NULL) {
		seatTouched = PartInstance::event_Touched.connect(this, boost::bind(&Seat::onEvent_seatTouched, this, _1), 10);
	}

	if (newProvider == NULL) {
		seatTouched.disconnect();
	}
}

// FUNCTION: WEBSERVICE 0x100fd180
Seat::Seat() : sleepTime(0.0)
{
	setName(sSeat);
}

} // namespace RBX

template class RBX::FactoryProduct<RBX::Seat, RBX::PartInstance, RBX::sSeat>;
template class RBX::Reflection::
	Described<RBX::Seat, RBX::sSeat, RBX::FactoryProduct<RBX::Seat, RBX::PartInstance, RBX::sSeat> >;
template class RBX::DescribedCreatable<RBX::Seat, RBX::PartInstance, RBX::sSeat>;
