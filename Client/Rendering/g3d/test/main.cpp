/**
 @file Test/main.cpp

 This file runs unit conformance and performance tests for G3D.  
 To write a new test, add a file named t<class>.cpp to the project
 and provide two entry points: test<class> and perf<class> (even if they are
 empty).

 Call those two methods from main() in main.cpp.

 @maintainer Morgan McGuire, matrix@graphics3d.com
 @created 2002-01-01
 @edited  2006-04-10
 */

#include "../include/G3DAll.h"
#include <iostream>

using namespace G3D;

#ifdef G3D_WIN32
	#include "conio.h"
#endif
#include <string>

#define RUN_SLOW_TESTS

// Forward declarations
void perfArray();
void testArray();

void testMatrix();

void testMatrix3();
void perfMatrix3();

void testCollisionDetection();
void perfCollisionDetection();

void testGChunk();

void testQuat();

void perfAABSPTree();
void testAABSPTree();

void testAABox();

void testReliableConduit(NetworkDevice*);

void perfSystemMemcpy();
void testSystemMemcpy();
void testSystemMemset();

void testReferenceCount();

void testRandom();

void perfTextOutput();

void testMeshAlgTangentSpace();

void perfQueue();
void testQueue();

void testBinaryIO();
void testHugeBinaryIO();
void perfBinaryIO();

void testTextInput();

void testAdjacency();

void testTable();
void perfTable();

void testAtomicInt32();

void testCoordinateFrame();

void testGThread();


void testConvexPolygon2D() {
    Array<Vector2> v;
    v.append(Vector2(0, 0), Vector2(1,1), Vector2(2, 0));
    ConvexPolygon2D C(v);
    debugAssert(! C.contains(Vector2(10, 2)));
    debugAssert(C.contains(Vector2(1, 0.5)));
}



void testWildcards() {
    printf("filenameContainsWildcards\n");
    debugAssert(!filenameContainsWildcards("file1.exe"));
	debugAssert(filenameContainsWildcards("file?.exe"));
	debugAssert(filenameContainsWildcards("f*.exe"));
	debugAssert(filenameContainsWildcards("f*.e?e"));
	debugAssert(filenameContainsWildcards("*1.exe"));
	debugAssert(filenameContainsWildcards("?ile1.exe"));
}


void testBox() {
    printf("Box\n");
    Box box = Box(Vector3(0,0,0), Vector3(1,1,1));

    debugAssert(box.contains(Vector3(0,0,0)));
    debugAssert(box.contains(Vector3(1,1,1)));
    debugAssert(box.contains(Vector3(0.5,0.5,0.5)));
    debugAssert(! box.contains(Vector3(1.5,0.5,0.5)));
    debugAssert(! box.contains(Vector3(0.5,1.5,0.5)));
    debugAssert(! box.contains(Vector3(0.5,0.5,1.5)));
    debugAssert(! box.contains(-Vector3(0.5,0.5,0.5)));
    debugAssert(! box.contains(-Vector3(1.5,0.5,0.5)));
    debugAssert(! box.contains(-Vector3(0.5,1.5,0.5)));
    debugAssert(! box.contains(-Vector3(0.5,0.5,1.5)));

    Vector3 v0, v1, v2, v3, n1, n2;

    v0 = box.getCorner(0);
    v1 = box.getCorner(1);
    v2 = box.getCorner(2);
    v3 = box.getCorner(3);

    debugAssert(v0 == Vector3(0,0,1));
    debugAssert(v1 == Vector3(1,0,1));
    debugAssert(v2 == Vector3(1,1,1));
    debugAssert(v3 == Vector3(0,1,1));

    Vector3 n[2] = {Vector3(0,0,1), Vector3(1,0,0)};
    (void)n;

    int i;
    for (i = 0; i < 2; ++i) {
        box.getFaceCorners(i, v0, v1, v2, v3);
        n1 = (v1 - v0).cross(v3 - v0);
        n2 = (v2 - v1).cross(v0 - v1);

        debugAssert(n1 == n2);
        debugAssert(n1 == n[i]);
    }

}



