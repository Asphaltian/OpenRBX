#ifndef RBXVIEWBASE_VIEWBASE_H
#define RBXVIEWBASE_VIEWBASE_H

#include "placeholder.h"

#include <boost/shared_ptr.hpp>

class RenderStats;

namespace RBX {

class Instance;

class ViewBase
{
public:
	ViewBase() {}

	virtual ~ViewBase() {}

	virtual void render(void* rd) = 0;
	virtual float getShadingQuality() const = 0;
	virtual float getMeshDetail() const = 0;
	virtual void updateSettings(float, float, bool, float) = 0;
	virtual void suppressSkybox() = 0;
	virtual Instance* getWorkspace() = 0;

	static bool& getShadowsEnabled()
	{
		static bool _shadowsEnabled = false;
		return _shadowsEnabled;
	}

	virtual RenderStats& getRenderStats() = 0;

private:
	virtual void onWorkspaceDescendentAdded(boost::shared_ptr<Instance>) = 0;
	virtual void updateLighting() = 0;
	virtual void invalidateLighting(bool) = 0;
};

} // namespace RBX

#endif // RBXVIEWBASE_VIEWBASE_H
