#ifndef V8KERNEL_KERNEL_H
#define V8KERNEL_KERNEL_H

#include "decomp.h"
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

// VTABLE: WEBSERVICE 0x1023c81c
// SIZE 0x30
class Kernel : public IStage
{
public:
	Kernel(IStage* upstream);

	// SYNTHETIC: WEBSERVICE 0x100d5bc0
	// RBX::Kernel::`scalar deleting destructor'

	virtual ~Kernel(); // vtable+0x00

	virtual StageType getStageType() { return KERNEL_STAGE; }

	virtual void stepWorld(int worldStepId, int uiStepId, bool throttling);

	int numBodies() const;
	int numPoints() const;
	int numConnectors() const;

	float connectorSpringEnergy();
	float bodyKineticEnergy();

	void insertConnector(Connector* connector);
	void insertConnector2ndPass(Connector* connector);
	void removeConnector(Connector* connector);
	void removeConnector2ndPass(Connector* connector);

	Kernel* getKernel();

	Point* newPoint(Body* body, const Vector3& worldPos);
	void deletePoint(Point* point);

	void insertBody(Body* body);
	void removeBody(Body* body);

private:
	void removePoint(Point* point);

	static int numKernels;

	bool inStepCode;                           // 0x0c
	KernelData* kernelData;                    // 0x10
	G3D::Array<Connector*> realTimeConnectors; // 0x14
	int maxBodies;                             // 0x20
	int maxPoints;                             // 0x24
	int maxConnectors;                         // 0x28

public:
	boost::scoped_ptr<Profiling::CodeProfiler> profilingKernel; // 0x2c
};

DECOMP_SIZE_ASSERT(Kernel, 0x30)

} // namespace RBX

#endif // V8KERNEL_KERNEL_H
