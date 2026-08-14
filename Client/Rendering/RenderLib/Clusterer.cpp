#include "Clusterer.h"

#include <algorithm>
#include <cstdlib>

namespace RBX {
namespace Render {

// STUB: WEBSERVICE 0x101f9fa0
Clusterer::Cluster* Clusterer::findClosestCluster(Sample* sample)
{
	Cluster* bestCluster = NULL;
	float bestDistance = G3D::inf();

	Clusters::iterator end = clusters.end();

	for (Clusters::iterator it = clusters.begin(); it != end; ++it) {
		float distance = it->getDistanceFromCentroid(sample);

		if (distance < bestDistance) {
			bestDistance = distance;
			bestCluster = &(*it);
		}
	}

	return bestCluster;
}

// STUB: WEBSERVICE 0x101fa0c0
void Clusterer::Cluster::computeCentroid()
{
	G3D::Vector3 c(0, 0, 0);

	Samples::iterator end = samples.end();

	for (Samples::iterator it = samples.begin(); it != end; ++it) {
		c += (*it)->cframe().translation;
	}

	centroid = c / (float) samples.size();
}

// STUB: WEBSERVICE 0x101fa2c0
unsigned int Clusterer::moveSample(Sample* sample, Cluster* cluster)
{
	while (cluster->visitIndex != cluster->samples.size()) {
		Sample* visited = cluster->samples[cluster->visitIndex];
		Cluster* bestCluster = findClosestCluster(visited);

		if (bestCluster != cluster) {
			cluster->samples[cluster->visitIndex] = sample;
			return moveSample(visited, bestCluster) + 1;
		}

		cluster->visitIndex++;
	}

	cluster->samples.push_back(sample);
	cluster->visitIndex++;

	return 1;
}

// STUB: WEBSERVICE 0x101fa380
unsigned int Clusterer::moveSamples()
{
	unsigned int moveCount = 0;

	for (Clusters::iterator it = clusters.begin(); it != clusters.end(); ++it) {
		it->visitIndex = 0;
	}

	for (Clusters::iterator it = clusters.begin(); it != clusters.end(); ++it) {
		while (it->visitIndex < it->samples.size()) {
			Sample* sample = it->samples[it->visitIndex];
			Cluster* bestCluster = findClosestCluster(sample);

			if (bestCluster == &(*it)) {
				it->visitIndex++;
			}
			else {
				it->samples[it->visitIndex] = it->samples.back();
				it->samples.pop_back();
				moveCount += moveSample(sample, bestCluster);
			}
		}
	}

	return moveCount;
}

// STUB: WEBSERVICE 0x101fa520
Clusterer::Clusters* Clusterer::go(unsigned int maxSteps)
{
	if (clusterCount < 2) {
		return &clusters;
	}

	Clusters::iterator end = clusters.end();

	for (Clusters::iterator it = clusters.begin(); it != end; ++it) {
		if (it->samples.size() != 0) {
			it->centroid = it->samples[rand() % it->samples.size()]->cframe().translation;
		}
	}

	unsigned int threshold = std::max(sampleSize / 20, 1u);
	unsigned int step = 1;

	while (moveSamples() > threshold && step != maxSteps) {
		for (Clusters::iterator it = clusters.begin(); it != clusters.end(); ++it) {
			it->computeCentroid();
		}

		step++;
	}

	return &clusters;
}

// FUNCTION: WEBSERVICE 0x101fa8a0
Clusterer::Clusterer(unsigned int clusterCount) : sampleSize(0), clusterCount(clusterCount), clusters(clusterCount)
{
}

} // namespace Render
} // namespace RBX
