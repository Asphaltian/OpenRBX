#include "../include/G3DAll.h"

static void measureTriangleCollisionPerformance() {
    printf("----------------------------------------------------------\n");
    {
    uint64 raw, opt;

    Vector3 v0(0, 0, 0);
    Vector3 v1(0, 0, -1);
    Vector3 v2(-1, 0, 0);
    Sphere sphere(Vector3(.5,1,-.5), 1);
    Vector3 vel(0, -1, 0);
    Vector3 location, normal;
    Triangle triangle(v0, v1, v2);
    int n = 1024;
    int i;

    System::beginCycleCount(raw);
    for (i = 0; i < n; ++i) {
        double t = CollisionDetection::collisionTimeForMovingSphereFixedTriangle(sphere, vel, Triangle(v0, v1, v2), location, normal);
        (void)t;
    }
    System::endCycleCount(raw);

    System::beginCycleCount(opt);
    for (i = 0; i < n; ++i) {
        double t = CollisionDetection::collisionTimeForMovingSphereFixedTriangle(sphere, vel, triangle, location, normal);
        (void)t;
    }
    System::endCycleCount(opt);

    printf("Sphere-Triangle collision detection on 3 vertices: %d cycles\n", (int)(raw / n));
    printf("Sphere-Triangle collision detection on Triangle:   %d cycles\n", (int)(opt / n));
    }
    {
        uint64 raw;

        Vector3 v0(0, 0, 0);
        Vector3 v1(0, 0, -1);
        Vector3 v2(-1, 0, 0);
        Sphere sphere(Vector3(.5,1,-.5), 1);
        Vector3 vel(0, -1, 0);
        Vector3 location, normal;
        Triangle triangle(v0, v1, v2);
        int n = 1024;
        int i;
        Ray ray = Ray::fromOriginAndDirection(Vector3(3,-1,-.25), vel);

        System::beginCycleCount(raw);
        for (i = 0; i < n; ++i) {
            double t = ray.intersectionTime(triangle);
                //CollisionDetection::collisionTimeForMovingPointFixedTriangle(
                //sphere, vel, triangle, location, normal);
            (void)t;
        }
        System::endCycleCount(raw);
        printf("Miss:\n");
        printf("ray.intersectionTime(triangle): %d cycles\n", (int)(raw / n));
    }
    {
        uint64 raw;

        Vector3 v0(0, 0, 0);
        Vector3 v1(0, 0, -1);
        Vector3 v2(-1, 0, 0);
        Vector3 vel(0, -1, 0);
        Vector3 location, normal;
        Triangle triangle(v0, v1, v2);
        int n = 1024;
        int i;
        Ray ray = Ray::fromOriginAndDirection(Vector3(-.15f,1,-.15f), vel);

        System::beginCycleCount(raw);
        for (i = 0; i < n; ++i) {
            double t = ray.intersectionTime(triangle);
            (void)t;
        }
        System::endCycleCount(raw);

        printf("Hit:\n");
        printf("ray.intersectionTime(triangle): %d cycles\n", (int)(raw / n));
    }
}


static void measureAABoxCollisionPerformance() {
    printf("----------------------------------------------------------\n");

    uint64 raw, opt;

    AABox aabox(Vector3(-1, -1, -1), Vector3(1,2,3));
    Box   box = aabox.toBox();

    Vector3 pt1(0,10,0);
    Vector3 vel1(0,-1,0);
    Vector3 location, normal;
    int n = 1024;
    int i;

    System::beginCycleCount(raw);
    for (i = 0; i < n; ++i) {
        double t = CollisionDetection::collisionTimeForMovingPointFixedBox(
            pt1, vel1, box, location, normal);
        (void)t;
    }
    System::endCycleCount(raw);

    System::beginCycleCount(opt);
    for (i = 0; i < n; ++i) {
        double t = CollisionDetection::collisionTimeForMovingPointFixedAABox(
            pt1, vel1, aabox, location);
        (void)t;
    }
    System::endCycleCount(opt);

    printf("Ray-Box:   %d cycles\n", (int)(raw / n));
    printf("Ray-AABox: %d cycles\n", (int)(opt / n));
}


