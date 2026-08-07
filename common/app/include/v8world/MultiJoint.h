#ifndef V8WORLD_MULTIJOINT_H
#define V8WORLD_MULTIJOINT_H

#include "decomp.h"
#include "v8world/Joint.h"

namespace RBX {

class Connector;
class Point;

// VTABLE: WEBSERVICE 0x10248074
// SIZE 0xc0
class MultiJoint : public Joint
{
private:
	void init(int numBreakingConnectors);

protected:
	virtual void putInKernel(Kernel* _kernel); // vtable+0x04
	virtual void removeFromKernel();           // vtable+0x08

	virtual bool isBroken() const; // vtable+0x1c

	void addToMultiJoint(Point* point0, Point* point1, Connector* connector);

	Point* getPoint(int index) const;

	Connector* getConnector(int index) const { return connector[index]; }

	float getJointK();

	virtual ~MultiJoint(); // vtable+0x00

	// SYNTHETIC: WEBSERVICE 0x1011fe30
	// RBX::MultiJoint::`scalar deleting destructor'

	MultiJoint(int numBreakingConnectors);

	MultiJoint(
		Primitive* prim0,
		Primitive* prim1,
		const CoordinateFrame& coord0,
		const CoordinateFrame& coord1,
		int numBreakingConnectors
	);

private:
	int numConnector;          // 0x88
	Point* point[8];           // 0x8c
	Connector* connector[4];   // 0xac
	int numBreakingConnectors; // 0xbc
};

DECOMP_SIZE_ASSERT(MultiJoint, 0xc0)

} // namespace RBX

#endif // V8WORLD_MULTIJOINT_H
