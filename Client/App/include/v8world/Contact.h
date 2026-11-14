#ifndef V8WORLD_CONTACT_H
#define V8WORLD_CONTACT_H

#include "placeholder.h"
#include "v8kernel/Connector.h"
#include "v8world/Ball.h"
#include "v8world/Block.h"
#include "v8world/Primitive.h"

#include <G3D/Vector3int16.h>
#include <vector>

namespace RBX {

using G3D::Vector3int16;

class Kernel;

class __declspec(novtable) Contact : public Edge
{
public:
	Contact(Primitive* p0, Primitive* p1);
	virtual ~Contact() {}

	int& steppingIndexFunc() { return steppingIndex; }

	bool step(int uiStepId);

	bool computeIsAdjacent(float spaceAllowed);
	static bool isContact(Edge*);

	void onPrimitiveContactParametersChanged();

private:
	virtual void putInKernel(Kernel* _kernel);
	virtual void removeFromKernel();
	virtual EdgeType getEdgeType() const;

protected:
	Body* getBody(int index) const { return getPrimitive(index)->getBody(); }

	ContactConnector* createConnector();
	void deleteConnector(ContactConnector*& c);

	virtual void deleteAllConnectors() = 0;
	virtual bool stepContact() = 0;

public:
	virtual bool computeIsColliding(float overlapIgnored) = 0;

private:
	friend class BlockBlockContact;

	static bool ignoreBool;

protected:
	static int contactPairMatches;
	static int contactPairMisses;

private:
	int lastContactStep;
	int steppingIndex;
	float jointK;
	float elasticJointK;
	float kFriction;
};

class BallBallContact : public Contact
{
public:
	BallBallContact(Primitive* p0, Primitive* p1);

	virtual ~BallBallContact() {}

private:
	Ball* ball(int index) { return static_cast<Ball*>(getPrimitive(index)->getGeometry()); }

	virtual void deleteAllConnectors();
	virtual bool stepContact();
	virtual bool computeIsColliding(float overlapIgnored);

	ContactConnector* ballBallConnector;
};

class BallBlockContact : public Contact
{
public:
	BallBlockContact(Primitive* p0, Primitive* p1);

	virtual ~BallBlockContact() {}

private:
	Primitive* ballPrim() { return getPrimitive(0); }
	Primitive* blockPrim() { return getPrimitive(1); }

	Ball* ball() { return static_cast<Ball*>(ballPrim()->getGeometry()); }
	Block* block() { return static_cast<Block*>(blockPrim()->getGeometry()); }

	virtual void deleteAllConnectors();
	virtual bool stepContact();
	virtual bool computeIsColliding(float overlapIgnored);

	bool computeIsColliding(int& onBorder, Vector3int16& clip, Vector3& projectionInBlock, float overlapIgnored);

	ContactConnector* ballBlockConnector;
};

class BlockBlockContact : public Contact
{
public:
	BlockBlockContact(Primitive* p0, Primitive* p1);

	static float contactPairHitRatio();

private:
	Block* block(int index) { return static_cast<Block*>(getPrimitive(index)->getGeometry()); }

	virtual void deleteAllConnectors();
	virtual bool stepContact();
	virtual bool computeIsColliding(float overlapIgnored);

	bool getBestPlaneEdge(bool& temp, float overlapIgnored);

	ContactConnector* matchContactConnector(Body* b0, Body* b1, GeoPairType _pairType, int param0, int param1);
	void deleteUnmatchedConnectors();

	void loadGeoPairEdgeEdge(int b0, int b1, int edge0, int edge1);
	void loadGeoPairPointPlane(int pointBody, int planeBody, int pointID, NormalId pointFaceID, NormalId planeFaceID);
	void loadGeoPairEdgeEdgePlane(int edgeBody, int planeBody, int edge0, int edge1);

	int computePlaneContact();
	int intersectRectQuad(Vector2& planeRect, Vector2 (&otherQuad)[4]);

	std::vector<ContactConnector*> connectors;
	std::vector<bool> matched;

	int separatingAxisId;
	int separatingBodyId;

	int feature[2];

	int bPlane;
	int bOther;

	NormalId planeID;
	NormalId otherPlaneID;
};

inline float BlockBlockContact::contactPairHitRatio()
{
	int denom = contactPairMatches + contactPairMisses;

	return denom == 0 ? -1.0f : (float) contactPairMatches / denom;
}

inline BallBallContact::BallBallContact(Primitive* p0, Primitive* p1) : Contact(p0, p1), ballBallConnector(NULL)
{
}

inline void Contact::putInKernel(Kernel* _kernel)
{
	IPipelined::putInKernel(_kernel);

	onPrimitiveContactParametersChanged();
}

inline void Contact::removeFromKernel()
{
	deleteAllConnectors();

	IPipelined::removeFromKernel();
}

inline Edge::EdgeType Contact::getEdgeType() const
{
	return CONTACT;
}

inline void BallBallContact::deleteAllConnectors()
{
	deleteConnector(ballBallConnector);
}

inline void BallBlockContact::deleteAllConnectors()
{
	deleteConnector(ballBlockConnector);
}

inline BallBlockContact::BallBlockContact(Primitive* p0, Primitive* p1) : Contact(p0, p1), ballBlockConnector(NULL)
{
}

inline BlockBlockContact::BlockBlockContact(Primitive* p0, Primitive* p1)
	: Contact(p0, p1), separatingAxisId(0), separatingBodyId(0)
{
	feature[0] = -1;
	feature[1] = -1;
}

} // namespace RBX

#endif // V8WORLD_CONTACT_H
