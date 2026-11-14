#ifndef V8DATAMODEL_MODELINSTANCE_H
#define V8DATAMODEL_MODELINSTANCE_H

#include "placeholder.h"
#include "util/ComputeProp.h"
#include "util/Extents.h"
#include "util/ICameraSubject.h"
#include "util/IRenderable.h"
#include "util/ISelectable3d.h"
#include "v8datamodel/IPrimaryPart.h"
#include "v8datamodel/PVInstance.h"

#include <G3D/CoordinateFrame.h>
#include <boost/shared_ptr.hpp>

namespace RBX {

class PartInstance;

extern char sModel[];

class ModelInstance : public DescribedCreatable<ModelInstance, PVInstance, sModel>,
					  public IRenderable,
					  public virtual IPrimaryPart,
					  public virtual ICameraSubject,
					  public virtual ISelectable3d
{
public:
	static bool showModelCoord;

	void FUN_00565040(const G3D::Vector3& front);

private:
	mutable CoordinateFrame modelInPrimary;
	mutable PartInstance* primaryPart;
	boost::shared_ptr<PartInstance> candidatePrimaryPart;

	void updatePrimaryPart(PartInstance* part) const;

	ComputeProp<float, ModelInstance> FlagHeight;
	ComputeProp<Extents, ModelInstance> LocalGridExtents;
	ComputeProp<Extents, ModelInstance> WorldGridExtents;

	float computeFlagHeight() const;
	Extents computeLocalGridExtents() const;
	Extents computeWorldGridExtents() const;
	void dirtyAll() const
	{
		LocalGridExtents.setDirty();
		WorldGridExtents.setDirty();
		FlagHeight.setDirty();
	}

	virtual bool shouldRender3dAdorn() const;
	virtual void render3dAdorn(Adorn* adorn);
	virtual void render3dSelect(Adorn* adorn, SelectState selectState);

protected:
	virtual void onDescendentAdded(Instance* instance);
	virtual void onDescendentRemoving(const shared_ptr<Instance>& instance);
	virtual bool askSetParent(const Instance* instance) const;

private:
	virtual void onLastChildRemoved();

public:
	ModelInstance();
	virtual ~ModelInstance();

	PartInstance* getPrimaryPartInternal() const;
	void setPrimaryPart(PartInstance* value);

	const CoordinateFrame& getModelInPrimary() const { return modelInPrimary; }
	void setModelInPrimary(const CoordinateFrame& set) { modelInPrimary = set; }

	virtual void onExtentsChanged() const;

	virtual const Primitive* getBiggestPrimitive() const;
	virtual bool hitTest(const G3D::Ray& worldRay, G3D::Vector3& worldHitPoint);

	virtual Extents getExtentsWorld() const;
	virtual Extents getExtentsLocal() const;

	virtual const CoordinateFrame getLocation() const;

	virtual void onCameraNear(float distance);
	virtual void getCameraIgnorePrimitives(std::vector<const Primitive*>& primitives);

	virtual PartInstance* getPrimaryPart();
	virtual const PartInstance* getPrimaryPartConst() const;
	void setFrontAndTop(const G3D::Vector3&);
	void destroyIfNoParts(const ModelInstance*);

	void makeJoints();
	void breakJoints();
	float computeTotalMass() const;
	float computeLargestMoment() const;

protected:
	virtual void legacyTraverseState(const CoordinateFrame& parentState);

public:
};

} // namespace RBX

#endif // V8DATAMODEL_MODELINSTANCE_H
