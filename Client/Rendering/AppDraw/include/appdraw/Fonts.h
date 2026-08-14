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

// clang-format off
// SYNTHETIC: WEBSERVICE 0x10218150
// `dynamic initializer for 'RBX::Fonts::fontRef''
// SYNTHETIC: WEBSERVICE 0x10221390
// `dynamic atexit destructor for 'RBX::Fonts::fontRef''
// clang-format on

} // namespace RBX

#endif // APPDRAW_FONTS_H
