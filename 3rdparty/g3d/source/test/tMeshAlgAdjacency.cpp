#include "../include/G3DAll.h"

void testAdjacency() {
    printf("MeshAlg::computeAdjacency\n");

    {
        //          2
        //        /|
        //       / |
        //      /  |
        //     /___|
        //    0     1
        //
        //


        MeshAlg::Geometry       geometry;
        Array<int>              index;
        Array<MeshAlg::Face>    faceArray;
        Array<MeshAlg::Edge>    edgeArray;
        Array<MeshAlg::Vertex>  vertexArray;

        geometry.vertexArray.append(Vector3(0,0,0));
        geometry.vertexArray.append(Vector3(1,0,0));
        geometry.vertexArray.append(Vector3(1,1,0));

        index.append(0, 1, 2);

        MeshAlg::computeAdjacency(
            geometry.vertexArray,
            index,
            faceArray,
            edgeArray,
            vertexArray);

        debugAssert(faceArray.size() == 1);
        debugAssert(edgeArray.size() == 3);

        debugAssert(faceArray[0].containsVertex(0));
        debugAssert(faceArray[0].containsVertex(1));
        debugAssert(faceArray[0].containsVertex(2));

        debugAssert(faceArray[0].containsEdge(0));
        debugAssert(faceArray[0].containsEdge(1));
        debugAssert(faceArray[0].containsEdge(2));

        debugAssert(edgeArray[0].inFace(0));
        debugAssert(edgeArray[1].inFace(0));
        debugAssert(edgeArray[2].inFace(0));

        MeshAlg::debugCheckConsistency(faceArray, edgeArray, vertexArray);

        // Severely weld, creating a degenerate face
        MeshAlg::weldAdjacency(geometry.vertexArray, faceArray, edgeArray, vertexArray, 1.1);

        MeshAlg::debugCheckConsistency(faceArray, edgeArray, vertexArray);
        debugAssert(! faceArray[0].containsVertex(0));

    }

    {
        //      
        //    0====1
        //  (degenerate face)
        //

        MeshAlg::Geometry       geometry;
        Array<int>              index;
        Array<MeshAlg::Face>    faceArray;
        Array<MeshAlg::Edge>    edgeArray;
        Array<MeshAlg::Vertex>  vertexArray;

        geometry.vertexArray.append(Vector3(0,0,0));
        geometry.vertexArray.append(Vector3(1,0,0));

        index.append(0, 1, 0);

        MeshAlg::computeAdjacency(
            geometry.vertexArray,
            index,
            faceArray,
            edgeArray,
            vertexArray);

        debugAssert(faceArray.size() == 1);
        debugAssert(edgeArray.size() == 2);

        debugAssert(faceArray[0].containsVertex(0));
        debugAssert(faceArray[0].containsVertex(1));

        debugAssert(faceArray[0].containsEdge(0));
        debugAssert(faceArray[0].containsEdge(1));

        debugAssert(edgeArray[0].inFace(0));
        debugAssert(edgeArray[1].inFace(0));
        MeshAlg::debugCheckConsistency(faceArray, edgeArray, vertexArray);
    }

    {
        //          2                       .
        //        /|\                       .
        //       / | \                      .
        //      /  |  \                     . 
        //     /___|___\                    . 
        //    0     1    3
        //
        //


        MeshAlg::Geometry       geometry;
        Array<int>              index;
        Array<MeshAlg::Face>    faceArray;
        Array<MeshAlg::Edge>    edgeArray;
        Array<MeshAlg::Vertex>  vertexArray;

        geometry.vertexArray.append(Vector3(0,0,0));
        geometry.vertexArray.append(Vector3(1,0,0));
        geometry.vertexArray.append(Vector3(1,1,0));
        geometry.vertexArray.append(Vector3(2,0,0));

        index.append(0, 1, 2);
        index.append(1, 3, 2);

        MeshAlg::computeAdjacency(
            geometry.vertexArray,
            index,
            faceArray,
            edgeArray,
            vertexArray);

        debugAssert(faceArray.size() == 2);
        debugAssert(edgeArray.size() == 5);
        debugAssert(vertexArray.size() == 4);

        debugAssert(faceArray[0].containsVertex(0));
        debugAssert(faceArray[0].containsVertex(1));
        debugAssert(faceArray[0].containsVertex(2));

        debugAssert(faceArray[1].containsVertex(3));
        debugAssert(faceArray[1].containsVertex(1));
        debugAssert(faceArray[1].containsVertex(2));

        // The non-boundary edge must be first
        debugAssert(! edgeArray[0].boundary());
        debugAssert(edgeArray[1].boundary());
        debugAssert(edgeArray[2].boundary());
        debugAssert(edgeArray[3].boundary());
        debugAssert(edgeArray[4].boundary());

        MeshAlg::debugCheckConsistency(faceArray, edgeArray, vertexArray);

        MeshAlg::weldAdjacency(geometry.vertexArray, faceArray, edgeArray, vertexArray);

        MeshAlg::debugCheckConsistency(faceArray, edgeArray, vertexArray);

        debugAssert(faceArray.size() == 2);
        debugAssert(edgeArray.size() == 5);
        debugAssert(vertexArray.size() == 4);
    }


    {
        // Test Welding


        //         2                  .
        //        /|\                 .
        //       / | \                .
        //      /  |  \               .
        //     /___|___\              .
        //    0   1,4   3
        //


        MeshAlg::Geometry       geometry;
        Array<int>              index;
        Array<MeshAlg::Face>    faceArray;
        Array<MeshAlg::Edge>    edgeArray;
        Array<MeshAlg::Vertex>  vertexArray;

        geometry.vertexArray.append(Vector3(0,0,0));
        geometry.vertexArray.append(Vector3(1,0,0));
        geometry.vertexArray.append(Vector3(1,1,0));
        geometry.vertexArray.append(Vector3(2,0,0));
        geometry.vertexArray.append(Vector3(1,0,0));

        index.append(0, 1, 2);
        index.append(2, 4, 3);

        MeshAlg::computeAdjacency(
            geometry.vertexArray,
            index,
            faceArray,
            edgeArray,
            vertexArray);

        debugAssert(faceArray.size() == 2);
        debugAssert(edgeArray.size() == 6);
        debugAssert(vertexArray.size() == 5);

        debugAssert(edgeArray[0].boundary());
        debugAssert(edgeArray[1].boundary());
        debugAssert(edgeArray[2].boundary());
        debugAssert(edgeArray[3].boundary());
        debugAssert(edgeArray[4].boundary());
        debugAssert(edgeArray[5].boundary());

        debugAssert(faceArray[0].containsVertex(0));
        debugAssert(faceArray[0].containsVertex(1));
        debugAssert(faceArray[0].containsVertex(2));

        debugAssert(faceArray[1].containsVertex(2));
        debugAssert(faceArray[1].containsVertex(3));
        debugAssert(faceArray[1].containsVertex(4));

        MeshAlg::debugCheckConsistency(faceArray, edgeArray, vertexArray);

        MeshAlg::weldAdjacency(geometry.vertexArray, faceArray, edgeArray, vertexArray);

        MeshAlg::debugCheckConsistency(faceArray, edgeArray, vertexArray);

        debugAssert(faceArray.size() == 2);
        debugAssert(edgeArray.size() == 5);
        debugAssert(vertexArray.size() == 5);

        debugAssert(! edgeArray[0].boundary());
        debugAssert(edgeArray[1].boundary());
        debugAssert(edgeArray[2].boundary());
        debugAssert(edgeArray[3].boundary());
        debugAssert(edgeArray[4].boundary());
    }
    {
        // Test Welding


        //        2,5 
        //        /|\         . 
        //       / | \        .
        //      /  |  \       .
        //     /___|___\      .
        //    0   1,4   3
        //


        MeshAlg::Geometry       geometry;
        Array<int>              index;
        Array<MeshAlg::Face>    faceArray;
        Array<MeshAlg::Edge>    edgeArray;
        Array<MeshAlg::Vertex>  vertexArray;

        geometry.vertexArray.append(Vector3(0,0,0));
        geometry.vertexArray.append(Vector3(1,0,0));
        geometry.vertexArray.append(Vector3(1,1,0));
        geometry.vertexArray.append(Vector3(2,0,0));
        geometry.vertexArray.append(Vector3(1,0,0));
        geometry.vertexArray.append(Vector3(1,1,0));

        index.append(0, 1, 2);
        index.append(5, 4, 3);

        MeshAlg::computeAdjacency(
            geometry.vertexArray,
            index,
            faceArray,
            edgeArray,
            vertexArray);

        debugAssert(faceArray.size() == 2);
        debugAssert(edgeArray.size() == 6);
        debugAssert(vertexArray.size() == 6);

        debugAssert(edgeArray[0].boundary());
        debugAssert(edgeArray[1].boundary());
        debugAssert(edgeArray[2].boundary());
        debugAssert(edgeArray[3].boundary());
        debugAssert(edgeArray[4].boundary());
        debugAssert(edgeArray[5].boundary());

        debugAssert(faceArray[0].containsVertex(0));
        debugAssert(faceArray[0].containsVertex(1));
        debugAssert(faceArray[0].containsVertex(2));

        debugAssert(faceArray[1].containsVertex(5));
        debugAssert(faceArray[1].containsVertex(3));
        debugAssert(faceArray[1].containsVertex(4));

        MeshAlg::debugCheckConsistency(faceArray, edgeArray, vertexArray);

        MeshAlg::weldAdjacency(geometry.vertexArray, faceArray, edgeArray, vertexArray);

        MeshAlg::debugCheckConsistency(faceArray, edgeArray, vertexArray);

        debugAssert(faceArray.size() == 2);
        debugAssert(edgeArray.size() == 5);
        debugAssert(vertexArray.size() == 6);

        debugAssert(! edgeArray[0].boundary());
        debugAssert(edgeArray[1].boundary());
        debugAssert(edgeArray[2].boundary());
        debugAssert(edgeArray[3].boundary());
        debugAssert(edgeArray[4].boundary());

    }
    
}
