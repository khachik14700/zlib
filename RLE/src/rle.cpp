#include "rle.h"
#include <stdexcept>

std::vector<uint8_t> RLE::compress(const std::vector<uint8_t>& input)
{
    std::vector<uint8_t> output = {};
    if (input.empty())
    {
        return output;
    }

    std::size_t i = 0;
    while (i < input.size())
    {
        uint8_t current = input[i];
        uint8_t count = 1;
        while (count < 255 && (i + count) < input.size() && input[i + count] == current)
        {
            count++;
        }
        output.emplace_back(count);
        output.emplace_back(current);
        i += count;
    }

    return output;
}

std::vector<uint8_t> RLE::decompress(const std::vector<uint8_t>& input)
{
    std::vector<uint8_t> output = {};
    if (input.empty())
    {
        return output;
    }
    if (input.size() % 2 != 0)
    {
        throw std::runtime_error("RLE decompress: corrupted data, expected even number of bytes.");
    }

    for (size_t i = 0; i < input.size(); i+= 2)
    {
        uint8_t count = input[i];
        uint8_t current = input[i + 1];
        for(uint8_t j = 0; j < count; j++)
        {
            output.emplace_back(current);
        }
    }

    return output;
}

std::vector<uint8_t> RLE::compress(const std::string& input)
{
    std::vector<uint8_t> original(input.begin(), input.end());
    return RLE::compress(original);
}

std::string RLE::decompressToString(const std::vector<uint8_t>& input)
{
    std::vector<uint8_t> decompresed = RLE::decompress(input);
    std::string result(decompresed.begin(), decompresed.end());
    return result;
}