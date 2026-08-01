#include "v8world/ClumpStage2.h"

#include "decomp.h"
#include "util/Guid.h"
#include "v8world/Assembly2.h"
#include "v8world/Primitive.h"

#include <cmath>
#include <cstddef>

namespace RBX {

// STUB: WEBSERVICE 0x1011b330
float getPrimitiveSize(Primitive* primitive)
{
	const Vector3& size = primitive->getGeometry()->getGridSize();

	float area;

	if (size.y <= size.x) {
		area = (size.z <= size.y ? size.y : size.z) * size.x;
	}
	else if (size.x < size.z) {
		area = size.z * size.y;
	}
	else {
		area = size.y * size.x;
	}

	return (float) ((unsigned __int64) primitive->getSizeMultiplier() * (int) floor(area));
}

// STUB: WEBSERVICE 0x1011b3d0
int biggerJointGuid(Joint* joint0, Joint* joint1)
{
	const Guid* guid0 = &joint0->getPrimitive(0)->getGuid();
	const Guid* guid1 = joint0->getPrimitive(1) != NULL ? &joint0->getPrimitive(1)->getGuid() : NULL;
	const Guid* other0 = &joint1->getPrimitive(0)->getGuid();
	const Guid* other1 = joint1->getPrimitive(1) != NULL ? &joint1->getPrimitive(1)->getGuid() : NULL;

	return Guid::compare(guid0, guid1, other0, other1);
}

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

// FUNCTION: WEBSERVICE 0x1011b5e0
int biggerJointSize(Joint* joint0, Joint* joint1)
{
	float size0 = getPrimitiveSize(joint0->getPrimitive(0));

	if (joint0->getPrimitive(1) != NULL) {
		size0 = G3D::max(size0, getPrimitiveSize(joint0->getPrimitive(1)));
	}

	float size1 = getPrimitiveSize(joint1->getPrimitive(0));

	if (joint1->getPrimitive(1) != NULL) {
		size1 = G3D::max(size1, getPrimitiveSize(joint1->getPrimitive(1)));
	}

	if (size1 < size0) {
		return 1;
	}

	if (size0 < size1) {
		return -1;
	}

	return 0;
}

namespace JointSort {

// STUB: WEBSERVICE 0x1011b690
bool lighterJoint(Joint* joint0, Joint* joint1)
{
	if (joint0 == joint1) {
		return false;
	}

	if (joint0->getJointType() != joint1->getJointType()) {
		return joint0->getJointType() < joint1->getJointType();
	}

	int bigger = biggerJointSize(joint0, joint1);

	if (bigger == 1) {
		return true;
	}

	if (bigger == -1) {
		return false;
	}

	bigger = biggerJointGuid(joint0, joint1);

	if (bigger == 1) {
		return true;
	}

	if (bigger == -1) {
		return false;
	}

	return joint0 < joint1;
}

} // namespace JointSort

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

// STUB: WEBSERVICE 0x1011c0b0
void TreeStage::destroyAssembly(Assembly* assembly)
{
	STUB(0x1011c0b0);
}

// STUB: WEBSERVICE 0x1011c630
void TreeStage::traverse(Joint* joint, Primitive* root)
{
	STUB(0x1011c630);
}

// STUB: WEBSERVICE 0x1011c6e0
void TreeStage::swap(Joint* remove, Joint* add, Primitive* root)
{
	if (remove != NULL) {
		dirtyAssemblies(remove);
		remove->setActive(false);

		if (add == NULL) {
			destroyAssembly(remove->getPrimitive(1)->getAssembly());
			return;
		}
	}
	else if (add == NULL) {
		return;
	}

	dirtyAssemblies(add);
	add->setActive(true);
	traverse(add, root);
}

} // namespace RBX
