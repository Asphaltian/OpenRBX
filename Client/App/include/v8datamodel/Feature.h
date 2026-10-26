#ifndef V8DATAMODEL_FEATURE_H
#define V8DATAMODEL_FEATURE_H

#include "decomp.h"
#include "util/IRenderable.h"
#include "util/NormalId.h"
#include "v8datamodel/JointInstance.h"
#include "v8tree/Instance.h"
#include "v8world/MotorJoint.h"

#include <boost/shared_ptr.hpp>
#include <boost/signals/connection.hpp>

namespace RBX {

extern const char sVelocityMotor[];

extern const char sHole[];

extern const char sMotorFeature[];

extern const char sFeature[];

class Hole;

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

	typedef NormalId FaceId;

	NormalId faceId;     // 0x108
	TopBottom topBottom; // 0x10c
	LeftRight leftRight; // 0x110
	InOut inOut;         // 0x114

private:
	// FUNCTION: WEBSERVICE 0x100e44b0 FOLDED
	virtual bool askSetParent(const Instance* instance) const { return true; }

	// FUNCTION: WEBSERVICE 0x100e6150 FOLDED
	virtual bool shouldRender3dAdorn() const { return true; }

	virtual void render3dSelect(Adorn* adorn, SelectState selectState);

protected:
	bool getRenderCoord(CoordinateFrame& c) const;

	enum InOutZ
	{
		Z_IN = 0,
		Z_OUT = 1
	};

	// FUNCTION: WEBSERVICE 0x10108040 FOLDED
	virtual InOutZ getCoordOrientation() const { return Z_OUT; } // vtable+0x40

public:
	Feature();
	virtual ~Feature();

	// FUNCTION: WEBSERVICE 0x1019c9f0
	NormalId getFaceId() const { return faceId; }

	void setFaceId(NormalId value);
	// FUNCTION: WEBSERVICE 0x100f9de0 FOLDED
	TopBottom getTopBottom() const { return topBottom; }

	void setTopBottom(TopBottom value);
	// FUNCTION: WEBSERVICE 0x100e3c20
	LeftRight getLeftRight() const { return leftRight; }

	void setLeftRight(LeftRight value);
	// FUNCTION: WEBSERVICE 0x100e3c30 FOLDED
	InOut getInOut() const { return inOut; }

	void setInOut(InOut value);

	CoordinateFrame computeLocalCoordinateFrame() const;
};

DECOMP_SIZE_ASSERT(Feature, 0x118)

// SIZE 0x128
class VelocityMotor : public DescribedCreatable<VelocityMotor, JointInstance, sVelocityMotor>
{
private:
	MotorJoint* motorJoint() { return static_cast<MotorJoint*>(joint); }
	const MotorJoint* motorJoint() const { return static_cast<const MotorJoint*>(joint); }

	boost::shared_ptr<Hole> hole;                          // 0x10c
	boost::signals::scoped_connection holeAncestorChanged; // 0x114

	void onEvent_HoleAncestorChanged();
	void setPart(int i, Feature* feature);

	// FUNCTION: WEBSERVICE 0x100e44b0 FOLDED
	virtual bool askSetParent(const Instance* instance) const { return true; }

	virtual void onAncestorChanged(const AncestorChanged& event);

public:
	VelocityMotor();
	virtual ~VelocityMotor();

	Hole* getHole() const;
	void setHole(Hole* value);

	float getMaxVelocity() const;
	float getDesiredAngle() const;
	float getCurrentAngle() const;

	void setMaxVelocity(float value);
	void setDesiredAngle(float value);
	void setCurrentAngle(float value);
};

DECOMP_SIZE_ASSERT(VelocityMotor, 0x128)

// SIZE 0x118
class Hole : public DescribedCreatable<Hole, Feature, sHole>
{
private:
	virtual void render3dAdorn(Adorn* adorn);

	// FUNCTION: WEBSERVICE 0x10059d20 FOLDED
	virtual InOutZ getCoordOrientation() const { return Z_IN; }

public:
	Hole();
};

DECOMP_SIZE_ASSERT(Hole, 0x118)

// SIZE 0x118
class MotorFeature : public DescribedCreatable<MotorFeature, Feature, sMotorFeature>
{
private:
	virtual void render3dAdorn(Adorn* adorn);

public:
	MotorFeature();
};

DECOMP_SIZE_ASSERT(MotorFeature, 0x118)

// clang-format off
// FUNCTION: WEBSERVICE 0x1021cca0
// RBX::`dynamic initializer for 'prop_Hole''
// FUNCTION: WEBSERVICE 0x1021cce0
// RBX::`dynamic initializer for 'prop_MaxVelocity''
// FUNCTION: WEBSERVICE 0x1021cd20
// RBX::`dynamic initializer for 'prop_DesiredAngle''
// FUNCTION: WEBSERVICE 0x1021cd60
// RBX::`dynamic initializer for 'prop_CurrentAngle''
// FUNCTION: WEBSERVICE 0x1021cda0
// RBX::`dynamic initializer for 'prop_FaceId''
// FUNCTION: WEBSERVICE 0x1021cde0
// RBX::`dynamic initializer for 'prop_TopBottom''
// FUNCTION: WEBSERVICE 0x1021ce20
// RBX::`dynamic initializer for 'prop_LeftRight''
// FUNCTION: WEBSERVICE 0x1021ce60
// RBX::`dynamic initializer for 'prop_InOut''
// SYNTHETIC: WEBSERVICE 0x10224200
// RBX::`dynamic atexit destructor for 'prop_Hole''
// SYNTHETIC: WEBSERVICE 0x10224220
// RBX::`dynamic atexit destructor for 'prop_CurrentAngle''
// SYNTHETIC: WEBSERVICE 0x10224240
// RBX::`dynamic atexit destructor for 'prop_InOut''
// SYNTHETIC: WEBSERVICE 0x10224260
// RBX::`dynamic atexit destructor for 'prop_LeftRight''
// SYNTHETIC: WEBSERVICE 0x10224280
// RBX::`dynamic atexit destructor for 'prop_TopBottom''
// SYNTHETIC: WEBSERVICE 0x102242a0
// RBX::`dynamic atexit destructor for 'prop_FaceId''
// SYNTHETIC: WEBSERVICE 0x102242c0
// RBX::`dynamic atexit destructor for 'prop_DesiredAngle''
// SYNTHETIC: WEBSERVICE 0x102242e0
// RBX::`dynamic atexit destructor for 'prop_MaxVelocity''
// SYNTHETIC: WEBSERVICE 0x100e4c60
// RBX::Feature::`scalar deleting destructor'
// SYNTHETIC: WEBSERVICE 0x100e4c80
// RBX::VelocityMotor::`scalar deleting destructor'
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_FEATURE_H