void testAABoxCollision() {
    printf("intersectionTimeForMovingPointFixedAABox\n");

    Vector3 boxlocation, aaboxlocation, normal;

    for (int i = 0; i < 1000; ++i) {

        Vector3 pt1 = Vector3::random() * random(0, 10);
        Vector3 vel1 = Vector3::random();

        Vector3 low = Vector3::random() * 5;
        Vector3 extent(random(0,4), random(0,4), random(0,4));
        AABox aabox(low, low + extent);
        Box   box = aabox.toBox();

        double boxTime = CollisionDetection::collisionTimeForMovingPointFixedBox(
            pt1, vel1, box, boxlocation, normal);
        (void)boxTime;

        double aaTime = CollisionDetection::collisionTimeForMovingPointFixedAABox(
            pt1, vel1, aabox, aaboxlocation);
        (void)aaTime;

        Ray ray = Ray::fromOriginAndDirection(pt1, vel1);
        double rayboxTime = ray.intersectionTime(box);
        (void)rayboxTime;

        double rayaaTime = ray.intersectionTime(aabox);
        (void)rayaaTime;

        debugAssert(fuzzyEq(boxTime, aaTime));
        if (boxTime < inf()) {
            debugAssert(boxlocation.fuzzyEq(aaboxlocation));
        }

        debugAssert(fuzzyEq(rayboxTime, rayaaTime));
    }
}


void testPlane() {
    printf("Plane\n");
    {
        Plane p(Vector3(1, 0, 0),
                Vector3(0, 1, 0),
                Vector3(0, 0, 0));

        Vector3 n = p.normal();
        debugAssert(n == Vector3(0,0,1));
    }

    {
        Plane p(Vector3(4, 6, .1f),
                Vector3(-.2f, 6, .1f),
                Vector3(-.2f, 6, -.1f));

        Vector3 n = p.normal();
        debugAssert(n.fuzzyEq(Vector3(0,-1,0)));
    }

    {
        Plane p(Vector4(1,0,0,0),
                Vector4(0,1,0,0),
                Vector4(0,0,0,1));
        Vector3 n = p.normal();
        debugAssert(n.fuzzyEq(Vector3(0,0,1)));
    }

    {
        Plane p(
                Vector4(0,0,0,1),
                Vector4(1,0,0,0),
                Vector4(0,1,0,0));
        Vector3 n = p.normal();
        debugAssert(n.fuzzyEq(Vector3(0,0,1)));
    }

    {
        Plane p(Vector4(0,1,0,0),
                Vector4(0,0,0,1),
                Vector4(1,0,0,0));
        Vector3 n = p.normal();
        debugAssert(n.fuzzyEq(Vector3(0,0,1)));
    }
}



class A {
public:
    int x;

    A() : x(0) {
        printf("Default constructor\n");
    }

    A(int y) : x(y) {
        printf("Construct %d\n", x);
    }

    A(const A& a) : x(a.x) {
        printf("Copy %d\n", x);
    }

    A& operator=(const A& other) {
        printf("Assign %d\n", other.x);
        x = other.x;
        return *this;
    }

    ~A() {
        printf("Destruct %d\n", x);
    }
};



void measureMemsetPerformance() {
    printf("----------------------------------------------------------\n");

    uint64 native, g3d;

    int n = 1024 * 1024;
    void* m1 = malloc(n);

    // First iteration just primes the system
    for (int i=0; i < 2; ++i) {
        System::beginCycleCount(native);
            memset(m1, 31, n);
        System::endCycleCount(native);
        System::beginCycleCount(g3d);
            System::memset(m1, 31, n);
        System::endCycleCount(g3d);
    }
    free(m1);

    printf("System::memset:                     %d cycles/kb\n", (int)(g3d / (n / 1024)));
    printf("::memset      :                     %d cycles/kb\n", (int)(native / (n / 1024)));
}





