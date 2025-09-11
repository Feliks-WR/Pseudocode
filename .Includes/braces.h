#pragma once
#ifndef BRACES_H_
#define BRACES_H_

#include <algorithm>
#include <string>
#include <initializer_list>


inline bool starts_with(const std::string& line, const std::initializer_list<std::string>& vars)
{
    std::ranges::any_of(vars, [&line](const auto& x) { return line.starts_with(x); });
    
    return false;
}


inline std::string replaceOpening(const std::string& line)
{
    if (::starts_with(line, {"THEN"})) {
        return "{";
    }
    return line;
}


inline std::string replaceClosing(const std::string& line)
{
    if (::starts_with(line, {"ENDIF", "ENDWHILE", "ENDPROCEDURE", "ENDFUNCTION"})) {
        return "}";
    }
    return line;
}


#endif // !BRACES_H_