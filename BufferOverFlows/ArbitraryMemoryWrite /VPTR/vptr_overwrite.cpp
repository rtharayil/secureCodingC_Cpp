#include <iostream>
#include <cstring>
using namespace std;

class Base {
public:
    virtual void greet() {
        cout << "Hello from Base!" << endl;
    }
};

class Evil {
public:
    static void evil_function() {
        cout << "🚨 Evil function executed! 🚨" << endl;
    }
};

int main() {
    Base *obj = new Base();

    cout << "[Before] ";
    obj->greet();  // Calls Base::greet

    // Simulate raw memory access to overwrite vptr
    void **fake_vtable = new void*[1];
    fake_vtable[0] = (void *)Evil::evil_function;  // Redirect to Evil::evil_function

    // Overwrite obj's vptr
    void **raw_obj = (void **)obj;
    raw_obj[0] = fake_vtable;

    cout << "[After] ";
    obj->greet();  // Calls Evil::evil_function!

    delete[] fake_vtable;
    delete obj;

    return 0;
}
