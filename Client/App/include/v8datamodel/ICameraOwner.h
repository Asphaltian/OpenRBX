#ifndef V8DATAMODEL_ICAMERAOWNER_H
#define V8DATAMODEL_ICAMERAOWNER_H

#include "placeholder.h"
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

class __declspec(novtable) ICameraOwner
{
public:
	ICameraOwner() {}

private:
	std::vector<boost::weak_ptr<PartInstance> > cameraIgnoreParts;

public:
	virtual ~ICameraOwner() {}

	virtual Camera* getCamera() const = 0;
	virtual const G3D::GCamera& getGCamera() const = 0;
	virtual void cameraMoved() = 0;
	virtual Extents computeCameraOwnerExtents() = 0;

	void setCameraIgnoreParts(PartInstance* _set);
	void setCameraIgnoreParts(const std::vector<PartInstance*>& _set);
	void clearCameraIgnoreParts() { cameraIgnoreParts.clear(); }
	void getCameraIgnorePrimitives(std::vector<const Primitive*>& primitives);
};

} // namespace RBX

#endif // V8DATAMODEL_ICAMERAOWNER_H
