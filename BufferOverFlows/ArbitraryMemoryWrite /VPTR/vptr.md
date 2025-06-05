

### 🔍 Code Snippet

```cpp
void **fake_vtable = new void*[1];
fake_vtable[0] = (void *)Evil::evil_function;  // Redirect to Evil::evil_function

// Overwrite obj's vptr
void **raw_obj = (void **)obj;
raw_obj[0] = fake_vtable;
```

---

## ✅ Background: How C++ Virtual Functions Work

When you declare a C++ class with virtual methods:

* The compiler adds a **hidden pointer** (called the **vptr**) at the **start of the object’s memory layout**.
* The vptr points to a **vtable**, which is an array of function pointers — one per virtual method.
* So, when you call a virtual method like `obj->greet()`, it internally becomes:

  ```cpp
  obj->vptr[0](obj);  // First function in the vtable
  ```

---

## 📦 Step-by-Step Explanation

### 🔸 `void **fake_vtable = new void*[1];`

* This allocates a **new array** on the heap with **1 slot for a function pointer**.
* We're simulating a **fake vtable** with one virtual method.

### 🔸 `fake_vtable[0] = (void *)Evil::evil_function;`

* We're saying: “In this fake vtable, the first function should be `Evil::evil_function()`.”
* `(void *)` is a cast to allow placing any function address into the vtable.
* Now the fake vtable looks like:

  ```text
  [ fake_vtable ]
       |
       +--> address of Evil::evil_function
  ```

---

### 🔸 `void **raw_obj = (void **)obj;`

* `obj` is a pointer to a `Base` object.
* We are casting it to `void**` so we can **manually access and overwrite the vptr**.
* This is a **dangerous cast**, but it’s legal in C++ for low-level memory manipulation.

  The memory layout of `obj` looks like this:

  ```
  obj
   |
   +--> [ vptr ] ------> [ real vtable ]
                         [ greet() ]
  ```

### 🔸 `raw_obj[0] = fake_vtable;`

* Now we overwrite the **first pointer inside the object (`vptr`)**.

* So instead of pointing to the real vtable (`Base::greet()`), it now points to our **fake vtable**:

  ```
  obj
   |
   +--> [ vptr ] ------> [ fake_vtable ]
                         [ Evil::evil_function ]
  ```

* So the next time we do `obj->greet();`, the program will actually execute `Evil::evil_function()`!

---

## 🧪 Visualization

### 🧠 Memory Before Overwrite

```
obj (Base*)
 |
 +--> [ vptr ] ---------> [ real vtable ]
                           +--> Base::greet
```

### 🧠 Memory After Overwrite

```
obj (Base*)
 |
 +--> [ vptr ] ---------> [ fake vtable ]
                           +--> Evil::evil_function
```

---

## 🚨 Why This Matters

This kind of overwrite is:

* A common technique in **heap exploitation**, especially with **Use-After-Free**.
* Dangerous because it **bypasses C++ type safety**.
* Used in real-world exploits to gain **arbitrary code execution**.


