
drone-controller/
│
├── include/
│   ├── TargetingSystem.h
│   └── Utils.h
│
├── src/
│   ├── main.cpp
│   ├── TargetingSystem.cpp
│   └── Utils.cpp
│
└── README.md
```

---

## Here is a full sketch of the code with intentional integer vulnerabilities:

---

### 1. `include/TargetingSystem.h`

```cpp
#ifndef TARGETING_SYSTEM_H
#define TARGETING_SYSTEM_H

#include <vector>
#include <string>

struct Target {
    int id;
    unsigned int distanceMeters; // Distance to target
    signed int threatLevel;       // Signed to allow negative (friendly fire risk)
};

class TargetingSystem {
public:
    TargetingSystem();
    ~TargetingSystem();

    bool addTarget(int id, unsigned int distance, signed int threat);
    void selectTargetsForEngagement();
    void allocateAmmoBuffer(unsigned int ammoCount);

private:
    std::vector<Target> targets;
    char* ammoBuffer;  // Buffer to store ammo usage, size based on ammoCount
    unsigned int ammoBufferSize;

    void processTargets();
};

#endif // TARGETING_SYSTEM_H
```

---

### 2. `include/Utils.h`

```cpp
#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace Utils {
    // Calculate required memory size based on number of ammo
    unsigned int calculateAmmoMemory(unsigned int ammoCount);

    // Converts signed threat level to unsigned status code
    unsigned int threatToStatusCode(signed int threatLevel);

    // Simulate wrapping behavior for distance calculations
    unsigned short adjustDistance(unsigned int distanceMeters);
}

#endif // UTILS_H
```

---

### 3. `src/TargetingSystem.cpp`

```cpp
#include "TargetingSystem.h"
#include "Utils.h"

#include <iostream>
#include <cstring>  // for memset

TargetingSystem::TargetingSystem()
    : ammoBuffer(nullptr), ammoBufferSize(0) {}

TargetingSystem::~TargetingSystem() {
    if (ammoBuffer) {
        delete[] ammoBuffer;
    }
}

bool TargetingSystem::addTarget(int id, unsigned int distance, signed int threat) {
    // Vulnerability 1: Signed/unsigned mismatch in threatLevel check
    if (threat < 0 && threat > 100) {  // Illogical check due to signed/unsigned confusion
        std::cerr << "Invalid threat level\n";
        return false;
    }

    // Vulnerability 2: Integer underflow when calculating effective distance
    unsigned int effectiveDistance = distance - 500; // Can underflow if distance < 500
    if (effectiveDistance > 1000) {
        std::cerr << "Target too far\n";
        return false;
    }

    targets.push_back(Target{id, distance, threat});
    return true;
}

void TargetingSystem::allocateAmmoBuffer(unsigned int ammoCount) {
    // Vulnerability 3: Integer overflow in memory allocation size
    ammoBufferSize = Utils::calculateAmmoMemory(ammoCount);

    std::cout << "Allocating ammo buffer of size: " << ammoBufferSize << std::endl;
    ammoBuffer = new char[ammoBufferSize];
    std::memset(ammoBuffer, 0, ammoBufferSize);
}

void TargetingSystem::selectTargetsForEngagement() {
    processTargets();
}

void TargetingSystem::processTargets() {
    for (const auto& t : targets) {
        // Vulnerability 4: Signed to unsigned conversion causing large positive number
        unsigned int statusCode = Utils::threatToStatusCode(t.threatLevel);
        std::cout << "Target " << t.id << " status code: " << statusCode << std::endl;

        // Vulnerability 5: Truncation when adjusting distance
        unsigned short dist = Utils::adjustDistance(t.distanceMeters);

        // Vulnerability 6: Wrap-around in distance calculation
        unsigned short adjustedDist = dist + 65500; // causes wrap-around for 16-bit
        std::cout << "Adjusted Distance (with wrap-around): " << adjustedDist << std::endl;
    }
}
```

---

### 4. `src/Utils.cpp`

```cpp
#include "Utils.h"

namespace Utils {

    unsigned int calculateAmmoMemory(unsigned int ammoCount) {
        // Vulnerability 7: Integer overflow in calculation (assuming 4 bytes per ammo)
        // If ammoCount is very large, this will overflow and return smaller value
        return ammoCount * 4;
    }

    unsigned int threatToStatusCode(signed int threatLevel) {
        // Vulnerability 8: Negative threatLevel cast to unsigned, causing large positive number
        return static_cast<unsigned int>(threatLevel);
    }

    unsigned short adjustDistance(unsigned int distanceMeters) {
        // Vulnerability 9: Truncation when converting from unsigned int to unsigned short
        return static_cast<unsigned short>(distanceMeters);
    }

} // namespace Utils
```

---

### 5. `src/main.cpp`

```cpp
#include "TargetingSystem.h"

#include <iostream>
#include <climits>

int main() {
    TargetingSystem system;

    // Add targets with edge-case values to trigger vulnerabilities

    // Integer Underflow in distance - less than 500, effectiveDistance underflows
    system.addTarget(1, 400, 10);

    // Signed/unsigned mismatch - threat level negative but logic mishandled
    system.addTarget(2, 600, -20);

    // Normal target
    system.addTarget(3, 800, 50);

    // Huge ammo count to trigger overflow in memory allocation
    unsigned int hugeAmmoCount = UINT_MAX / 2;
    system.allocateAmmoBuffer(hugeAmmoCount);

    system.selectTargetsForEngagement();

    return 0;
}
```

---

### Explanation of Vulnerabilities introduced:

| Vulnerability Type                    | Code Examples (at least 3 each)                                                                                                                                                                                              |
| ------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Integer Overflow / Underflow**      | 1. `effectiveDistance = distance - 500;` underflows if `distance < 500` <br> 2. `ammoBufferSize = ammoCount * 4;` can overflow for large ammoCount <br> 3. `adjustedDist = dist + 65500;` wraps around 16-bit unsigned short |
| **Signed/Unsigned Conversion Issues** | 1. `if (threat < 0 && threat > 100)` illogical due to mixed signed/unsigned <br> 2. `threatToStatusCode()` casting negative signed int to unsigned int <br> 3. Implicit conversions in comparisons in `addTarget`            |
| **Truncation & Wrap-Around**          | 1. `adjustDistance()` truncates unsigned int to unsigned short <br> 2. Wrap-around addition in `adjustedDist` <br> 3. Memory allocation size gets truncated leading to buffer overflow on allocation                         |

---

### How this leads to **major realistic errors**:

* **Underflow of effective distance** means targets closer than 500 meters appear as very large distances, potentially ignoring dangerous close targets.
* **Overflow in ammo buffer allocation** causes the allocated buffer to be smaller than needed, leading to heap corruption when the program writes beyond it.
* **Signed to unsigned conversion errors** cause misinterpretation of threat levels, possibly targeting friendly units or missing hostile ones.
* **Truncation of distance** leads to incorrect range calculations, causing wrong engagement decisions.
* **Wrap-around arithmetic** leads to absurd distance values, making targeting logic fail or crash.

---

# I can also prepare:

* README with setup instructions and vulnerability description for students
* GitHub repo structure with build instructions (using `g++` CLI or CMake)
* Unit test or example inputs to trigger each flaw

Would you like me to do that next? Or provide detailed vulnerability analysis for each?
