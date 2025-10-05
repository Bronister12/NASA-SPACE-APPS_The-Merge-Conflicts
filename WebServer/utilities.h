#pragma once

#include <string>
#include <nlohmann/json.hpp>
nlohmann::json getNeoData(const std::string& api_key, const std::string& start_date, const std::string& end_date, bool only_hazardous);