#include "v8datamodel/MouseCommand.h"

namespace RBX {

// GLOBAL: WEBSERVICE 0x102f9ff4
G3D::Vector3 MouseCommand::ignoreVector3;

// STUB: WEBSERVICE 0x100becf0
G3D::Ray MouseCommand::getUnitMouseRay(const UIEvent& uiEvent) const
{
	STUB(0x100becf0);
	return G3D::Ray();
}

// STUB: WEBSERVICE 0x100bee40
PartInstance* MouseCommand::getPart(const UIEvent& uiEvent, const HitTestFilter* filter, G3D::Vector3& hitWorld)
{
	STUB(0x100bee40);
	return NULL;
}

// STUB: WEBSERVICE 0x100bef80
PartInstance* MouseCommand::getPartByLocalCharacter(const UIEvent& uiEvent, G3D::Vector3& hitWorld)
{
	STUB(0x100bef80);
	return NULL;
}

} // namespace RBX
