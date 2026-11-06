#ifndef RENDERLIBNEW_MATERIALFACTORY_H
#define RENDERLIBNEW_MATERIALFACTORY_H

#include "decomp.h"

namespace G3D {
class TextureManager;
} // namespace G3D

namespace RBX {
namespace ViewNew {

// SIZE 0x28
class MaterialFactory
{
private:
	undefined m_unk0x00[0x28]; // 0x00

public:
	MaterialFactory(G3D::TextureManager* textureManager);
	~MaterialFactory();
};

DECOMP_SIZE_ASSERT(MaterialFactory, 0x28)

} // namespace ViewNew
} // namespace RBX

#endif // RENDERLIBNEW_MATERIALFACTORY_H
