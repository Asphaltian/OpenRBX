#ifndef V8KERNEL_KERNEL_H
#define V8KERNEL_KERNEL_H

#include "placeholder.h"
#include "util/Profiling.h"
#include "v8kernel/IStage.h"

#include <G3D/Array.h>
#include <G3D/Vector3.h>
#include <boost/scoped_ptr.hpp>

namespace RBX {

class Body;
class Connector;
class KernelData;
class Point;

using G3D::Vector3;

class Kernel : public IStage
{
private:
	static int numKernels;

	bool inStepCode;
	KernelData* kernelData;
	G3D::Array<Connector*> realTimeConnectors;
	int maxBodies;
	int maxPoints;
	int maxConnectors;

public:
	Kernel(IStage* upstream);

	virtual ~Kernel();

	virtual StageType getStageType() { return KERNEL_STAGE; }

	virtual void stepWorld(int worldStepId, int uiStepId, bool throttling);

	virtual Kernel* getKernel();

	void insertBody(Body* body);
	void insertPoint(Point*);
	void insertConnector(Connector* connector);
	void insertConnector2ndPass(Connector* connector);
	void removeBody(Body* body);
	void removePoint(Point* point);
	void removeConnector(Connector* connector);
	void removeConnector2ndPass(Connector* connector);

	Point* newPoint(Body* body, const Vector3& worldPos);
	void deletePoint(Point* point);

	void report();
	static void reportMemorySizes();

	float connectorSpringEnergy() const;
	float bodyPotentialEnergy() const;
	float bodyKineticEnergy() const;
	float totalEnergy() const;

	float totalKineticEnergy() const { return connectorSpringEnergy() + bodyKineticEnergy(); }

	int numBodies() const;
	int numPoints() const;
	int numConnectors() const;

	boost::scoped_ptr<Profiling::CodeProfiler> profilingKernel;
};

} // namespace RBX

#endif // V8KERNEL_KERNEL_H
