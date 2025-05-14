#pragma once
#ifndef DECLARE_H_
#define DECLARE_H_

#include "type.h"
#include "split.h"
#include "whitespace.h"
#include <sstream>
#include <string>
#include <ranges>
#include <vector>
#include <iostream>
#include <stdexcept>
#include "cstring.h"


std::string parseDeclareString(const std::string& line)
{
    std::string processed_line = trim_whitespace(line);

    if (line.length() < 8 || line.substr(0, 7) != "DECLARE") {
         std::cerr << "Error: DECLARE statement format invalid (missing 'DECLARE ' prefix) in line: " << line << std::endl;
         return "#error Invalid DECLARE statement (prefix)\n";
    }
    processed_line = processed_line.substr(7);

    const std::size_t colon_pos = processed_line.find(':');

    if (colon_pos == std::string::npos || colon_pos == 0 || colon_pos == processed_line.length() - 1) {
        std::cerr << "Error: DECLARE format invalid (missing or misplaced ':') in line: " << line << std::endl;
        return "#error Invalid DECLARE statement (format)\n";
    }

    const std::string names_str = processed_line.substr(0, colon_pos);
    const std::string type_str = processed_line.substr(colon_pos + 1);

    const std::string trimmed_type = trim_whitespace(type_str);
    std::string cpp_type;

    const std::size_t array_pos = trimmed_type.find("ARRAY");

    std::vector<std::string> sizes{};

    if (array_pos != std::string::npos) {
        const std::size_t start_pos = trimmed_type.find('[');
        const std::size_t end_pos = trimmed_type.find(']', start_pos + 1);
        if (start_pos != std::string::npos && end_pos != std::string::npos) {
            sizes = ::split(trimmed_type.substr(start_pos + 1, end_pos - start_pos - 1), ',');
        }
        else {
            std::cerr << "Error: Invalid array declaration in DECLARE statement in line: " << line << std::endl;
            return "#error \"Invalid array declaration\"\n";
        }

        const std::size_t of_pos = trimmed_type.find("OF");
        if (of_pos != std::string::npos) {
            cpp_type = type_map.at(trimmed_type.substr(of_pos + 2));
        }
        else {
            std::cerr << "Error: Invalid array type in DECLARE statement in line: " << line << std::endl;
            return "#error Invalid array type\n";
        }
    }
    else
        cpp_type = type_map.at(trimmed_type);

    std::string trimmed_names_str = trim_whitespace(names_str);

    if (trimmed_names_str.empty()) {
         std::cerr << "Error: No variable names specified in DECLARE statement in line: " << line << std::endl;
         return "#error DECLARE statement has no variable names\n";
    }

    std::stringstream ss(trimmed_names_str);
    std::string segment;
    std::vector<std::string> names;

    while(std::getline(ss, segment, ',')) {
       std::string trimmed_segment = trim_whitespace(segment);
       if (!trimmed_segment.empty()) { // Only add non-empty names
           names.push_back(trimmed_segment);
       }
    }

    if (names.empty()) {
         std::cerr << "Error: No valid variable names found after parsing in line: " << line << std::endl;
         return "#error DECLARE statement parsing resulted in no valid names\n";
    }

    std::string joined_names;
    
    for (size_t i = 0; i < names.size(); ++i) {
        joined_names += names[i];
        if (i < names.size() - 1) {
            joined_names += ", ";
        }
    }

    if (array_pos != std::string::npos) {
        const std::string array_type = cpp_type;
        cpp_type = "";
        for (const std::string& size : sizes)
            cpp_type += "_1_ARRAY<";
        
        cpp_type += array_type;

        for (const std::string& size : sizes | std::views::reverse) 
            cpp_type += "," + size + ">";
        
    }

    return cpp_type + " " + joined_names + ";";
}


#endif // !DECLARE_H_