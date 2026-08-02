#include "v8datamodel/Gyro.h"

namespace RBX {

char sRocket[] = "RocketPropulsion";

char sBodyVelocity[] = "BodyVelocity";

char sBodyPosition[] = "BodyPosition";

char sBodyThrust[] = "BodyThrust";

char sBodyForce[] = "BodyForce";

char sBodyGyro[] = "BodyGyro";

} // namespace RBX

template class RBX::FactoryProduct<RBX::BodyForce, RBX::BodyMover, RBX::sBodyForce>;
template class RBX::Reflection::
	Described<RBX::BodyForce, RBX::sBodyForce, RBX::FactoryProduct<RBX::BodyForce, RBX::BodyMover, RBX::sBodyForce> >;
template class RBX::DescribedCreatable<RBX::BodyForce, RBX::BodyMover, RBX::sBodyForce>;

template class RBX::FactoryProduct<RBX::BodyGyro, RBX::BodyMover, RBX::sBodyGyro>;
template class RBX::Reflection::
	Described<RBX::BodyGyro, RBX::sBodyGyro, RBX::FactoryProduct<RBX::BodyGyro, RBX::BodyMover, RBX::sBodyGyro> >;
template class RBX::DescribedCreatable<RBX::BodyGyro, RBX::BodyMover, RBX::sBodyGyro>;

template class RBX::FactoryProduct<RBX::BodyPosition, RBX::BodyMover, RBX::sBodyPosition>;
template class RBX::Reflection::Described<
	RBX::BodyPosition,
	RBX::sBodyPosition,
	RBX::FactoryProduct<RBX::BodyPosition, RBX::BodyMover, RBX::sBodyPosition> >;
template class RBX::DescribedCreatable<RBX::BodyPosition, RBX::BodyMover, RBX::sBodyPosition>;

template class RBX::FactoryProduct<RBX::BodyThrust, RBX::BodyMover, RBX::sBodyThrust>;
template class RBX::Reflection::Described<
	RBX::BodyThrust,
	RBX::sBodyThrust,
	RBX::FactoryProduct<RBX::BodyThrust, RBX::BodyMover, RBX::sBodyThrust> >;
template class RBX::DescribedCreatable<RBX::BodyThrust, RBX::BodyMover, RBX::sBodyThrust>;

template class RBX::FactoryProduct<RBX::BodyVelocity, RBX::BodyMover, RBX::sBodyVelocity>;
template class RBX::Reflection::Described<
	RBX::BodyVelocity,
	RBX::sBodyVelocity,
	RBX::FactoryProduct<RBX::BodyVelocity, RBX::BodyMover, RBX::sBodyVelocity> >;
template class RBX::DescribedCreatable<RBX::BodyVelocity, RBX::BodyMover, RBX::sBodyVelocity>;

template class RBX::FactoryProduct<RBX::Rocket, RBX::BodyMover, RBX::sRocket>;
template class RBX::Reflection::
	Described<RBX::Rocket, RBX::sRocket, RBX::FactoryProduct<RBX::Rocket, RBX::BodyMover, RBX::sRocket> >;
template class RBX::DescribedCreatable<RBX::Rocket, RBX::BodyMover, RBX::sRocket>;
