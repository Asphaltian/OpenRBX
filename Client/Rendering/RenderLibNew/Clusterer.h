#ifndef RENDERLIBNEW_CLUSTERER_H
#define RENDERLIBNEW_CLUSTERER_H

#include "placeholder.h"
#include "renderlibnew/Chunk.h"

#include <G3D/Vector3.h>
#include <vector>

namespace RBX {
namespace RenderNew {

class Clusterer
{
public:
	typedef Chunk Sample;
	typedef std::vector<Sample*> Samples;

	class Cluster
	{
	public:
		G3D::Vector3 centroid;
		Samples samples;
		unsigned int visitIndex;

		void computeCentroid();

		float getDistanceFromCentroid(Sample* sample) { return (sample->cframe().translation - centroid).length(); }
	};

	typedef std::vector<Cluster> Clusters;

private:
	unsigned int sampleSize;
	const unsigned int clusterCount;
	Clusters clusters;

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

} // namespace RenderNew
} // namespace RBX

#endif // RENDERLIBNEW_CLUSTERER_H
