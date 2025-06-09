#include <iostream>
#include <string>
#include <sqlite3.h>
#include "httplib.h"

// Initialize SQLite DB and create a users table
void init_db(sqlite3* &db) {
    int rc = sqlite3_open(":memory:", &db); // in-memory DB
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << "\n";
        exit(1);
    }
    const char* create_table_sql =
        "CREATE TABLE users (id INTEGER PRIMARY KEY, username TEXT, password TEXT);"
        "INSERT INTO users (username, password) VALUES ('alice', 'alicepass');"
        "INSERT INTO users (username, password) VALUES ('bob', 'bobpass');";

    char* errmsg = nullptr;
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errmsg << "\n";
        sqlite3_free(errmsg);
        exit(1);
    }
}

// Simple HTML login form
const char* login_page_html = R"(
<!DOCTYPE html>
<html>
<head>
  <title>Login</title>
  <style>
    body {
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
      background: #f0f2f5;
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
    }

    .container {
      background: white;
      padding: 40px;
      border-radius: 10px;
      box-shadow: 0 8px 16px rgba(0, 0, 0, 0.2);
      text-align: center;
      width: 300px;
    }

    h2 {
      margin-bottom: 20px;
      color: #333;
    }

    input[type="text"], input[type="password"] {
      width: 90%;
      padding: 10px;
      margin: 8px 0;
      border: 1px solid #ccc;
      border-radius: 5px;
    }

    input[type="submit"] {
      width: 100%;
      padding: 10px;
      background-color: #4CAF50;
      border: none;
      color: white;
      font-weight: bold;
      border-radius: 5px;
      cursor: pointer;
    }

    input[type="submit"]:hover {
      background-color: #45a049;
    }

    .error {
      color: red;
      margin-top: 10px;
    }
  </style>
</head>
<body>
  <div class="container">
    <h2>Login</h2>
    <form action="/login" method="post">
      <input name="username" type="text" placeholder="Username" required /><br/>
      <input name="password" type="password" placeholder="Password" required /><br/>
      <input type="submit" value="Login" />
    </form>
    %ERROR_MSG%
  </div>
</body>
</html>
)";


int main() {
    sqlite3* db = nullptr;
    init_db(db);

    httplib::Server svr;

    // Serve login page at /
    svr.Get("/", [&](const httplib::Request& req, httplib::Response& res) {
        std::string page(login_page_html);
        // no error initially
        size_t pos = page.find("%ERROR_MSG%");
        if (pos != std::string::npos) {
            page.replace(pos, 11, "");
        }
        res.set_content(page, "text/html");
    });

    // Vulnerable login endpoint
    svr.Post("/login", [&](const httplib::Request& req, httplib::Response& res) {
        auto username = req.get_param_value("username");
        auto password = req.get_param_value("password");

        // Vulnerable SQL query
        std::string query = "SELECT * FROM users WHERE username = '" + username +
                            "' AND password = '" + password + "';";

        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            res.status = 500;
            res.set_content("SQL error during prepare.", "text/plain");
            return;
        }

        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            // User found — show welcome page
            std::string user = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            std::string welcome_page = R"(
<!DOCTYPE html>
<html>
<head>
  <title>Welcome</title>
  <style>
    body {
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
      background-color: #e8f0fe;
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
    }

    .welcome-box {
      background: white;
      padding: 40px;
      border-radius: 10px;
      box-shadow: 0 8px 16px rgba(0, 0, 0, 0.2);
      text-align: center;
    }

    h2 {
      color: #333;
    }

    p {
      font-size: 16px;
      color: #555;
    }
  </style>
</head>
<body>
  <div class="welcome-box">
    <h2>Welcome, )" + user + R"(!</h2>
    <p>Login successful.</p>
  </div>
</body>
</html>
)";

            res.set_content(welcome_page, "text/html");
        } else {
            // Login failed — show login page with error message
            std::string page(login_page_html);
size_t pos = page.find("%ERROR_MSG%");
if (pos != std::string::npos) {
    page.replace(pos, 11, "<div class='error'>Invalid username or password</div>");
}
res.set_content(page, "text/html");
        }
        sqlite3_finalize(stmt);
    });

    std::cout << "Server started at http://localhost:8080\n";
    svr.listen("0.0.0.0", 8080);

    sqlite3_close(db);
    return 0;
}
