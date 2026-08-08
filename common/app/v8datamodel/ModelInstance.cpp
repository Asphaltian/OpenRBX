#include "v8datamodel/ModelInstance.h"

#include "humanoid/Humanoid.h"
#include "util/Name.h"
#include "v8datamodel/PartInstance.h"

namespace RBX {

char sModel[] = "Model";

bool ModelInstance::showModelCoord;

bool ModelInstance::askSetParent(const Instance* instance) const
{
	STUB(0x10059c10);
	return false;
}

// STUB: WEBSERVICE 0x10059c40
float ModelInstance::computeFlagHeight() const
{
	STUB(0x10059c40);
	return 0;
}

// STUB: WEBSERVICE 0x1005a1a0
void ModelInstance::onExtentsChanged() const
{
	STUB(0x1005a1a0);
}

// STUB: WEBSERVICE 0x1005a1c0
const Primitive* ModelInstance::getBiggestPrimitive() const
{
	STUB(0x1005a1c0);
	return NULL;
}

// STUB: WEBSERVICE 0x1005a1f0
void ModelInstance::updatePrimaryPart(PartInstance* part) const
{
	STUB(0x1005a1f0);
}

// STUB: WEBSERVICE 0x1005a350
Extents ModelInstance::computeWorldGridExtents() const
{
	STUB(0x1005a350);
	return Extents();
}

// STUB: WEBSERVICE 0x1005a3b0
const CoordinateFrame ModelInstance::getLocation() const
{
	STUB(0x1005a3b0);
	return CoordinateFrame();
}

// STUB: WEBSERVICE 0x1005a460
void ModelInstance::render3dSelect(Adorn* adorn, SelectState selectState)
{
	STUB(0x1005a460);
}

// STUB: WEBSERVICE 0x1005a890
ModelInstance::~ModelInstance()
{
	STUB(0x1005a890);
}

// STUB: WEBSERVICE 0x1005ab00
Extents ModelInstance::getExtentsWorld() const
{
	STUB(0x1005ab00);
	return Extents();
}

// STUB: WEBSERVICE 0x1005ab50
Extents ModelInstance::getExtentsLocal() const
{
	STUB(0x1005ab50);
	return Extents();
}

// STUB: WEBSERVICE 0x1005abb0
PartInstance* ModelInstance::getPrimaryPartInternal() const
{
	STUB(0x1005abb0);

	return NULL;
}

// STUB: WEBSERVICE 0x1005ac90
void ModelInstance::legacyTraverseState(const CoordinateFrame& parentState)
{
	STUB(0x1005ac90);
}

// STUB: WEBSERVICE 0x1005ad40
bool ModelInstance::hitTest(const G3D::Ray& worldRay, G3D::Vector3& worldHitPoint)
{
	STUB(0x1005ad40);
	return false;
}

// STUB: WEBSERVICE 0x1005add0
bool ModelInstance::shouldRender3dAdorn() const
{
	STUB(0x1005add0);
	return false;
}

// STUB: WEBSERVICE 0x1005ae20
void ModelInstance::render3dAdorn(Adorn* adorn)
{
	STUB(0x1005ae20);
}

// STUB: WEBSERVICE 0x1005ae80
void ModelInstance::onCameraNear(float distance)
{
	STUB(0x1005ae80);
}

// STUB: WEBSERVICE 0x1005b220
PartInstance* ModelInstance::getPrimaryPart()
{
	return getPrimaryPartInternal();
}

const PartInstance* ModelInstance::getPrimaryPartConst() const
{
	STUB(0x1005b221);
	return NULL;
}

// STUB: WEBSERVICE 0x1005b230
void ModelInstance::onDescendentAdded(Instance* instance)
{
	STUB(0x1005b230);
}

// STUB: WEBSERVICE 0x1005b2c0
void ModelInstance::onDescendentRemoving(const shared_ptr<Instance>& instance)
{
	STUB(0x1005b2c0);
}

// STUB: WEBSERVICE 0x1005b440
void ModelInstance::getCameraIgnorePrimitives(std::vector<const Primitive*>& primitives)
{
	STUB(0x1005b440);
}

// STUB: WEBSERVICE 0x1005b560
Extents ModelInstance::computeLocalGridExtents() const
{
	STUB(0x1005b560);
	return Extents();
}

// STUB: WEBSERVICE 0x1005c000
ModelInstance::ModelInstance()
	: PVInstance(sModel), primaryPart(NULL), FlagHeight(this, &ModelInstance::computeFlagHeight),
	  LocalGridExtents(this, &ModelInstance::computeLocalGridExtents),
	  WorldGridExtents(this, &ModelInstance::computeWorldGridExtents)
{
	STUB(0x1005c000);
}

void ModelInstance::onLastChildRemoved()
{
	STUB(0x1015b611);
}

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sPart>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sHumanoid>();
template void RBX::Name::callDoDeclare<RBX::sPart>();
template void RBX::Name::callDoDeclare<RBX::sHumanoid>();
