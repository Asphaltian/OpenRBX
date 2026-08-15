#ifndef RENDERLIB_AGGREGATINGSCENEMANAGER_H
#define RENDERLIB_AGGREGATINGSCENEMANAGER_H

#include "decomp.h"
#include "renderlib/Chunk.h"

#include <G3D/ReferenceCount.h>

namespace RBX {
namespace Render {

class RenderScene;

// SIZE 0x8
class __declspec(novtable) SceneManager
{
public:
	virtual void invalidateModel(const G3D::ReferenceCountedPointer<Chunk>&, bool) = 0; // vtable+0x00
	virtual void addModel(const G3D::ReferenceCountedPointer<Chunk>&) = 0;              // vtable+0x04

protected:
	RenderScene* renderScene; // 0x04
};

DECOMP_SIZE_ASSERT(SceneManager, 0x8)

} // namespace Render
} // namespace RBX

#endif // RENDERLIB_AGGREGATINGSCENEMANAGER_H
