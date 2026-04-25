#include "src/huffman.h"
#include <iostream>

int main()
{
    std::cout << "\n--- Huffman Test ---\n";
    std::string text = "AAAAABBBCCDEAAAAABBBCCDEAAAAABBBCCDEAAAAABBBCCDEAAAA"
                       "ABBBCCDEAAAAABBBCCDEAAAAABBBCCDEAAAAABBBCCDEAAAAABBB"
                       "CCDEAAAAABBBCCDEAAAAABBBCCDEAAAAABBBCCDEAAAAABBBCCDE"
                       "AAAAABBBCCDEAAAAABBBCCDE";
    std::vector<uint8_t> original(text.begin(), text.end());
    std::vector<uint8_t> compressed = Huffman::compress(original);
    std::vector<uint8_t> decompressed = Huffman::decompress(compressed);
    std::cout << "Original size:    " << text.size() << " bytes\n";
    std::cout << "Compressed size:  " << compressed.size() << " bytes\n";
    std::cout << "Ratio: " << (compressed.size() * 100 / text.size()) << "%\n";
    std::cout << "Match: " << ((original == decompressed) ? "YES" : "NO") << std::endl;

    std::vector<uint8_t> compressed_str = Huffman::compress(text);
    std::string decompressed_str = Huffman::decompressToString(compressed_str);
    std::cout << "Match strings: " << ((text == decompressed_str) ? "YES" : "NO") << std::endl;
    return 0;
}