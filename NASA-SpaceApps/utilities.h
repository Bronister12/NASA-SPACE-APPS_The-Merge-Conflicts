// utilities.h
#pragma once

#include <string>
#include <nlohmann/json.hpp>

/**
 * @brief Fetches Near Earth Object (NEO) data from the NASA API.
 *
 * @param api_key Your personal NASA API key.
 * @param start_date The start date for the query in "YYYY-MM-DD" format.
 * @param end_date The end date for the query in "YYYY-MM-DD" format.
 * @return A JSON object containing the API response. Returns an empty object on failure.
 */
nlohmann::json getNeoData(const std::string& api_key, const std::string& start_date, const std::string& end_date);