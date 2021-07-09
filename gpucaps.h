#include <iomanip>
#include <iostream>
#include <string>
#include <climits>

static std::streamsize width = 0;

inline std::string uint32String(uint32_t limit)
{
    return (UINT_MAX == limit) ? "0xFFFFFFFF"
        : (USHRT_MAX == limit) ? "0xFFFF"
        : std::to_string(limit);
}

template<typename Bool>
inline std::string booleanString(Bool value)
{
    return value ? "Yes" : "No";
}

inline void setFieldWidth(std::streamsize fieldWidth)
{
    width = fieldWidth;
}

template<typename Type>
inline void printLn(const char *description, const Type& value)
{
    std::cout << std::setw(width) << std::left << description << value << std::endl;
}

template<typename Type>
inline void printArgs(const Type& arg)
{
    std::cout << arg;
}

template<typename Type, typename... Args>
inline void printArgs(const Type& arg, Args... args)
{
    std::cout << arg << ", ";
    printArgs(args...);
}

template<typename... Args>
inline void printLn(const char *description, Args... args)
{
    std::cout << std::setw(width) << std::left << description << "[";
    printArgs(args...);
    std::cout << "]" << std::endl;
}

inline void printEndLn()
{
    std::cout << std::endl;
}

inline void printHeading(const char *description)
{
    const std::size_t length = strlen(description);
    std::size_t dashedLength = (80 - length)/2;
    printEndLn();
    for (std::size_t i = 0; i < dashedLength - 1; ++i)
        std::cout << "=";
    std::cout << " " << description << " ";
    if (length % 2)
        ++dashedLength;
    for (std::size_t i = 1; i < dashedLength; ++i)
        std::cout << "=";
    printEndLn();
}
