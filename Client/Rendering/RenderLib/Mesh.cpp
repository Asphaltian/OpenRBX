#include "renderlib/Mesh.h"

#include "renderlib/Chunk.h"

namespace RBX {
namespace Render {

// STUB: WEBSERVICE 0x10165fc0
const Mesh::LevelRef Mesh::detailLevel(float detail) const
{
	return levels[detailIndex(levels.size() * detail)];
}

// FUNCTION: WEBSERVICE 0x10166020
void Mesh::sendGeometry(const Level* lvl, G3D::RenderDevice* rd)
{
	if (lvl != NULL && lvl->indexArray.size() > 0) {
		rd->sendIndices(lvl->primitive, lvl->indexArray);
	}
}

// STUB: WEBSERVICE 0x101676c0
void Mesh::computeDirectionalShadowVolume(
	const G3D::CoordinateFrame& cframe,
	const G3D::Vector3& worldLight,
	G3D::Array<unsigned int>& indexArray,
	G3D::Array<G3D::Vector3>& vertexArray,
	bool caps
) const
{
	G3D::Vector3 objectLight = cframe.rotation.transpose() * worldLight;
	const G3D::Array<G3D::Vector3>& geometry = shadowSurface.geometry;

	static G3D::Array<bool> backface;
	backface.resize(shadowSurface.faceNormalArray.size(), false);

	for (int f = 0; f < shadowSurface.faceNormalArray.size(); f++) {
		backface[f] = shadowSurface.faceNormalArray[f].dot(objectLight) < 0;
	}

	if (caps) {
		for (int f = 0; f < backface.size(); f++) {
			if (!backface[f]) {
				int n = vertexArray.size();
				indexArray.append(n, n + 1, n + 2);

				const G3D::MeshAlg::Face& face = shadowSurface.faceArray[f];
				const G3D::Vector3& v0 = geometry[face.vertexIndex[0]];
				const G3D::Vector3& v1 = geometry[face.vertexIndex[1]];
				const G3D::Vector3& v2 = geometry[face.vertexIndex[2]];

				vertexArray
					.append(cframe.pointToWorldSpace(v0), cframe.pointToWorldSpace(v1), cframe.pointToWorldSpace(v2));
			}
		}
	}

	for (int e = 0; e < shadowSurface.edgeArray.size(); e++) {
		const G3D::MeshAlg::Edge& edge = shadowSurface.edgeArray[e];
		bool backface0 = backface[edge.faceIndex[0]];

		if (backface0 != backface[edge.faceIndex[1]]) {
			int n = vertexArray.size();
			const G3D::Vector3& v0 = geometry[edge.vertexIndex[0]];
			const G3D::Vector3& v1 = geometry[edge.vertexIndex[1]];

			G3D::Vector3 p0 = cframe.pointToWorldSpace(v0);
			G3D::Vector3 p1 = cframe.pointToWorldSpace(v1);

			vertexArray.append(p0, p1);

			if (backface0) {
				indexArray.append(n, n + 1, 0);
			}
			else {
				indexArray.append(n + 1, n, 0);
			}
		}
	}
}

// STUB: WEBSERVICE 0x10167d70
G3D::ReferenceCountedPointer<Mesh> Mesh::aggregate(
	const std::vector<G3D::ReferenceCountedPointer<Chunk> >& chunkArray,
	G3D::CoordinateFrame& outCFrame,
	float& outBoundingRadius
)
{
	STUB(0x10167d70);
	return NULL;
}

} // namespace Render
} // namespace RBX
