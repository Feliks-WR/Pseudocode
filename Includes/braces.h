#pragma once
#ifndef BRACES_H_
#define BRACES_H_

#include <string>
#include <initializer_list>


bool starts_with(const std::string& line, const std::initializer_list<std::string>& vars)
{
    for (std::string x : vars)
        if (line.starts_with(x))
            return true;
    
    return false;
}


std::string replaceOpening(const std::string& line)
{
    if (::starts_with(line, {"THEN"})) {
        return "{";
    }
    return line;
}


std::string replaceClosing(const std::string& line)
{
    if (::starts_with(line, {"ENDIF", "ENDWHILE", "ENDPROCEDURE", "ENDFUNCTION"})) {
        return "}";
    }
    return line;
}


#endif // !BRACES_H_