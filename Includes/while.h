#pragma once
#ifndef WHILE_H_
#define WHILE_H_

#include <string>
#include <cctype>

std::string trim(const std::string &s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

std::string parseWhile(const std::string& line)
{
    std::string result;
    std::size_t do_pos = line.find("DO");
    if (do_pos != std::string::npos)
        result = line.substr(5, do_pos);
    else 
        result = line.substr(5);

    result = trim(result);
    result.insert(0, "while(");
    result.append("){");
    
    return result;
}

#endif // !WHILE_H_