#include "v8datamodel/PartInstance.h"

#include "v8kernel/Body.h"
#include "v8world/Geometry.h"

namespace RBX {

bool PartInstance::highlightSleepParts;
bool PartInstance::highlightAwakeParts;
bool PartInstance::showAnchoredParts;
bool PartInstance::showPartCoord;
bool PartInstance::showUnalignedParts;
bool PartInstance::showSpanningTree;

// FUNCTION: WEBSERVICE 0x1009ae40
bool PartInstance::nonNullInWorkspace(shared_ptr<PartInstance> part)
{
	return part.get() != NULL && part->myWorld != NULL;
}

// FUNCTION: WEBSERVICE 0x1009b080
const CoordinateFrame& PartInstance::getCoordinateFrame() const
{
	return primitive->getCoordinateFrame();
}

// FUNCTION: WEBSERVICE 0x1009b0b0
float PartInstance::getMass() const
{
	return primitive->getBody()->getMass();
}

// FUNCTION: WEBSERVICE 0x1009b100
const Vector3& PartInstance::getPartSizeXml() const
{
	return primitive->getGeometry()->getGridSize();
}

// FUNCTION: WEBSERVICE 0x1009b110
Vector3 PartInstance::getPartSizeUi() const
{
	return primitive->getGeometry()->getGridSize();
}

// FUNCTION: WEBSERVICE 0x1009b140
bool PartInstance::getDragging() const
{
	return primitive->getDragging();
}

// FUNCTION: WEBSERVICE 0x1009b150
bool PartInstance::getCanCollide() const
{
	if (!primitive->getDragging() && primitive->getCanCollide()) {
		return true;
	}

	return false;
}

// FUNCTION: WEBSERVICE 0x1009b170
bool PartInstance::getAnchored() const
{
	return primitive->getAnchorObject() != NULL;
}

// FUNCTION: WEBSERVICE 0x1009b190
float PartInstance::getFriction() const
{
	return primitive->getFriction();
}

// FUNCTION: WEBSERVICE 0x1009b1a0
float PartInstance::getElasticity() const
{
	return primitive->getElasticity();
}

// FUNCTION: WEBSERVICE 0x1009b1d0
Extents PartInstance::getExtentsLocal() const
{
	const Vector3 corner = primitive->getGeometry()->getGridSize() * 0.5f;

	return Extents(-corner, corner);
}

} // namespace RBX
