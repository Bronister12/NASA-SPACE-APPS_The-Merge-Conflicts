#include "utilities.h"
#include <httplib.h>
#include <iostream>

nlohmann::json getNeoData(const std::string& api_key, const std::string& start_date, const std::string& end_date, bool only_hazardous) {
    httplib::Client cli("https://api.nasa.gov");

    std::string path = "/neo/rest/v1/feed?";
    path += "start_date=" + start_date;
    path += "&end_date=" + end_date;
    path += "&api_key=" + api_key;

    auto res = cli.Get(path);

    if (!res || res->status != 200) {
        // ... (error handling is the same as before)
        std::cerr << "Error: Failed to fetch data from NASA API.\n";
        // ...
        return nlohmann::json{{"error", "Failed to fetch data from NASA API."}};
    }

    nlohmann::json full_data = nlohmann::json::parse(res->body);

    // --- NEW CHANGE: SERVER-SIDE FILTERING LOGIC ---
    if (only_hazardous) {
        std::cout << "Filtering for potentially hazardous objects on the server...\n";
        nlohmann::json filtered_data;
        int hazardous_count = 0;
        
        // Create a new 'near_earth_objects' map to store only hazardous items
        nlohmann::json filtered_neos = nlohmann::json::object();

        for (auto& [date, neos] : full_data["near_earth_objects"].items()) {
            nlohmann::json hazardous_neos_for_date = nlohmann::json::array();
            for (auto& neo : neos) {
                if (neo["is_potentially_hazardous_asteroid"]) {
                    hazardous_neos_for_date.push_back(neo);
                    hazardous_count++;
                }
            }
            // Only add the date to our new map if hazardous objects were found
            if (!hazardous_neos_for_date.empty()) {
                filtered_neos[date] = hazardous_neos_for_date;
            }
        }
        
        // Build the final, smaller JSON object to send to the client
        filtered_data["element_count"] = hazardous_count;
        filtered_data["near_earth_objects"] = filtered_neos;
        return filtered_data;
    }

    // If not filtering, return the original full data
    return full_data;
}