#pragma once
#include <vector>
#include <cstdint>
#include <string>

class Deflate
{
    public:
        static std::vector<uint8_t> compress(const std::vector<uint8_t>& input);
        static std::vector<uint8_t> decompress(const std::vector<uint8_t>& input);
        static std::vector<uint8_t> compress(const std::string& input);
        static std::string decompressToString(const std::vector<uint8_t>& input);
};