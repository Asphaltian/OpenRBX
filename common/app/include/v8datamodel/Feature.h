#ifndef V8DATAMODEL_FEATURE_H
#define V8DATAMODEL_FEATURE_H

#include "decomp.h"
#include "util/IRenderable.h"
#include "util/NormalId.h"
#include "v8datamodel/JointInstance.h"
#include "v8tree/Instance.h"
#include "v8world/MotorJoint.h"

namespace RBX {

extern const char sVelocityMotor[];

extern const char sHole[];

extern const char sMotorFeature[];

extern const char sFeature[];

// SIZE 0x118
class Feature : public DescribedNonCreatable<Feature, Instance, sFeature>, public IRenderable
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

	// FUNCTION: WEBSERVICE 0x1019c9f0
	NormalId getFaceId() const { return faceId; }

	void setFaceId(NormalId value);
	// FUNCTION: WEBSERVICE 0x100f9de0
	TopBottom getTopBottom() const { return topBottom; }

	void setTopBottom(TopBottom value);
	// FUNCTION: WEBSERVICE 0x100e3c20
	LeftRight getLeftRight() const { return leftRight; }

	void setLeftRight(LeftRight value);
	// FUNCTION: WEBSERVICE 0x100e3c30
	InOut getInOut() const { return inOut; }

	void setInOut(InOut value);

private:
	NormalId faceId;     // 0x108
	TopBottom topBottom; // 0x10c
	LeftRight leftRight; // 0x110
	InOut inOut;         // 0x114
};

DECOMP_SIZE_ASSERT(Feature, 0x118)

// SIZE 0x128
class VelocityMotor : public DescribedCreatable<VelocityMotor, JointInstance, sVelocityMotor>
{
public:
	float getMaxVelocity() const;
	float getDesiredAngle() const;
	float getCurrentAngle() const;

	void setMaxVelocity(float value);
	void setDesiredAngle(float value);
	void setCurrentAngle(float value);

private:
	undefined m_unk0x10c[0x128 - 0x10c]; // 0x10c
};

DECOMP_SIZE_ASSERT(VelocityMotor, 0x128)

// SIZE 0x118
class Hole : public DescribedCreatable<Hole, Feature, sHole>
{
};

DECOMP_SIZE_ASSERT(Hole, 0x118)

// SIZE 0x118
class MotorFeature : public DescribedCreatable<MotorFeature, Feature, sMotorFeature>
{
};

DECOMP_SIZE_ASSERT(MotorFeature, 0x118)

// clang-format off
// STUB: WEBSERVICE 0x100e4410
// RBX::Feature::~Feature
// STUB: WEBSERVICE 0x10223dd0
// RBX::`dynamic atexit destructor for 'prop_CurrentAngle''
// STUB: WEBSERVICE 0x10223e50
// RBX::`dynamic atexit destructor for 'prop_MaxVelocity''
// clang-format on

// clang-format off
// STUB: WEBSERVICE 0x1021cce0
// RBX::`dynamic initializer for 'prop_MaxVelocity''
// STUB: WEBSERVICE 0x1021cd20
// RBX::`dynamic initializer for 'prop_DesiredAngle''
// STUB: WEBSERVICE 0x1021cd60
// RBX::`dynamic initializer for 'prop_CurrentAngle''
// STUB: WEBSERVICE 0x1021cda0
// RBX::`dynamic initializer for 'prop_FaceId''
// STUB: WEBSERVICE 0x1021cde0
// RBX::`dynamic initializer for 'prop_TopBottom''
// STUB: WEBSERVICE 0x1021ce20
// RBX::`dynamic initializer for 'prop_LeftRight''
// STUB: WEBSERVICE 0x1021ce60
// RBX::`dynamic initializer for 'prop_InOut''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_FEATURE_H
