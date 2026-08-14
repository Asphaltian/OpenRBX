#ifndef RENDERLIB_RENDERSTATS_H
#define RENDERLIB_RENDERSTATS_H

#include "decomp.h"

#include <G3D/Stopwatch.h>

// SIZE 0x238
class RenderStats
{
public:
	G3D::Stopwatch render2D;           // 0x000
	G3D::Stopwatch swapBuffer;         // 0x058
	G3D::Stopwatch sort;               // 0x0b0
	G3D::Stopwatch cpuRenderTotal;     // 0x108
	G3D::Stopwatch cpuShadow;          // 0x160
	G3D::Stopwatch computeProxyArrays; // 0x1b8
	int majorStateChanges;             // 0x210
	int majorGLStateChanges;           // 0x214
	int minorStateChanges;             // 0x218
	int minorGLStateChanges;           // 0x21c
	int diffuseProxyCount;             // 0x220
	int pushPopCount;                  // 0x224
	int totalTriangles;                // 0x228
	int markShadowsTriangles;          // 0x22c
	int shadowedLightTriangles;        // 0x230
	int unshadowedTriangles;           // 0x234

	static int chunkCount;
	static int aggregatedChunkCount;
	static int aggregateChunkCount;

	RenderStats();
};

DECOMP_SIZE_ASSERT(RenderStats, 0x238)

#endif // RENDERLIB_RENDERSTATS_H
