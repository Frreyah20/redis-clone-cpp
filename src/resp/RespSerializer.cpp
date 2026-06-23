#include "RespSerializer.h"

std::string RespSerializer::simpleString(
    const std::string& value)
{
    return "+" + value + "\r\n";
}

std::string RespSerializer::bulkString(
    const std::string& value)
{
    return "$" + std::to_string(value.size()) +
           "\r\n" + value + "\r\n";
}

std::string RespSerializer::integer(long value)
{
    return ":" + std::to_string(value) + "\r\n";
}

std::string RespSerializer::nullBulkString()
{
    return "$-1\r\n";
}

std::string RespSerializer::error(
    const std::string& message)
{
    return "-ERR " + message + "\r\n";
}