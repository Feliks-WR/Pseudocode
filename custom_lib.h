#pragma once
#ifndef CUSTOM_LIB_H_
#define CUSTOM_LIB_H_

#include <concepts>
#include <string>
#include <variant>
#include <optional>
#include <initializer_list>
#include <stdexcept>
#include <iostream>
#include <cmath>


enum {
    FALSE = 0,
    TRUE = 1
};

class _BASE_OBJECT {};

class _TYPE {
    std::variant<
        int,
        unsigned,
        short,
        unsigned short,
        long,
        unsigned long,
        unsigned long long,
        long long,
        float,
        double,
        long double,
        char,
        wchar_t,
        bool,
        std::nullptr_t,
        std::string,
        _BASE_OBJECT*,
        _BASE_OBJECT
    > value;

public:
    _TYPE() = default;

    template <typename T>
    _TYPE(T val) : value(val) {}

    template <typename T>
    operator T() const {
        return std::get<T>(value);
    }

    template <typename T>
    bool operator == (const T& other) const {
        return std::get<T>(value) == other;
    }

    template <typename T>
    bool operator != (const T& other) const {
        return std::get<T>(value) != other;
    }
};


namespace {
    template <typename T>
    concept _NUMERIC = std::integral<T> || std::floating_point<T>;
}


class _1_STRING : public std::string
{
public:
    char operator[](std::size_t pos) const noexcept
    {
        return this->std::string::operator[](pos - 1);
    }    

    operator char () const
    {
        if (this->length() == 0) {
            return '\0';
        }
        if (this -> length() != 1) {
            throw std::out_of_range("String length is not 1");
        }
        return this->std::string::operator[](0);
    }
};


template <typename _CharT, typename _Traits>
std::basic_istream<_CharT, _Traits>& operator>>(std::basic_istream<_CharT, _Traits>& is, _1_STRING& str)
{
    std::string temp;
    is >> temp;
    str = _1_STRING(temp);
    return is;
}


template <typename T>
concept _SIZE_T_T = std::integral<T> && std::same_as<T, std::size_t>;


template <typename T, std::size_t INITIAL_SIZE = 0>
class _1_ARRAY : public _BASE_OBJECT
{
public:
    _1_ARRAY() : _1_ARRAY(INITIAL_SIZE) {};

    explicit _1_ARRAY(std::size_t size) : data(new T[size]),size(size) {}

    _1_ARRAY(std::initializer_list<std::convertible_to<T> auto> list) : data(new T[list.size() + 2]), size(list.size())
    {
        this->size = list.size();
        std::size_t i = 0;
        for (const auto& item : list) {
            this->data[i++] = item;
        }
    }

    ~_1_ARRAY() { 
        delete[] this->data;
        this->data = nullptr; 
    }

    T& operator[](std::size_t pos) noexcept
    {
        return this->data[pos - 1];
    }

    const T& operator[](std::size_t pos) const noexcept
    {
        return this->data[pos - 1];
    }

    T* raw() const noexcept
    {
        return this->data;
    }

    std::size_t length() const noexcept
    {
        return this->size;
    }

    void set(std::size_t pos, T value) noexcept
    {
        this->data[pos - 1] = value;
    }

    template <std::size_t size>
    _1_ARRAY operator = (std::array<T, size> arr) noexcept
    {
        if (this->size != size) {
            delete[] this->data;
            this->data = new T[size];
            this->size = size;
        }

        for (std::size_t i = 0; i < this->size; ++i) {
            this->data[i + 1] = arr[i];
        }
        return *this;
    }

    _1_ARRAY operator = (const _1_ARRAY& other) noexcept
    {
        if (this->size != other.size) {
            delete[] this->data;
            this->data = new T[other.size];
            this->size = other.size;
        }

        for (std::size_t i = 0; i < this->size; ++i) {
            this->data[i] = (T) other.data[i];
        }
        return *this;
    }

    operator T*() const noexcept
    {
        return this->data;
    }

    template <std::size_t size>
    operator std::array<T, size>() const noexcept
    {
        std::array<T, this->size> arr{};
        for (std::size_t i = 0; i < this->size; ++i) {
            arr[i] = this->data[i];
        }
        return arr;
    }

    bool operator == (const _1_ARRAY& other) const noexcept
    {
        if (this->size != other.size) 
            return false;

        for (std::size_t i = 0; i < this->size; ++i) {
            if (this->data[i] != other.data[i]) 
                return false;
        }
        return true;
    }
private:
    T* data;
    std::size_t size;
};


template <typename T>
inline constexpr long LENGTH (_1_ARRAY<T> x) {
    return x.length();
}


_1_STRING operator""_s(const char* str, std::size_t len) noexcept
{
    return _1_STRING(str);
}

inline constexpr short __SPACESHIP(auto x, auto y)  { 
    return (x < y) ? -1 : ((x > y) ? 1 : 0); 
}

inline constexpr long __DIV(auto x, auto y) { 
    return static_cast<long>((x < 0) ? -(-x / y) : (x / y)); 
}

inline constexpr long __MOD(auto x, auto y) { 
    return static_cast<long>(x % y); 
}

inline constexpr _1_STRING MID (const std::string& x, std::size_t start, std::size_t length) {
    return _1_STRING(x.substr(start - 1, length));
}

inline constexpr std::string LEFT (const std::string& x, std::size_t length) {
    return x.substr(0, length);
}

inline constexpr std::string RIGHT (const std::string& x, std::size_t length) {
    return x.substr(x.length() - length);
}

inline constexpr long LENGTH (const std::string& x) {
    return x.length();
}

inline constexpr char UCASE (char x) noexcept {
    return static_cast<char>(std::toupper(x));
}

inline constexpr char LCASE (char x) noexcept {
    return static_cast<char>(std::tolower(x));
}

inline constexpr std::string TO_UPPER(const std::string& x) noexcept {
    std::string result{};
    result.reserve(x.length());
    for (const char& c : x) {
        result += UCASE(c);
    }
    return result;
}

inline constexpr std::string TO_LOWER(const std::string& x) noexcept {
    std::string result{};
    result.reserve(x.length());
    for (const char& c : x) {
        result += LCASE(c);
    }
    return result;
}


inline constexpr std::string NUM_TO_STR(const _NUMERIC auto x) noexcept {
    return std::to_string(x);
}


inline auto STR_TO_NUM(const std::string& x) -> decltype(x.find('.') != std::string::npos ? std::stold(x) : std::stoll(x)) 
{
    if (x.find('.') != std::string::npos) {
        return std::stold(x);
    }
    else {
        return std::stoll(x);
    }
}


inline constexpr char CHAR(long long n) noexcept {
    return static_cast<char>(n);
}


inline constexpr long ASC(char c) noexcept {
    return static_cast<long>(c);
}


inline constexpr auto abs (_NUMERIC auto x) noexcept {
    return (x < 0) ? -x : x;
}


template <typename _Ty>
std::basic_ostream<_Ty>& operator << (std::basic_ostream<_Ty>& os, const _TYPE val) {
    
    
    return os;
}


#endif // !CUSTOM_LIB_H_