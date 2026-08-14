#include "appdraw/Textures.h"

#include "util/ContentProvider.h"

#include <string>

namespace RBX {

// FUNCTION: WEBSERVICE 0x101eccb0
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
