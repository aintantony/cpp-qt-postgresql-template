#include "env.h"

#include <cstdlib>
#include <fstream>
#include <sstream>

std::unordered_map<std::string, std::string> Env::values;

std::string Env::trim(const std::string &value)
{
    const std::string whitespace = " \t\r\n";
    const size_t start = value.find_first_not_of(whitespace);
    if (start == std::string::npos) {
        return "";
    }

    const size_t end = value.find_last_not_of(whitespace);
    return value.substr(start, end - start + 1);
}

bool Env::load(const std::string &filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        line = trim(line);

        if (line.empty() || line[0] == '#') {
            continue;
        }

        const size_t eqPos = line.find('=');
        if (eqPos == std::string::npos) {
            continue;
        }

        std::string key = trim(line.substr(0, eqPos));
        std::string value = trim(line.substr(eqPos + 1));

        if (!value.empty() && value.front() == '"' && value.back() == '"' && value.size() >= 2) {
            value = value.substr(1, value.size() - 2);
        }

        values[key] = value;
    }

    return true;
}

std::string Env::get(const std::string &key)
{
    auto it = values.find(key);
    if (it != values.end()) {
        return it->second;
    }

    const char *envValue = std::getenv(key.c_str());
    if (envValue != nullptr) {
        return std::string(envValue);
    }

    return "";
}