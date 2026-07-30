#ifndef V8WORLD_BLOCK_H
#define V8WORLD_BLOCK_H

#include "v8world/Geometry.h"

namespace RBX {

class BlockTemplate;

class Block : public Geometry
{
public:
	// FUNCTION: WEBSERVICE 0x100a7750
	virtual float getRadius() const { return cornerRadius; }

	virtual float getGridVolume() const;

private:
	BlockTemplate* vertices; // 0x10
	float cornerRadius;      // 0x14
};

} // namespace RBX

#endif // V8WORLD_BLOCK_H
