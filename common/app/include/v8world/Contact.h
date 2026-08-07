#ifndef V8WORLD_CONTACT_H
#define V8WORLD_CONTACT_H

#include "decomp.h"
#include "v8kernel/Connector.h"
#include "v8world/Ball.h"
#include "v8world/Block.h"
#include "v8world/Primitive.h"

#include <G3D/Vector3int16.h>
#include <vector>

namespace RBX {

using G3D::Vector3int16;

class Kernel;

// SIZE 0x34
class __declspec(novtable) Contact : public Edge
{
public:
	Contact(Primitive* p0, Primitive* p1);

	int& steppingIndexFunc() { return steppingIndex; }

	bool step(int uiStepId);

	bool computeIsAdjacent(float spaceAllowed);

	void onPrimitiveContactParametersChanged();

private:
	virtual void putInKernel(Kernel* _kernel); // vtable+0x04
	virtual void removeFromKernel();           // vtable+0x08
	virtual EdgeType getEdgeType() const;      // vtable+0x0c

protected:
	Body* getBody(int index) const { return getPrimitive(index)->getBody(); }

	ContactConnector* createConnector();
	void deleteConnector(ContactConnector*& c);

	virtual void deleteAllConnectors() = 0; // vtable+0x14
	virtual bool stepContact() = 0;         // vtable+0x18

public:
	virtual bool computeIsColliding(float overlapIgnored) = 0; // vtable+0x1c

private:
	friend class BlockBlockContact;

	static bool ignoreBool;

protected:
	static int contactPairMatches;
	static int contactPairMisses;

private:
	int lastContactStep; // 0x20
	int steppingIndex;   // 0x24
	float jointK;        // 0x28
	float elasticJointK; // 0x2c
	float kFriction;     // 0x30
};

DECOMP_SIZE_ASSERT(Contact, 0x34)

// VTABLE: WEBSERVICE 0x10246ddc
// SIZE 0x38
class BallBallContact : public Contact
{
public:
	BallBallContact(Primitive* p0, Primitive* p1);

	// SYNTHETIC: WEBSERVICE 0x10108060
	// RBX::BallBallContact::`scalar deleting destructor'

	virtual ~BallBallContact() {} // vtable+0x00

private:
	Ball* ball(int index) { return static_cast<Ball*>(getPrimitive(index)->getGeometry()); }

	virtual void deleteAllConnectors();                    // vtable+0x14
	virtual bool stepContact();                            // vtable+0x18
	virtual bool computeIsColliding(float overlapIgnored); // vtable+0x1c

	ContactConnector* ballBallConnector; // 0x34
};

DECOMP_SIZE_ASSERT(BallBallContact, 0x38)

// VTABLE: WEBSERVICE 0x10246e00
// SIZE 0x38
class BallBlockContact : public Contact
{
public:
	BallBlockContact(Primitive* p0, Primitive* p1);

	// SYNTHETIC: WEBSERVICE 0x101080b0
	// RBX::BallBlockContact::`scalar deleting destructor'

	virtual ~BallBlockContact() {} // vtable+0x00

private:
	Primitive* ballPrim() { return getPrimitive(0); }
	Primitive* blockPrim() { return getPrimitive(1); }

	Ball* ball() { return static_cast<Ball*>(ballPrim()->getGeometry()); }
	Block* block() { return static_cast<Block*>(blockPrim()->getGeometry()); }

	virtual void deleteAllConnectors();                    // vtable+0x14
	virtual bool stepContact();                            // vtable+0x18
	virtual bool computeIsColliding(float overlapIgnored); // vtable+0x1c

	bool computeIsColliding(int& onBorder, Vector3int16& clip, Vector3& projectionInBlock, float overlapIgnored);

	ContactConnector* ballBlockConnector; // 0x34
};

DECOMP_SIZE_ASSERT(BallBlockContact, 0x38)

// VTABLE: WEBSERVICE 0x10246e24
// SIZE 0x78
class BlockBlockContact : public Contact
{
public:
	BlockBlockContact(Primitive* p0, Primitive* p1);

	// SYNTHETIC: WEBSERVICE 0x10108e40
	// RBX::BlockBlockContact::`scalar deleting destructor'

	static float contactPairHitRatio();

private:
	Block* block(int index) { return static_cast<Block*>(getPrimitive(index)->getGeometry()); }

	virtual void deleteAllConnectors();                    // vtable+0x14
	virtual bool stepContact();                            // vtable+0x18
	virtual bool computeIsColliding(float overlapIgnored); // vtable+0x1c

	bool getBestPlaneEdge(bool& temp, float overlapIgnored);

	ContactConnector* matchContactConnector(Body* b0, Body* b1, GeoPairType _pairType, int param0, int param1);
	void deleteUnmatchedConnectors();

	void loadGeoPairEdgeEdge(int b0, int b1, int edge0, int edge1);
	void loadGeoPairPointPlane(int pointBody, int planeBody, int pointID, NormalId pointFaceID, NormalId planeFaceID);
	void loadGeoPairEdgeEdgePlane(int edgeBody, int planeBody, int edge0, int edge1);

	int computePlaneContact();
	int intersectRectQuad(Vector2& planeRect, Vector2 (&otherQuad)[4]);

	std::vector<ContactConnector*> connectors; // 0x34
	std::vector<bool> matched;                 // 0x44

	int separatingAxisId; // 0x58
	int separatingBodyId; // 0x5c

	int feature[2]; // 0x60

	int bPlane; // 0x68
	int bOther; // 0x6c

	NormalId planeID;      // 0x70
	NormalId otherPlaneID; // 0x74
};

DECOMP_SIZE_ASSERT(BlockBlockContact, 0x78)

inline float BlockBlockContact::contactPairHitRatio()
{
	int denom = contactPairMatches + contactPairMisses;

	if (denom == 0) {
		return -1.0f;
	}

	return (float) contactPairMatches / denom;
}

// FUNCTION: WEBSERVICE 0x10107fd0
inline BallBallContact::BallBallContact(Primitive* p0, Primitive* p1) : Contact(p0, p1), ballBallConnector(NULL)
{
}

// FUNCTION: WEBSERVICE 0x10108000
inline void Contact::putInKernel(Kernel* _kernel)
{
	IPipelined::putInKernel(_kernel);

	onPrimitiveContactParametersChanged();
}

// FUNCTION: WEBSERVICE 0x10108020
inline void Contact::removeFromKernel()
{
	deleteAllConnectors();

	IPipelined::removeFromKernel();
}

// FUNCTION: WEBSERVICE 0x10108040 FOLDED
inline Edge::EdgeType Contact::getEdgeType() const
{
	return CONTACT;
}

// FUNCTION: WEBSERVICE 0x10108050 FOLDED
inline void BallBallContact::deleteAllConnectors()
{
	deleteConnector(ballBallConnector);
}

// FUNCTION: WEBSERVICE 0x10108050 FOLDED
inline void BallBlockContact::deleteAllConnectors()
{
	deleteConnector(ballBlockConnector);
}

// FUNCTION: WEBSERVICE 0x10108080
inline BallBlockContact::BallBlockContact(Primitive* p0, Primitive* p1) : Contact(p0, p1), ballBlockConnector(NULL)
{
}

// FUNCTION: WEBSERVICE 0x10108a30
inline BlockBlockContact::BlockBlockContact(Primitive* p0, Primitive* p1)
	: Contact(p0, p1), separatingAxisId(0), separatingBodyId(0)
{
	feature[0] = -1;
	feature[1] = -1;
}

} // namespace RBX

#endif // V8WORLD_CONTACT_H
