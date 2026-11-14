#include "renderlib/RenderSurface.h"

#include <algorithm>

namespace RBX {
namespace Render {

bool materialDepthPtrSortProc(RenderSurface* const& A, RenderSurface* const& B)
{
	return A->material < B->material || (A->material == B->material && A->z > B->z);
}

bool materialPtrSortProc(RenderSurface* const& A, RenderSurface* const& B)
{
	return A->material < B->material;
}

bool depthPtrSortProc(RenderSurface* const& A, RenderSurface* const& B)
{
	return A->z < B->z;
}

void sortByMaterialAndDepth(G3D::Array<RenderSurface*>& array)
{
	std::sort(array.begin(), array.end(), materialDepthPtrSortProc);
}

void sortByMaterial(G3D::Array<RenderSurface*>& array)
{
	std::sort(array.begin(), array.end(), materialPtrSortProc);
}

void sortByDepth(G3D::Array<RenderSurface*>& array)
{
	std::sort(array.begin(), array.end(), depthPtrSortProc);
}

} // namespace Render
} // namespace RBX
