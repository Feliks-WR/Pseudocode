#pragma once
#ifndef SYMBOLS_H_
#define SYMBOLS_H_


#include <string>
#include <initializer_list>
#include <stdexcept>


bool contextIsString(const std::string& line, size_t pos, std::initializer_list<char> delims = {'\"'})
{
    bool isInString { false };

    for (size_t i = 0; i < pos; i++) {
        for (char c : delims) {
            if (line[i] == c) {
                isInString = !isInString;
            }
        }
    }

    return isInString;
}


std::string parseSymbols(const std::string& _line)
{    
    std::string line = _line;

    size_t pos = line.find('=');
    while (pos != std::string::npos && !contextIsString(line, pos)) {
        try {
            if (line.at(pos - 1) != '=' && line.at(pos + 1) != '=' && line.at(pos - 1) != '!'
                && line.at(pos + 1) != '>' && line.at(pos + 1) != '<' && line.at(pos - 1) != '>' && line.at(pos - 1) != '<')
                line.insert(pos, "=");
        }
        catch (const std::out_of_range& e) {
            line.insert(pos, "=");
        }

        pos = line.find('=', pos + 1);
    }

    pos = line.find("\"");
    while (pos != std::string::npos && line.substr(pos + 1, 2) != "_s") {
        if (!contextIsString(line, pos+1))
            line.insert(pos+1, "_s");

        pos = line.find("\"", pos + 1);
    }

    pos = line.find(',');
    while (pos != std::string::npos && !contextIsString(line, pos) 
        && !line.starts_with("DECLARE") && contextIsString(line, pos, {'[', ']'})) {
        line.replace(pos, 1, "][");
        pos = line.find(',', pos + 1);
    }

    pos = line.find("<>");
    if (pos != std::string::npos && !contextIsString(line, pos))
        line.replace(pos, 2, "!=");

    pos = line.find("&");
    if (pos != std::string::npos && !contextIsString(line, pos))
        line.replace(pos, 1, "+");

    pos = line.find("AND");
    if (pos != std::string::npos && !contextIsString(line, pos))
        line.replace(pos, 3, "&&");

    pos = line.find("OR");
    if (pos != std::string::npos && !contextIsString(line, pos) && line.at(pos - 1) != 'F')
        line.replace(pos, 2, "||");
    
    pos = line.find("NOT");
    if (pos != std::string::npos && !contextIsString(line, pos))
        line.replace(pos, 3, "!");
    
    pos = line.find("DIV");
    if (pos != std::string::npos && !contextIsString(line, pos))
        line.replace(pos, 3, "/");

    pos = line.find("MOD");
    if (pos != std::string::npos && !contextIsString(line, pos))
        line.replace(pos, 3, "%");

    if (line == _line) {
        pos = line.find("<--");
        while (pos != std::string::npos && !contextIsString(line, pos))
        {
            line.replace(pos, 3, "=");
            pos = line.find("<--");
        }
        
        return line;
    }
    else
        return parseSymbols(line);
}


#endif // SYMBOLS_H_