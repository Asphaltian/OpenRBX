#ifndef V8DATAMODEL_FEATURE_H
#define V8DATAMODEL_FEATURE_H

#include "placeholder.h"
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

	NormalId faceId;
	TopBottom topBottom;
	LeftRight leftRight;
	InOut inOut;

private:
	virtual bool askSetParent(const Instance* instance) const { return true; }

	virtual bool shouldRender3dAdorn() const { return true; }

	virtual void render3dSelect(Adorn* adorn, SelectState selectState);

protected:
	bool getRenderCoord(CoordinateFrame& c) const;

	enum InOutZ
	{
		Z_IN = 0,
		Z_OUT = 1
	};

	virtual InOutZ getCoordOrientation() const { return Z_OUT; }

public:
	Feature();
	virtual ~Feature();

	NormalId getFaceId() const { return faceId; }

	void setFaceId(NormalId value);
	TopBottom getTopBottom() const { return topBottom; }

	void setTopBottom(TopBottom value);
	LeftRight getLeftRight() const { return leftRight; }

	void setLeftRight(LeftRight value);
	InOut getInOut() const { return inOut; }

	void setInOut(InOut value);

	CoordinateFrame computeLocalCoordinateFrame() const;
};

class VelocityMotor : public DescribedCreatable<VelocityMotor, JointInstance, sVelocityMotor>
{
private:
	MotorJoint* motorJoint() { return static_cast<MotorJoint*>(joint); }
	const MotorJoint* motorJoint() const { return static_cast<const MotorJoint*>(joint); }

	boost::shared_ptr<Hole> hole;
	boost::signals::scoped_connection holeAncestorChanged;

	void onEvent_HoleAncestorChanged();
	void setPart(int i, Feature* feature);

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

class Hole : public DescribedCreatable<Hole, Feature, sHole>
{
private:
	virtual void render3dAdorn(Adorn* adorn);

	virtual InOutZ getCoordOrientation() const { return Z_IN; }

public:
	Hole();
};

class MotorFeature : public DescribedCreatable<MotorFeature, Feature, sMotorFeature>
{
	void otherFeatureChanged();

private:
	virtual void render3dAdorn(Adorn* adorn);

public:
	MotorFeature();
	static bool canJoin(Instance*, Instance*);
	static void join(Instance*, Instance*);
};

bool FUN_00617200(Instance* first, Instance* second);
void FUN_0061a840(Instance* first, Instance* second);

} // namespace RBX

#endif // V8DATAMODEL_FEATURE_H
