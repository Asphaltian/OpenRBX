#include "../include/G3DAll.h"

void testAtomicInt32() {
    printf("G3D::AtomicInt32 ");

    // Test first in the absence of threads.
    {
        AtomicInt32 a(1);
        debugAssert(a.value() == 1);

        a = 1;
        a.increment();
        debugAssert(a.value() == 2);

        a = 2;
        a.decrement();
        debugAssert(a.value() == 1);
		debugAssert(a.decrement() == 0);

        a = 10;
        debugAssert(a.value() == 10);

        a = 10;
        a.compareAndSet(10, 1);
        debugAssert(a.value() == 1);

        a = 1;
        a.compareAndSet(10, 15);
        debugAssert(a.value() == 1);

        a = 1;
        a.add(5);
        debugAssert(a.value() == 6);

        a = 6;
        a.sub(3);
        debugAssert(a.value() == 3);

        a = 6;
        a.sub(-3);
        debugAssert(a.value() == 9);
    }

    printf("passed\n");
}

