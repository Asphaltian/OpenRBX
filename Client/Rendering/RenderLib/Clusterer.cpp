#include "Clusterer.h"

namespace RBX {
namespace Render {

// STUB: WEBSERVICE 0x101f9fa0
Clusterer::Cluster* Clusterer::findClosestCluster(Sample* sample)
{
	STUB(0x101f9fa0);
	return NULL;
}

// STUB: WEBSERVICE 0x101fa0c0
void Clusterer::Cluster::computeCentroid()
{
	G3D::Vector3 c(0, 0, 0);

	for (Samples::iterator it = samples.begin(); it != samples.end(); ++it) {
		c += (*it)->cframe().translation;
	}

	centroid = c / (float) samples.size();
}

// STUB: WEBSERVICE 0x101fa2c0
unsigned int Clusterer::moveSample(Sample* sample, Cluster* cluster)
{
	STUB(0x101fa2c0);
	return 0;
}

// STUB: WEBSERVICE 0x101fa380
unsigned int Clusterer::moveSamples()
{
	STUB(0x101fa380);
	return 0;
}

// STUB: WEBSERVICE 0x101fa520
Clusterer::Clusters* Clusterer::go(unsigned int maxSteps)
{
	STUB(0x101fa520);
	return NULL;
}

// FUNCTION: WEBSERVICE 0x101fa8a0
Clusterer::Clusterer(unsigned int clusterCount) : sampleSize(0), clusterCount(clusterCount), clusters(clusterCount)
{
}

} // namespace Render
} // namespace RBX
