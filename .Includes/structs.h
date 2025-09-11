#pragma once
#ifndef STRUCTS_H_
#define STRUCTS_H_

#include <string>
#include "whitespace.h"


inline std::string parseType(const std::string& _line)
{
    return "struct " + trim_whitespace(_line).substr(4) + " {\n";
}


inline std::string endTypeEquivalent() {
    return "};";
}


#endif // !STRUCTS_H_