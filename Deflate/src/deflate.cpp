#include "deflate.h"
#include "../../LZ77/src/lz77.h"
#include "../../Huffman/src/huffman.h"
#include <stdexcept>

std::vector<uint8_t> Deflate::compress(const std::vector<uint8_t>& input)
{
    std::vector<Token> lz77_compressed = LZ77::compress(input);
    std::vector<uint8_t> serialized;
    for (const auto& token : lz77_compressed)
    {
        uint16_t offset = token.offset;
        serialized.emplace_back((offset>>8) & 0xFF);
        serialized.emplace_back((offset>>0) & 0xFF);
        uint16_t length = token.length;
        serialized.emplace_back((length>>8) & 0xFF);
        serialized.emplace_back((length>>0) & 0xFF);
        uint8_t next = token.next;
        serialized.emplace_back((next>>0) & 0xFF);
    }
    std::vector<uint8_t> huffman_compressed = Huffman::compress(serialized);
    return huffman_compressed;
}

std::vector<uint8_t> Deflate::decompress(const std::vector<uint8_t>& input)
{
    std::vector<uint8_t> huffman_decompressed  = Huffman::decompress(input);
    std::vector<Token> tokenized;
    size_t i = 0;
    if (huffman_decompressed.size() % 5 != 0)
    {
        throw std::runtime_error("Deflate decompress: corrupted data, invalid token stream.");
    }

    while (i < huffman_decompressed.size())
    {
        uint16_t offset = 0;
        offset = (offset << 8) | huffman_decompressed[i];
        offset = (offset << 8) | huffman_decompressed[i + 1];
        uint16_t length = 0;
        length = (length << 8) | huffman_decompressed[i + 2];
        length = (length << 8) | huffman_decompressed[i + 3];
        uint8_t next = 0;
        next = (next << 8) | huffman_decompressed[i + 4];
        tokenized.emplace_back(offset, length, next);
        i += 5;
    }
    std::vector<uint8_t> lz77_decompressed = LZ77::decompress(tokenized);
    return lz77_decompressed;
}

std::vector<uint8_t> Deflate::compress(const std::string& input)
{
    std::vector<uint8_t> original(input.begin(), input.end());
    return Deflate::compress(original);
}

std::string Deflate::decompressToString(const std::vector<uint8_t>& input)
{
    std::vector<uint8_t> decompressed = Deflate::decompress(input);
    std::string result(decompressed.begin(), decompressed.end());
    return result;
}