#include <iostream>
#include <string>
#include <cstring> // for memset (optional)

using namespace std;

class TimerConfig {
public:
    int cookTimeSeconds;
    int powerLevel;
    int weightGrams;
    bool safetyOverride;
    string cookingMode;

    // Intentionally no constructor to demonstrate uninitialized values
};

void startMicrowave(const TimerConfig& config) {
    cout << "🟢 Starting microwave...\n";

    // Debug info
    cout << "[DEBUG] Configuration:\n";
    cout << "  cookTimeSeconds: " << config.cookTimeSeconds << "\n";
    cout << "  powerLevel: " << config.powerLevel << "\n";
    cout << "  weightGrams: " << config.weightGrams << "\n";
    cout << "  safetyOverride: " << (config.safetyOverride ? "true" : "false") << "\n";
    cout << "  cookingMode: \"" << config.cookingMode << "\"\n";

    if (config.safetyOverride) {
        cout << "❌ Safety override activated! Aborting.\n";
        return;
    }

    if (config.cookTimeSeconds <= 0 || config.powerLevel < 1 || config.powerLevel > 10) {
        cout << "❌ Invalid time or power level!\n";
        return;
    }

    cout << "✔️ Cooking for " << config.cookTimeSeconds << " seconds at power level " << config.powerLevel << ".\n";

    if (config.cookingMode == "auto") {
        if (config.weightGrams > 0)
            cout << "📦 Auto mode: Cooking " << config.weightGrams << " grams.\n";
        else
            cout << "⚠️ Invalid weight for auto-cook!\n";
    } else {
        cout << "👨 Manual cooking mode.\n";
    }

    cout << "✅ Microwave started!\n";
}

int main() {
    cout << "=== 🧪 Uninitialized Configuration Example ===\n";
    TimerConfig badConfig;



    // Only partially initialized
    badConfig.cookTimeSeconds = 60;
    badConfig.powerLevel = 7;
    // Note: safetyOverride, weightGrams, and cookingMode left uninitialized

    startMicrowave(badConfig);

    cout << "\n=== ✅ Properly Initialized Configuration ===\n";
    TimerConfig goodConfig;
    goodConfig.cookTimeSeconds = 90;
    goodConfig.powerLevel = 5;
    goodConfig.weightGrams = 500;
    goodConfig.safetyOverride = false;
    goodConfig.cookingMode = "auto";

    startMicrowave(goodConfig);

    return 0;
}
