#pragma once
#ifndef IF_H_
#define IF_H_

#include <string>


std::string parseIf(const std::string& line)
{
    std::string result = line.substr(2);
    std::size_t then_pos = line.find("THEN");
    result.insert(0, "if(");
    
    if (then_pos != std::string::npos) {
        result = result.substr(0, then_pos);
        result.append("){");
    }
    else
        result.append(")");
    
    return result;
}


std::string parseElse(const std::string& line)
{
    return "}else{";
}


#endif // !IF_H_