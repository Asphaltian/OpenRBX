#ifndef RENDERLIB_DEPTHBLUR_H
#define RENDERLIB_DEPTHBLUR_H

#include "placeholder.h"

#include <G3D/Rect2D.h>
#include <GLG3D/RenderDevice.h>
#include <GLG3D/Shader.h>
#include <GLG3D/Texture.h>

namespace RBX {
namespace Render {

class DepthBlur
{
private:
	G3D::ShaderRef shader;
	G3D::TextureRef depth;
	G3D::TextureRef color;

	void allocateTextures(const G3D::Rect2D& screenRect);

public:
	DepthBlur();

	void apply(G3D::RenderDevice* rd);
};

} // namespace Render
} // namespace RBX

#endif // RENDERLIB_DEPTHBLUR_H
