#include <iostream>
#include <string>
#include "httplib.h"

std::string render_offer_html(const std::string& name) {
    return R"(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Offer Letter</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f4f4f4;
            margin: 0;
            padding: 20px;
        }
        .email-container {
            max-width: 600px;
            margin: 0 auto;
            background: #ffffff;
            padding: 20px;
            border-radius: 8px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
        }
        h1 {
            color: #333;
            text-align: center;
        }
        p {
            color: #555;
            line-height: 1.6;
        }
        .offer-details {
            background: #f9f9f9;
            padding: 15px;
            border-radius: 8px;
            margin-top: 20px;
        }
        .footer {
            text-align: center;
            margin-top: 20px;
            color: #888;
        }
    </style>
</head>
<body>
    <div class="email-container">
        <h1>🎉 Congratulations, )" + name + R"( ! 🎉</h1>
        <p>We are thrilled to extend an offer for you to join our team at <strong>Awesome Company</strong>!</p>
        <div class="offer-details">
            <h2>Offer Details</h2>
            <p><strong>Position:</strong> Software Engineer</p>
            <p><strong>Start Date:</strong> January 1, 2024</p>
            <p><strong>Salary:</strong> $100,000 per year</p>
            <p><strong>Benefits:</strong> Health insurance, 401(k), and more!</p>
        </div>
        <p>Please review the details and let us know if you have any questions. We look forward to having you on board!</p>
        <div class="footer">
            <p>Best regards,</p>
            <p><strong>The Awesome Company Team</strong></p>
        </div>
    </div>
</body>
</html>
)";
}

int main() {
    httplib::Server svr;

    svr.Get("/", [](const httplib::Request& req, httplib::Response& res) {
        std::string name = "New Hire";
        if (req.has_param("name")) {
            name = req.get_param_value("name");
        }

        std::string html = render_offer_html(name);
        res.set_content(html, "text/html");

        res.set_header("Set-Cookie", "sessionid=abc123; Path=/");
        res.set_header("Set-Cookie", "userid=42; Path=/");
        res.set_header("Set-Cookie", "role=admin; Path=/");
        res.set_header("Set-Cookie", "skey=156e4c789ik; Path=/");
    });

    std::cout << "Server running at http://localhost:8080\n";
    svr.listen("0.0.0.0", 8080);
}
