#pragma once
#ifndef REPEAT_H_
#define REPEAT_H_

#include <string>


std::string repeatEquivilant()
{
    return "do{";
}


std::string parseUntil(const std::string& line)
{
    std::string result = "}while(!(";
    result.append(line.substr(6)).append("));");
    return result;
}


#endif // REPEAT_H_