void measureNormalizationPerformance() {
    printf("----------------------------------------------------------\n");
    uint64 raw, opt, overhead;
    int n = 1024 * 1024;

    double y;
    Vector3 x = Vector3(10,-20,3);

    int i, j;

    for (j = 0; j < 2; ++j) {
        x = Vector3(10,-20,3);
		y = 0;
        System::beginCycleCount(overhead);
        for (i = n - 1; i >= 0; --i) {
            x.z = i;
            y += x.z;
        }
        System::endCycleCount(overhead);
    }

    x = Vector3(10,-20,3);
    y = 0;
    System::beginCycleCount(raw);
    for (i = n - 1; i >= 0; --i) {
        x.z = i;
        y += x.direction().z;
        y += x.direction().z;
        y += x.direction().z;
    }
    System::endCycleCount(raw);
    
    x = Vector3(10,-20,3);
    y = 0;
    System::beginCycleCount(opt);
    for (i = n - 1; i >= 0; --i) {
        x.z = i;
        y += x.fastDirection().z;
        y += x.fastDirection().z;
        y += x.fastDirection().z;
    }
    System::endCycleCount(opt);

    double r = raw;
    double o = opt;
    double h = overhead;

    printf("%g %g %g\n", r-h, o-h, h);

    printf("Vector3::direction():               %d cycles\n", (int)((r-h)/(n*3.0)));
    printf("Vector3::fastDirection():           %d cycles\n", (int)((o-h)/(n*3.0)));

}


void testColor3uint8Array() {
    printf("Array<Color3uint8>\n");
    Array<Color3uint8> x(2);

    debugAssert(sizeof(Color3uint8) == 3);
    x[0].r = 60;
    x[0].g = 61;
    x[0].b = 62;
    x[1].r = 63;
    x[1].g = 64;
    x[1].b = 65;

    uint8* y = (uint8*)x.getCArray();
    (void)y;
    debugAssert(y[0] == 60);
    debugAssert(y[1] == 61);
    debugAssert(y[2] == 62);
    debugAssert(y[3] == 63);
    debugAssert(y[4] == 64);
    debugAssert(y[5] == 65);
}



void testFloat() {
    printf("Test Float\n");
    /* changed from "nan" by ekern.  does this work on windows? */
    double x = nan();
    bool b1  = (x < 0.0);
    bool b2  = (x >= 0.0);
    bool b3  = !(b1 || b2);
    (void)b1;
    (void)b2;
    (void)b3;
    debugAssert(isNaN(nan()));
    debugAssert(! isNaN(4));
    debugAssert(! isNaN(0));
    debugAssert(! isNaN(inf()));
    debugAssert(! isNaN(-inf()));
    debugAssert(! isFinite(nan()));
    debugAssert(! isFinite(-inf()));
    debugAssert(! isFinite(inf()));
    debugAssert(isFinite(0));
		    
}

void testglFormatOf() {
    printf("glFormatOf\n");

    debugAssert(glFormatOf(Color3) == GL_FLOAT);
    debugAssert(glFormatOf(Color3uint8) == GL_UNSIGNED_BYTE);
    debugAssert(glFormatOf(Vector3int16) == GL_SHORT);
    debugAssert(glFormatOf(float) == GL_FLOAT);
    debugAssert(glFormatOf(int16) == GL_SHORT);
    debugAssert(glFormatOf(int32) == GL_INT);

    debugAssert(sizeOfGLFormat(GL_FLOAT) == 4);
}



void testSwizzle() {
    Vector4 v1(1,2,3,4);
    Vector2 v2;

    v2 = v1.xy() + v1.yz();
}


