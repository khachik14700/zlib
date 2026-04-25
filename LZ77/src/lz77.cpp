#include "lz77.h"
#include <stdexcept>

static const uint16_t WINDOW_SIZE = 1024;
static const uint16_t MIN_MATCH = 3;

std::vector<Token> LZ77::compress(const std::vector<uint8_t>& input)
{
    std::vector<Token> output = {};
    if (input.empty())
    {
        return output;
    }

    size_t i =0;
    while (i < input.size())
    {
        size_t search_start = 0;
        if (i > WINDOW_SIZE)
        {
            search_start = i - WINDOW_SIZE;
        }
        uint16_t best_length = 0;
        uint16_t best_offset = 0;

        for (size_t j = search_start; j < i; j++)
        {
            size_t len = 0;
            while ((i + len) < input.size() && len < UINT16_MAX && input[j + len] == input[i + len])
            {
                len++;
            }
            if (len > best_length)
            {
                best_length = len;
                best_offset = i - j;
            }
        }
        if (best_length >= MIN_MATCH)
        {
            uint8_t next_byte = (i + best_length < input.size()) ? input[i + best_length] : 0;
            output.emplace_back(best_offset, best_length, next_byte);
            i += best_length + 1;
        }
        else
        {
            output.emplace_back(0, 0, input[i]);
            i++;
        }
    }

    return output;
}

std::vector<uint8_t> LZ77::decompress(const std::vector<Token>& input)
{
    std::vector<uint8_t> output;

    for (const Token& token: input)
    {
        if (token.length == 0)
        {
            output.emplace_back(token.next);
        }
        else if (token.length >= MIN_MATCH)
        {
            if (token.offset > output.size())
            {
                throw std::runtime_error("LZ77 decompress: invalid offset, reference points before start of buffer.");
            }
            size_t start = output.size() - token.offset;
            for (size_t i = 0; i < token.length; i++)
            {
                uint8_t byte = output[start + i];
                output.emplace_back(byte);
            }
            if (token.next != 0)
            {
                output.emplace_back(token.next);
            }
        }
    }

    return output;
}

std::vector<Token> LZ77::compress(const std::string& input)
{
    std::vector<uint8_t> original(input.begin(), input.end());
    return LZ77::compress(original);
}

std::string LZ77::decompressToString(const std::vector<Token>& input)
{
    std::vector<uint8_t> decompressed = LZ77::decompress(input);
    std::string result(decompressed.begin(), decompressed.end());
    return result;
}