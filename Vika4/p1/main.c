#include "testlib.h"
#include "scheduler.h"

// You schedule one thread with priority MAX_PRIORITY every time.
int main() {
    test_start("scheduler.c");

    initScheduler();
    startThread(0, MAX_PRIORITY);
    onThreadReady(0);
    for (int i = 0; i < 10; i++) {
        test_equals_int(scheduleNextThread(), 0);
        onThreadPreempted(0);
    }

    return test_end();
}