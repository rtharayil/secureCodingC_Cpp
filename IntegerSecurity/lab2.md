Sure! Here’s a **multi-file C++ example** simulating an **Advanced Missile Controller system** with realistic integer vulnerabilities in these categories:

* Integer Overflow and Underflow
* Signed/Unsigned Conversion Issues
* Truncation and Wrap-Around Behavior
* Integer overflow causing incorrect memory allocation and buffer corruption

This is designed as a GitHub repo style project with multiple files to be used as a hands-on exercise for secure coding training.

---

# Repo Structure

```
missile-controller/
│
├── include/
│   ├── MissileController.h
│   └── Utils.h
│
├── src/
│   ├── main.cpp
│   ├── MissileController.cpp
│   └── Utils.cpp
│
└── README.md
```

---

# Code with vulnerabilities

---

### 1. `include/MissileController.h`

```cpp
#ifndef MISSILE_CONTROLLER_H
#define MISSILE_CONTROLLER_H

#include <vector>
#include <string>

struct Target {
    int id;
    unsigned int distanceMeters;  // Distance to target
    signed int priorityLevel;     // Signed priority (-ve means no engagement)
};

class MissileController {
public:
    MissileController();
    ~MissileController();

    bool addTarget(int id, unsigned int distance, signed int priority);
    void engageTargets();
    void allocateWarheadBuffer(unsigned int warheadCount);

private:
    std::vector<Target> targets;
    char* warheadBuffer;
    unsigned int warheadBufferSize;

    void processTargets();
};

#endif // MISSILE_CONTROLLER_H
```

---

### 2. `include/Utils.h`

```cpp
#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace Utils {
    unsigned int calculateWarheadMemory(unsigned int count);
    unsigned int priorityToCode(signed int priorityLevel);
    unsigned short adjustDistance(unsigned int distanceMeters);
}

#endif // UTILS_H
```

---

### 3. `src/MissileController.cpp`

```cpp
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
```

---

### 4. `src/Utils.cpp`

```cpp
#include "Utils.h"

namespace Utils {

    unsigned int calculateWarheadMemory(unsigned int count) {
        // Vulnerability 7: Integer overflow in buffer size calc
        return count * 8;  // 8 bytes per warhead assumed
    }

    unsigned int priorityToCode(signed int priorityLevel) {
        // Vulnerability 8: Negative signed int cast to large unsigned int
        return static_cast<unsigned int>(priorityLevel);
    }

    unsigned short adjustDistance(unsigned int distanceMeters) {
        // Vulnerability 9: Truncation from unsigned int to unsigned short
        return static_cast<unsigned short>(distanceMeters);
    }

}
```

---

### 5. `src/main.cpp`

```cpp
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
```

---

# Explanation of Vulnerabilities:

| Category                       | Examples & Description                                                                                                                                                                                         |
| ------------------------------ | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Integer Overflow/Underflow** | - `adjustedDistance = distance - 1000;` underflows if `distance < 1000` <br> - `warheadBufferSize = count * 8` overflows with large count <br> - `finalDist = dist + 65000` wraps around 16-bit unsigned short |
| **Signed/Unsigned Conversion** | - Illogical condition `priority < 0 && priority > 10` due to signed/unsigned mixup <br> - `priorityToCode()` casts negative signed int to large unsigned int <br> - Implicit conversions in comparisons        |
| **Truncation & Wrap-Around**   | - Truncation when converting `distanceMeters` to unsigned short <br> - Wrap-around addition on distance calculations <br> - Buffer size truncation causes buffer overflow in `allocateWarheadBuffer`           |

---

# Realistic Impact Examples:

* Underflow on distance calculation causes valid targets to be rejected or misclassified.
* Overflow in warhead buffer size allocates too little memory, causing heap corruption and crashes.
* Misinterpreted priority levels lead to wrong engagement decisions or ignoring critical targets.
* Distance truncation and wrap-around yield faulty range measurements, breaking targeting accuracy.

---

# Next Steps?

I can help prepare:

* README with build & run instructions and explanation of vulnerabilities for students
* CMakeLists.txt or Makefile for easy compilation
* Detailed lab instructions with steps to identify and fix each vulnerability
* Sample tests or input sets that trigger these bugs

Would you like me to do that?
