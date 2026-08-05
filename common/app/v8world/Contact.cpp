#include "v8world/Contact.h"

#include "decomp.h"
#include "v8kernel/Connector.h"
#include "v8kernel/Kernel.h"
#include "v8world/Primitive.h"

namespace RBX {

int Contact::contactPairMatches;
int Contact::contactPairMisses;

// FUNCTION: WEBSERVICE 0x100d1fb0
Contact::Contact(Primitive* p0, Primitive* p1)
	: Edge(p0, p1), lastContactStep(-1), steppingIndex(-1), jointK(0), elasticJointK(0), kFriction(0)
{
}

// FUNCTION: WEBSERVICE 0x100d1fe0
bool Contact::step(int uiStepId)
{
	bool touching = stepContact();

	if (touching) {
		if (lastContactStep == -1) {
			Primitive::onNewTouch(getPrimitive(0), getPrimitive(1));
		}

		lastContactStep = uiStepId;
	}
	else {
		if (lastContactStep < uiStepId) {
			lastContactStep = -1;
		}
	}

	return touching;
}

// FUNCTION: WEBSERVICE 0x100d2030
bool Contact::computeIsAdjacent(float spaceAllowed)
{
	if (computeIsColliding(spaceAllowed)) {
		return false;
	}

	return computeIsColliding(-spaceAllowed);
}

// FUNCTION: WEBSERVICE 0x100d2070
void Contact::deleteConnector(Connector*& connector)
{
	if (connector != NULL) {
		getKernel()->removeConnector(connector);

		delete connector;

		connector = NULL;
	}
}

// STUB: WEBSERVICE 0x100d21e0
DECOMP_NOINLINE bool Primitive::aaBoxCollide(const Primitive& p0, const Primitive& p1)
{
	STUB(0x100d21e0);

	return false;
}

// STUB: WEBSERVICE 0x100d22b0
void Contact::createConnector()
{
	STUB(0x100d22b0);
}

// STUB: WEBSERVICE 0x100d2330
bool BallBallContact::computeIsColliding(float overlapIgnored)
{
	STUB(0x100d2330);

	return false;
}

// STUB: WEBSERVICE 0x100d2430
bool BallBallContact::stepContact()
{
	STUB(0x100d2430);

	return false;
}

// STUB: WEBSERVICE 0x100d24c0
bool BallBlockContact::computeIsColliding(
	int& onBorder,
	G3D::Vector3int16& clip,
	Vector3& projectionInBlock,
	float overlapIgnored
)
{
	STUB(0x100d24c0);

	return false;
}

// STUB: WEBSERVICE 0x100d25f0
bool BallBlockContact::stepContact()
{
	STUB(0x100d25f0);

	return false;
}

// STUB: WEBSERVICE 0x100d2710
DECOMP_NOINLINE bool BlockBlockContact::getBestPlaneEdge(bool& temp, float overlapIgnored)
{
	STUB(0x100d2710);

	return false;
}

// STUB: WEBSERVICE 0x100d2da0
DECOMP_NOINLINE void Contact::onPrimitiveContactParametersChanged()
{
	STUB(0x100d2da0);
}

// STUB: WEBSERVICE 0x100d2ea0
bool BallBlockContact::computeIsColliding(float overlapIgnored)
{
	STUB(0x100d2ea0);

	return false;
}

// STUB: WEBSERVICE 0x100d2ef0
bool BlockBlockContact::computeIsColliding(float overlapIgnored)
{
	bool temp;

	return Primitive::aaBoxCollide(*getPrimitive(0), *getPrimitive(1)) && getBestPlaneEdge(temp, overlapIgnored);
}

// STUB: WEBSERVICE 0x100d4150
void BlockBlockContact::deleteAllConnectors()
{
	STUB(0x100d4150);
}

// STUB: WEBSERVICE 0x100d4df0
bool BlockBlockContact::stepContact()
{
	STUB(0x100d4df0);

	return false;
}

} // namespace RBX
