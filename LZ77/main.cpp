#include "src/lz77.h"
#include <iostream>

int main()
{
    std::cout << "\n--- LZ77 Test ---\n";
    std::string text = "ABCABCABCABCABC";
    std::vector<Token> compressed = LZ77::compress(text);
    std::string decompressed = LZ77::decompressToString(compressed);
    std::cout << "Original size: " << text.size() << " bytes" << std::endl;
    std::cout << "Token count after compressing: " << compressed.size() << std::endl;
    std::cout << "Token size: " << sizeof(Token) << " bytes" << std::endl;
    std::cout << "Compressed size: " << (sizeof(Token) * compressed.size()) << " bytes" << std::endl;
    std::cout << "Original text: " << text << std::endl;
    std::cout << "Match: " << ((text == decompressed) ? "YES" : "NO") << std::endl;

    return 0;
}