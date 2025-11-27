#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
// #include "json.hpp" // https://github.com/nlohmann/json

using json = nlohmann::json;


struct PricingInput {
    double date;
    std::string model_type;
    std::string option_type;
    double spot;
    double strike;
    double interest_rate;
    double volatility;
    double maturity;
    int number_of_steps;
    int number_of_paths;
    double shift; 
    std::vector<double> past;
};

PricingInput parseJson(const std::string& json_str) {
    json j = json::parse(json_str);
    PricingInput input;
    input.date = j["date"];
    input.model_type = j["model_type"];
    input.option_type = j["option_type"];
    input.spot = j["spot"];
    input.strike = j["strike"];
    input.interest_rate = j["interest_rate"];
    input.volatility = j["volatility"];
    input.maturity = j["maturity"];
    input.number_of_steps = j["number_of_steps"];
    input.number_of_paths = j["number_of_paths"];
    input.shift = j["shift"];

    // Parse past
    input.past = j["past"].get<std::vector<double>>();
    
    return input;
}
