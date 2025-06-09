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
