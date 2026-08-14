#ifndef RENDERLIB_CLUSTERER_H
#define RENDERLIB_CLUSTERER_H

#include "decomp.h"
#include "renderlib/Chunk.h"

#include <G3D/Vector3.h>
#include <vector>

namespace RBX {
namespace Render {

// SIZE 0x18
class Clusterer
{
public:
	typedef Chunk Sample;
	typedef std::vector<Sample*> Samples;

	// SIZE 0x20
	class Cluster
	{
	public:
		G3D::Vector3 centroid;   // 0x00
		Samples samples;         // 0x0c
		unsigned int visitIndex; // 0x1c

		void computeCentroid();

		float getDistanceFromCentroid(Sample* sample) { return (sample->cframe().translation - centroid).length(); }
	};

	typedef std::vector<Cluster> Clusters;

private:
	unsigned int sampleSize;         // 0x00
	const unsigned int clusterCount; // 0x04
	Clusters clusters;               // 0x08

	unsigned int moveSamples();
	unsigned int moveSample(Sample* sample, Cluster* cluster);
	Cluster* findClosestCluster(Sample* sample);

public:
	Clusterer(unsigned int clusterCount);

	Clusters* go(unsigned int maxSteps);
};

DECOMP_SIZE_ASSERT(Clusterer, 0x18)
DECOMP_SIZE_ASSERT(Clusterer::Cluster, 0x20)

} // namespace Render
} // namespace RBX

#endif // RENDERLIB_CLUSTERER_H
