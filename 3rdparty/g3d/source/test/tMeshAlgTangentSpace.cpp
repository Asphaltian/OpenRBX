#include "../include/G3DAll.h"

void testMeshAlgTangentSpace() {
    printf("MeshAlg::computeTangentSpaceBasis ");

    MeshAlg::Geometry geometry;
    Array<MeshAlg::Face> face;
    Array<MeshAlg::Edge> edge;
    Array<MeshAlg::Vertex> vertex;

	Array<Vector2> texCoord;
    Array<Vector3> tangent;
    Array<Vector3> binormal;
	Array<int>	   index;

    // Create a quad
	double s = 2;
	geometry.vertexArray.append(Vector3(-s, -s, 0));
	geometry.vertexArray.append(Vector3( s, -s, 0));
	geometry.vertexArray.append(Vector3( s,  s, 0));
	geometry.vertexArray.append(Vector3(-s,  s, 0));

    texCoord.append(Vector2(0, 1));
	texCoord.append(Vector2(1, 1));
	texCoord.append(Vector2(1, 0));
	texCoord.append(Vector2(0, 0));

	index.append(0, 1, 2);
	index.append(0, 2, 3);

    MeshAlg::computeAdjacency(geometry.vertexArray, index, face, edge, vertex);
    MeshAlg::computeNormals(geometry, index);

    for (int i = 0; i < 4; ++i) {
        debugAssert(geometry.normalArray[i].fuzzyEq(Vector3::unitZ()));
    }

    MeshAlg::computeTangentSpaceBasis(geometry.vertexArray, texCoord, geometry.normalArray, face, tangent, binormal);

    for (int i = 0; i < 4; ++i) {
        debugAssert(tangent[i].fuzzyEq(Vector3::unitX()));
        debugAssert(binormal[i].fuzzyEq(Vector3::unitY()));
    }

    printf("passed\n");
}
