#ifndef ENV_H
#define ENV_H

#include <string>
#include <unordered_map>

class Env
{
public:
    static bool load(const std::string &filePath);
    static std::string get(const std::string &key);

private:
    static std::unordered_map<std::string, std::string> values;
    static std::string trim(const std::string &value);
};

#endif