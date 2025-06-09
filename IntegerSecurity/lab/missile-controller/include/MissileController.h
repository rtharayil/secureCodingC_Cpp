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
