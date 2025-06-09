#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace Utils {
    unsigned int calculateWarheadMemory(unsigned int count);
    unsigned int priorityToCode(signed int priorityLevel);
    unsigned short adjustDistance(unsigned int distanceMeters);
}

#endif // UTILS_H
