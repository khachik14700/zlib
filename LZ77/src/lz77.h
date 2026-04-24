#pragma once
#include <vector>
#include <cstdint>
#include <string>

struct Token
{
    uint16_t offset;
    uint16_t length;
    uint8_t next;

    Token(uint16_t o, uint16_t l, uint8_t n)
        : offset(o), length(l), next(n) {}
};

class LZ77
{
    public:
        static std::vector<Token> compress(const std::vector<uint8_t>& input);
        static std::vector<uint8_t> decompress(const std::vector<Token>& input);
        static std::vector<Token> compress(const std::string& input);
        static std::string decompressToString(const std::vector<Token>& input);
};