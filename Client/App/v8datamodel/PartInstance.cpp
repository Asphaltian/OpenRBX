#include "v8datamodel/PartInstance.h"

#include "appdraw/Draw.h"
#include "appdraw/DrawAdorn.h"
#include "appdraw/HitTest.h"
#include "rbxgraphics/Adorn.h"
#include "tool/Dragger.h"
#include "util/Math.h"
#include "util/standardout.h"
#include "v8datamodel/ModelInstance.h"
#include "v8datamodel/Workspace.h"
#include "v8kernel/Body.h"
#include "v8world/Assembly2.h"
#include "v8world/Geometry.h"
#include "v8world/MotorJoint.h"
#include "v8world/RigidJoint.h"
#include "v8world/World.h"

#include <G3D/AABox.h>
#include <G3D/CollisionDetection.h>
#include <G3D/Ray.h>
#include <G3D/Sphere.h>

namespace RBX {

char sPart[] = "Part";

const char* category_Part = "Part";

bool PartInstance::highlightSleepParts;
bool PartInstance::highlightAwakeParts;
bool PartInstance::showAnchoredParts;
bool PartInstance::showPartCoord;
bool PartInstance::showUnalignedParts;
bool PartInstance::showSpanningTree;

static const Name& namePart = Name::declare(sPart, 1000);
static const Name& nameCFrame = Name::declare("CFrame", 1001);
static const Name& namePosition = Name::declare("Position", 1002);

const Reflection::PropDescriptor<PartInstance, CoordinateFrame> PartInstance::prop_CFrame(
	"CFrame", "Data", &PartInstance::getCoordinateFrame, &PartInstance::setCoordinateFrame
);

static Reflection::PropDescriptor<PartInstance, G3D::Vector3> prop_PositionUi(
	"Position",
	"Data",
	&PartInstance::getTranslationUi,
	&PartInstance::setTranslationUi,
	Reflection::PropertyDescriptor::UI
);

static Reflection::PropDescriptor<PartInstance, G3D::Vector3> prop_Velocity(
	"Velocity", "Data", &PartInstance::getLinearVelocity, &PartInstance::setLinearVelocity
);

static Reflection::PropDescriptor<PartInstance, G3D::Vector3> prop_RotVelocity(
	"RotVelocity", "Data", &PartInstance::getRotationalVelocity, &PartInstance::setRotationalVelocity
);

static Reflection::BoundFuncDesc<PartInstance, void(), 0> desc_unjoin(
	&PartInstance::destroyJoints, "BreakJoints", Reflection::FunctionDescriptor::AnyCaller
);

static Reflection::BoundFuncDesc<PartInstance, void(), 0> desc_join(
	&PartInstance::join, "MakeJoints", Reflection::FunctionDescriptor::AnyCaller
);

static Reflection::BoundFuncDesc<PartInstance, float(), 0> desc_getMass(
	&PartInstance::getMass, "GetMass", Reflection::FunctionDescriptor::AnyCaller
);

const Reflection::EnumPropDescriptor<PartInstance, Part::PartType> PartInstance::prop_shapeXml(
	"shape",
	category_Part,
	&PartInstance::getPartType,
	&PartInstance::setPartTypeXml,
	Reflection::PropertyDescriptor::STREAMING
);

static Reflection::EnumPropDescriptor<PartInstance, Part::PartType> prop_shapeUi(
	"Shape", category_Part, &PartInstance::getPartType, &PartInstance::setPartTypeUi, Reflection::PropertyDescriptor::UI
);

const Reflection::PropDescriptor<PartInstance, G3D::Color3> PartInstance::prop_Color(
	"Color", "Appearance", &PartInstance::getColor3, &PartInstance::setColor3, Reflection::PropertyDescriptor::LEGACY
);

const Reflection::PropDescriptor<PartInstance, BrickColor> PartInstance::prop_BrickColor(
	"BrickColor", "Appearance", &PartInstance::getColor, &PartInstance::setColor
);

const Reflection::PropDescriptor<PartInstance, float> PartInstance::prop_Transparency(
	"Transparency", "Appearance", &PartInstance::getTransparencyXml, &PartInstance::setTransparency
);

const Reflection::PropDescriptor<PartInstance, float> PartInstance::prop_Reflectance(
	"Reflectance", "Appearance", &PartInstance::getReflectance, &PartInstance::setReflectance
);

const Reflection::PropDescriptor<PartInstance, bool> PartInstance::prop_Locked(
	"Locked", "Behavior", &PartInstance::getPartLocked, &PartInstance::setPartLocked
);

const Reflection::PropDescriptor<PartInstance, bool> PartInstance::prop_Anchored(
	"Anchored", "Behavior", &PartInstance::getAnchored, &PartInstance::setAnchored
);

const Reflection::PropDescriptor<PartInstance, bool> PartInstance::prop_CanCollide(
	"CanCollide", "Behavior", &PartInstance::getCanCollide, &PartInstance::setCanCollide
);

const Reflection::PropDescriptor<PartInstance, float> PartInstance::prop_RenderImportance(
	"RenderImportance",
	"Behavior",
	&PartInstance::getRenderImportance,
	&PartInstance::setRenderImportance,
	Reflection::PropertyDescriptor::LEGACY
);

static Reflection::PropDescriptor<PartInstance, G3D::Vector3> prop_SizeUi(
	"Size",
	category_Part,
	&PartInstance::getPartSizeUi,
	&PartInstance::setPartSizeUi,
	Reflection::PropertyDescriptor::UI
);

const Reflection::PropDescriptor<PartInstance, G3D::Vector3> PartInstance::prop_Size(
	"size",
	category_Part,
	&PartInstance::getPartSizeXml,
	&PartInstance::setPartSizeXml,
	Reflection::PropertyDescriptor::STREAMING
);

static Reflection::EnumPropDescriptor<PartInstance, PartInstance::FormFactor> prop_FormFactorUi(
	"formFactor",
	category_Part,
	&PartInstance::getFormFactor,
	&PartInstance::setFormFactorUi,
	Reflection::PropertyDescriptor::UI
);

static Reflection::EnumPropDescriptor<PartInstance, PartInstance::FormFactor> prop_formFactor(
	"FormFactor",
	category_Part,
	&PartInstance::getFormFactor,
	&PartInstance::setFormFactorXml,
	Reflection::PropertyDescriptor::STREAMING
);

static Reflection::PropDescriptor<PartInstance, float> prop_Elasticity(
	"Elasticity", category_Part, &PartInstance::getElasticity, &PartInstance::setElasticity
);

static Reflection::PropDescriptor<PartInstance, float> prop_Friction(
	"Friction", category_Part, &PartInstance::getFriction, &PartInstance::setFriction
);

Reflection::SignalDesc<PartInstance, void(shared_ptr<Instance>)> PartInstance::event_Touched("Touched", "part");

static Reflection::PropDescriptor<PartInstance, bool> prop_Dragging(
	"DraggingV1",
	"Behavior",
	&PartInstance::getDragging,
	&PartInstance::setDragging,
	Reflection::PropertyDescriptor::STREAMING
);

bool PartInstance::askSetParent(const Instance* instance) const
{
	return dynamic_cast<const ModelInstance*>(instance) != NULL;
}

void PartInstance::onChildAdded(Instance* child)
{
	PVInstance::onChildAdded(child);
}

void PartInstance::writeResizeData(XmlState* state)
{
	state->addProperty(Reflection::Property(prop_Size, this));
}

Surfaces& PartInstance::getSurfaces()
{
	return surfaces;
}

const Surfaces& PartInstance::getSurfaces() const
{
	return surfaces;
}

void PartInstance::setName(const std::string& value)
{
	Instance::setName(value);

	if (value == "Torso") {
		getPrimitive()->setSizeMultiplier(5);
	}
	else {
		getPrimitive()->setSizeMultiplier(1);
	}
}

bool PartInstance::lockedInPlace() const
{
	bool top = false;
	bool bottom = false;
	for (Joint* joint = primitive->getFirstJoint(); joint; joint = primitive->getNextJoint(joint)) {
		if (RigidJoint::isRigidJoint(joint) || MotorJoint::isMotorJoint(joint)) {
			NormalId normalId = primitive.get() == joint->getPrimitive(0)
									? Matrix3ToNormalId(joint->getJointCoord(0).rotation)
									: normalIdOpposite(Matrix3ToNormalId(joint->getJointCoord(1).rotation));
			if (normalId == NORM_Y) {
				top = true;
			}
			else if (normalId == NORM_Y_NEG) {
				bottom = true;
			}
		}
	}
	return top && bottom;
}

bool PartInstance::nonNullInWorkspace(shared_ptr<PartInstance> part)
{
	return part.get() != NULL && part->myWorld != NULL;
}

PartInstance* PartInstance::fromPrimitive(Primitive* p)
{
	if (p != NULL) {
		IMoving* owner = p->getOwner();

		if (owner != NULL) {
			return static_cast<PartInstance*>(owner);
		}
	}

	return NULL;
}

const PartInstance* PartInstance::fromPrimitiveConst(const Primitive* p)
{
	if (p != NULL) {
		IMoving* owner = p->getOwner();

		if (owner != NULL) {
			return static_cast<const PartInstance*>(owner);
		}
	}

	return NULL;
}

void PartInstance::destroyJoints()
{
	if (World* world = Workspace::getWorldIfInWorkspace(this)) {
		world->destroyJoints(getPrimitive());
	}
}

void PartInstance::join()
{
	if (World* world = Workspace::getWorldIfInWorkspace(this)) {
		world->createJoints(getPrimitive());
	}
}

bool PartInstance::computeSurfacesNeedAdorn() const
{
	for (int i = 0; i < 6; i++) {
		const Surface& surface = surfaces[(NormalId) i];

		if (surface.getSurfaceType() == ROTATE || surface.getSurfaceType() == ROTATE_P ||
			surface.getSurfaceType() == ROTATE_V) {
			return true;
		}
	}

	return false;
}

void PartInstance::legacyTraverseState(const CoordinateFrame& parentState)
{
	CoordinateFrame myState = parentState * *legacyOffset;

	primitive->setGridCorner(myState);
}

bool PartInstance::isControllable() const
{
	for (int i = 0; i < 6; i++) {
		const Surface& surface = surfaces[(NormalId) i];

		if ((surface.getSurfaceType() == ROTATE_P || surface.getSurfaceType() == ROTATE_V) &&
			surface.isControllable()) {
			return true;
		}
	}

	return false;
}

void PartInstance::onSurfaceChanged(NormalId surfId)
{
	PersistentPart.setDirty();
	IsControllable.setDirty();
	SurfacesNeedAdorn.setDirty();
	shouldRenderSetDirty();
}

void PartInstance::setPhysics(const CoordinateFrame& cf, const Velocity& vel)
{
	primitive->getBody()->setVelocity(vel);

	if (cf != primitive->getCoordinateFrame()) {
		primitive->setCoordinateFrame(cf);
		PersistentPart.setDirty();
		onExtentsChanged();
	}
}

const CoordinateFrame& PartInstance::getCoordinateFrame() const
{
	return primitive->getCoordinateFrame();
}

const Vector3& PartInstance::getTranslationUi() const
{
	return primitive->getBody()->getCoordinateFrame().translation;
}

float PartInstance::getMass()
{
	return primitive->getBody()->getMass();
}

const Velocity& PartInstance::getVelocity() const
{
	return primitive->getBody()->getVelocity();
}

const Vector3& PartInstance::getLinearVelocity() const
{
	return primitive->getBody()->getVelocity().linear;
}

const Vector3& PartInstance::getRotationalVelocity() const
{
	return primitive->getBody()->getVelocity().rotational;
}

const Vector3& PartInstance::getPartSizeXml() const
{
	return primitive->getGeometry()->getGridSize();
}

Vector3 PartInstance::getPartSizeUi() const
{
	return primitive->getGeometry()->getGridSize();
}

bool PartInstance::getDragging() const
{
	return primitive->getDragging();
}

bool PartInstance::getCanCollide() const
{
	if (!primitive->getDragging() && primitive->getCanCollide()) {
		return true;
	}

	return false;
}

bool PartInstance::getAnchored() const
{
	return primitive->getAnchorObject() != NULL;
}

float PartInstance::getFriction() const
{
	return primitive->getFriction();
}

float PartInstance::getElasticity() const
{
	return primitive->getElasticity();
}

Extents PartInstance::getExtentsWorld() const
{
	return primitive->getExtentsWorld();
}

Extents PartInstance::getExtentsLocal() const
{
	const Vector3 corner = primitive->getGeometry()->getGridSize() * 0.5f;

	return Extents(-corner, corner);
}

Surface* PartInstance::getSurface(const G3D::Ray& worldRay, int& surfaceId)
{
	const CoordinateFrame& cframe = primitive->getCoordinateFrame();
	G3D::Ray localRay = cframe.toObjectSpace(worldRay);
	const Vector3 corner = primitive->getGeometry()->getGridSize() * 0.5f;
	Extents extents(-corner, corner);
	Vector3 location;
	G3D::CollisionDetection::collisionLocationForMovingPointFixedAABox(
		localRay.origin, localRay.direction, G3D::AABox(extents.min(), extents.max()), location
	);

	for (int i = 0; i < 3; i++) {
		if (location[i] == extents.max()[i]) {
			surfaceId = i;
			return &surfaces[(NormalId) i];
		}
		if (location[i] == extents.min()[i]) {
			surfaceId = i + 3;
			return &surfaces[(NormalId) (i + 3)];
		}
	}
	return NULL;
}

Vector3 PartInstance::xmlToUiSize(const Vector3& size) const
{
	Vector3 uiSize = size;

	switch (formFactor) {
	case BRICK:
		uiSize.y = size.y / 1.2f;
		break;
	case PLATE:
		uiSize.y = size.y / 0.4f;
		break;
	}

	return uiSize;
}

namespace Reflection {

template <>
const Type& Type::singleton<CoordinateFrame>()
{
	static Type type("CoordinateFrame", typeid(CoordinateFrame));

	return type;
}

} // namespace Reflection

CoordinateFrame PartInstance::worldSnapLocation() const
{
	Vector3 halfSize = primitive->getGeometry()->getGridSize() * 0.5f;

	Vector3 snapInPart;
	snapInPart.y = -halfSize.y;
	snapInPart.x = halfSize.x - G3D::iRound(floor(halfSize.x));
	snapInPart.z = halfSize.z - G3D::iRound(floor(halfSize.z));

	CoordinateFrame localSnapLocation(snapInPart);

	return primitive->getCoordinateFrame() * localSnapLocation;
}

bool PartInstance::aligned() const
{
	CoordinateFrame myCoord = worldSnapLocation();

	CoordinateFrame myCoordSnapped = Math::snapToGrid(myCoord, Dragger::dragSnap());

	return Math::fuzzyEq(myCoord, myCoordSnapped, 0.00001f, 0.00001f);
}

Vector3 PartInstance::uiToXmlSize(const Vector3& uiSize) const
{
	Vector3 gridUiSize = Math::iRoundVector3(uiSize.max(Vector3(1, 1, 1)));

	Vector3 xmlSize = gridUiSize;

	switch (formFactor) {
	case BRICK:
		xmlSize.y *= 1.2f;
		break;
	case PLATE:
		xmlSize.y *= 0.4f;
		break;
	}

	return xmlSize;
}

Part PartInstance::computePersistentPart() const
{
	return Part(partType, getPartSizeXml(), G3D::Color4(getColor3(), alpha()), surfaces.surf6(), getCoordinateFrame());
}

bool PartInstance::shouldRender3dAdorn() const
{
	return partType == Part::CYLINDER_PART || SurfacesNeedAdorn || highlightSleepParts || highlightAwakeParts ||
		   showPartCoord || showAnchoredParts || showUnalignedParts || showSpanningTree;
}

bool PartInstance::getLocked(Instance* instance)
{
	if (PartInstance* part = dynamic_cast<PartInstance*>(instance)) {
		return part->getPartLocked();
	}

	for (unsigned int i = 0; i < instance->numChildren(); i++) {
		if (getLocked(instance->getChild(i))) {
			return true;
		}
	}

	return false;
}

bool PartInstance::reportTouches() const
{
	return !event_Touched.empty(this);
}

const Part& PartInstance::getPart() const
{
	Part& part = const_cast<Part&>(PersistentPart.getValueRef());

	part.coordinateFrame = getPrimitive()->getCoordinateFrame();

	return part;
}

void PartInstance::onAncestorChanged(const AncestorChanged& event)
{
	Instance::onAncestorChanged(event);

	World* world = Workspace::getWorldIfInWorkspace(this);

	if (world != myWorld) {
		if (myWorld != NULL) {
			setMovingManager(NULL);

			myWorld->removePrimitive(getPrimitive());
		}

		myWorld = world;

		if (world != NULL) {
			world->insertPrimitive(getPrimitive());

			setMovingManager(Workspace::getMyWorkspaceFast(this));

			getPrimitive()->setController(getTopPVController());
		}
	}
}

void PartInstance::onParentControllerChanged()
{
	PVInstance::onParentControllerChanged();

	PersistentPart.setDirty();

	primitive->setController(getTopPVController());
}

void PartInstance::render3dSelect(Adorn* adorn, SelectState selectState)
{
	Draw::selectionBox(getPart(), adorn, selectState);
}

bool PartInstance::hitTest(const G3D::Ray& worldRay, G3D::Vector3& worldHitPoint)
{
	G3D::Ray rayInPartCoords = getCoordinateFrame().toObjectSpace(worldRay);

	Vector3 hitPointInPartCoords;

	bool hit = HitTest::hitTest(getPart(), rayInPartCoords, hitPointInPartCoords, 1.0f);

	if (hit) {
		worldHitPoint = getCoordinateFrame().pointToWorldSpace(hitPointInPartCoords);
		return true;
	}

	return false;
}

static float readFloat(const XmlElement* element, const Name& tag)
{
	float value;
	element->findFirstChildByTag(tag)->getValue(value);
	return value;
}

namespace Reflection {

template <>
void TypedPropertyDescriptor<CoordinateFrame>::readValue(
	DescribedBase* instance, const XmlElement* element, IReferenceBinder& binder
) const
{
	if (!element->isXsiNil()) {
		CoordinateFrame c;
		c.translation.x = readFloat(element, tag_X);
		c.translation.y = readFloat(element, tag_Y);
		c.translation.z = readFloat(element, tag_Z);
		c.rotation[0][0] = readFloat(element, tag_R00);
		c.rotation[0][1] = readFloat(element, tag_R01);
		c.rotation[0][2] = readFloat(element, tag_R02);
		c.rotation[1][0] = readFloat(element, tag_R10);
		c.rotation[1][1] = readFloat(element, tag_R11);
		c.rotation[1][2] = readFloat(element, tag_R12);
		c.rotation[2][0] = readFloat(element, tag_R20);
		c.rotation[2][1] = readFloat(element, tag_R21);
		c.rotation[2][2] = readFloat(element, tag_R22);
		setValue(instance, c);
	}
}

template <>
void TypedPropertyDescriptor<CoordinateFrame>::writeValue(const DescribedBase* instance, XmlElement* element) const
{
	CoordinateFrame c = getValue(instance);
	element->addChild(new XmlElement(tag_X, c.translation.x));
	element->addChild(new XmlElement(tag_Y, c.translation.y));
	element->addChild(new XmlElement(tag_Z, c.translation.z));
	element->addChild(new XmlElement(tag_R00, c.rotation[0][0]));
	element->addChild(new XmlElement(tag_R01, c.rotation[0][1]));
	element->addChild(new XmlElement(tag_R02, c.rotation[0][2]));
	element->addChild(new XmlElement(tag_R10, c.rotation[1][0]));
	element->addChild(new XmlElement(tag_R11, c.rotation[1][1]));
	element->addChild(new XmlElement(tag_R12, c.rotation[1][2]));
	element->addChild(new XmlElement(tag_R20, c.rotation[2][0]));
	element->addChild(new XmlElement(tag_R21, c.rotation[2][1]));
	element->addChild(new XmlElement(tag_R22, c.rotation[2][2]));
}

} // namespace Reflection

void PartInstance::onCanAggregateChanged(bool canAggregate)
{
	Notifier<PartInstance, CanAggregateChanged>::raise(canAggregate);

	shouldRenderSetDirty();
}

void PartInstance::safeMove()
{
	if (World* world = Workspace::getWorldIfInWorkspace(this)) {
		G3D::Array<Primitive*> temp;

		temp.append(primitive.get());

		Dragger::safeMoveNoDrop(temp, Vector3::zero(), world->getContactManager());
	}
}

void PartInstance::primitivesToParts(
	const G3D::Array<Primitive*>& primitives, G3D::Array<shared_ptr<PartInstance> >& parts
)
{
	for (int i = 0; i < primitives.size(); i++) {
		PartInstance* part = fromPrimitive(primitives[i]);
		parts.append(shared_from(part));
	}
}

PartInstance::~PartInstance()
{
}

PartInstance* PartInstance::getPrimaryPart()
{
	return this;
}

const PartInstance* PartInstance::getPrimaryPartConst() const
{
	return this;
}

const Primitive* PartInstance::getBiggestPrimitive() const
{
	return primitive.get();
}

const CoordinateFrame PartInstance::getLocation() const
{
	return primitive->getCoordinateFrame();
}

void PartInstance::getCameraIgnorePrimitives(std::vector<const Primitive*>& primitives)
{
	primitives.push_back(getPrimitive());
}

namespace Reflection {

template <>
EnumDesc<PartInstance::FormFactor>::EnumDesc() : EnumDescriptor("FormFactor", typeid(PartInstance::FormFactor))
{
	addPair(PartInstance::SYMETRIC, "Symmetric");
	addPair(PartInstance::BRICK, "Brick");
	addPair(PartInstance::PLATE, "Plate");

	addLegacyName("Block", PartInstance::BRICK);
}

} // namespace Reflection

void PartInstance::findParts(Instance* instance, std::vector<boost::weak_ptr<PartInstance> >& parts)
{
	if (PartInstance* part = dynamic_cast<PartInstance*>(instance)) {
		parts.push_back(shared_from(part));
	}

	for (unsigned int i = 0; i < instance->numChildren(); i++) {
		findParts(instance->getChild(i), parts);
	}
}

void PartInstance::onGuidChanged()
{
	getPrimitive()->setGuid(getGuid());
}

PartInstance::PartInstance()
	: DescribedCreatable<PartInstance, PVInstance, sPart>("Part"), partType(Part::BLOCK_PART), formFactor(BRICK),
	  transparency(0.0f), reflectance(0.0f), locked(false), surfaces(this), renderImportance(0.0f),
	  primitive(new Primitive(Geometry::GEOMETRY_BLOCK)), myWorld(NULL), alphaModifier(1.0f),
	  PersistentPart(this, &PartInstance::computePersistentPart),
	  SurfacesNeedAdorn(this, &PartInstance::computeSurfacesNeedAdorn)
{
	primitive->setSurfaceType(NORM_Y, STUDS);
	primitive->setSurfaceType(NORM_Y_NEG, INLET);
	primitive->setGridSize(Vector3(4.0f, 1.2f, 2.0f));
	primitive->setFriction(0.3f);
	primitive->setElasticity(0.5f);
	primitive->setAnchor(false);
	primitive->setCanCollide(true);
	primitive->setOwner(this);

	primitive->setGuid(getGuid());
}

void PartInstance::onTouchThisStep(shared_ptr<PartInstance> other)
{
	event_Touched.fire(this, other);
}

void PartInstance::setCoordinateFrame(const CoordinateFrame& value)
{
	if (value != primitive->getCoordinateFrame()) {
		CoordinateFrame orthoValue = value;
		Math::orthonormalizeIfNecessary(orthoValue.rotation);

		if (orthoValue != primitive->getCoordinateFrame()) {
			primitive->setCoordinateFrame(orthoValue);
			PersistentPart.setDirty();
			onExtentsChanged();
		}

		raisePropertyChanged(prop_CFrame);
		raisePropertyChanged(prop_PositionUi);
	}
}

void PartInstance::setTranslationUi(const Vector3& set)
{
	CoordinateFrame current = getCoordinateFrame();

	if (current.translation != set) {
		current.translation = set;
		destroyJoints();
		setCoordinateFrame(current);
		safeMove();
		join();
	}
}

void PartInstance::setLinearVelocity(const Vector3& set)
{
	Velocity vel = getVelocity();

	if (vel.linear != set) {
		vel.linear = set;
		primitive->getBody()->setVelocity(vel);
		raisePropertyChanged(prop_Velocity);
	}
}

void PartInstance::setRotationalVelocity(const Vector3& set)
{
	Velocity vel = getVelocity();

	if (vel.rotational != set) {
		vel.rotational = set;
		primitive->getBody()->setVelocity(vel);
		raisePropertyChanged(prop_RotVelocity);
	}
}

void PartInstance::setPartSizeXml(const Vector3& rbxSize)
{
	if (rbxSize != getPartSizeXml()) {
		Vector3 newRbxSize = partType == Part::BLOCK_PART ? rbxSize : rbxSize.xxx();

		if (legacyOffset) {
			CoordinateFrame oldCorner = primitive->getGridCorner();

			primitive->setGridSize(newRbxSize);
			primitive->setGridCorner(oldCorner);
		}
		else {
			primitive->setGridSize(newRbxSize);
		}

		raisePropertyChanged(prop_Size);
		raisePropertyChanged(prop_SizeUi);

		PersistentPart.setDirty();

		onExtentsChanged();
	}
}

void PartInstance::setPartSizeUi(const Vector3& uiSize)
{
	World* world = Workspace::getWorldIfInWorkspace(this);

	if (world != NULL) {
		world->destroyJoints(primitive.get());
	}

	setPartSizeXml(uiToXmlSize(xmlToUiSize(uiSize)));

	safeMove();

	world = Workspace::getWorldIfInWorkspace(this);

	if (world != NULL) {
		world->createJoints(primitive.get());
	}
}

void PartInstance::setDragging(bool value)
{
	if (value != primitive->getDragging()) {
		primitive->setDragging(value);
		raisePropertyChanged(prop_Dragging);
	}
}

void PartInstance::setRenderImportance(float value)
{
	if (value != renderImportance) {
		renderImportance = value;
		raisePropertyChanged(prop_RenderImportance);
	}
}

void PartInstance::setCanCollide(bool value)
{
	if (value != getCanCollide()) {
		primitive->setCanCollide(value);
		raisePropertyChanged(prop_CanCollide);
	}
}

void PartInstance::setAnchored(bool value)
{
	if (value != getAnchored()) {
		primitive->setAnchor(value);
		raisePropertyChanged(prop_Anchored);
	}
}

void PartInstance::setPartLocked(bool value)
{
	if (value != locked) {
		locked = value;
		raisePropertyChanged(prop_Locked);
	}
}

void PartInstance::setLocked(Instance* instance, bool value)
{
	if (PartInstance* part = dynamic_cast<PartInstance*>(instance)) {
		part->setPartLocked(value);
	}
	else {
		for (unsigned int i = 0; i < instance->numChildren(); i++) {
			setLocked(instance->getChild(i), value);
		}
	}
}

void PartInstance::setTransparency(float value)
{
	if (value != transparency) {
		transparency = value;
		raisePropertyChanged(prop_Transparency);
	}
}

void PartInstance::setAlphaModifier(float value)
{
	if (value != alphaModifier) {
		alphaModifier = value;
		raisePropertyChanged(prop_Transparency);
	}
}

void PartInstance::setReflectance(float value)
{
	if (value != reflectance) {
		reflectance = value;
		raisePropertyChanged(prop_Reflectance);
	}
}

void PartInstance::setColor(BrickColor value)
{
	if (value.number != color.number) {
		color = value;
		raisePropertyChanged(prop_BrickColor);
		raisePropertyChanged(prop_Color);
	}
}

void PartInstance::setFormFactorXml(FormFactor value)
{
	if (value != formFactor) {
		formFactor = value;
		raisePropertyChanged(prop_formFactor);
	}
}

void PartInstance::setFormFactorUi(FormFactor value)
{
	if (partType != Part::BLOCK_PART) {
		value = SYMETRIC;
	}

	if (value != formFactor) {
		destroyJoints();
		setFormFactorXml(value);

		Vector3 uiSize = getPartSizeUi();

		uiSize.y = std::max(1.0f, (float) G3D::iRound(uiSize.y));

		setPartSizeUnjoined(uiSize);
		join();
	}
}

void PartInstance::setFriction(float friction)
{
	friction = G3D::clamp(friction, 0.0f, 2.0f);

	if (friction != primitive->getFriction()) {
		primitive->setFriction(friction);
		raisePropertyChanged(prop_Friction);
	}
}

void PartInstance::setElasticity(float elasticity)
{
	elasticity = G3D::clamp(elasticity, 0.0f, 1.0f);

	if (elasticity != primitive->getElasticity()) {
		primitive->setElasticity(elasticity);
		raisePropertyChanged(prop_Elasticity);
	}
}

bool PartInstance::resize(NormalId p_unk0x04, int p_unk0x08)
{
	if (p_unk0x08 != 0) {
		Vector3 oldSizeXml = getPartSizeXml();
		CoordinateFrame oldCoord = getCoordinateFrame();
		Vector3 deltaSizeUi;
		float posNeg = p_unk0x04 < 3 ? 1.0f : -1.0f;

		if (partType == Part::BLOCK_PART) {
			deltaSizeUi[p_unk0x04 % 3] = (float) p_unk0x08;
		}
		else {
			deltaSizeUi = Vector3((float) p_unk0x08, (float) p_unk0x08, (float) p_unk0x08);
		}

		Vector3 newSizeUi = xmlToUiSize(oldSizeXml) + deltaSizeUi;
		Vector3 newSizeXml = uiToXmlSize(newSizeUi);
		Vector3 deltaSizeXml = newSizeXml - oldSizeXml;
		Vector3 offset = oldCoord.rotation * deltaSizeXml;
		Vector3 newTranslation = offset * 0.5f * posNeg + oldCoord.translation;
		CoordinateFrame newCoord(oldCoord.rotation, newTranslation);

		setCoordinateFrame(newCoord);
		setPartSizeXml(uiToXmlSize(newSizeUi));
		safeMove();

		if (Math::fuzzyEq(getCoordinateFrame(), newCoord, 0.00001f, 0.00001f)) {
			return true;
		}

		setCoordinateFrame(oldCoord);
		setPartSizeXml(oldSizeXml);
	}

	return false;
}

void PartInstance::setColor3(const G3D::Color3& value)
{
	setColor(BrickColor::closest(value));
}

void PartInstance::onCameraNear(float distance)
{
	if (distance < 3.0f) {
		setAlphaModifier(0.0f);
	}
	else if (distance < 6.0f) {
		setAlphaModifier(0.5f);
	}
	else {
		setAlphaModifier(1.0f);
	}
}

void PartInstance::render3dAdorn(Adorn* adorn)
{
	if (partType == Part::CYLINDER_PART || SurfacesNeedAdorn) {
		Draw::partAdorn(getPart(), adorn, Controller::controllerTypeToColor(getTopPVController()->getControllerType()));
	}

	if (highlightSleepParts && primitive->getAssembly()) {
		Sim::AssemblyState status = primitive->getAssembly()->getRootAssemblyConst()->getSleepStatus();

		if (status != Sim::ANCHORED && status != Sim::AWAKE) {
			Color3 sleepColor;

			switch (status) {
			case Sim::RECURSIVE_WAKE_PENDING:
			case Sim::WAKE_PENDING:
				sleepColor = Color3::purple();
				break;
			case Sim::SLEEPING_CHECKING:
				sleepColor = Color3::orange();
				break;
			case Sim::SLEEPING_DEEPLY:
				sleepColor = Color3::yellow();
				break;
			}

			Draw::selectionBox(getPart(), adorn, sleepColor);
		}
	}

	if (highlightAwakeParts && primitive->getAssembly()) {
		Sim::AssemblyState status = primitive->getAssembly()->getRootAssemblyConst()->getSleepStatus();

		if (status != Sim::ANCHORED && status != Sim::SLEEPING_DEEPLY) {
			Color3 wakeColor;

			switch (status) {
			case Sim::AWAKE:
				wakeColor = Color3::red();
				break;
			case Sim::SLEEPING_CHECKING:
				wakeColor = Color3::orange();
				break;
			case Sim::RECURSIVE_WAKE_PENDING:
			case Sim::WAKE_PENDING:
				wakeColor = Color3::purple();
				break;
			}

			Draw::selectionBox(getPart(), adorn, wakeColor);
		}
	}

	if (showPartCoord) {
		renderCoordinateFrame(adorn);
	}

	if (showAnchoredParts && primitive->getAnchorObject()) {
		DrawAdorn::partSurface(
			getPart(),
			Math::getClosestObjectNormalId(Vector3(0.0f, -1.0f, 0.0f), getCoordinateFrame().rotation),
			adorn,
			G3D::Color4(Color3::gray(), alpha())
		);
	}

	if (showUnalignedParts && !aligned()) {
		Draw::selectionBox(getPart(), adorn, Color3::yellow());
	}

	if (showSpanningTree) {
		setAlphaModifier(0.3f);

		if (Assembly* assembly = primitive->getAssembly()) {
			if (primitive.get() == assembly->getAssemblyPrimitiveConst()) {
				adorn->setObjectToWorldMatrix(
					CoordinateFrame(Matrix3::identity(), primitive->getCoordinateFrame().translation)
				);
				if (primitive->getAnchorObject()) {
					adorn->sphere(G3D::Sphere(Vector3::zero(), 0.5f), Color3::green(), G3D::Color4::clear());
				}
				else {
					Vector3 boxSize = Vector3::one() * 0.25f;
					adorn->box(G3D::AABox(-boxSize, boxSize), Color3::green(), G3D::Color4::clear());
				}
			}
		}
	}
}

void PartInstance::setPartTypeXml(Part::PartType _type)
{
	if (partType != _type) {
		partType = _type;
		primitive->setPrimitiveType(partType == Part::BLOCK_PART ? Geometry::GEOMETRY_BLOCK : Geometry::GEOMETRY_BALL);

		if (partType != Part::BLOCK_PART) {
			if (formFactor != SYMETRIC) {
				formFactor = SYMETRIC;

				raisePropertyChanged(prop_formFactor);
			}
		}

		raisePropertyChanged(prop_shapeXml);
		raisePropertyChanged(prop_shapeUi);

		PersistentPart.setDirty();
		shouldRenderSetDirty();

		onExtentsChanged();
	}
}

void PartInstance::setPartTypeUi(Part::PartType _type)
{
	if (partType != _type) {
		World* world = Workspace::getWorldIfInWorkspace(this);

		if (world != NULL) {
			world->destroyJoints(primitive.get());
		}

		setPartTypeXml(_type);

		if (_type != Part::BLOCK_PART) {
			Vector3 xmlSize = uiToXmlSize(getPartSizeUi());

			setPartSizeXml(xmlSize);
			safeMove();
		}

		safeMove();

		world = Workspace::getWorldIfInWorkspace(this);

		if (world != NULL) {
			world->createJoints(primitive.get());
		}
	}
}

} // namespace RBX
