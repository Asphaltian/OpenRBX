#include "v8datamodel/PartInstance.h"

#include "v8datamodel/ModelInstance.h"
#include "v8kernel/Body.h"
#include "v8world/Geometry.h"

namespace RBX {

// GLOBAL: WEBSERVICE 0x102d4568
const char* const category_Part = "Part";

char sPart[] = "Part";

bool PartInstance::highlightSleepParts;
bool PartInstance::highlightAwakeParts;
bool PartInstance::showAnchoredParts;
bool PartInstance::showPartCoord;
bool PartInstance::showUnalignedParts;
bool PartInstance::showSpanningTree;

const Reflection::PropDescriptor<PartInstance, float> PartInstance::prop_RenderImportance(
	"RenderImportance",
	"Behavior",
	&RBX::PartInstance::getRenderImportance,
	&RBX::PartInstance::setRenderImportance,
	Reflection::PropertyDescriptor::LEGACY
);

const Reflection::PropDescriptor<PartInstance, float> PartInstance::prop_Transparency(
	"Transparency",
	"Appearance",
	&RBX::PartInstance::getTransparencyXml,
	&RBX::PartInstance::setTransparency
);

const Reflection::PropDescriptor<PartInstance, float> PartInstance::prop_Reflectance(
	"Reflectance",
	"Appearance",
	&RBX::PartInstance::getReflectance,
	&RBX::PartInstance::setReflectance
);

const Reflection::PropDescriptor<PartInstance, bool> PartInstance::prop_Locked(
	"Locked",
	"Behavior",
	&RBX::PartInstance::getPartLocked,
	&RBX::PartInstance::setPartLocked
);

const Reflection::PropDescriptor<PartInstance, G3D::Color3> PartInstance::prop_Color(
	"Color",
	"Appearance",
	&RBX::PartInstance::getColor3,
	&RBX::PartInstance::setColor3
);

const Reflection::PropDescriptor<PartInstance, BrickColor> PartInstance::prop_BrickColor(
	"BrickColor",
	"Appearance",
	&RBX::PartInstance::getColor,
	&RBX::PartInstance::setColor
);

const Reflection::PropDescriptor<PartInstance, bool> PartInstance::prop_CanCollide(
	"CanCollide",
	"Behavior",
	&RBX::PartInstance::getCanCollide,
	&RBX::PartInstance::setCanCollide
);

const Reflection::PropDescriptor<PartInstance, bool> PartInstance::prop_Anchored(
	"Anchored",
	"Behavior",
	&RBX::PartInstance::getAnchored,
	&RBX::PartInstance::setAnchored
);

static Reflection::EnumPropDescriptor<PartInstance, PartInstance::FormFactor> prop_formFactor(
	"FormFactor",
	category_Part,
	&RBX::PartInstance::getFormFactor,
	&RBX::PartInstance::setFormFactorXml,
	Reflection::PropertyDescriptor::STREAMING
);

static Reflection::PropDescriptor<PartInstance, bool> prop_Dragging(
	"DraggingV1",
	"Behavior",
	&RBX::PartInstance::getDragging,
	&RBX::PartInstance::setDragging,
	Reflection::PropertyDescriptor::STREAMING
);

// FUNCTION: WEBSERVICE 0x10059c10
bool PartInstance::askSetParent(const Instance* instance) const
{
	return dynamic_cast<const ModelInstance*>(instance) != NULL;
}

// FUNCTION: WEBSERVICE 0x1009a8d0
void PartInstance::onChildAdded(Instance* child)
{
	PVInstance::onChildAdded(child);
}

// STUB: WEBSERVICE 0x1009ae00
void PartInstance::setName(const std::string& value)
{
	STUB(0x1009ae00);
}

// FUNCTION: WEBSERVICE 0x1009ae40
bool PartInstance::nonNullInWorkspace(shared_ptr<PartInstance> part)
{
	return part.get() != NULL && part->myWorld != NULL;
}

// FUNCTION: WEBSERVICE 0x1009ae90 FOLDED
PartInstance* PartInstance::fromPrimitive(Primitive* primitive)
{
	if (primitive != NULL) {
		IMoving* owner = primitive->getOwner();

		if (owner != NULL) {
			return static_cast<PartInstance*>(owner);
		}
	}

	return NULL;
}

// FUNCTION: WEBSERVICE 0x1009ae90 FOLDED
const PartInstance* PartInstance::fromPrimitiveConst(const Primitive* primitive)
{
	if (primitive != NULL) {
		IMoving* owner = primitive->getOwner();

		if (owner != NULL) {
			return static_cast<const PartInstance*>(owner);
		}
	}

	return NULL;
}

// STUB: WEBSERVICE 0x1009aef0
bool PartInstance::computeSurfacesNeedAdorn() const
{
	STUB(0x1009aef0);
	return false;
}

// STUB: WEBSERVICE 0x1009af50
void PartInstance::legacyTraverseState(const CoordinateFrame& parentState)
{
	STUB(0x1009af50);
}

// STUB: WEBSERVICE 0x1009af90
bool PartInstance::isControllable() const
{
	STUB(0x1009af90);
	return false;
}

// FUNCTION: WEBSERVICE 0x1009b080
const CoordinateFrame& PartInstance::getCoordinateFrame() const
{
	return primitive->getCoordinateFrame();
}

// FUNCTION: WEBSERVICE 0x1009b0b0
float PartInstance::getMass()
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

// FUNCTION: WEBSERVICE 0x1009b1b0
Extents PartInstance::getExtentsWorld() const
{
	return primitive->getExtentsWorld();
}

// FUNCTION: WEBSERVICE 0x1009b1d0
Extents PartInstance::getExtentsLocal() const
{
	const Vector3 corner = primitive->getGeometry()->getGridSize() * 0.5f;

	return Extents(-corner, corner);
}

// STUB: WEBSERVICE 0x1009b610
Vector3 PartInstance::uiToXmlSize(const Vector3& uiSize) const
{
	STUB(0x1009b610);
	return uiSize;
}

// STUB: WEBSERVICE 0x1009b6e0
Part PartInstance::computePersistentPart() const
{
	STUB(0x1009b6e0);
	return Part();
}

// STUB: WEBSERVICE 0x1009b780
bool PartInstance::shouldRender3dAdorn() const
{
	STUB(0x1009b780);
	return false;
}

// STUB: WEBSERVICE 0x1009bf50
bool PartInstance::reportTouches() const
{
	STUB(0x1009bf50);
	return false;
}

// STUB: WEBSERVICE 0x1009bff0
void PartInstance::onAncestorChanged(const AncestorChanged& event)
{
	STUB(0x1009bff0);
}

// FUNCTION: WEBSERVICE 0x1009c090
void PartInstance::onParentControllerChanged()
{
	PVInstance::onParentControllerChanged();

	PersistentPart.setDirty();

	primitive->setController(getTopPVController());
}

// STUB: WEBSERVICE 0x1009c0c0
void PartInstance::render3dSelect(Adorn* adorn, SelectState selectState)
{
	STUB(0x1009c0c0);
}

// STUB: WEBSERVICE 0x1009c0f0
bool PartInstance::hitTest(const G3D::Ray& worldRay, G3D::Vector3& worldHitPoint)
{
	STUB(0x1009c0f0);
	return false;
}

// STUB: WEBSERVICE 0x1009cc10
void PartInstance::onCanAggregateChanged(bool canAggregate)
{
	STUB(0x1009cc10);
}

// STUB: WEBSERVICE 0x1009cc30
void PartInstance::safeMove()
{
	STUB(0x1009cc30);
}

// STUB: WEBSERVICE 0x1009d150
PartInstance::~PartInstance()
{
	STUB(0x1009d150);
}

// FUNCTION: WEBSERVICE 0x1009d2e0
PartInstance* PartInstance::getPrimaryPart()
{
	return this;
}

const PartInstance* PartInstance::getPrimaryPartConst() const
{
	STUB(0x1009d2e1);
	return NULL;
}

// FUNCTION: WEBSERVICE 0x1009d3b0
const Primitive* PartInstance::getBiggestPrimitive() const
{
	return primitive.get();
}

// STUB: WEBSERVICE 0x1009d3c0
const CoordinateFrame PartInstance::getLocation() const
{
	STUB(0x1009d3c0);
	return CoordinateFrame();
}

// STUB: WEBSERVICE 0x1009d7d0
void PartInstance::getCameraIgnorePrimitives(std::vector<const Primitive*>& primitives)
{
	STUB(0x1009d7d0);
}

namespace Reflection {

// FUNCTION: WEBSERVICE 0x1009daf0
template <>
EnumDesc<PartInstance::FormFactor>::EnumDesc() : EnumDescriptor("FormFactor", typeid(PartInstance::FormFactor))
{
	addPair(PartInstance::SYMETRIC, "Symmetric");
	addPair(PartInstance::BRICK, "Brick");
	addPair(PartInstance::PLATE, "Plate");

	addLegacyName("Block", PartInstance::BRICK);
}

} // namespace Reflection

// STUB: WEBSERVICE 0x1009f220
PartInstance::PartInstance()
	: DescribedCreatable<PartInstance, PVInstance, sPart>(sPart),
	  PersistentPart(this, &PartInstance::computePersistentPart),
	  SurfacesNeedAdorn(this, &PartInstance::computeSurfacesNeedAdorn)
{
	STUB(0x1009f220);
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
// STUB: WEBSERVICE 0x1009fe60
void PartInstance::setColor3(const G3D::Color3& value)
{
	BrickColor closest = BrickColor::closest(value);

	if (closest != color) {
		color = closest;

		raisePropertyChanged(prop_Color);
		raisePropertyChanged(prop_BrickColor);
	}
}

// STUB: WEBSERVICE 0x1009fec0
void PartInstance::onCameraNear(float distance)
{
	STUB(0x1009fec0);
}

// STUB: WEBSERVICE 0x1009ff60
void PartInstance::render3dAdorn(Adorn* adorn)
{
	STUB(0x1009ff60);
}

} // namespace RBX
