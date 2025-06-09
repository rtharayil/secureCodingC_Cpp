
## 🔍 **Understanding SQL Injection Using a C++ Example**

---

### 🔧 What is SQL Injection?

**SQL Injection** is a type of security vulnerability that allows an attacker to interfere with the queries that an application makes to its database. It typically occurs when:

* User inputs are directly embedded into SQL statements,
* Without proper sanitization or escaping.

This can allow attackers to:

* Bypass authentication,
* View, modify, or delete sensitive data,
* Execute administrative operations on the database.

---

### 📄 The Vulnerable Code

Here’s the relevant portion of your C++ server:

```cpp
std::string query = "SELECT * FROM users WHERE username = '" + username +
                    "' AND password = '" + password + "';";
```

This line dynamically builds the SQL query using raw input from the user — **without sanitizing or escaping**.

If the input is:

```bash
username=alice' -- 
password=anything
```

The resulting SQL becomes:

```sql
SELECT * FROM users WHERE username = 'alice' -- ' AND password = 'anything';
```

The `--` starts a comment in SQL, so everything after it is ignored. This turns the query into:

```sql
SELECT * FROM users WHERE username = 'alice'
```

If a user named `alice` exists, the login is **bypassed without knowing the password**.

---

## 🚨 Demonstrating the Attack

Using `curl`:

```bash
curl -X POST -d "username=alice' -- &password=wrong" http://localhost:8080/login
```

This returns:

```
Welcome, alice!
```

✅ Attack Successful.

---

## 🧯 How to Fix It

### ✅ Use Prepared Statements with Bound Parameters

**Prepared statements** separate SQL logic from user input. Instead of inserting input directly into the query, we use placeholders (`?`) and bind user data.

---

### 🔐 Secure Replacement for the Login Handler

Replace this vulnerable code:

```cpp
std::string query = "SELECT * FROM users WHERE username = '" + username +
                    "' AND password = '" + password + "';";
sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
```

With **this secure version**:

```cpp
const char* query = "SELECT * FROM users WHERE username = ? AND password = ?";
sqlite3_stmt* stmt;
int rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
if (rc != SQLITE_OK) {
    res.status = 500;
    res.set_content("SQL error during prepare.", "text/plain");
    return;
}

// Bind username and password to the query
sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);
```

Why this works:

* The query is compiled first (`prepare_v2`),
* Input is passed in separately using `bind_text`,
* SQLite handles escaping and ensures that input is treated as **data**, not code.

---

## 🔐 Comparison Table

| Technique          | Vulnerable                  | Secure                         |
| ------------------ | --------------------------- | ------------------------------ |
| Query construction | Concatenates raw user input | Uses `?` placeholders          |
| Input escaping     | None                        | Automatic via `sqlite3_bind_*` |
| Risk of injection  | ✅ High                      | ❌ None                         |
| Readability        | Poor                        | Better with practice           |

---

## ✅ Final Thoughts

### When designing secure systems:

* **Never trust user input.**
* **Always use parameterized queries**.
* Validate and sanitize inputs as a **defense-in-depth** strategy.
* Use tools like **static analyzers** to catch injection-prone patterns.

---

### 🛠 Want More?

You can extend this example by adding:

* A “safe login” version next to the vulnerable one
* Logging attempts for auditing
* Rate-limiting or CAPTCHA to prevent brute force
* A logout function and CSRF protection for full coverage


