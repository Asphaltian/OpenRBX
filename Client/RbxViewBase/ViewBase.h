#ifndef RBXVIEWBASE_VIEWBASE_H
#define RBXVIEWBASE_VIEWBASE_H

#include "decomp.h"

#include <boost/shared_ptr.hpp>

class RenderStats;

namespace RBX {

class Instance;

// SIZE 0x4
class ViewBase
{
public:
	ViewBase() {}

	// FUNCTION: WEBSERVICE 0x1016a250
	virtual ~ViewBase() {} // vtable+0x00

	virtual void render(void* rd) = 0;                          // vtable+0x04
	virtual float getShadingQuality() const = 0;                // vtable+0x08
	virtual float getMeshDetail() const = 0;                    // vtable+0x0c
	virtual void updateSettings(float, float, bool, float) = 0; // vtable+0x10
	virtual void suppressSkybox() = 0;                          // vtable+0x14
	virtual Instance* getWorkspace() = 0;                       // vtable+0x18

	static bool& getShadowsEnabled()
	{
		static bool _shadowsEnabled = false;
		return _shadowsEnabled;
	}

	virtual RenderStats& getRenderStats() = 0; // vtable+0x1c

private:
	virtual void onWorkspaceDescendentAdded(boost::shared_ptr<Instance>) = 0; // vtable+0x20
	virtual void updateLighting() = 0;                                        // vtable+0x24
	virtual void invalidateLighting(bool) = 0;                                // vtable+0x28

	// SYNTHETIC: WEBSERVICE 0x1016a260
	// RBX::ViewBase::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(ViewBase, 0x4)

} // namespace RBX

#endif // RBXVIEWBASE_VIEWBASE_H
