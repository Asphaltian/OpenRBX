#ifndef V8WORLD_CONTACT_H
#define V8WORLD_CONTACT_H

#include "decomp.h"
#include "v8world/Primitive.h"

#include <G3D/Vector3int16.h>
#include <vector>

namespace RBX {

class Connector;
class ContactConnector;
class Kernel;

// SIZE 0x34
class __declspec(novtable) Contact : public Edge
{
public:
	Contact(Primitive* p0, Primitive* p1);

	int& steppingIndexFunc() { return steppingIndex; }

	bool step(int uiStepId);

	bool computeIsAdjacent(float spaceAllowed);

	DECOMP_NOINLINE void onPrimitiveContactParametersChanged();

private:
	virtual void putInKernel(Kernel* kernel); // vtable+0x04
	virtual void removeFromKernel();          // vtable+0x08
	virtual EdgeType getEdgeType() const;     // vtable+0x0c

protected:
	void createConnector();
	void deleteConnector(Connector*& connector);

	virtual void deleteAllConnectors() = 0; // vtable+0x14
	virtual bool stepContact() = 0;         // vtable+0x18

public:
	virtual bool computeIsColliding(float overlapIgnored) = 0; // vtable+0x1c

private:
	friend class BlockBlockContact;

	static bool ignoreBool;

	static int contactPairMatches;
	static int contactPairMisses;

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
	BallBallContact(Primitive* prim0, Primitive* prim1);

	// SYNTHETIC: WEBSERVICE 0x10108060
	// RBX::BallBallContact::`scalar deleting destructor'

	virtual ~BallBallContact() {} // vtable+0x00

private:
	virtual void deleteAllConnectors();                    // vtable+0x14
	virtual bool stepContact();                            // vtable+0x18
	virtual bool computeIsColliding(float overlapIgnored); // vtable+0x1c

	Connector* ballBallConnector; // 0x34
};

DECOMP_SIZE_ASSERT(BallBallContact, 0x38)

// VTABLE: WEBSERVICE 0x10246e00
// SIZE 0x38
class BallBlockContact : public Contact
{
public:
	BallBlockContact(Primitive* prim0, Primitive* prim1);

	// SYNTHETIC: WEBSERVICE 0x101080b0
	// RBX::BallBlockContact::`scalar deleting destructor'

	virtual ~BallBlockContact() {} // vtable+0x00

private:
	virtual void deleteAllConnectors();                    // vtable+0x14
	virtual bool stepContact();                            // vtable+0x18
	virtual bool computeIsColliding(float overlapIgnored); // vtable+0x1c

	bool computeIsColliding(int& onBorder, G3D::Vector3int16& clip, Vector3& projectionInBlock, float overlapIgnored);

	Connector* ballBlockConnector; // 0x34
};

DECOMP_SIZE_ASSERT(BallBlockContact, 0x38)

// VTABLE: WEBSERVICE 0x10246e24
// SIZE 0x78
class BlockBlockContact : public Contact
{
public:
	BlockBlockContact(Primitive* prim0, Primitive* prim1);

	static float contactPairHitRatio();

private:
	virtual void deleteAllConnectors();                    // vtable+0x14
	virtual bool stepContact();                            // vtable+0x18
	virtual bool computeIsColliding(float overlapIgnored); // vtable+0x1c

	bool getBestPlaneEdge(bool& temp, float overlapIgnored);

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
inline BallBallContact::BallBallContact(Primitive* prim0, Primitive* prim1)
	: Contact(prim0, prim1), ballBallConnector(NULL)
{
}

// FUNCTION: WEBSERVICE 0x10108000
inline void Contact::putInKernel(Kernel* kernel)
{
	IPipelined::putInKernel(kernel);

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
inline BallBlockContact::BallBlockContact(Primitive* prim0, Primitive* prim1)
	: Contact(prim0, prim1), ballBlockConnector(NULL)
{
}

// FUNCTION: WEBSERVICE 0x10108a30
inline BlockBlockContact::BlockBlockContact(Primitive* prim0, Primitive* prim1)
	: Contact(prim0, prim1), separatingAxisId(0), separatingBodyId(0)
{
	feature[0] = -1;
	feature[1] = -1;
}

} // namespace RBX

#endif // V8WORLD_CONTACT_H
