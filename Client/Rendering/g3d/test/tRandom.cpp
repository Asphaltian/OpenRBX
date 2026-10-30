#include "../include/G3DAll.h"

void testRandom() {
    printf("Random number generators ");

    int num0 = 0;
    int num1 = 0;
    for (int i = 0; i < 10000; ++i) {
        switch (iRandom(0, 1)) {
        case 0:
            ++num0;
            break;

        case 1:
            ++num1;
            break;

        default:
            debugAssertM(false, "Random number outside the range [0, 1] from iRandom(0,1)");
        }
    }
    int difference = iAbs(num0 - num1);
    debugAssertM(difference < 300, "Integer random number generator appears skewed.");
    (void)difference;

    for (int i = 0; i < 100; ++i) {
        double r = random(0, 1);
        debugAssert(r >= 0.0);
        debugAssert(r <= 1.0);
        (void)r;
    }


    // Triangle.randomPoint
    Triangle tri(Vector3(0,0,0), Vector3(1,0,0), Vector3(0,1,0));
    for (int i = 0; i < 1000; ++i) {
        Vector3 p = tri.randomPoint();
        debugAssert(p.z == 0);
        debugAssert(p.x <= 1.0 - p.y);
        debugAssert(p.x >= 0);
        debugAssert(p.y >= 0);
        debugAssert(p.x <= 1.0);
        debugAssert(p.y <= 1.0);
    }


    for (int i = 0; i < 100; i++) {
         Vector3 point = tri.randomPoint();
         debugAssert(CollisionDetection::isPointInsideTriangle(
                 tri.vertex(0),
                 tri.vertex(1),
                 tri.vertex(2),
                 tri.normal(),
                 point));
    }

    printf("passed\n");
}
