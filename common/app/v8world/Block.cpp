#include "v8world/Block.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x101091f0
float Block::getGridVolume() const
{
	return gridSize.z * gridSize.y * gridSize.x;
}

} // namespace RBX
