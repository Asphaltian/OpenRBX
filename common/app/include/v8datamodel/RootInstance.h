#ifndef V8DATAMODEL_ROOTINSTANCE_H
#define V8DATAMODEL_ROOTINSTANCE_H

#include "decomp.h"
#include "util/ComputeProp.h"
#include "util/Extents.h"
#include "v8datamodel/ICameraOwner.h"
#include "v8datamodel/ModelInstance.h"
#include "v8world/Controller.h"

#include <G3D/Rect2D.h>
#include <G3D/Vector3.h>
#include <memory>

namespace RBX {

class World;

extern const char sRootInstance[];

// SIZE 0x2ac
class __declspec(novtable) RootInstance : public ModelInstance, public ICameraOwner
{
private:
	ComputeProp<ControllerTypeArray, RootInstance> ControllersUsed; // 0x250
	G3D::Vector3 insertPoint;                                       // 0x270
	G3D::Rect2D viewPort;                                           // 0x27c
	std::auto_ptr<World> world;                                     // 0x28c

	ControllerTypeArray computeControllersUsed() const;
	G3D::Vector3 computeIdeInsertPoint() const;

protected:
	RootInstance();

	virtual ~RootInstance();
};

DECOMP_SIZE_ASSERT(RootInstance, 0x2ac)

} // namespace RBX

#endif // V8DATAMODEL_ROOTINSTANCE_H
