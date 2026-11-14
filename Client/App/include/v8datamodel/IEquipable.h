#ifndef V8DATAMODEL_IEQUIPABLE_H
#define V8DATAMODEL_IEQUIPABLE_H

#include "placeholder.h"

#include <G3D/CoordinateFrame.h>
#include <boost/shared_ptr.hpp>
#include <string>

namespace RBX {

class PartInstance;
class Weld;
class Workspace;

using G3D::CoordinateFrame;

class __declspec(novtable) IEquipable
{
protected:
	boost::shared_ptr<Weld> weld;
	Workspace* workspace;

	void buildWeld(
		PartInstance* humanoidPart,
		PartInstance* gadgetPart,
		const CoordinateFrame& humanoidCoord,
		const CoordinateFrame& gadgetCoord,
		const std::string& name
	);

	IEquipable();

	virtual ~IEquipable();
};

} // namespace RBX

#endif // V8DATAMODEL_IEQUIPABLE_H
