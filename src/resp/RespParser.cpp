#include "RespParser.h"

#include <sstream>
#include <stdexcept>

std::vector<std::string> RespParser::parse(
    const std::string& request)
{
    std::vector<std::string> tokens;
    std::istringstream stream(request);
    std::string line;
    std::getline(stream, line);
    if(line.empty() || line[0] != '*')
    {
        throw std::runtime_error("Invalid RESP array");
    }
    int count = std::stoi(line.substr(1));
    for(int i = 0; i < count; i++)
    {
        std::getline(stream, line);
        if(line.empty() || line[0] != '$')
        {
            throw std::runtime_error("Invalid bulk string");
        }
        std::getline(stream, line);
        if(!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }
        tokens.push_back(line);
    }

    return tokens;
}