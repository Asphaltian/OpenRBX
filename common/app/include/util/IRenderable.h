#ifndef UTIL_IRENDERABLE_H
#define UTIL_IRENDERABLE_H

#include "decomp.h"

namespace RBX {

class Adorn;

enum SelectState
{
	SELECT_NORMAL = 0,
	SELECT_LIMIT = 1
};

// SIZE 0x10
class IRenderable
{
protected:
	virtual bool shouldRender2d() const { return false; }      // vtable+0x00
	virtual bool shouldRender3dAdorn() const { return false; } // vtable+0x04

public:
	virtual void render2d(Adorn* adorn) {}                                // vtable+0x08
	virtual void render3dAdorn(Adorn* adorn) {}                           // vtable+0x0c
	virtual void render3dSelect(Adorn* adorn, SelectState selectState) {} // vtable+0x10

private:
	undefined4 index2d; // 0x04
	undefined4 index3d; // 0x08
	undefined4 bucket;  // 0x0c
};

DECOMP_SIZE_ASSERT(IRenderable, 0x10)

} // namespace RBX

#endif // UTIL_IRENDERABLE_H
