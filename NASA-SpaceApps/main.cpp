// main.cpp
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <cstdlib> // For std::getenv
#include "utilities.h" // Include our new utility functions

// A helper function to safely get the NASA API key from an environment variable
// A helper function to safely get the NASA API key from an environment variable
std::string getNasaApiKey() {
    // Correctly look for the environment variable named "NASA_API_KEY"
    const char* api_key_env = std::getenv("NASA_API_KEY");
    
    // If the variable isn't set in the terminal, it will be nullptr
    if (api_key_env == nullptr) {
        std::cout << "Warning: NASA_API_KEY environment variable not set. Using DEMO_KEY.\n";
        return "DEMO_KEY";
    }

    // Otherwise, return the value of the variable
    return std::string(api_key_env);
}
int main() {
    httplib::Server svr;

    // This serves files from the "./public" directory, like index.html
    // Note: The path in your original file was "public", but "./public" is safer.
    if (!svr.set_mount_point("/", "./public")) {
        std::cerr << "Error: Could not find the ./public directory.\n";
        return 1;
    }

    // Get the API key once when the server starts
    const std::string nasa_api_key = getNasaApiKey();

    // Define our new API endpoint to get NEO data
    svr.Get("/api/neo", [&](const httplib::Request& req, httplib::Response& res) {
        // Get start_date and end_date from the request's query parameters
        // Example URL: /api/neo?start_date=2025-10-04&end_date=2025-10-05
        std::string start_date = req.get_param_value("start_date");
        std::string end_date = req.get_param_value("end_date");

        if (start_date.empty() || end_date.empty()) {
            res.status = 400; // Bad Request
            nlohmann::json error_json = {{"error", "start_date and end_date query parameters are required."}};
            res.set_content(error_json.dump(), "application/json");
            return;
        }

        // Fetch data from NASA using our utility function
        nlohmann::json neo_data = getNeoData(nasa_api_key, start_date, end_date);

        // Send the JSON data back to the client
        res.set_content(neo_data.dump(), "application/json");
    });

    std::cout << "Server starting on http://localhost:8080\n";
    svr.listen("0.0.0.0", 8080);
    return 0;
}