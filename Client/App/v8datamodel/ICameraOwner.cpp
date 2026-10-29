#include "v8datamodel/ICameraOwner.h"

#include "v8datamodel/PartInstance.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x10115e30
// FUNCTION: ROBLOX 0x0063dfb0
void ICameraOwner::getCameraIgnorePrimitives(std::vector<const Primitive*>& primitives)
{
	for (unsigned int i = 0; i < cameraIgnoreParts.size(); i++) {
		if (!cameraIgnoreParts[i].expired()) {
			PartInstance* part = cameraIgnoreParts[i].lock().get();
			const Primitive* prim = part->getPrimitive();
			primitives.push_back(prim);
		}
	}
}

// FUNCTION: WEBSERVICE 0x10115f20
// FUNCTION: ROBLOX 0x0063e0a0
void ICameraOwner::setCameraIgnoreParts(const std::vector<PartInstance*>& _set)
{
	cameraIgnoreParts.clear();

	for (unsigned int i = 0; i < _set.size(); i++) {
		PartInstance* part = _set[i];
		boost::weak_ptr<PartInstance> ptr = shared_from(part);
		cameraIgnoreParts.push_back(ptr);
	}
}

// FUNCTION: WEBSERVICE 0x10116070
// FUNCTION: ROBLOX 0x0063e1f0
void ICameraOwner::setCameraIgnoreParts(PartInstance* _set)
{
	std::vector<PartInstance*> temp;
	temp.push_back(_set);
	setCameraIgnoreParts(temp);
}

} // namespace RBX
