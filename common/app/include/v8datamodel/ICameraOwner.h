#ifndef V8DATAMODEL_ICAMERAOWNER_H
#define V8DATAMODEL_ICAMERAOWNER_H

#include "decomp.h"
#include "util/Extents.h"

#include <boost/weak_ptr.hpp>
#include <vector>

namespace G3D {
class GCamera;
}

namespace RBX {

class Camera;
class PartInstance;
class Primitive;

// SIZE 0x14
class __declspec(novtable) ICameraOwner
{
private:
	std::vector<boost::weak_ptr<PartInstance> > cameraIgnoreParts; // 0x04

public:
	virtual ~ICameraOwner() {} // vtable+0x00

	virtual Camera* getCamera() const = 0;              // vtable+0x04
	virtual const G3D::GCamera& getGCamera() const = 0; // vtable+0x08
	virtual void cameraMoved() = 0;                     // vtable+0x0c
	virtual Extents computeCameraOwnerExtents() = 0;    // vtable+0x10

	void clearCameraIgnoreParts();
	void getCameraIgnorePrimitives(std::vector<const Primitive*>& primitives);
};

DECOMP_SIZE_ASSERT(ICameraOwner, 0x14)

} // namespace RBX

#endif // V8DATAMODEL_ICAMERAOWNER_H
