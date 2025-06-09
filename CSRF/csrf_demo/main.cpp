#include "httplib.h"
#include <unordered_map>
#include <random>
#include <sstream>
#include <ctime>

using namespace httplib;

// Session store
struct Session {
    std::string username;
    std::string email;
    std::string csrf_token;
};

std::unordered_map<std::string, Session> sessions;

// Utility: Generate random strings
std::string generate_token(int length = 32) {
    static const char charset[] =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, sizeof(charset) - 2);
    std::string token;
    for (int i = 0; i < length; ++i)
        token += charset[dis(gen)];
    return token;
}

// Utility: Get session ID from cookies
std::string get_session_id(const Request& req) {
    auto it = req.headers.find("Cookie");
    if (it == req.headers.end()) return "";
    std::string cookie = it->second;
    std::string prefix = "SESSION_ID=";
    size_t pos = cookie.find(prefix);
    if (pos == std::string::npos) return "";
    size_t end = cookie.find(";", pos);
    return cookie.substr(pos + prefix.size(), end - pos - prefix.size());
}

int main() {
    Server svr;

    // Login Page
    svr.Get("/", [](const Request& req, Response& res) {
        std::string html = R"(
            <h2>Login</h2>
            <form method="POST" action="/login">
                Username: <input name="username" type="text"><br>
                Password: <input name="password" type="password"><br>
                <input type="submit" value="Login">
            </form>
        )";
        res.set_content(html, "text/html");
    });

    // Handle login
    svr.Post("/login", [](const Request& req, Response& res) {
        std::string username = req.get_param_value("username");
        std::string password = req.get_param_value("password");

        // 🔒 Insecure: hardcoded auth for demo
        if (username == "admin" && password == "admin") {
            std::string session_id = generate_token();
            std::string csrf_token = generate_token(16);
            sessions[session_id] = {username, "admin@example.com", csrf_token};

            res.set_header("Set-Cookie", "SESSION_ID=" + session_id + "; HttpOnly");
            res.set_redirect("/profile");
        } else {
            res.set_content("Invalid login. <a href='/'>Try again</a>", "text/html");
        }
    });

    // Profile Page
    svr.Get("/profile", [](const Request& req, Response& res) {
        std::string session_id = get_session_id(req);
        if (sessions.find(session_id) == sessions.end()) {
            res.set_redirect("/");
            return;
        }

        Session& session = sessions[session_id];
        std::string html = R"(
            <h2>Welcome, )" + session.username + R"(</h2>
            <p>Current email: )" + session.email + R"(</p>
            <form method="POST" action="/change_email">
                New Email: <input name="email" type="text">
                <input type="hidden" name="csrf_token" value=")" + session.csrf_token + R"(">
                <input type="submit" value="Update Email">
            </form>
            <br><a href="/logout">Logout</a>
        )";

        res.set_content(html, "text/html");
    });

    // Email change with CSRF validation
    svr.Post("/change_email2", [](const Request& req, Response& res) {
        std::string session_id = get_session_id(req);
        if (sessions.find(session_id) == sessions.end()) {
            res.set_redirect("/");
            return;
        }

        Session& session = sessions[session_id];
        std::string new_email = req.get_param_value("email");
        std::string submitted_token = req.get_param_value("csrf_token");

        if (submitted_token != session.csrf_token) {
            res.status = 403;
            res.set_content("CSRF token invalid", "text/plain");
            return;
        }

        session.email = new_email;
        res.set_content("Email updated to: " + session.email + "<br><a href='/profile'>Back</a>", "text/html");
    });


    // Email change with CSRF validation
    svr.Post("/change_email", [](const Request& req, Response& res) {
        std::string session_id = get_session_id(req);
        if (sessions.find(session_id) == sessions.end()) {
            res.set_redirect("/");
            return;
        }

        Session& session = sessions[session_id];
        std::string new_email = req.get_param_value("email");
        
       

        session.email = new_email;
        res.set_content("Email updated to: " + session.email + "<br><a href='/profile'>Back</a>", "text/html");
    });

    // Logout
    svr.Get("/logout", [](const Request& req, Response& res) {
        std::string session_id = get_session_id(req);
        sessions.erase(session_id);
        res.set_header("Set-Cookie", "SESSION_ID=deleted; Max-Age=0");
        res.set_redirect("/");
    });

    std::cout << "Server running at http://localhost:8080\n";
    svr.listen("0.0.0.0", 8080);
}
