#ifndef V8DATAMODEL_PARTINSTANCE_H
#define V8DATAMODEL_PARTINSTANCE_H

#include "appdraw/Part.h"
#include "placeholder.h"
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
#include <boost/weak_ptr.hpp>

namespace RBX {

extern char sPart[];

using boost::shared_ptr;

class World;
class XmlState;

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

	Part::PartType partType;
	FormFactor formFactor;
	BrickColor color;
	float transparency;
	float reflectance;
	bool locked;
	Surfaces surfaces;
	float renderImportance;
	boost::scoped_ptr<Primitive> primitive;
	World* myWorld;
	float alphaModifier;
	ComputeProp<Part, PartInstance> PersistentPart;
	ComputeProp<bool, PartInstance> SurfacesNeedAdorn;

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

	Part::PartType getPartType() const { return partType; }

	FormFactor getFormFactor() const { return formFactor; }

	float getRenderImportance() const { return renderImportance; }

	float getTransparencyXml() const { return transparency; }

	float getTransparencyUi() const { return 1.0f - (1.0f - transparency) * alphaModifier; }

	float getReflectance() const { return reflectance; }

	bool getPartLocked() const { return locked; }

	Surfaces& getSurfaces();
	Surface* getSurface(const G3D::Ray&, int&);
	const Surfaces& getSurfaces() const;

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
	bool resize(NormalId, int);
	void writeResizeData(XmlState*);
	void setFormFactorXml(FormFactor value);
	void setDragging(bool value);
	void setCanCollide(bool value);
	void setAnchored(bool value);
	BrickColor getColor() const { return color; }

	G3D::Color3 getColor3() const { return color.color3(); }

	void setColor3(const G3D::Color3& value);

	float alpha() const { return 1.0f - getTransparencyUi(); }
	bool lockedInPlace() const;

	void setColor(BrickColor value);

private:
	friend class Surface;

	void onSurfaceChanged(NormalId normalId);
	void writeSize(XmlState*);
	void raiseSurfacePropertyChanged(const Reflection::PropertyDescriptor&);
	static float defaultFriction();
	static float defaultElasticity();

public:
	virtual void onCanAggregateChanged(bool canAggregate);
	virtual bool reportTouches() const;

	virtual void onAncestorChanged(const AncestorChanged& event);
	virtual bool askSetParent(const Instance* instance) const;
	virtual void onChildAdded(Instance* child);
	virtual void onGuidChanged();
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
		const G3D::Array<Primitive*>& primitives, G3D::Array<shared_ptr<PartInstance> >& parts
	);
	static void primitivesToParts(const G3D::Array<Primitive*>&, std::vector<boost::weak_ptr<PartInstance> >&);
	static bool getLocked(Instance* instance);
	static void setLocked(Instance*, bool);

	void setPhysics(const CoordinateFrame& cf, const Velocity& vel);
	void setPhysics(const CoordinateFrame&);
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

} // namespace RBX

#endif // V8DATAMODEL_PARTINSTANCE_H
