#include "../include/G3DAll.h"
#ifdef G3D_LINUX
#include <pthread.h>
#endif

class WKFoo : public ReferenceCountedObject {
public:

    std::string name;

    WKFoo(const std::string& x) : name(x) {
        //printf("Foo(\"%s\") = 0x%x\n", name.c_str(), this);
    }

    ~WKFoo() {
        //printf("~Foo(\"%s\") = 0x%x\n", name.c_str(), this);
    }
};
typedef ReferenceCountedPointer<WKFoo>     WKFooRef;
typedef WeakReferenceCountedPointer<WKFoo> WKFooWeakRef;


namespace Circle {
class A;
class B : public G3D::ReferenceCountedObject {
public:
    G3D::WeakReferenceCountedPointer<A> weakRefToA;
};

class A : public G3D::ReferenceCountedObject {
public:
    G3D::ReferenceCountedPointer<B> refToB;
};
}

void testCycle() {
    G3D::ReferenceCountedPointer<Circle::A> a = new Circle::A();
    a->refToB = new Circle::B();
    a->refToB->weakRefToA = a;
    a = NULL;
}

void testWeakPointer() {
    printf("WeakReferenceCountedPointer ");

    testCycle();

    WKFooWeakRef wB;
    {
        WKFooRef A = new WKFoo("A");

        WKFooWeakRef wA(A);

        debugAssert(wA.createStrongPtr().isNull() == false);

        A = NULL;

        debugAssert(wA.createStrongPtr().isNull() == true);

        
        WKFooRef B = new WKFoo("B");
        
        A = B;
        debugAssert(wA.createStrongPtr().isNull() == true);
        debugAssert(wB.createStrongPtr().isNull() == true);
        
        wA = A;

        debugAssert(wA.createStrongPtr().isNull() == false);

        wB = B;

        debugAssert(A == B);
        debugAssert(wA == wB);

        wA = NULL;
        debugAssert(wA.createStrongPtr().isNull() == true);

        {
            WKFooRef C = new WKFoo("C");
        }

        debugAssert(wB.createStrongPtr().isNull() == false);
    }
    debugAssert(wB.createStrongPtr().isNull() == true);

    printf("passed\n");
}


int numRCPFoo = 0;
class RCPFoo : public G3D::ReferenceCountedObject {
public:
    int x;
    RCPFoo() {
        ++numRCPFoo;
    }
    ~RCPFoo() {
        --numRCPFoo;
    }
};

typedef G3D::ReferenceCountedPointer<RCPFoo> RCPFooRef;



class RefSubclass : public RCPFoo {
};
typedef G3D::ReferenceCountedPointer<RefSubclass> RefSubclassRef;


class Reftest : public ReferenceCountedObject {
public:
    static Array<std::string> sequence;
    const char* s;
    Reftest(const char* s) : s(s){
        debugPrintf("alloc 0x%x (%s)\n", this, s);
        sequence.append(format("%s", s));
    }
    ~Reftest() {
        debugPrintf("free 0x%x (~%s)\n", this, s);
        sequence.append(format("~%s", s));
    }
};
class Reftest2 : public Reftest {
public:
    Reftest2() : Reftest("2") {
    }
};
typedef ReferenceCountedPointer<Reftest> ARef;
typedef ReferenceCountedPointer<Reftest2> ARef2;
Array<std::string> Reftest::sequence;


/* Called from testRCP to test automatic down-casting */
static void subclasstest(const RCPFooRef& b) {
    (void)b;
}

static void testRCP() {
    printf("ReferenceCountedPointer");

    debugAssert(numRCPFoo == 0);
    RCPFooRef a = new RCPFoo();
    debugAssert(numRCPFoo == 1);
    debugAssert(a.isLastReference());

    {
        RCPFooRef b = new RCPFoo();
        debugAssert(numRCPFoo == 2);
        b = a;
        debugAssert(numRCPFoo == 1);
        debugAssert(! a.isLastReference());
        debugAssert(! b.isLastReference());
    }

    debugAssert(a.isLastReference());
    debugAssert(numRCPFoo == 1);


    // Test allocation and deallocation of 
    // reference counted values.
    {
        ARef a = new Reftest("a");
        ARef b = new Reftest("b");

        a = b;
        Reftest::sequence.append("--");
        debugPrintf("---------\n");
        b = NULL;
        Reftest::sequence.append("--");
        debugPrintf("---------\n");
    }

    debugAssert(Reftest::sequence[0] == "a");
    debugAssert(Reftest::sequence[1] == "b");
    debugAssert(Reftest::sequence[2] == "~a");
    debugAssert(Reftest::sequence[3] == "--");
    debugAssert(Reftest::sequence[4] == "--");
    debugAssert(Reftest::sequence[5] == "~b");

    Reftest::sequence.clear();

    // Test type hierarchies with reference counted values.

    {
        ARef one = new Reftest("1");
        ARef2 two = new Reftest2();

        one = (ARef)two;
    }
    debugAssert(Reftest::sequence[0] == "1");
    debugAssert(Reftest::sequence[1] == "2");
    debugAssert(Reftest::sequence[2] == "~1");
    debugAssert(Reftest::sequence[3] == "~2");
    Reftest::sequence.clear();

    {
        ARef one = new Reftest2();
    }
    debugAssert(Reftest::sequence[0] == "2");
    debugAssert(Reftest::sequence[1] == "~2");
    Reftest::sequence.clear();

    {
        // Should not compile
//        ARef2 one = new Reftest("1");
    }


    // Test subclassing

    {
        RefSubclassRef s = new RefSubclass();
        RCPFooRef b;

        // s is a subclass, so this should call the templated
        // constructor and succeed.
        b = s;

        // Likewise
        subclasstest(s);
    }
}

RCPFooRef f1, f2, f3;

RCPFooRef someFunction(const RCPFooRef& f) {
    return f;
}


/** Causes many accesses against global variables f1, f2, and f3 in an attempt
    to create race conditions.
*/
void* threadProc(void* x) {
    for (int i = 0; i < 100000; ++i) {
        // Increase and decrease counter
        {
            RCPFooRef x = f1;
        }
        
        // Swap; exercises counters
        {
            RCPFooRef temp = f2;
            f2 = f3;
            f3 = f2;
        }

        // Let counter hit zero (maybe; depends on other thread)
        f1 = NULL;
        
        // Swap; exercises counters
        {
            RCPFooRef temp = f2;
            f2 = f3;
            f3 = f2;
        }
        
        f1 = new RCPFoo();

        f2 = someFunction(f2);
        {
            // Copy constructor
            RCPFooRef x(f2);
            f2 = x;
        }

        // Subclass assignment
        f3 = new RefSubclass();

        {
            RCPFooRef temp = f2;
            f2 = f3;
            f3 = f2;
        }

        // Swap; exercises counters
        {
            RCPFooRef temp = f1;
            f1 = f3;
            f3 = f1;
        }
    }

    return NULL;
}

static void testThreads() {
    f1 = f2 = f3 = NULL;

    f1 = new RCPFoo();
    f2 = new RCPFoo();
    f3 = new RCPFoo();

#   ifdef G3D_LINUX
    {
        pthread_t thread;

        // Race two copies of the thread proc
        pthread_create(&thread, NULL, threadProc, NULL);
        threadProc(NULL);
        
        pthread_join(thread, NULL);
    }
#   endif
}

void testReferenceCount() {
    testWeakPointer();
    testRCP();
    testThreads();
}
