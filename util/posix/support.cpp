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

	typedef void (*GlProc)();

	GlProc _glapi_get_proc_address(const char* name);

	GlProc glXGetProcAddressARB(const GLubyte* name)
	{
		return _glapi_get_proc_address(reinterpret_cast<const char*>(name));
	}

	void* glXGetCurrentContext()
	{
		return NULL;
	}

	void* glXGetCurrentDisplay()
	{
		return NULL;
	}

	unsigned long glXGetCurrentDrawable()
	{
		return 0;
	}

	int glXMakeCurrent(void*, unsigned long, void*)
	{
		return 0;
	}
}
