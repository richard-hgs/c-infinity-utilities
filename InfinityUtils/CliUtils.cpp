#include "pch.h"            // Default header
#include <string>
#include "CliUtils.h"

char* CliUtils::getCmdOption(char** begin, char** end, const std::string& option)
{
    char** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

bool CliUtils::cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}