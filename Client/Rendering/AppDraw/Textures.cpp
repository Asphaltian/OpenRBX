#include "appdraw/Textures.h"

#include "util/ContentProvider.h"

#include <string>

namespace RBX {

G3D::ReferenceCountedPointer<TextureProxyBase> Textures::getTextureProxy(Adorn* adorn, const ContentId& id)
{
	try {
		std::string fileName = ContentProvider::singleton().getFile(id);

		return adorn->createTextureProxy(fileName);
	}
	catch (...) {
		return NULL;
	}
}

} // namespace RBX
