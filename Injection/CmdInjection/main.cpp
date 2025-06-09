#include <iostream>
#include <cstdlib>
#include <string>
#include "httplib.h"

// Basic HTML UI for entering a hostname
const char* html_form = R"(
<!DOCTYPE html>
<html>
<head>
  <title>Ping Host</title>
</head>
<body>
  <h2>Ping a Host</h2>
  <form action="/ping" method="get">
    Host: <input name="host" type="text" />
    <input type="submit" value="Ping" />
  </form>
  <pre>%OUTPUT%</pre>
</body>
</html>
)";

int main() {
    httplib::Server svr;

    // Show form
    svr.Get("/", [](const httplib::Request& req, httplib::Response& res) {
        std::string page(html_form);
        size_t pos = page.find("%OUTPUT%");
        if (pos != std::string::npos)
            page.replace(pos, 9, ""); // no output yet
        res.set_content(page, "text/html");
    });

    // Vulnerable handler
    svr.Get("/ping", [](const httplib::Request& req, httplib::Response& res) {
        std::string host = req.get_param_value("host");

        // ❌ Unsafe: user input is passed directly to system()
        std::string cmd = "ping -c 2 " + host + " > output.txt";
        int ret = system(cmd.c_str());

        std::string output;
        FILE* fp = fopen("output.txt", "r");
        if (fp) {
            char buf[1024];
            while (fgets(buf, sizeof(buf), fp)) {
                output += buf;
            }
            fclose(fp);
        } else {
            output = "Failed to run ping.";
        }

        std::string page(html_form);
        size_t pos = page.find("%OUTPUT%");
        if (pos != std::string::npos)
            page.replace(pos, 9, output);

        res.set_content(page, "text/html");
    });

    std::cout << "Running on http://localhost:8080\n";
    svr.listen("0.0.0.0", 8080);
    return 0;
}
