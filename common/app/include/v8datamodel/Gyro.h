#ifndef V8DATAMODEL_GYRO_H
#define V8DATAMODEL_GYRO_H

#include "decomp.h"
#include "util/RunStateOwner.h"
#include "v8kernel/Connector.h"
#include "v8tree/Instance.h"

namespace RBX {

extern char sRocket[];

extern char sBodyVelocity[];

extern char sBodyPosition[];

extern char sBodyThrust[];

extern char sBodyForce[];

extern char sBodyGyro[];
// SIZE 0x10c
class BodyMover : public Instance, public Connector, public Listener<RunService, Stepped>
{
protected:
	virtual void onEvent(const RunService* source, Stepped event);

private:
	undefined m_unk0x104[0x10c - 0x104]; // 0x104
};

DECOMP_SIZE_ASSERT(BodyMover, 0x10c)

// SIZE 0x118
class BodyForce : public DescribedCreatable<BodyForce, BodyMover, sBodyForce>
{
public:
	virtual void computeForce(float dt, bool throttling);

private:
	undefined m_unk0x10c[0x118 - 0x10c]; // 0x10c
};

DECOMP_SIZE_ASSERT(BodyForce, 0x118)

// SIZE 0x150
class BodyGyro : public DescribedCreatable<BodyGyro, BodyMover, sBodyGyro>
{
public:
	virtual void computeForce(float dt, bool throttling);

private:
	undefined m_unk0x10c[0x150 - 0x10c]; // 0x10c
};

DECOMP_SIZE_ASSERT(BodyGyro, 0x150)

// SIZE 0x138
class BodyPosition : public DescribedCreatable<BodyPosition, BodyMover, sBodyPosition>
{
public:
	virtual void computeForce(float dt, bool throttling);

private:
	undefined m_unk0x10c[0x138 - 0x10c]; // 0x10c
};

DECOMP_SIZE_ASSERT(BodyPosition, 0x138)

// SIZE 0x124
class BodyThrust : public DescribedCreatable<BodyThrust, BodyMover, sBodyThrust>
{
public:
	virtual void computeForce(float dt, bool throttling);

private:
	undefined m_unk0x10c[0x124 - 0x10c]; // 0x10c
};

DECOMP_SIZE_ASSERT(BodyThrust, 0x124)

// SIZE 0x134
class BodyVelocity : public DescribedCreatable<BodyVelocity, BodyMover, sBodyVelocity>
{
public:
	virtual void computeForce(float dt, bool throttling);

private:
	undefined m_unk0x10c[0x134 - 0x10c]; // 0x10c
};

DECOMP_SIZE_ASSERT(BodyVelocity, 0x134)

// SIZE 0x154
class Rocket : public DescribedCreatable<Rocket, BodyMover, sRocket>
{
public:
	virtual void computeForce(float dt, bool throttling);

private:
	undefined m_unk0x10c[0x154 - 0x10c]; // 0x10c
};

DECOMP_SIZE_ASSERT(Rocket, 0x154)

// clang-format off
// STUB: WEBSERVICE 0x100ea340
// RBX::BodyMover::~BodyMover
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_GYRO_H
