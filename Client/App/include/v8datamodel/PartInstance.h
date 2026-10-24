#ifndef V8DATAMODEL_PARTINSTANCE_H
#define V8DATAMODEL_PARTINSTANCE_H

#include "appdraw/Part.h"
#include "decomp.h"
#include "reflection/signal.h"
#include "util/ComputeProp.h"
#include "util/Extents.h"
#include "util/ICameraSubject.h"
#include "util/IRenderable.h"
#include "util/ISelectable3d.h"
#include "v8datamodel/BrickColor.h"
#include "v8datamodel/IPrimaryPart.h"
#include "v8datamodel/PVInstance.h"
#include "v8datamodel/Surfaces.h"
#include "v8world/IMoving.h"
#include "v8world/Primitive.h"

#include <G3D/Color3.h>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

namespace RBX {

extern char sPart[];

using boost::shared_ptr;

class World;

// SIZE 0x2ac
class PartInstance : public DescribedCreatable<PartInstance, PVInstance, sPart>,
					 public Notifier<PartInstance, CanAggregateChanged>,
					 public IMoving,
					 public IRenderable,
					 public virtual IPrimaryPart,
					 public virtual ICameraSubject,
					 public virtual ISelectable3d
{
public:
	static bool highlightSleepParts;
	static bool highlightAwakeParts;
	static bool showAnchoredParts;
	static bool showPartCoord;
	static bool showUnalignedParts;
	static bool showSpanningTree;

	enum FormFactor
	{
		SYMETRIC = 0,
		BRICK = 1,
		PLATE = 2
	};

	static float plateHeight();
	static float brickHeight();
	static float cameraTransparentDistance();
	static float cameraTranslucentDistance();

private:
	G3D::Vector3 xmlToUiSize(const G3D::Vector3& size) const;
	G3D::Vector3 uiToXmlSize(const G3D::Vector3& uiSize) const;

	Part::PartType partType;                           // 0x19c
	FormFactor formFactor;                             // 0x1a0
	BrickColor color;                                  // 0x1a4
	float transparency;                                // 0x1a8
	float reflectance;                                 // 0x1ac
	bool locked;                                       // 0x1b0
	Surfaces surfaces;                                 // 0x1b4
	float renderImportance;                            // 0x1e4
	boost::scoped_ptr<Primitive> primitive;            // 0x1e8
	World* myWorld;                                    // 0x1ec
	float alphaModifier;                               // 0x1f0
	ComputeProp<Part, PartInstance> PersistentPart;    // 0x1f8
	ComputeProp<bool, PartInstance> SurfacesNeedAdorn; // 0x278

	Part computePersistentPart() const;

	bool computeSurfacesNeedAdorn() const;
	void safeMove();

public:
	const Part& getPart() const;

	PartInstance();
	virtual ~PartInstance();

	static const Reflection::PropDescriptor<PartInstance, float> prop_RenderImportance;
	static const Reflection::EnumPropDescriptor<PartInstance, Part::PartType> prop_shapeXml;
	static const Reflection::PropDescriptor<PartInstance, float> prop_Transparency;
	static const Reflection::PropDescriptor<PartInstance, float> prop_Reflectance;
	static const Reflection::PropDescriptor<PartInstance, bool> prop_Locked;
	static const Reflection::PropDescriptor<PartInstance, G3D::Color3> prop_Color;
	static const Reflection::PropDescriptor<PartInstance, BrickColor> prop_BrickColor;
	static const Reflection::PropDescriptor<PartInstance, bool> prop_CanCollide;
	static const Reflection::PropDescriptor<PartInstance, bool> prop_Anchored;
	static const Reflection::PropDescriptor<PartInstance, G3D::Vector3> prop_Size;
	static const Reflection::PropDescriptor<PartInstance, CoordinateFrame> prop_CFrame;
	static Reflection::SignalDesc<PartInstance, void(shared_ptr<Instance>)> event_Touched;

	const Primitive* getPrimitive() const { return primitive.get(); }
	Primitive* getPrimitive() { return primitive.get(); }

	static bool nonNullInWorkspace(shared_ptr<PartInstance> part);

	CoordinateFrame worldSnapLocation() const;
	bool aligned() const;

	// FUNCTION: WEBSERVICE 0x100a07e0
	Part::PartType getPartType() const { return partType; }

	FormFactor getFormFactor() const { return formFactor; }

	float getRenderImportance() const { return renderImportance; }

	float getTransparencyXml() const { return transparency; }

	float getTransparencyUi() const { return 1.0f - (1.0f - transparency) * alphaModifier; }

	float getReflectance() const { return reflectance; }

	bool getPartLocked() const { return locked; }

	Surfaces& getSurfaces();
	const Surfaces& getSurfaces() const;

	// FUNCTION: WEBSERVICE 0x10115a90
	bool getIsTransparent() const { return 1.0f - (1.0f - transparency) * alphaModifier > 0.1f; }

	const CoordinateFrame& getCoordinateFrame() const;

	float getMass();
	const Velocity& getVelocity() const;
	const Vector3& getLinearVelocity() const;
	const Vector3& getRotationalVelocity() const;
	const Vector3& getTranslationUi() const;
	void setTranslationUi(const Vector3& set);

	const Vector3& getPartSizeXml() const;
	Vector3 getPartSizeUi() const;

	bool getDragging() const;
	bool getCanCollide() const;
	bool getAnchored() const;

	float getFriction() const;
	float getElasticity() const;

	void setPartTypeXml(Part::PartType _type);
	void setPartTypeUi(Part::PartType _type);
	void setPartSizeXml(const Vector3& rbxSize);
	void setPartSizeUi(const Vector3& uiSize);
	void setRenderImportance(float value);
	void setPartLocked(bool value);
	void setTransparency(float value);
	void setAlphaModifier(float value);
	void setReflectance(float value);
	void setFormFactorXml(FormFactor value);
	void setDragging(bool value);
	void setCanCollide(bool value);
	void setAnchored(bool value);
	// FUNCTION: WEBSERVICE 0x1009a7d0
	BrickColor getColor() const { return color; }

	// FUNCTION: WEBSERVICE 0x1009a7e0
	G3D::Color3 getColor3() const { return color.color3(); }

	void setColor3(const G3D::Color3& value);

	void setColor(BrickColor value);

	void onSurfaceChanged(NormalId normalId);

	virtual void onCanAggregateChanged(bool canAggregate);
	virtual bool reportTouches() const;

	virtual void onAncestorChanged(const AncestorChanged& event);
	virtual bool askSetParent(const Instance* instance) const;
	virtual void onChildAdded(Instance* child);
	virtual void setName(const std::string& value);

	virtual const CoordinateFrame getLocation() const;

	virtual void onCameraNear(float distance);
	virtual void getCameraIgnorePrimitives(std::vector<const Primitive*>& primitives);

	virtual bool shouldRender3dAdorn() const;
	virtual void render3dAdorn(Adorn* adorn);
	virtual void render3dSelect(Adorn* adorn, SelectState selectState);

	virtual bool isControllable() const;

	virtual PartInstance* getPrimaryPart();
	virtual const PartInstance* getPrimaryPartConst() const;

	virtual void legacyTraverseState(const CoordinateFrame& parentState);
	virtual void onParentControllerChanged();

	static PartInstance* fromPrimitive(Primitive* primitive);
	static const PartInstance* fromPrimitiveConst(const Primitive* primitive);
	static void primitivesToParts(
		const G3D::Array<Primitive*>& primitives,
		G3D::Array<shared_ptr<PartInstance> >& parts
	);
	static bool getLocked(Instance* instance);

	void setPhysics(const CoordinateFrame& cf, const Velocity& vel);
	void setFormFactorUi(FormFactor value);
	void onTouchThisStep(shared_ptr<PartInstance> other);

	void setPartSizeUnjoined(const Vector3& uiSize)
	{
		Vector3 xmlSize = uiToXmlSize(uiSize);
		setPartSizeXml(xmlSize);
		safeMove();
	}
	void setFriction(float friction);
	void setElasticity(float elasticity);

	void destroyJoints();
	void join();
	void setCoordinateFrame(const CoordinateFrame& value);
	void setLinearVelocity(const Vector3& set);
	void setRotationalVelocity(const Vector3& set);

	static void findParts(Instance* instance, std::vector<boost::weak_ptr<PartInstance> >& parts);

	virtual const Primitive* getBiggestPrimitive() const;
	virtual bool hitTest(const G3D::Ray& worldRay, G3D::Vector3& worldHitPoint);

	virtual Extents getExtentsWorld() const;
	virtual Extents getExtentsLocal() const;
};

DECOMP_SIZE_ASSERT(PartInstance, 0x2ac)

// clang-format off
// FUNCTION: WEBSERVICE 0x1009c730
// RBX::Notifier<RBX::PartInstance,RBX::CanAggregateChanged>::raise(struct RBX::CanAggregateChanged)
// clang-format on

// clang-format off
// FUNCTION: WEBSERVICE 0x1021a810
// `dynamic initializer for 'RBX::PartInstance::prop_CFrame''
// FUNCTION: WEBSERVICE 0x1021a870
// RBX::`dynamic initializer for 'prop_PositionUi''
// FUNCTION: WEBSERVICE 0x1021a8d0
// RBX::`dynamic initializer for 'prop_Velocity''
// FUNCTION: WEBSERVICE 0x1021a930
// RBX::`dynamic initializer for 'prop_RotVelocity''
// STUB: WEBSERVICE 0x1021ab10
// `dynamic initializer for 'RBX::PartInstance::prop_Color''
// FUNCTION: WEBSERVICE 0x1021ab70
// `dynamic initializer for 'RBX::PartInstance::prop_BrickColor''
// FUNCTION: WEBSERVICE 0x1021abd0
// `dynamic initializer for 'RBX::PartInstance::prop_Transparency''
// FUNCTION: WEBSERVICE 0x1021ac30
// `dynamic initializer for 'RBX::PartInstance::prop_Reflectance''
// FUNCTION: WEBSERVICE 0x1021ac90
// `dynamic initializer for 'RBX::PartInstance::prop_Locked''
// FUNCTION: WEBSERVICE 0x1021acf0
// `dynamic initializer for 'RBX::PartInstance::prop_Anchored''
// FUNCTION: WEBSERVICE 0x1021ad50
// `dynamic initializer for 'RBX::PartInstance::prop_CanCollide''
// FUNCTION: WEBSERVICE 0x1021adb0
// `dynamic initializer for 'RBX::PartInstance::prop_RenderImportance''
// STUB: WEBSERVICE 0x1021af30
// RBX::`dynamic initializer for 'prop_formFactor''
// STUB: WEBSERVICE 0x1021af90
// RBX::`dynamic initializer for 'prop_Elasticity''
// STUB: WEBSERVICE 0x1021aff0
// RBX::`dynamic initializer for 'prop_Friction''
// FUNCTION: WEBSERVICE 0x1021b050
// `dynamic initializer for 'RBX::PartInstance::event_Touched''
// FUNCTION: WEBSERVICE 0x1021b070
// RBX::`dynamic initializer for 'prop_Dragging''
// SYNTHETIC: WEBSERVICE 0x10223080
// RBX::`dynamic atexit destructor for 'prop_RotVelocity''
// SYNTHETIC: WEBSERVICE 0x102230a0
// RBX::`dynamic atexit destructor for 'prop_shapeUi''
// SYNTHETIC: WEBSERVICE 0x10223140
// RBX::`dynamic atexit destructor for 'prop_SizeUi''
// SYNTHETIC: WEBSERVICE 0x102230e0
// `dynamic atexit destructor for 'RBX::PartInstance::prop_CanCollide''
// SYNTHETIC: WEBSERVICE 0x10223100
// `dynamic atexit destructor for 'RBX::PartInstance::prop_RenderImportance''
// SYNTHETIC: WEBSERVICE 0x10223160
// RBX::`dynamic atexit destructor for 'prop_formFactor''
// SYNTHETIC: WEBSERVICE 0x102231a0
// RBX::`dynamic atexit destructor for 'prop_Elasticity''
// SYNTHETIC: WEBSERVICE 0x102231c0
// RBX::`dynamic atexit destructor for 'prop_Friction''
// SYNTHETIC: WEBSERVICE 0x102231e0
// RBX::`dynamic atexit destructor for 'prop_Dragging''
// SYNTHETIC: WEBSERVICE 0x10223200
// RBX::`dynamic atexit destructor for 'prop_Velocity''
// SYNTHETIC: WEBSERVICE 0x10223220
// RBX::`dynamic atexit destructor for 'prop_PositionUi''
// SYNTHETIC: WEBSERVICE 0x10223240
// `dynamic atexit destructor for 'RBX::PartInstance::prop_CFrame''
// SYNTHETIC: WEBSERVICE 0x10223260
// `dynamic atexit destructor for 'RBX::PartInstance::prop_Reflectance''
// SYNTHETIC: WEBSERVICE 0x10223280
// `dynamic atexit destructor for 'RBX::PartInstance::prop_Transparency''
// SYNTHETIC: WEBSERVICE 0x102232a0
// `dynamic atexit destructor for 'RBX::PartInstance::prop_BrickColor''
// SYNTHETIC: WEBSERVICE 0x102232c0
// `dynamic atexit destructor for 'RBX::PartInstance::prop_Color''
// SYNTHETIC: WEBSERVICE 0x102232e0
// `dynamic atexit destructor for 'RBX::PartInstance::prop_Anchored''
// SYNTHETIC: WEBSERVICE 0x10223300
// `dynamic atexit destructor for 'RBX::PartInstance::prop_Locked''
// SYNTHETIC: WEBSERVICE 0x10223360
// `dynamic atexit destructor for 'RBX::PartInstance::event_Touched''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_PARTINSTANCE_H
