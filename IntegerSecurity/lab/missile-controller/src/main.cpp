#include "MissileController.h"
#include <iostream>
#include <climits>

int main() {
    MissileController controller;

    // Add targets with edge values to trigger vulnerabilities

    // Underflow: distance < 1000 triggers underflow
    controller.addTarget(1, 800, 5);

    // Signed/Unsigned issue: invalid priority level check
    controller.addTarget(2, 1500, -5);

    // Normal target
    controller.addTarget(3, 3000, 7);

    // Large warhead count to trigger overflow in memory allocation
    unsigned int largeCount = UINT_MAX / 2;
    controller.allocateWarheadBuffer(largeCount);

    controller.engageTargets();

    return 0;
}
