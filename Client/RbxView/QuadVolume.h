#ifndef RBXVIEW_QUADVOLUME_H
#define RBXVIEW_QUADVOLUME_H

#include "MeshFactory.h"
#include "placeholder.h"
#include "renderlib/Mesh.h"
#include "util/NormalId.h"

#include <G3D/Rect2D.h>
#include <G3D/ReferenceCount.h>
#include <G3D/Vector2.h>
#include <G3D/Vector2int16.h>
#include <G3D/Vector3.h>
#include <G3D/g3dmath.h>
#include <G3D/vectorMath.h>
#include <algorithm>
#include <math.h>

namespace RBX {
namespace View {

template <int face>
inline G3D::Vector3 uvwToObject(const G3D::Vector3& uvw)
{
	return RBX::uvwToObject<face>(uvw);
}

class __declspec(novtable) LevelBuilder
{
protected:
	const G3D::Vector3 halfSize;
	const RenderSurfaceTypes surfaceTypes;
	G3D::ReferenceCountedPointer<Render::Mesh::Level> level;

public:
	G3D::Vector2 textureScale;

	enum Purpose
	{
		Surface = 0,
		Decal = 1,
		Shadow = 2
	};

protected:
	LevelBuilder(
		G3D::ReferenceCountedPointer<Render::Mesh::Level>& level, G3D::Vector3 size, RenderSurfaceTypes surfaceTypes
	);

public:
	virtual void build(Purpose purpose);

	void buildFace(NormalId normalId, Purpose purpose);

	virtual void buildTop(Purpose purpose) = 0;
	virtual void buildBottom(Purpose purpose) = 0;
	virtual void buildLeft(Purpose purpose) = 0;
	virtual void buildRight(Purpose purpose) = 0;
	virtual void buildFront(Purpose purpose) = 0;
	virtual void buildBack(Purpose purpose) = 0;

protected:
	static float offset(RenderSurfaceType surfaceType)
	{
		switch (surfaceType) {
		case STUDS:
			return 0.0f;
		case INLET:
			return 0.5f;
		case WELD:
			return 1.0f;
		case GLUE:
			return 1.5f;
		}

		return 0.0f;
	}

	template <int face, class Deform>
	inline void buildFace(Deform deform, unsigned int uElements, unsigned int vElements, Purpose purpose)
	{
		buildFace<face>(deform, G3D::Vector2int16(uElements, vElements), purpose);
	}

	template <int face, class Deform>
	inline void buildFace(Deform deform, G3D::Vector2int16 elements, Purpose purpose)
	{
		RenderSurfaceType surfaceType = surfaceTypes.getSurfaceType((NormalId) face);

		G3D::Vector3 localRadius = G3D::abs(uvwToObject<face>(halfSize));

		bool vertical = purpose == Surface && localRadius.y > localRadius.x;
		int stripIndex = vertical ? 0 : 1;

		int strips =
			(purpose == Surface && surfaceType != FLAT) ? G3D::iRound(ceil(localRadius[stripIndex] * 0.5f)) : 1;

		G3D::Vector2int16 stripElements;
		stripElements[stripIndex] = std::max<G3D::int16>(1, elements[1 - stripIndex] / strips);
		stripElements[1 - stripIndex] = elements[1 - stripIndex];

		const float faceZ = localRadius.z;

		G3D::Vector2 minCorner = -localRadius.xy();
		G3D::Vector2 maxCorner = localRadius.xy();

		G3D::Vector2 texCoord;
		G3D::Vector2 texDelta;
		switch (purpose) {
		case Surface:
			if (surfaceType == FLAT) {
				texCoord = G3D::Vector2(0.25f, 0.5f);
				texDelta[1 - stripIndex] = 0.0f;
				texDelta[stripIndex] = 0.0f;
			}
			else {
				texCoord[1 - stripIndex] = 0.0f;
				texCoord[stripIndex] = offset(surfaceType);
				texDelta[1 - stripIndex] = 2 * localRadius[1 - stripIndex];
				texDelta[stripIndex] = 0.5f;
				if (stripIndex == 1) {
					texDelta.x = -texDelta.x;
				}
			}
			break;
		case Shadow:
			texCoord.x = 0.0f;
			texCoord.y = 0.0f;
			texDelta.x = 0.0f;
			texDelta.y = 0.0f;
			break;
		case Decal:
			texCoord.x = 0.0f;
			texCoord.y = textureScale.y;
			texDelta.x = textureScale.x;
			texDelta.y = -textureScale.y;
			break;
		}

		for (int i = 0; i < strips; ++i) {
			if (i == strips - 1) {
				maxCorner[stripIndex] = localRadius[stripIndex];
				if (purpose == Surface && surfaceType != FLAT) {
					texDelta[stripIndex] *= (maxCorner[stripIndex] - minCorner[stripIndex]) * 0.25;
				}
			}
			else {
				maxCorner[stripIndex] = minCorner[stripIndex] + 4.0;
			}

			buildFace<face>(
				deform, G3D::Rect2D::xyxy(minCorner, maxCorner), faceZ, texCoord, texDelta, stripIndex, stripElements
			);

			minCorner[stripIndex] = maxCorner[stripIndex];
		}
	}

