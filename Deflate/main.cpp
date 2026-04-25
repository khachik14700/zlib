#include "src/deflate.h"
#include <iostream>

int main()
{
    std::cout << "\n--- Deflate Test ---\n";
    std::string text = "AAAAABBBCCDEAAAAABBBCCDEAAAAABBBCCDEAAAAABBBCCDEAAAA"
                       "ABBBCCDEAAAAABBBCCDEAAAAABBBCCDEAAAAABBBCCDEAAAAABBB"
                       "CCDEAAAAABBBCCDEAAAAABBBCCDEAAAAABBBCCDEAAAAABBBCCDE"
                       "AAAAABBBCCDEAAAAABBBCCDE";

    std::vector<uint8_t> compressed = Deflate::compress(text);
    std::string decompressed = Deflate::decompressToString(compressed);

    std::cout << "Original size:    " << text.size() << " bytes\n";
    std::cout << "Compressed size:  " << compressed.size() << " bytes\n";
    std::cout << "Ratio: " << (compressed.size() * 100 / text.size()) << "%\n";
    std::cout << "Match: " << ((text == decompressed) ? "YES" : "NO") << std::endl;

    return 0;
}