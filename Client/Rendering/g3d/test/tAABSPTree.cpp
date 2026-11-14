#include "../include/G3DAll.h"


static void testSerialize() {
    AABSPTree<Vector3> tree;
    int N = 1000;

    for (int i = 0; i < N; ++i) {
        tree.insert(Vector3::random());
    }
    tree.balance();

    // Save the struture
    BinaryOutput b("test-bsp.dat", G3D_LITTLE_ENDIAN);
    tree.serializeStructure(b);
    b.commit();

}


static void testBoxIntersect() {

	AABSPTree<Vector3> tree;

	// Make a tree containing a regular grid of points
	for (int x = -5; x <= 5; ++x) {
		for (int y = -5; y <= 5; ++y) {
			for (int z = -5; z <= 5; ++z) {
				tree.insert(Vector3(x, y, z));
			}
		}
	}
	tree.balance();

	AABox box(Vector3(-1.5, -1.5, -1.5), Vector3(1.5, 1.5, 1.5));

	AABSPTree<Vector3>::BoxIntersectionIterator it = tree.beginBoxIntersection(box);
	const AABSPTree<Vector3>::BoxIntersectionIterator end = tree.endBoxIntersection();

	int hits = 0;
	while (it != end) { 
		const Vector3& v = *it;

		debugAssert(box.contains(v));

		++hits;
		++it;
	}

	debugAssertM(hits == 3*3*3, "Wrong number of intersections found in testBoxIntersect for AABSPTree");
}


void perfAABSPTree() {

    Array<AABox>                array;
    AABSPTree<AABox>            tree;
    
    const int NUM_POINTS = 100000;
    
    for (int i = 0; i < NUM_POINTS; ++i) {
        Vector3 pt = Vector3(random(-10, 10), random(-10, 10), random(-10, 10));
        AABox box(pt, pt + Vector3(.1f, .1f, .1f));
        array.append(box);
        tree.insert(box);
    }

    tree.balance();

    uint64 bspcount, arraycount, boxcount;

    // Run twice to get cache issues out of the way
    for (int it = 0; it < 2; ++it) {
        Array<Plane> plane;
        plane.append(Plane(Vector3(-1, 0, 0), Vector3(3, 1, 1)));
        plane.append(Plane(Vector3(1, 0, 0), Vector3(1, 1, 1)));
        plane.append(Plane(Vector3(0, 0, -1), Vector3(1, 1, 3)));
        plane.append(Plane(Vector3(0, 0, 1), Vector3(1, 1, 1)));
        plane.append(Plane(Vector3(0,-1, 0), Vector3(1, 3, 1)));
        plane.append(Plane(Vector3(0, 1, 0), Vector3(1, -3, 1)));

        AABox box(Vector3(1, 1, 1), Vector3(3,3,3));

        Array<AABox> point;

        System::beginCycleCount(bspcount);
        tree.getIntersectingMembers(plane, point);
        System::endCycleCount(bspcount);

        point.clear();

        System::beginCycleCount(boxcount);
        tree.getIntersectingMembers(box, point);
        System::endCycleCount(boxcount);

        point.clear();

        System::beginCycleCount(arraycount);
        for (int i = 0; i < array.size(); ++i) {
            if (! array[i].culledBy(plane)) {
                point.append(array[i]);
            }
        }
        System::endCycleCount(arraycount);
    }

    printf("AABSPTree<AABox>::getIntersectingMembers(plane) %g Mcycles\n"
           "AABSPTree<AABox>::getIntersectingMembers(box)   %g Mcycles\n"
           "Culled by on Array<AABox>                       %g Mcycles\n\n", 
           bspcount / 1e6, 
           boxcount / 1e6,
           arraycount / 1e6);
}


void testAABSPTree() {
	printf("AABSPTree ");

	testBoxIntersect();
	testSerialize();

	printf("passed\n");
}
