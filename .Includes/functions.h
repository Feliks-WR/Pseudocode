#pragma once
#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "type.h"

#include <string>


std::string parseArguments(const std::string& _line)
{
    std::string line{ trim_whitespace(_line) };

    std::string result{};
    std::size_t comma_pos{ static_cast<std::size_t>(-1) };
    std::size_t colon_pos{ line.find(':') };
    std::size_t next_comma_pos{ line.find(',', comma_pos + 1) };

    enum class context {
        BYVAL,
        BYREF
    } current_context{ context::BYVAL };

    while (colon_pos != std::string::npos) {
        auto handleBy = [&]() {
            line.replace(comma_pos + 1, 5, "");
            
            if (comma_pos != std::string::npos)
                comma_pos = line.rfind(',', comma_pos);
            
            colon_pos = line.rfind(':', colon_pos);
            next_comma_pos = line.find(',', comma_pos + 1);
        };

        [[unlikely]] if (line.rfind("BYREF", colon_pos) != std::string::npos) {
            current_context = context::BYREF;
            handleBy();
        } else [[unlikely]] if (line.rfind("BYVAL", colon_pos) != std::string::npos) {
            current_context = context::BYVAL;
            handleBy();
        }

        const std::string variables = line.substr(comma_pos + 1, colon_pos - comma_pos - 1);
        std::string type = line.substr(colon_pos + 1, next_comma_pos - colon_pos - 1);

        std::string cpp_type;

        if (type.find("ARRAY") != std::string::npos) {
            const std::size_t of_pos = type.find("OF");
            if (of_pos != std::string::npos) {
                const std::string array_type = type_map.at(type.substr(of_pos + 2));
                cpp_type = "_1_ARRAY<" + array_type + ">";
            }
            else {
                cpp_type = "_1_ARRAY<std::convertible_to<_TYPE> auto>";
            }
        }
        else
            cpp_type = type_map.at(type);

        std::size_t var_start = 0;
        std::size_t var_end = variables.find(',');
        while (var_end != std::string::npos) {
            const std::string name = variables.substr(var_start, var_end - var_start);
            result += cpp_type + (current_context == context::BYREF ? "& " : " ") + name + ", ";
            var_start = var_end + 1;
            var_end = variables.find(',', var_start);
        }

        const std::string name = variables.substr(var_start);
        result += cpp_type + (current_context == context::BYREF ? "& " : " ") + name + ", ";

        colon_pos = line.find(':', colon_pos + 1);

        comma_pos = next_comma_pos;
        next_comma_pos = line.find(',', comma_pos + 1);
    }

    if (result.ends_with(", ")) {
        result.replace(result.end() - 2, result.end(), "");
    }

    return result;
}


std::string parseReturn(const std::string& line)
{
    return "return " + line.substr(6) + ";";
}


std::string parseProcedure(const std::string& line)
{
    const std::string name = line.substr(9, line.find('(') - 9);

    const std::size_t pos_opening_bracket = line.find('(');
    const std::size_t pos_closing_bracket = line.rfind(')');
    return "void " + name + '(' 
        + parseArguments(line.substr(pos_opening_bracket + 1, pos_closing_bracket - pos_opening_bracket - 1)) + ") {\n";
}


std::string callEquivalent(const std::string& line)
{
    return line.substr(5) + ";\n";
}


std::string parseFunction(const std::string& _line)
{
    std::string line = _line;
    for (std::size_t i = 0; i < line.size(); i++) {
        if (line[i] == ' ') {
            line.erase(i, 1);
        }
    }

    const std::size_t pos_opening_bracket = line.find('(');
    const std::size_t pos_closing_bracket = line.rfind(')');

    const std::string name = line.substr(8, pos_opening_bracket - 8);

    const std::string return_type{
        type_map.at(
            line.substr(line.rfind("RETURNS") + 7)
        )
    };

    return return_type + ' ' + name + '(' 
        + parseArguments(line.substr(pos_opening_bracket + 1, pos_closing_bracket - pos_opening_bracket - 1)) 
        + ") {\n";
}


#endif // !FUNCTIONS_H_