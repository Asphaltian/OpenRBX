#ifndef V8DATAMODEL_MODELINSTANCE_H
#define V8DATAMODEL_MODELINSTANCE_H

#include "decomp.h"
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

// SIZE 0x254
class ModelInstance : public PVInstance,
					  public IRenderable,
					  public virtual IPrimaryPart,
					  public virtual ICameraSubject,
					  public virtual ISelectable3d
{
public:
	static bool showModelCoord;

private:
	CoordinateFrame modelInPrimary;                       // 0x178
	PartInstance* primaryPart;                            // 0x1a8
	boost::shared_ptr<PartInstance> candidatePrimaryPart; // 0x1ac

	void updatePrimaryPart();

	ComputeProp<float, ModelInstance> FlagHeight;         // 0x1b8
	ComputeProp<Extents, ModelInstance> LocalGridExtents; // 0x1d8
	ComputeProp<Extents, ModelInstance> WorldGridExtents; // 0x208

	float computeFlagHeight() const;
	Extents computeLocalGridExtents() const;
	Extents computeWorldGridExtents() const;
	void dirtyAll();

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

	PartInstance* getPrimaryPartInternal();
	void setPrimaryPart(PartInstance* value);

	const CoordinateFrame& getModelInPrimary() const;
	void setModelInPrimary(const CoordinateFrame& value);

	virtual void onExtentsChanged() const;

	virtual const Primitive* getBiggestPrimitive() const;
	virtual bool hitTest(const G3D::Ray& ray, G3D::Vector3& hitPoint);

	virtual Extents getExtentsWorld() const;
	virtual Extents getExtentsLocal() const;

	virtual const CoordinateFrame getLocation() const;

	virtual void onCameraNear(float distance);
	virtual void getCameraIgnorePrimitives(std::vector<const Primitive*>& primitives);

	virtual PartInstance* getPrimaryPart();
	virtual const PartInstance* getPrimaryPartConst() const;

	virtual void legacyTraverseState(const CoordinateFrame& state);
};

DECOMP_SIZE_ASSERT(ModelInstance, 0x254)

} // namespace RBX

#endif // V8DATAMODEL_MODELINSTANCE_H
