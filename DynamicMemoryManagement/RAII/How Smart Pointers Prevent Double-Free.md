
## ✅ How Smart Pointers Prevent Double-Free

### 1. `std::unique_ptr`

* Represents **sole ownership** of a dynamically allocated object.
* Automatically deletes the object when it goes out of scope.
* Cannot be copied—only moved.
* Prevents double-free because **only one pointer** can own the object at any time.

**Example:**

```cpp
#include <memory>
#include <iostream>

struct Node {
    int value;
    Node(int v) : value(v) {}
    ~Node() { std::cout << "Destroying Node " << value << "\n"; }
};

int main() {
    std::unique_ptr<Node> ptrA(new Node(10));

    // std::unique_ptr<Node> ptrB = ptrA;  ❌ Compile error (copying not allowed)

    std::unique_ptr<Node> ptrB = std::move(ptrA);  // ✅ Ownership moved to ptrB

    // Node will be destroyed once ptrB goes out of scope
    return 0;
}
```

---

### 2. `std::shared_ptr`

* Allows **shared ownership** of a dynamically allocated object.
* Internally uses a **reference count**.
* Deletes the object when the last shared pointer goes out of scope.

**Example:**

```cpp
#include <memory>
#include <iostream>

struct Node {
    int value;
    Node(int v) : value(v) {}
    ~Node() { std::cout << "Destroying shared Node " << value << "\n"; }
};

int main() {
    std::shared_ptr<Node> sharedNode = std::make_shared<Node>(100);

    std::shared_ptr<Node> listA = sharedNode;
    std::shared_ptr<Node> listB = sharedNode;

    std::cout << "Reference count: " << sharedNode.use_count() << "\n";  // 3

    // Automatically deleted when all shared_ptrs go out of scope
    return 0;
}
```

---

## 🔁 Smart Pointer vs Manual Memory

| Feature                 | Raw Pointers / `malloc`/`free` | `unique_ptr` | `shared_ptr` |
| ----------------------- | ------------------------------ | ------------ | ------------ |
| Automatic cleanup       | ❌                              | ✅            | ✅            |
| Prevent double-free     | ❌                              | ✅            | ✅            |
| Multiple owners allowed | ✅ (unsafe)                     | ❌            | ✅            |
| Reference counting      | ❌                              | ❌            | ✅            |
| Destructor call (C++)   | ❌ (if using `free`)            | ✅            | ✅            |

---

## ⚙️ Embedded/RTOS Consideration

Smart pointers are generally safe in **RTOS environments** with C++ support, but:

* Avoid `shared_ptr` if memory overhead or reference counting is too costly.
* Prefer `unique_ptr` with custom deleters for peripheral/resource cleanup (e.g., closing handles, freeing buffers).
* Some embedded C++ standard libraries may omit full smart pointer support—check your compiler (e.g., `libstdc++`, `libc++`).

---

## 🧩 Summary

Yes, **smart pointers can absolutely prevent double-free** bugs:

* Use `std::unique_ptr` when you have a **clear single owner**.
* Use `std::shared_ptr` when **multiple objects share ownership**.

They promote safer code and eliminate most manual memory bugs when used correctly.


