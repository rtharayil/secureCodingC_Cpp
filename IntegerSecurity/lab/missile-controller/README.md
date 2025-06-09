# Advanced Missile Controller (Secure Coding Lab)

This project simulates an **Advanced Missile Controller** used in a military context. It includes intentional **integer security vulnerabilities** across multiple categories to help students practice secure coding techniques in C++.

> ⚠️ **This is an educational project designed to demonstrate vulnerabilities like integer overflow, truncation, and signed/unsigned errors.** Do not use this code in production or safety-critical systems.

---

## 🔧 Project Structure

missile-controller/
├── include/
│ ├── launcher.h
│ ├── targeting.h
│ └── telemetry.h
├── src/
│ ├── main.cpp
│ ├── launcher.cpp
│ ├── targeting.cpp
│ └── telemetry.cpp
├── obj/ # Auto-created by Makefile
├── bin/ # Contains compiled executable
├── Makefile
└── README.md

yaml
Copy
Edit

---

## 🛠️ Build Instructions

### Prerequisites

- GCC or Clang with C++17 support
- `make`
- (Optional) AddressSanitizer and UBSan for detecting runtime bugs

### Build the Project

```bash
make
This will:

Compile all .cpp files inside src/

Output the executable to bin/missile_controller

🚀 Run the Program
bash
Copy
Edit
./bin/missile_controller
You may also pass input from a file to simulate field usage:

bash
Copy
Edit
./bin/missile_controller < input/testdata.txt