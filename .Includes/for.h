#pragma once
#ifndef FOR_H_
#define FOR_H_

#include <string>


std::string parseFor(const std::string& _line)
{
    std::string result = "for(";
    const std::string line = _line.substr(3);
    
    const std::size_t eq_pos = line.find("=");
    const std::size_t to_pos = line.find("TO", eq_pos);
    const std::size_t step_pos = line.find("STEP", to_pos);

    const std::string var = line.substr(0, eq_pos);
    const std::string from = line.substr(eq_pos + 1, to_pos - eq_pos - 1);
    const std::string to = line.substr(to_pos + 2, step_pos - to_pos - 2);
    const std::string step = step_pos == std::string::npos ? "1" : line.substr(step_pos + 5);

    result += "auto " + var + "=" + from + ";";
    result += var + "!=(" + to + "+(" + "std::abs(" + step + ")" + "*__SPACESHIP(" + to + "," + from + "))" + ");";
    result += var + "+=" + step + ") {";

    return result;
}


std::string nextEquivalent()
{
    return "}";
}


#endif // !FOR_H_