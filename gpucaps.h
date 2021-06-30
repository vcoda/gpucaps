static std::streamsize fieldWidth = 0;

inline std::string versionString(uint32_t version)
{
    uint32_t major = (version >> 22) & 0b1111111111; // 10
    uint32_t minor = (version >> 12) & 0b1111111111; // 10
    uint32_t patch = version & 0b111111111111; // 11
    return std::to_string(major) + "." +
           std::to_string(minor) + "." +
           std::to_string(patch);
}

inline std::string vendorIDString(uint32_t vendorID)
{
    uint16_t pciVendorID = vendorID & 0xFFFF;
    switch (pciVendorID)
    { // https://www.reddit.com/r/vulkan/comments/4ta9nj/is_there_a_comprehensive_list_of_the_names_and/
    case 0x1002: return "AMD";
    case 0x1010: return "ImgTec";
    case 0x10DE: return "NVIDIA";
    case 0x13B5: return "ARM";
    case 0x5143: return "Qualcomm";
    case 0x8086: return "Intel";
    }
    return "Unknown";
}

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
