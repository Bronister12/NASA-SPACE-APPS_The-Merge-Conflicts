// utilities.cpp
#include "utilities.h"
#include <httplib.h>
#include <iostream>

nlohmann::json getNeoData(const std::string& api_key, const std::string& start_date, const std::string& end_date) {
    // Note: Your vcpkg.json correctly requests httplib with "openssl",
    // which is required for making HTTPS requests.
    httplib::Client cli("https://api.nasa.gov");

    // Construct the request path with query parameters
    std::string path = "/neo/rest/v1/feed?";
    path += "start_date=" + start_date;
    path += "&end_date=" + end_date;
    path += "&api_key=" + api_key;

    std::cout << "Requesting data from NASA API...\n";

    // Make the GET request
    auto res = cli.Get(path);

    // Check if the request was successful (HTTP status 200 OK)
    if (res && res->status == 200) {
        std::cout << "Successfully fetched data from NASA.\n";
        // Parse the JSON response body and return it
        return nlohmann::json::parse(res->body);
    } else {
        std::cerr << "Error: Failed to fetch data from NASA API.\n";
        if (res) {
            std::cerr << "HTTP Status: " << res->status << std::endl;
            std::cerr << "Response Body: " << res->body << std::endl;
        } else {
            std::cerr << "A critical error occurred with the HTTP request." << std::endl;
        }
        // Return an empty JSON object to indicate an error
        return nlohmann::json{};
    }
}