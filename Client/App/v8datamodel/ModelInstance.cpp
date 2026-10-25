#include "v8datamodel/ModelInstance.h"

#include "appdraw/Draw.h"
#include "humanoid/Humanoid.h"
#include "tool/DragUtilities.h"
#include "util/Name.h"
#include "v8datamodel/PartInstance.h"

namespace RBX {

char sModel[] = "Model";

bool ModelInstance::showModelCoord;

static const Name& partName = Name::declare(sModel, 2000);

static const Reflection::PropDescriptor<ModelInstance, CoordinateFrame> desc_ModelInPrimary(
	"ModelInPrimary",
	"Data",
	&ModelInstance::getModelInPrimary,
	&ModelInstance::setModelInPrimary,
	Reflection::PropertyDescriptor::STREAMING
);
static const Reflection::RefPropDescriptor<ModelInstance, PartInstance> primaryPartProp(
	"PrimaryPart",
	"Data",
	&ModelInstance::getPrimaryPartInternal,
	&ModelInstance::setPrimaryPart
);
static Reflection::BoundFuncDesc<ModelInstance, void(), 0> desc_breakJoints(
	&ModelInstance::breakJoints,
	"BreakJoints",
	Reflection::FunctionDescriptor::AnyCaller
);
static Reflection::BoundFuncDesc<ModelInstance, void(), 0> desc_makeJoints(
	&ModelInstance::makeJoints,
	"MakeJoints",
	Reflection::FunctionDescriptor::AnyCaller
);

static Reflection::BoundFuncDesc<ModelInstance, void(G3D::Vector3), 1> model_moveFunctionOld(
	&ModelInstance::moveToPoint,
	"move",
	"location",
	Reflection::FunctionDescriptor::AnyCaller
);
static Reflection::BoundFuncDesc<ModelInstance, void(G3D::Vector3), 1> model_moveFunction(
	&ModelInstance::moveToPoint,
	"MoveTo",
	"location",
	Reflection::FunctionDescriptor::AnyCaller
);

// FUNCTION: WEBSERVICE 0x10059c10 FOLDED
bool ModelInstance::askSetParent(const Instance* instance) const
{
	return dynamic_cast<const ModelInstance*>(instance) != NULL;
}

// FUNCTION: WEBSERVICE 0x10059c40
float ModelInstance::computeFlagHeight() const
{
	float height = 6.0f;

	if (primaryPart != NULL) {
		float partTop = primaryPart->getExtentsWorld().max().y;

		height = getExtentsWorld().max().y - partTop + 6.0f;
	}

	return height;
}

// FUNCTION: WEBSERVICE 0x1005a1a0
void ModelInstance::onExtentsChanged() const
{
	LocalGridExtents.setDirty();
	WorldGridExtents.setDirty();
	FlagHeight.setDirty();

	PVInstance::onExtentsChanged();
}

// FUNCTION: WEBSERVICE 0x1005a1c0
const Primitive* ModelInstance::getBiggestPrimitive() const
{
	const PartInstance* part = getPrimaryPartConst();

	if (part != NULL) {
		return part->getPrimitive();
	}

	return NULL;
}

// FUNCTION: WEBSERVICE 0x1005a1f0
void ModelInstance::updatePrimaryPart(PartInstance* part) const
{
	if (primaryPart != part) {
		if (part != NULL) {
			if (primaryPart != NULL) {
				Matrix3 rotation = getLocation().rotation;

				modelInPrimary =
					CoordinateFrame(primaryPart->getCoordinateFrame().rotation.transpose() * rotation, Vector3::zero());
			}
			else {
				modelInPrimary = CoordinateFrame();
			}

			primaryPart = part;
		}
		else {
			modelInPrimary = CoordinateFrame();
			primaryPart = NULL;
		}
	}
}

// FUNCTION: WEBSERVICE 0x1005a350
Extents ModelInstance::computeWorldGridExtents() const
{
	return LocalGridExtents.getValue().express(getLocation(), CoordinateFrame());
}

// FUNCTION: WEBSERVICE 0x1005a3b0
const CoordinateFrame ModelInstance::getLocation() const
{
	if (getPrimaryPartConst()) {
		CoordinateFrame c = primaryPart->getCoordinateFrame();
		return CoordinateFrame(c.rotation * modelInPrimary.rotation, c.translation);
	}

	return CoordinateFrame();
}

// FUNCTION: WEBSERVICE 0x1005a460
void ModelInstance::render3dSelect(Adorn* adorn, SelectState selectState)
{
	Extents localExtents = getExtentsLocal();
	CoordinateFrame location = getLocation();

	location.translation = location.pointToWorldSpace(localExtents.center());

	Part part(Part::BLOCK_PART, localExtents.size(), G3D::Color4(Color3::white()), location);

	Draw::selectionBox(part, adorn, selectState);
}

// FUNCTION: WEBSERVICE 0x1005a890
ModelInstance::~ModelInstance()
{
}

// FUNCTION: WEBSERVICE 0x1005ab00
Extents ModelInstance::getExtentsWorld() const
{
	return WorldGridExtents;
}

// FUNCTION: WEBSERVICE 0x1005ab50
Extents ModelInstance::getExtentsLocal() const
{
	return LocalGridExtents;
}

// FUNCTION: WEBSERVICE 0x1005abb0
PartInstance* ModelInstance::getPrimaryPartInternal() const
{
	if (primaryPart != NULL) {
		return primaryPart;
	}

	float bestArea = -1.0f;
	PartInstance* best = NULL;

	for (unsigned int i = 0; i < numChildren(); i++) {
		const PVInstance* pv = dynamic_cast<const PVInstance*>(getChild(i));

		if (pv != NULL) {
			const Primitive* primitive = pv->getBiggestPrimitive();

			if (primitive != NULL) {
				PartInstance* part = PartInstance::fromPrimitive(const_cast<Primitive*>(primitive));

				float area = part->getExtentsWorld().areaXZ();

				if (area > bestArea) {
					bestArea = area;
					best = part;
				}
			}
		}
	}

	updatePrimaryPart(best);

	return best;
}

// FUNCTION: WEBSERVICE 0x1005ac90
void ModelInstance::legacyTraverseState(const CoordinateFrame& parentState)
{
	int count = numChildren();

	if (count > 0) {
		CoordinateFrame state = parentState * *legacyOffset;

		for (int i = 0; i < count; i++) {
			PVInstance* pv = dynamic_cast<PVInstance*>(getChild(i));

			if (pv != NULL) {
				pv->legacyTraverseState(state);
			}
		}
	}
}

// FUNCTION: WEBSERVICE 0x1005ad40
bool ModelInstance::hitTest(const G3D::Ray& worldRay, G3D::Vector3& worldHitPoint)
{
	for (unsigned int i = 0; i < numChildren(); i++) {
		PVInstance* pv = dynamic_cast<PVInstance*>(getChild(i));

		if (pv != NULL && pv->hitTest(worldRay, worldHitPoint)) {
			return true;
		}
	}

	return false;
}

// STUB: WEBSERVICE 0x1005add0
bool ModelInstance::shouldRender3dAdorn() const
{
	return (dynamic_cast<const PVInstance*>(getParent()) == NULL || getTypedRoot<PVInstance>() == getParent()) &&
		   showModelCoord;
}

// STUB: WEBSERVICE 0x1005ae20
void ModelInstance::render3dAdorn(Adorn* adorn)
{
	if (dynamic_cast<const PVInstance*>(getParent()) == NULL || getTypedRoot<PVInstance>() == getParent()) {
		if (showModelCoord) {
			renderCoordinateFrame(adorn);
		}
	}
}

// FUNCTION: WEBSERVICE 0x1005ae80
void ModelInstance::onCameraNear(float distance)
{
	for (unsigned int i = 0; i < numChildren(); i++) {
		ICameraSubject* subject = dynamic_cast<ICameraSubject*>(getChild(i));

		if (subject != NULL) {
			subject->onCameraNear(distance);
		}
	}
}

// FUNCTION: WEBSERVICE 0x1005b220 FOLDED
PartInstance* ModelInstance::getPrimaryPart()
{
	return getPrimaryPartInternal();
}

// FUNCTION: WEBSERVICE 0x1005b220 FOLDED
const PartInstance* ModelInstance::getPrimaryPartConst() const
{
	return getPrimaryPartInternal();
}

// FUNCTION: WEBSERVICE 0x1005b230
void ModelInstance::onDescendentAdded(Instance* instance)
{
	PVInstance::onDescendentAdded(instance);

	LocalGridExtents.setDirty();
	WorldGridExtents.setDirty();
	FlagHeight.setDirty();

	if (instance == candidatePrimaryPart.get()) {
		candidatePrimaryPart.reset();

		updatePrimaryPart(static_cast<PartInstance*>(instance));
	}

	shouldRenderSetDirty();
}

// FUNCTION: WEBSERVICE 0x1005b2c0
void ModelInstance::onDescendentRemoving(const shared_ptr<Instance>& instance)
{
	if (primaryPart == instance.get()) {
		primaryPart = NULL;
	}

	if (candidatePrimaryPart.get() == instance.get()) {
		candidatePrimaryPart.reset();
	}

	LocalGridExtents.setDirty();
	WorldGridExtents.setDirty();
	FlagHeight.setDirty();

	shouldRenderSetDirty();

	PVInstance::onDescendentRemoving(instance);
}

// FUNCTION: WEBSERVICE 0x1005b360
static void makeJ(Instance* instance)
{
	if (PartInstance* part = dynamic_cast<PartInstance*>(instance)) {
		part->join();
	}
	else if (ModelInstance* model = dynamic_cast<ModelInstance*>(instance)) {
		model->for_eachChild(makeJ);
	}
}

// FUNCTION: WEBSERVICE 0x1005b3c0
void ModelInstance::makeJoints()
{
	for_eachChild(makeJ);
}

// FUNCTION: WEBSERVICE 0x1005b3d0
static void breakJ(Instance* instance)
{
	if (PartInstance* part = dynamic_cast<PartInstance*>(instance)) {
		part->destroyJoints();
	}
	else if (ModelInstance* model = dynamic_cast<ModelInstance*>(instance)) {
		model->for_eachChild(breakJ);
	}
}

// FUNCTION: WEBSERVICE 0x1005b430
void ModelInstance::breakJoints()
{
	for_eachChild(breakJ);
}

// FUNCTION: WEBSERVICE 0x1005b440
void ModelInstance::getCameraIgnorePrimitives(std::vector<const Primitive*>& primitives)
{
	DragUtilities::getPrimitivesConst(this, primitives);

	if (Humanoid* humanoid = findFirstChildOfType<Humanoid>()) {
		humanoid->getIgnorePrims(primitives);
	}
}

// FUNCTION: WEBSERVICE 0x1005b560
Extents ModelInstance::computeLocalGridExtents() const
{
	std::vector<const Primitive*> primitives;

	DragUtilities::getPrimitivesConst(this, primitives);

	Extents answer = Extents::negativeInfiniteExtents();

	for (unsigned int i = 0; i < primitives.size(); i++) {
		const Primitive* primitive = primitives[i];

		Vector3 halfSize = primitive->getGridSize() * 0.5f;
		Extents childExtentsInMe = Extents(-halfSize, halfSize).express(primitive->getCoordinateFrame(), getLocation());
		answer.unionWith(childExtentsInMe);
	}

	return primitives.size() > 0 ? answer : Extents::zero();
}

// FUNCTION: WEBSERVICE 0x1005c000
ModelInstance::ModelInstance()
	: DescribedCreatable<ModelInstance, PVInstance, sModel>("Model"), primaryPart(NULL),
	  FlagHeight(this, &ModelInstance::computeFlagHeight),
	  LocalGridExtents(this, &ModelInstance::computeLocalGridExtents),
	  WorldGridExtents(this, &ModelInstance::computeWorldGridExtents)
{
	static bool physicalCharacterHacked = false;

	if (!physicalCharacterHacked) {
		const Name& name = Name::declare("PhysicalCharacter", -1);
		getCreators()[&name] = &getCreator();
		physicalCharacterHacked = true;
	}
}

// FUNCTION: WEBSERVICE 0x1005c520
void ModelInstance::setPrimaryPart(PartInstance* set)
{
	if (set == NULL || set->isDescendentOf(this)) {
		candidatePrimaryPart.reset();
		updatePrimaryPart(set);
	}
	else {
		candidatePrimaryPart = shared_from(set);
		updatePrimaryPart(NULL);
	}

	raisePropertyChanged(primaryPartProp);
}

// FUNCTION: WEBSERVICE 0x1015b610 FOLDED
void ModelInstance::onLastChildRemoved()
{
}

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sPart>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sHumanoid>();
template void RBX::Name::callDoDeclare<RBX::sPart>();
template void RBX::Name::callDoDeclare<RBX::sHumanoid>();

template const RBX::FactoryProduct<RBX::PartInstance, RBX::PVInstance, RBX::sPart>::Creator
	RBX::FactoryProduct<RBX::PartInstance, RBX::PVInstance, RBX::sPart>::creator;