void testCoordinateFrame() {
    printf("CoordinateFrame ");

    {
        // Easy case
        CoordinateFrame c;
        c.lookAt(Vector3(-1, 0, -1));
        float h = c.getHeading();
        debugAssert(fuzzyEq(h, G3D_PI / 4));
    }

    // Test getHeading at a variety of angles
    for (int i = -175; i <= 175; i += 5) {
        CoordinateFrame c;
        float h = c.getHeading();
        debugAssert(h == 0);

        c.rotation = Matrix3::fromAxisAngle(Vector3::unitY(), toRadians(i));

        h = c.getHeading();
        debugAssert(fuzzyEq(h, toRadians(i)));
    }

    printf("passed\n");
}

void measureRDPushPopPerformance(RenderDevice* rd) {
    uint64 identityCycles;

    int N = 500;
    rd->pushState();
    rd->popState();

    System::beginCycleCount(identityCycles);
    for (int i = 0; i < N; ++i) {
        rd->pushState();
        rd->popState();
    }
    System::endCycleCount(identityCycles);

    printf("RenderDevice::push+pop:             %g cycles\n", identityCycles / (double)N);
}


int main(int argc, char* argv[]) {
    RenderDevice* renderDevice = NULL;

    NetworkDevice* networkDevice = new NetworkDevice();
    if (networkDevice) {networkDevice->init();}

    std::string s;
    System::describeSystem(s);
    printf("%s\n", s.c_str());

    if (networkDevice) {
        networkDevice->describeSystem(s);
        printf("%s\n", s.c_str());
    }


#    ifndef _DEBUG
        printf("Performance analysis:\n\n");

        perfArray();

        perfTable();

        printf("%s\n", System::mallocPerformance().c_str());

        perfQueue();

        perfMatrix3();

        perfTextOutput();

        perfSystemMemcpy();

        perfBinaryIO();

        perfAABSPTree();

		perfCollisionDetection();

        measureMemsetPerformance();
        measureNormalizationPerformance();


        GWindowSettings settings;
        settings.width = 800;
        settings.height = 600;
        settings.alphaBits = 0;
        settings.rgbBits = 8;
        settings.stencilBits = 0;
        settings.fsaaSamples = 1;

        if (!renderDevice) {
            renderDevice = new RenderDevice();
        }
    
        renderDevice->init(settings);

        if (renderDevice) {
            renderDevice->describeSystem(s);
            printf("%s\n", s.c_str());
        }

        measureRDPushPopPerformance(renderDevice);

#   else

    printf("\n\nTests:\n\n");

    testCollisionDetection();    

    testCoordinateFrame();

	testReliableConduit(networkDevice);

	testAABSPTree();

	testQuat();

    testReferenceCount();

    testAtomicInt32();

    testGThread();

    testSystemMemset();

    testSystemMemcpy();

    testQueue();

    testMatrix();

    testArray();

    testMeshAlgTangentSpace();

    testGChunk();

    printf("ConvexPolygon2D\n");
    testConvexPolygon2D();
    printf("  passed\n");

    testPlane();
    printf("  passed\n");

    testAABox();

    testRandom();
    printf("  passed\n");

    testAABoxCollision();
    printf("  passed\n");
    testAdjacency();
    printf("  passed\n");
    testWildcards();
    printf("  passed\n");

    testFloat();
    printf("  passed\n");
    
    testRandom();

    testTextInput();
    printf("  passed\n");

    testTable();

    testBox();
    printf("  passed\n");

    testColor3uint8Array();
    printf("  passed\n");
    testglFormatOf();
    printf("  passed\n");
    testSwizzle();

    testBinaryIO();

#   ifdef RUN_SLOW_TESTS
        testHugeBinaryIO();
        printf("  passed\n");
#   endif

    printf("%s\n", System::mallocPerformance().c_str());
    System::resetMallocPerformanceCounters();

    printf("\nAll tests succeeded.\n");
#endif

    if (renderDevice) {
        renderDevice->cleanup();
        delete renderDevice;
    }
    
    if (networkDevice) {
		networkDevice->cleanup();
	    delete networkDevice;
	}

    return 0;
}

