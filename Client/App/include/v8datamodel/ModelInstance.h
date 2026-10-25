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
class ModelInstance : public DescribedCreatable<ModelInstance, PVInstance, sModel>,
					  public IRenderable,
					  public virtual IPrimaryPart,
					  public virtual ICameraSubject,
					  public virtual ISelectable3d
{
public:
	static bool showModelCoord;

private:
	mutable CoordinateFrame modelInPrimary;               // 0x178
	mutable PartInstance* primaryPart;                    // 0x1a8
	boost::shared_ptr<PartInstance> candidatePrimaryPart; // 0x1ac

	void updatePrimaryPart(PartInstance* part) const;

	ComputeProp<float, ModelInstance> FlagHeight;         // 0x1b8
	ComputeProp<Extents, ModelInstance> LocalGridExtents; // 0x1d8
	ComputeProp<Extents, ModelInstance> WorldGridExtents; // 0x208

	float computeFlagHeight() const;
	Extents computeLocalGridExtents() const;
	Extents computeWorldGridExtents() const;
	void dirtyAll() const;

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

	// clang-format off
	// SYNTHETIC: WEBSERVICE 0x1005b520
	// RBX::ModelInstance::`scalar deleting destructor'
	// clang-format on

	PartInstance* getPrimaryPartInternal() const;
	void setPrimaryPart(PartInstance* value);

	// FUNCTION: WEBSERVICE 0x10059bd0
	const CoordinateFrame& getModelInPrimary() const { return modelInPrimary; }
	// FUNCTION: WEBSERVICE 0x10059be0
	void setModelInPrimary(const CoordinateFrame& value) { modelInPrimary = value; }

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

	void makeJoints();
	void breakJoints();

protected:
	virtual void legacyTraverseState(const CoordinateFrame& parentState);

public:
};

DECOMP_SIZE_ASSERT(ModelInstance, 0x254)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x102188d0
// RBX::`dynamic initializer for 'partName''
// SYNTHETIC: WEBSERVICE 0x102188f0
// RBX::`dynamic initializer for 'desc_ModelInPrimary''
// SYNTHETIC: WEBSERVICE 0x10218950
// RBX::`dynamic initializer for 'primaryPartProp''
// SYNTHETIC: WEBSERVICE 0x102189b0
// RBX::`dynamic initializer for 'desc_breakJoints''
// SYNTHETIC: WEBSERVICE 0x102189f0
// RBX::`dynamic initializer for 'desc_makeJoints''
// SYNTHETIC: WEBSERVICE 0x10218a30
// RBX::`dynamic initializer for 'model_moveFunctionOld''
// SYNTHETIC: WEBSERVICE 0x10218a70
// RBX::`dynamic initializer for 'model_moveFunction''
// SYNTHETIC: WEBSERVICE 0x10221910
// RBX::`dynamic atexit destructor for 'primaryPartProp''
// SYNTHETIC: WEBSERVICE 0x10221930
// RBX::`dynamic atexit destructor for 'desc_ModelInPrimary''
// SYNTHETIC: WEBSERVICE 0x10221970
// RBX::`dynamic atexit destructor for 'model_moveFunction''
// STUB: WEBSERVICE 0x10221a00
// RBX::`dynamic atexit destructor for 'desc_makeJoints''
// SYNTHETIC: WEBSERVICE 0x10221a10
// RBX::`dynamic atexit destructor for 'model_moveFunctionOld''
// STUB: WEBSERVICE 0x10221a20
// RBX::`dynamic atexit destructor for 'desc_breakJoints''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_MODELINSTANCE_H
