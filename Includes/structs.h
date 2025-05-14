#pragma once
#ifndef STRUCTS_H_
#define STRUCTS_H_

#include <string>
#include "whitespace.h"


std::string parseType(const std::string& _line)
{
    std::string line = trim_whitespace(_line);
    return "struct " + line.substr(4) + " {\n";
}


std::string endTypeEquivalent() {
    return "};";
}


#endif // !STRUCTS_H_