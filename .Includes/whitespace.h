#pragma once
#ifndef WHITESPACE_H_
#define WHITESPACE_H_

#include <string>


inline std::string trim_whitespace(const std::string& str) {
    std::string result{};
    result.reserve(str.length());
    for (const char& c : str) {
        if (c != ' ' && c != '\t' && c != '\r' && c != '\n') {
            result += c;
        }
    }
    return result;
}


#endif // !WHITESPACE_H_