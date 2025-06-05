#include <iostream>
#include <cstring>

void defaultShutdown() {
    std::cout << "[INFO] Executing default shutdown procedure.\n";
}

void overrideBrakeSystem() {
    std::cout << "[DANGER] Brake system overridden! Turbine running uncontrolled!\n";
}

class WindTurbineController {
public:
    char configBuffer[64];
    int rpmLimit;
    void (*safetyHandler)();

    WindTurbineController() {
        rpmLimit = 1500;
        safetyHandler = defaultShutdown;
    }

    void applyRemoteConfig(const char* input, size_t len) {
        memcpy(configBuffer, input, len);  // ⚠️ Vulnerable
        if (rpmLimit > 2000) {
            safetyHandler();
        }
    }
};

int main() {
    WindTurbineController turbine;

    std::cout << "[Before] RPM limit: " << turbine.rpmLimit << "\n";
    std::cout << "[Before] SafetyHandler @ " << (void*)turbine.safetyHandler << "\n";

    char payload[80];
    memset(payload, 'A', 64);
   *(int*)(payload + 64) = 2500;
    *(int*)(payload + 4) = 2500;
   *(void (**)(void))(payload + 72) = overrideBrakeSystem;
    
    turbine.applyRemoteConfig(payload, sizeof(payload));

    std::cout << "[After] RPM limit: " << turbine.rpmLimit << "\n";
    std::cout << "[After] SafetyHandler @ " << (void*)turbine.safetyHandler << "\n";

    return 0;
}