	template <int face, class Deform>
	inline void buildFace(
		Deform deform,
		G3D::Rect2D faceCoords,
		const float faceZ,
		G3D::Vector2 texCoord,
		G3D::Vector2 texDelta,
		const int stripIndex,
		G3D::Vector2int16 elements
	)
	{
		G3D::Vector3 localRadius = G3D::abs(uvwToObject<face>(halfSize));

		G3D::Vector2 step(faceCoords.width() / elements.x, faceCoords.height() / elements.y);
		G3D::Vector2 texStep(texDelta.x / elements.x, texDelta.y / elements.y);

		const unsigned int vertexCount = (elements.x + 1) * (elements.y + 1);
		unsigned int* vertexIndices = new unsigned int[vertexCount];

		float vi = faceCoords.x0();
		float ti = texCoord.x;
		for (int i = 0; i <= elements.x; ++i) {
			float vj = faceCoords.y0();
			float tj = texCoord.y;
			for (int j = 0; j <= elements.y; ++j) {
				G3D::Vector3 vertex(vi, vj, faceZ);
				G3D::Vector2 tex;
				tex[1 - stripIndex] = ti;
				tex[stripIndex] = tj;
				G3D::Vector3 normal(0, 0, 1);
				G3D::Vector2int16 ivertex(i, j);
				deform(vertex, normal, tex, localRadius, ivertex);

				if (texDelta == G3D::Vector2::zero()) {
					tex = texCoord;
				}

				vertexIndices[i * (elements.y + 1) + j] =
					Render::Mesh::allocVertex(uvwToObject<face>(vertex), uvwToObject<face>(normal), tex * 0.5, true);

				vj += step.y;
				tj += texStep.y;
			}
			vi += step.x;
			ti += texStep.x;
		}

		for (int i = 0; i < elements.x; ++i) {
			for (int j = 0; j < elements.y; ++j) {
				int v = i * (elements.y + 1) + j;
				appendQuadFromVertexIndices(
					vertexIndices[v + 1],
					vertexIndices[v],
					vertexIndices[v + elements.y + 1],
					vertexIndices[v + elements.y + 2]
				);
			}
		}

		for (unsigned int i = 0; i < vertexCount; ++i) {
			Render::Mesh::freeVertex(vertexIndices[i]);
		}

		delete[] vertexIndices;
	}

	void appendQuadFromIndexArray(int a, int b, int c, int d);
	void appendQuadFromIndexArray(int a, int b, int c);
	void appendQuadFromVertexIndices(unsigned int a, unsigned int b, unsigned int c, unsigned int d);
	void appendQuadFromVertexIndices(unsigned int a, unsigned int b, unsigned int c);
};

} // namespace View
} // namespace RBX

#endif // RBXVIEW_QUADVOLUME_H
