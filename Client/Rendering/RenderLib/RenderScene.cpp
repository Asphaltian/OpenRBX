#include "renderlib/RenderScene.h"

namespace RBX {
namespace Render {

// FUNCTION: WEBSERVICE 0x101ef990
void RenderScene::sendDiffuseProxyMeshGeometry(G3D::RenderDevice* renderDevice) const
{
	for (int i = diffuseProxyArray.size() - 1; i >= 0; i--) {
		RenderSurface* proxy = diffuseProxyArray[i];

		renderDevice->setObjectToWorldMatrix(proxy->cframe);
		renderDevice->setPolygonOffset(proxy->polygonOffset);
		proxy->material->configureRenderDevice(renderDevice);
		Mesh::sendGeometry(proxy->mesh, renderDevice);
	}
}

// FUNCTION: WEBSERVICE 0x101f0900
void RenderScene::classifyProxies()
{
	for (int i = 0; i < proxyArray.size(); i++) {
		RenderSurface* ptr = &proxyArray[i];

		if (ptr->material != NULL) {
			if (ptr->material->transparent() > 0) {
				transparentProxyArray.append(ptr);
			}
			else {
				diffuseProxyArray.append(ptr);

				if (ptr->material->reflect() > 0) {
					reflectProxyArray.append(ptr);
				}
			}
		}
	}
}

} // namespace Render
} // namespace RBX
