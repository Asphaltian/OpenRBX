#ifndef V8DATAMODEL_FEATURE_H
#define V8DATAMODEL_FEATURE_H

#include "decomp.h"
#include "util/NormalId.h"
#include "v8tree/Instance.h"
#include "v8world/MotorJoint.h"

namespace RBX {

// SIZE 0x118
class Feature : public Instance
{
public:
	enum TopBottom
	{
		TOP = 0,
		CENTER_TB = 1,
		BOTTOM = 2
	};

	enum LeftRight
	{
		LEFT = 0,
		CENTER_LR = 1,
		RIGHT = 2
	};

	enum InOut
	{
		EDGE = 0,
		INSET = 1,
		CENTER_IO = 2
	};

	void setFaceId(NormalId value);
	void setTopBottom(TopBottom value);
	void setLeftRight(LeftRight value);
	void setInOut(InOut value);

private:
	undefined m_unk0x0f8[0x108 - 0x0f8]; // 0x0f8
	NormalId faceId;                     // 0x108
	TopBottom topBottom;                 // 0x10c
	LeftRight leftRight;                 // 0x110
	InOut inOut;                         // 0x114
};

DECOMP_SIZE_ASSERT(Feature, 0x118)

// SIZE 0x128
class VelocityMotor : public Instance
{
public:
	void setMaxVelocity(float value);
	void setDesiredAngle(float value);
	void setCurrentAngle(float value);

private:
	undefined m_unk0x0f8[0x108 - 0x0f8]; // 0x0f8
	MotorJoint* joint;                   // 0x108
	undefined m_unk0x10c[0x128 - 0x10c]; // 0x10c
};

DECOMP_SIZE_ASSERT(VelocityMotor, 0x128)

} // namespace RBX

#endif // V8DATAMODEL_FEATURE_H
