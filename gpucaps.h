#include <iomanip>
#include <iostream>
#include <string>
#include <climits>

static std::streamsize fieldWidth = 0;

inline std::string uint32String(uint32_t limit)
{
    return (UINT_MAX == limit) ? "0xFFFFFFFF" : std::to_string(limit);
}

template<typename Bool>
inline std::string booleanString(Bool value)
{
    return value ? "Yes" : "No";
}

inline void setFieldWidth(std::streamsize width)
{
    fieldWidth = width;
}

template<typename Type>
inline void printLn(const char *description, const Type& value)
{
    std::cout << std::setw(fieldWidth) << std::left << description << value << std::endl;
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
