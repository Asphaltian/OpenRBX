#include "util/Name.h"
#include "v8datamodel/CharacterAppearance.h"
#include "v8datamodel/DebrisService.h"
#include "v8datamodel/Explosion.h"
#include "v8datamodel/Feature.h"
#include "v8datamodel/FlagStand.h"
#include "v8datamodel/ForceField.h"
#include "v8datamodel/GeometryService.h"
#include "v8datamodel/Gyro.h"
#include "v8datamodel/Message.h"
#include "v8datamodel/Seat.h"
#include "v8datamodel/SpawnLocation.h"
#include "v8datamodel/TimerService.h"
#include "v8datamodel/Value.h"
#include "v8datamodel/Visit.h"

template const RBX::Name& RBX::Name::doDeclare<RBX::sShirtGraphic>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sShirt>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sBodyColors>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sSkin>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sExplosion>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sMotorFeature>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sHole>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sVelocityMotor>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sFlagStand>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sFlagStandService>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sForceField>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sGeometryService>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sBodyGyro>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sBodyForce>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sBodyThrust>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sBodyPosition>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sBodyVelocity>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sRocket>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sMessage>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sHint>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sObjectValue>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sVisit>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sDebrisService>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sTimerService>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sSeat>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sSpawnLocation>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sSpawnerService>();
template void RBX::Name::callDoDeclare<RBX::sShirtGraphic>();
template void RBX::Name::callDoDeclare<RBX::sShirt>();
template void RBX::Name::callDoDeclare<RBX::sBodyColors>();
template void RBX::Name::callDoDeclare<RBX::sSkin>();
template void RBX::Name::callDoDeclare<RBX::sExplosion>();
template void RBX::Name::callDoDeclare<RBX::sMotorFeature>();
template void RBX::Name::callDoDeclare<RBX::sHole>();
template void RBX::Name::callDoDeclare<RBX::sVelocityMotor>();
template void RBX::Name::callDoDeclare<RBX::sFlagStand>();
template void RBX::Name::callDoDeclare<RBX::sFlagStandService>();
template void RBX::Name::callDoDeclare<RBX::sForceField>();
template void RBX::Name::callDoDeclare<RBX::sGeometryService>();
template void RBX::Name::callDoDeclare<RBX::sBodyGyro>();
template void RBX::Name::callDoDeclare<RBX::sBodyForce>();
template void RBX::Name::callDoDeclare<RBX::sBodyThrust>();
template void RBX::Name::callDoDeclare<RBX::sBodyPosition>();
template void RBX::Name::callDoDeclare<RBX::sBodyVelocity>();
template void RBX::Name::callDoDeclare<RBX::sRocket>();
template void RBX::Name::callDoDeclare<RBX::sMessage>();
template void RBX::Name::callDoDeclare<RBX::sHint>();
template void RBX::Name::callDoDeclare<RBX::sObjectValue>();
template void RBX::Name::callDoDeclare<RBX::sVisit>();
template void RBX::Name::callDoDeclare<RBX::sDebrisService>();
template void RBX::Name::callDoDeclare<RBX::sTimerService>();
template void RBX::Name::callDoDeclare<RBX::sSeat>();
template void RBX::Name::callDoDeclare<RBX::sSpawnLocation>();
template void RBX::Name::callDoDeclare<RBX::sSpawnerService>();
