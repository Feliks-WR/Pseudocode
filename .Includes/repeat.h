#pragma once
#ifndef REPEAT_H_
#define REPEAT_H_

#include <string>


inline std::string repeatEquivilant()
{
    return "do{";
}


// ReSharper disable once CppDFAConstantFunctionResult
inline std::string parseUntil(const std::string& line)
{
    std::string result = "}while(!(";
    result.append(line.substr(6)).append("));");
    return result;
}


#endif // REPEAT_H_