void testCollisionDetection() {
    printf("CollisionDetection ");

    {
        Vector3 pos(2.0f, 0.0f, 2.5f); 
        Vector3 dir(-1.0f, 0.0f, -1.0f); 
        dir.unitize(); 

        Box myBox(G3D::Vector3(-1,-1,-1), G3D::Vector3(1,1,1)); 
        Vector3 loc, normal;

        float t = CollisionDetection::collisionTimeForMovingPointFixedBox(pos, dir, myBox, loc, normal);
        debugAssert(normal.fuzzyEq(Vector3(0,0,1)));
    }


    {
        Sphere s(Vector3(0,1,0), 1);
        Plane  p = Plane::fromEquation(0,1,0,0);
        Array<Vector3> contact;
        Array<Vector3> outNormal;

        float depth = CollisionDetection::penetrationDepthForFixedSphereFixedPlane(
            s, p, contact, outNormal);

        debugAssert(outNormal[0] == p.normal());
        debugAssert(contact[0] == Vector3::zero());
        debugAssert(depth == 0);

        s = Sphere(Vector3(0,2,0), 1);

        depth = CollisionDetection::penetrationDepthForFixedSphereFixedPlane(
            s, p, contact, outNormal);

        debugAssert(depth < 0);
    }

    {
        Sphere s(Vector3(0,1,0), 1);
        Sphere r(Vector3(0,-.5,0), 1);
        Array<Vector3> contact;
        Array<Vector3> outNormal;

        float depth = CollisionDetection::penetrationDepthForFixedSphereFixedSphere(
            s, r, contact, outNormal);
        (void)depth;

        debugAssert(outNormal[0] == Vector3(0,-1,0));
        debugAssert(depth == .5);
    }

    {
        Sphere s(Vector3(5, 0, 0), 1);
        Sphere f(Vector3(4.5, 0, 0), 1);
        debugAssert(CollisionDetection::fixedSolidSphereIntersectsFixedSolidSphere(s, f));
    }

    {
        Sphere s(Vector3(5, 0, 0), 1);
        Sphere f(Vector3(1, 0, 0), 2);
        debugAssert(! CollisionDetection::fixedSolidSphereIntersectsFixedSolidSphere(s, f));
    }

    {
        Sphere s(Vector3(5, 0, 0), 1);
        Box    f(Vector3(-1, -1.5f, -2), Vector3(4.5f, 2, 8));
        debugAssert(CollisionDetection::fixedSolidSphereIntersectsFixedSolidBox(s, f));
    }

    {
        Sphere s(Vector3(0, 0, 0), .1f);
        Box    f(Vector3(-1, -1.5f, -2), Vector3(4.5f, 2, 8));
        debugAssert(CollisionDetection::fixedSolidSphereIntersectsFixedSolidBox(s, f));
    }

    {
        Sphere s(Vector3(6, 0, 0), 1);
        Box    f(Vector3(-1, -1.5f, -2), Vector3(4.5f, 2, 8));
        debugAssert(! CollisionDetection::fixedSolidSphereIntersectsFixedSolidBox(s, f));
    }

    {
        
        Vector3 v0(0, 0, 0);
        Vector3 v1(0, 0, -1);
        Vector3 v2(-1, 0, 0);
        Ray ray = Ray::fromOriginAndDirection(Vector3(-.15f, 1,-.15f), Vector3(0, -1, 0));
        Vector3 location, normal;
        double t = ray.intersectionTime(v0, v1, v2);
        debugAssert(t == 1.0);

        ray.origin.y = -ray.origin.y;
        t = ray.intersectionTime(v0, v1, v2);
        debugAssert(t == inf());

        // One-sided test
        ray.direction.y = -ray.direction.y;
        ray.origin.y = -ray.origin.y;
        t = ray.intersectionTime(v0, v1, v2);
        debugAssert(t == inf());

        // Time scale
        ray = Ray::fromOriginAndDirection(Vector3(-.15f, 1,-.15f), Vector3(0, -2, 0));
        t = ray.intersectionTime(v0, v1, v2);

        debugAssert(t == 0.5);
    }

    printf("passed\n");
}


void perfCollisionDetection() {
	measureTriangleCollisionPerformance();
	measureAABoxCollisionPerformance();
}
