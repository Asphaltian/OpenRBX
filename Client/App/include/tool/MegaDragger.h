#ifndef TOOL_MEGADRAGGER_H
#define TOOL_MEGADRAGGER_H

#include "decomp.h"

#include <G3D/Matrix3.h>
#include <G3D/Vector3.h>
#include <boost/weak_ptr.hpp>
#include <vector>

namespace RBX {

class ContactManager;
class PartInstance;
class PVInstance;
class RootInstance;
class UIEvent;

// SIZE 0x24
class MegaDragger
{
private:
	boost::weak_ptr<PartInstance> mousePart;               // 0x00
	std::vector<boost::weak_ptr<PartInstance> > dragParts; // 0x08
	bool joined;                                           // 0x18
	RootInstance* rootInstance;                            // 0x1c
	ContactManager* contactManager;                        // 0x20

public:
	MegaDragger(PartInstance* mousePartPtr, const std::vector<PVInstance*>& pvInstances, RootInstance* rootInstance);
	MegaDragger(
		PartInstance* mousePartPtr,
		const std::vector<boost::weak_ptr<PartInstance> >& partArray,
		RootInstance* rootInstance
	);
	~MegaDragger();

	void startDragging();
	void continueDragging();
	void finishDragging();
	bool mousePartAlive();

	// FUNCTION: WEBSERVICE 0x10126410
	boost::weak_ptr<PartInstance> getMousePart() { return mousePart; }

	void alignAndCleanParts();
	G3D::Vector3 hitObjectOrPlane(const UIEvent& uiEvent);
	G3D::Vector3 safeMoveYDrop(const G3D::Vector3& tryDrag);
	G3D::Vector3 safeMoveNoDrop(const G3D::Vector3& tryDrag);
	void safeRotate(const G3D::Matrix3& rotMatrix);
};

DECOMP_SIZE_ASSERT(MegaDragger, 0x24)

} // namespace RBX

#endif // TOOL_MEGADRAGGER_H
