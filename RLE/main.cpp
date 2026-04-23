#include "src/rle.h"
#include <iostream>

int main()
{
    std::string text = "AAAABBBCCDDDDDDE";
    std::vector<uint8_t> original(text.begin(), text.end());
    std::vector<uint8_t> compressed = RLE::compress(original);
    std::vector<uint8_t> decompressed = RLE::decompress(compressed);
    std::cout << "Original size: " << original.size() << " bytes" << std::endl;
    std::cout << "Compressed size: " << compressed.size() << " bytes" << std::endl;
    std::cout << "Ratio: " << ((compressed.size() * 100)/ original.size()) << "%" << std::endl;
    std::cout << "Match: " << ((original == decompressed) ? "YES" : "NO") << std::endl;   

    std::cout << "\n--- Test 2: string overload ---\n";
    std::vector<uint8_t> compressed_str = RLE::compress(text);
    std::string decompressed_str = RLE::decompressToString(compressed_str);
    std::cout << "Original text: " << text << std::endl;
    std::cout << "Decompressed text: " << decompressed_str << std::endl;
    std::cout << "Match: " << ((text == decompressed_str) ? "YES" : "NO") << std::endl;   

    return 0;
}