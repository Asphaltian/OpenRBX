#include "v8datamodel/IEquipable.h"

#include "v8datamodel/JointInstance.h"
#include "v8datamodel/PartInstance.h"

#include <cstddef>

namespace RBX {

IEquipable::~IEquipable()
{
}

IEquipable::IEquipable() : workspace(NULL)
{
}

void IEquipable::buildWeld(
	PartInstance* humanoidPart,
	PartInstance* gadgetPart,
	const CoordinateFrame& humanoidCoord,
	const CoordinateFrame& gadgetCoord,
	const std::string& name
)
{
	CoordinateFrame attachmentCoordWorld = humanoidPart->getCoordinateFrame() * humanoidCoord;
	CoordinateFrame gadgetCoordWorld = attachmentCoordWorld * gadgetCoord.inverse();

	gadgetPart->setCoordinateFrame(gadgetCoordWorld);

	weld = Creatable<Instance>::create<Weld>();

	weld->setName(name);
	weld->setPart0(humanoidPart);
	weld->setPart1(gadgetPart);

	weld->setC0(humanoidCoord);
	weld->setC1(gadgetCoord);
	weld->setParent(humanoidPart);
}

} // namespace RBX
