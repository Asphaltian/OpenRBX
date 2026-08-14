#include "renderlib/RenderSurface.h"

#include <algorithm>

namespace RBX {
namespace Render {

// FUNCTION: WEBSERVICE 0x101f7080
bool materialDepthPtrSortProc(RenderSurface* const& A, RenderSurface* const& B)
{
	return A->material < B->material || (A->material == B->material && A->z > B->z);
}

// FUNCTION: WEBSERVICE 0x101f70c0
bool materialPtrSortProc(RenderSurface* const& A, RenderSurface* const& B)
{
	return A->material < B->material;
}

// FUNCTION: WEBSERVICE 0x101f70e0
bool depthPtrSortProc(RenderSurface* const& A, RenderSurface* const& B)
{
	return A->z < B->z;
}

// FUNCTION: WEBSERVICE 0x101f76c0
void sortByMaterialAndDepth(G3D::Array<RenderSurface*>& array)
{
	std::sort(array.begin(), array.end(), materialDepthPtrSortProc);
}

// FUNCTION: WEBSERVICE 0x101f76f0
void sortByMaterial(G3D::Array<RenderSurface*>& array)
{
	std::sort(array.begin(), array.end(), materialPtrSortProc);
}

// FUNCTION: WEBSERVICE 0x101f7720
void sortByDepth(G3D::Array<RenderSurface*>& array)
{
	std::sort(array.begin(), array.end(), depthPtrSortProc);
}

} // namespace Render
} // namespace RBX
