#include "v8datamodel/ICharacterSubject.h"

#include <GL/gl.h>
#include <GLG3D/DXCaps.h>

namespace RBX {

void ICharacterSubject::getIgnorePrims(std::vector<const Primitive*>&)
{
}

void ICharacterSubject::tellCameraNear(float)
{
}

} // namespace RBX

namespace G3D {

uint64 DXCaps::videoMemorySize()
{
	return 0;
}

} // namespace G3D

extern "C"
{

	void glActiveTexture(GLenum texture);
	void glClientActiveTexture(GLenum texture);
	void glMultiTexCoord3fv(GLenum target, const GLfloat* v);
	void glMultiTexCoord4fv(GLenum target, const GLfloat* v);

	void glActiveTextureARB(GLenum texture)
	{
		glActiveTexture(texture);
	}

	void glClientActiveTextureARB(GLenum texture)
	{
		glClientActiveTexture(texture);
	}

	void glMultiTexCoord3fvARB(GLenum target, const GLfloat* v)
	{
		glMultiTexCoord3fv(target, v);
	}

	void glMultiTexCoord4fvARB(GLenum target, const GLfloat* v)
	{
		glMultiTexCoord4fv(target, v);
	}
}
