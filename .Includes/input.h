#pragma once
#ifndef INPUT_H_
#define INPUT_H_

#include <iostream>
#include <string>


std::string parseInputString(const std::string& line)
{
    return "std::cin>>" + line.substr(6) + ';';
}


#endif // !INPUT_H_