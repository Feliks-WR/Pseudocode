#pragma once
#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <vector>
#include <string>
#include <iostream>

bool contextIsParenthesis (const std::string& line, std::size_t pos) 
{
    if (line.rfind(")", pos) != std::string::npos || line.find("(", pos) != std::string::npos)
        return false;
    
    if (line.rfind("(", pos) != std::string::npos || line.find(")", pos) != std::string::npos)
        return true;

    return true;
}


std::string parseOutputString(const std::string& line) 
{
    std::string result = "std::cout";
    size_t pos = line.find("OUTPUT") + 6;

    while (pos < line.length() && isspace(line[pos])) {
        pos++;
    }

    size_t start = pos;
    while (pos <= line.length()) {
        if (pos == line.length() || (line[pos] == ',' && !contextIsParenthesis(line, pos))) 
        {
            std::string item = line.substr(start, pos - start);

            size_t end_trim = item.find_last_not_of(" \t");
            if (std::string::npos != end_trim) {
                item = item.substr(0, end_trim + 1);
            }
             size_t start_trim = item.find_first_not_of(" \t");
             if (std::string::npos != start_trim) {
                item = item.substr(start_trim);
             }


            if (!item.empty())
                result += "<<(" + item + ")";
            

            start = pos + 1;
        }
        pos++;
    }

    result += "<<std::endl;";
    return result;
}


#endif // !OUTPUT_H_