#include "appdraw/Fonts.h"

#include "util/ContentProvider.h"

namespace RBX {

G3D::ReferenceCountedPointer<G3D::GFont> Fonts::fontRef;

// FUNCTION: WEBSERVICE 0x10031af0
G3D::ReferenceCountedPointer<G3D::GFont> Fonts::getFont()
{
	if (fontRef.isNull()) {
		try {
			fontRef = G3D::GFont::fromFile(ContentProvider::singleton().getAssetFile("Fonts\\comics.fnt"));
		} catch (...) {
		}
	}

	return fontRef;
}

} // namespace RBX
