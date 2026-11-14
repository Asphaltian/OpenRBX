#ifndef APPDRAW_FONTS_H
#define APPDRAW_FONTS_H

#include <G3D/ReferenceCount.h>
#include <GLG3D/GFont.h>

namespace RBX {

class Fonts
{
public:
	static G3D::ReferenceCountedPointer<G3D::GFont> getFont();

private:
	static G3D::ReferenceCountedPointer<G3D::GFont> fontRef;
};

} // namespace RBX

#endif // APPDRAW_FONTS_H
