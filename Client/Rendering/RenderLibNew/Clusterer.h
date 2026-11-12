#ifndef RENDERLIBNEW_CLUSTERER_H
#define RENDERLIBNEW_CLUSTERER_H

#include "decomp.h"
#include "renderlibnew/Chunk.h"

#include <G3D/Vector3.h>
#include <vector>

namespace RBX {
namespace RenderNew {

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

	template <class Iterator>
	void addSamples(Iterator _Iter, Iterator _End)
	{
		unsigned int i = 0;
		for (; _Iter != _End; ++_Iter) {
			clusters[i].samples.push_back(&**_Iter);
			i = (i + 1) % clusterCount;
			sampleSize++;
		}
	}

	Clusters* go(unsigned int maxSteps);
};

DECOMP_SIZE_ASSERT(Clusterer, 0x18)
DECOMP_SIZE_ASSERT(Clusterer::Cluster, 0x20)

} // namespace RenderNew
} // namespace RBX

#endif // RENDERLIBNEW_CLUSTERER_H
