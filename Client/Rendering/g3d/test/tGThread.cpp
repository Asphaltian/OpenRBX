#include "../include/G3DAll.h"
#include <string>

class TGThread : public GThread {
public:
    TGThread(const std::string& n): GThread(n),
      _value(0) {}

    int value() {
        // Shouldn't need lock
        return _value;
    }

    void incValue() {
        getterMutex.lock();
        ++_value;
        getterMutex.unlock();
    }
protected:
    virtual void threadMain() {
        ++_value;
        return;
    }

    int _value;
    GMutex getterMutex;
};

void testGThread() {
    printf("G3D::GThread ");

    {
        TGThread tGThread("tGThread");
        debugAssert(tGThread.value() == 0);

        bool started = tGThread.start();
        debugAssert(started);

        tGThread.waitForCompletion();
        debugAssert(tGThread.completed());

        debugAssert(tGThread.value() == 1);

        tGThread.incValue();
        debugAssert(tGThread.value() == 2);
    }

    printf("passed\n");
}

