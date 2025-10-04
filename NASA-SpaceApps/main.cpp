#include <httplib.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <cstdlib> // For std::getenv
#include "utilities.h" // Include our new utility functions


std::string getNasaApiKey() {
    const char* api_key_env = std::getenv("NASA_API_KEY");
    //Justin case
    if (api_key_env == nullptr) {
        std::cout << "Warning: NASA_API_KEY environment variable not set. Using DEMO_KEY.\n";
        return "DEMO_KEY";
    }
    return std::string(api_key_env);
}
int main() {
    httplib::Server svr;
    if (!svr.set_mount_point("/", "./public")) {
        std::cerr << "Error: Could not find the ./public directory.\n";
        return 1;
    }
    const std::string nasa_api_key = getNasaApiKey();
    
    svr.Get("/api/neo", [&](const httplib::Request& req, httplib::Response& res) {
        std::string start_date = req.get_param_value("start_date");
        std::string end_date = req.get_param_value("end_date");

        if (start_date.empty() || end_date.empty()) {
            res.status = 400; // bad req
            nlohmann::json error_json = {{"error", "start_date and end_date query parameters are required."}};
            res.set_content(error_json.dump(), "application/json");
            return;
        }

        nlohmann::json neo_data = getNeoData(nasa_api_key, start_date, end_date);

        res.set_content(neo_data.dump(), "application/json");
    });
    std::cout << "Server starting on http://localhost:8080\n";
    svr.listen("0.0.0.0", 8080);
    return 0;
}