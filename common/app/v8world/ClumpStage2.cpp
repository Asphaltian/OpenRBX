#include "v8world/ClumpStage2.h"

#include "decomp.h"
#include "v8world/Assembly2.h"
#include "v8world/Primitive.h"

#include <cstddef>

namespace RBX {

// FUNCTION: WEBSERVICE 0x1011b440
Joint* getJoint(Primitive* primitive, Joint::JointType jointType)
{
	Joint* joint = primitive->getFirstJoint();

	while (joint != NULL) {
		if (joint->getJointType() == jointType) {
			return joint;
		}

		joint = primitive->getNextJoint(joint);
	}

	return NULL;
}

// FUNCTION: WEBSERVICE 0x1011bb90
int TreeStage::getMetric(MetricType metricType)
{
	switch (metricType) {
	case MAX_TREE_DEPTH:
		return maxTreeDepth;
	default:
		return static_cast<IWorldStage*>(getDownstream())->getMetric(metricType);
	}
}

// STUB: WEBSERVICE 0x1011c010
void TreeStage::dirtyAssemblies(Joint* joint)
{
	for (int i = 0; i < 2; i++) {
		Primitive* primitive = joint->getPrimitive(i);

		if (primitive != NULL) {
			Assembly* assembly = primitive->getAssembly();

			if (assembly != NULL) {
				assemblies.insert(assembly);
				dirty = true;
			}
		}
	}
}

} // namespace RBX
