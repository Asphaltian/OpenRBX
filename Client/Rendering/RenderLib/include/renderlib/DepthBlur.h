#ifndef RENDERLIB_DEPTHBLUR_H
#define RENDERLIB_DEPTHBLUR_H

#include "decomp.h"

#include <G3D/Rect2D.h>
#include <GLG3D/RenderDevice.h>
#include <GLG3D/Shader.h>
#include <GLG3D/Texture.h>

namespace RBX {
namespace Render {

// SIZE 0x0c
class DepthBlur
{
private:
	G3D::ShaderRef shader; // 0x00
	G3D::TextureRef depth; // 0x04
	G3D::TextureRef color; // 0x08

	void allocateTextures(const G3D::Rect2D& screenRect);

public:
	DepthBlur();

	void apply(G3D::RenderDevice* rd);
};

DECOMP_SIZE_ASSERT(DepthBlur, 0x0c)

} // namespace Render
} // namespace RBX

#endif // RENDERLIB_DEPTHBLUR_H
