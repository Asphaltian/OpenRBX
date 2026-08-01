#ifndef V8WORLD_MULTIJOINT_H
#define V8WORLD_MULTIJOINT_H

#include "decomp.h"
#include "v8world/Joint.h"

namespace RBX {

class Connector;
class Point;
class RotateConnector;

// VTABLE: WEBSERVICE 0x10248074
// SIZE 0xc0
class MultiJoint : public Joint
{
public:
	virtual ~MultiJoint(); // vtable+0x00

	// SYNTHETIC: WEBSERVICE 0x1011fe30
	// RBX::MultiJoint::`scalar deleting destructor'

	virtual void putInKernel(Kernel* kernel); // vtable+0x04
	virtual void removeFromKernel();          // vtable+0x08

	virtual bool isBroken() const; // vtable+0x1c

	Point* getPoint(int index) const;

protected:
	int numConnector;          // 0x88
	Point* point[8];           // 0x8c
	Connector* connector[4];   // 0xac
	int numBreakingConnectors; // 0xbc
};

DECOMP_SIZE_ASSERT(MultiJoint, 0xc0)

// VTABLE: WEBSERVICE 0x10248028
// SIZE 0xc4
class RotateJoint : public MultiJoint
{
public:
	virtual ~RotateJoint(); // vtable+0x00

	virtual void removeFromKernel(); // vtable+0x08

private:
	RotateConnector* rotateConnector; // 0xc0
};

DECOMP_SIZE_ASSERT(RotateJoint, 0xc4)

} // namespace RBX

#endif // V8WORLD_MULTIJOINT_H
