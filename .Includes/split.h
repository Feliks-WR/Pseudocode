#pragma once
#ifndef SPLIT_H_
#define SPLIT_H_

#include <string>
#include <string_view>
#include <vector>
#include <array>
#include <concepts>


template <typename T>
concept char_or_sv = std::same_as<T, char> || std::same_as<T, std::string_view>;


constexpr inline std::vector<std::string> split (std::string_view sv, char_or_sv auto delim) noexcept
{
    std::vector<std::string> result{};
    std::size_t pos{ std::string::npos };
    std::size_t old_pos{ pos };

    do {
        pos = sv.find(delim, pos + 1);
        result.push_back(std::string{ sv.substr(old_pos + 1, pos - old_pos - 1) });
        old_pos = pos;
    } while (pos != std::string::npos);

    return result;
}


#endif // SPLIT_H_