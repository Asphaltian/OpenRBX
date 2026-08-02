#include "v8datamodel/PartInstance.h"

#include "v8kernel/Body.h"
#include "v8world/Geometry.h"

namespace RBX {

char sPart[] = "Part";

bool PartInstance::highlightSleepParts;
bool PartInstance::highlightAwakeParts;
bool PartInstance::showAnchoredParts;
bool PartInstance::showPartCoord;
bool PartInstance::showUnalignedParts;
bool PartInstance::showSpanningTree;

const Reflection::PropDescriptor<PartInstance, float> PartInstance::prop_RenderImportance;
const Reflection::PropDescriptor<PartInstance, float> PartInstance::prop_Transparency;
const Reflection::PropDescriptor<PartInstance, float> PartInstance::prop_Reflectance;
const Reflection::PropDescriptor<PartInstance, bool> PartInstance::prop_Locked;

const Reflection::PropDescriptor<PartInstance, G3D::Color3> PartInstance::prop_Color;
const Reflection::PropDescriptor<PartInstance, BrickColor> PartInstance::prop_BrickColor;
const Reflection::PropDescriptor<PartInstance, bool> PartInstance::prop_CanCollide;
const Reflection::PropDescriptor<PartInstance, bool> PartInstance::prop_Anchored;

static Reflection::PropDescriptor<PartInstance, PartInstance::FormFactor> prop_formFactor;
static Reflection::PropDescriptor<PartInstance, bool> prop_Dragging;

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

// FUNCTION: WEBSERVICE 0x1009faa0
void PartInstance::setDragging(bool value)
{
	if (value != primitive->getDragging()) {
		primitive->setDragging(value);
		raisePropertyChanged(prop_Dragging);
	}
}

// FUNCTION: WEBSERVICE 0x1009fad0
void PartInstance::setRenderImportance(float value)
{
	if (value != renderImportance) {
		renderImportance = value;
		raisePropertyChanged(prop_RenderImportance);
	}
}

// FUNCTION: WEBSERVICE 0x1009fb00
void PartInstance::setCanCollide(bool value)
{
	if (value != getCanCollide()) {
		primitive->setCanCollide(value);
		raisePropertyChanged(prop_CanCollide);
	}
}

// FUNCTION: WEBSERVICE 0x1009fb40
void PartInstance::setAnchored(bool value)
{
	if (value != getAnchored()) {
		primitive->setAnchor(value);
		raisePropertyChanged(prop_Anchored);
	}
}

// FUNCTION: WEBSERVICE 0x1009fb80
void PartInstance::setPartLocked(bool value)
{
	if (value != locked) {
		locked = value;
		raisePropertyChanged(prop_Locked);
	}
}

// FUNCTION: WEBSERVICE 0x1009fbb0
void PartInstance::setTransparency(float value)
{
	if (value != transparency) {
		transparency = value;
		raisePropertyChanged(prop_Transparency);
	}
}

// FUNCTION: WEBSERVICE 0x1009fbe0
void PartInstance::setAlphaModifier(float value)
{
	if (value != alphaModifier) {
		alphaModifier = value;
		raisePropertyChanged(prop_Transparency);
	}
}

// FUNCTION: WEBSERVICE 0x1009fc10
void PartInstance::setReflectance(float value)
{
	if (value != reflectance) {
		reflectance = value;
		raisePropertyChanged(prop_Reflectance);
	}
}

// FUNCTION: WEBSERVICE 0x1009fc40
void PartInstance::setColor(BrickColor value)
{
	if (value.number != color.number) {
		color = value;
		raisePropertyChanged(prop_BrickColor);
		raisePropertyChanged(prop_Color);
	}
}

// FUNCTION: WEBSERVICE 0x1009fc70
void PartInstance::setFormFactorXml(FormFactor value)
{
	if (value != formFactor) {
		formFactor = value;
		raisePropertyChanged(prop_formFactor);
	}
}

} // namespace RBX
