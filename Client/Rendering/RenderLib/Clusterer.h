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

		// clang-format off
		// SYNTHETIC: WEBSERVICE 0x101f33c0
		// SYNTHETIC: ROBLOX 0x00496220
		// RBX::Render::Clusterer::Cluster::~Cluster
		// clang-format on

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

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x101f52e0
	// TEMPLATE: ROBLOX 0x00497a70
	// RBX::Render::Clusterer::addSamples<std::_Vector_iterator<G3D::ReferenceCountedPointer<RBX::Render::Chunk>,std::allocator<G3D::ReferenceCountedPointer<RBX::Render::Chunk> > > >
	// clang-format on
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

	// SYNTHETIC: WEBSERVICE 0x101f5db0
	// SYNTHETIC: ROBLOX 0x00498500
	// RBX::Render::Clusterer::~Clusterer

	Clusters* go(unsigned int maxSteps);
};

DECOMP_SIZE_ASSERT(Clusterer, 0x18)
DECOMP_SIZE_ASSERT(Clusterer::Cluster, 0x20)

} // namespace Render
} // namespace RBX

#endif // RENDERLIB_CLUSTERER_H
