#pragma once
#ifndef BRACES_H_
#define BRACES_H_

#include <algorithm>
#include <string>
#include <initializer_list>


inline bool starts_with(const std::string& line, const std::initializer_list<std::string>& vars)
{
    return std::ranges::any_of(vars, [&](const std::string& x) { return line.starts_with(x); });
}


inline std::string replaceOpening(const std::string& line)
{
    if (::starts_with(line, {"THEN"}))
    {
        return "{";
    }
    return line;
}


inline std::string replaceClosing(const std::string& line)
{
    if (::starts_with(line, {"ENDIF", "ENDWHILE", "ENDPROCEDURE", "ENDFUNCTION"}))
    {
        return "}";
    }
    return line;
}


#endif // !BRACES_H_