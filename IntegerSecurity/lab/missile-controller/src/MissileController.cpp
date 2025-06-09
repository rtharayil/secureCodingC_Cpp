#include "MissileController.h"
#include "Utils.h"

#include <iostream>
#include <cstring>

MissileController::MissileController()
    : warheadBuffer(nullptr), warheadBufferSize(0) {}

MissileController::~MissileController() {
    if (warheadBuffer) {
        delete[] warheadBuffer;
    }
}

bool MissileController::addTarget(int id, unsigned int distance, signed int priority) {
    // Vulnerability 1: Incorrect signed/unsigned logic for priority validation
    if (priority < 0 && priority > 10) {  // Illogical condition
        std::cerr << "Invalid priority level\n";
        return false;
    }

    // Vulnerability 2: Integer underflow on distance check
    unsigned int adjustedDistance = distance - 1000; // Underflow if distance < 1000
    if (adjustedDistance > 5000) {
        std::cerr << "Target out of range\n";
        return false;
    }

    targets.push_back(Target{id, distance, priority});
    return true;
}

void MissileController::allocateWarheadBuffer(unsigned int warheadCount) {
    // Vulnerability 3: Integer overflow in memory allocation size
    warheadBufferSize = Utils::calculateWarheadMemory(warheadCount);

    std::cout << "Allocating warhead buffer of size: " << warheadBufferSize << std::endl;
    warheadBuffer = new char[warheadBufferSize];
    std::memset(warheadBuffer, 0, warheadBufferSize);
}

void MissileController::engageTargets() {
    processTargets();
}

void MissileController::processTargets() {
    for (const auto& target : targets) {
        // Vulnerability 4: Signed to unsigned conversion causing large value
        unsigned int statusCode = Utils::priorityToCode(target.priorityLevel);
        std::cout << "Target " << target.id << " status code: " << statusCode << std::endl;

        // Vulnerability 5: Truncation on distance conversion
        unsigned short dist = Utils::adjustDistance(target.distanceMeters);

        // Vulnerability 6: Wrap-around on distance arithmetic
        unsigned short finalDist = dist + 65000;  // 16-bit wrap-around
        std::cout << "Final Distance (wrap-around): " << finalDist << std::endl;
    }
}
