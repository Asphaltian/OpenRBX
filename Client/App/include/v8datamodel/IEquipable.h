#ifndef V8DATAMODEL_IEQUIPABLE_H
#define V8DATAMODEL_IEQUIPABLE_H

#include "decomp.h"

#include <G3D/CoordinateFrame.h>
#include <boost/shared_ptr.hpp>
#include <string>

namespace RBX {

class PartInstance;
class Weld;
class Workspace;

using G3D::CoordinateFrame;

// SIZE 0x10
class __declspec(novtable) IEquipable
{
protected:
	boost::shared_ptr<Weld> weld; // 0x04
	Workspace* workspace;         // 0x0c

	void buildWeld(
		PartInstance* humanoidPart,
		PartInstance* gadgetPart,
		const CoordinateFrame& humanoidCoord,
		const CoordinateFrame& gadgetCoord,
		const std::string& name
	);

	IEquipable();

	virtual ~IEquipable(); // vtable+0x00
};

DECOMP_SIZE_ASSERT(IEquipable, 0x10)

} // namespace RBX

#endif // V8DATAMODEL_IEQUIPABLE_H
