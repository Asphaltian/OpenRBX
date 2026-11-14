#ifndef RENDERLIB_RENDERSTATS_H
#define RENDERLIB_RENDERSTATS_H

#include "placeholder.h"

#include <G3D/Stopwatch.h>

class RenderStats
{
public:
	G3D::Stopwatch render2D;
	G3D::Stopwatch swapBuffer;
	G3D::Stopwatch sort;
	G3D::Stopwatch cpuRenderTotal;
	G3D::Stopwatch cpuShadow;
	G3D::Stopwatch computeProxyArrays;
	int majorStateChanges;
	int majorGLStateChanges;
	int minorStateChanges;
	int minorGLStateChanges;
	int diffuseProxyCount;
	int pushPopCount;
	int totalTriangles;
	int markShadowsTriangles;
	int shadowedLightTriangles;
	int unshadowedTriangles;

	static int chunkCount;
	static int aggregatedChunkCount;
	static int aggregateChunkCount;

	RenderStats();
};

#endif // RENDERLIB_RENDERSTATS_H
