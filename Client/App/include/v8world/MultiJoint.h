#ifndef V8WORLD_MULTIJOINT_H
#define V8WORLD_MULTIJOINT_H

#include "placeholder.h"
#include "v8world/Joint.h"

namespace RBX {

class Connector;
class Point;

class MultiJoint : public Joint
{
	bool pointsAligned() const;

private:
	void init(int numBreakingConnectors);

protected:
	virtual void putInKernel(Kernel* _kernel);
	virtual void removeFromKernel();

	virtual bool isBroken() const;

	void addToMultiJoint(Point* point0, Point* point1, Connector* connector);

	Point* getPoint(int index);

	Connector* getConnector(int index) { return connector[index]; }

	float getJointK();

	virtual ~MultiJoint();

	MultiJoint(int numBreakingConnectors);

	MultiJoint(
		Primitive* prim0,
		Primitive* prim1,
		const CoordinateFrame& coord0,
		const CoordinateFrame& coord1,
		int numBreakingConnectors
	);

private:
	int numConnector;
	Point* point[8];
	Connector* connector[4];
	int numBreakingConnectors;
};

} // namespace RBX

#endif // V8WORLD_MULTIJOINT_H
