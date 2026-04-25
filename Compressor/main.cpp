#include "src/fileio.h"
#include <iostream>

int main(int argc, char **argv)
{
    auto filesMatch = [](const std::filesystem::path& a, const std::filesystem::path& b)
    {
        std::ifstream fa(a, std::ios::binary);
        std::ifstream fb(b, std::ios::binary);
        return std::equal(
            std::istreambuf_iterator<char>(fa),
            std::istreambuf_iterator<char>(),
            std::istreambuf_iterator<char>(fb),
            std::istreambuf_iterator<char>()
        );
    };

    if (argc < 2)
    {
        std::cerr << "./compressor help" << std::endl;
        return 1;
    }

    std::string command = argv[1];

    if (command == "help")
    {
        std::cout << "./compressor compress file.txt" << std::endl;
        std::cout << "./compressor compress file.txt out.bin" << std::endl;
        std::cout << "./compressor decompress file.txt.compressed" << std::endl;
        std::cout << "./compressor decompress file.txt.compressed out.txt" << std::endl;
        std::cout << "./compressor match file1.txt file2.txt" << std::endl;
        return 0;
    }
    if (argc < 3)
    {
        std::cerr << "Error: no file specified. Use ./compressor help\n";
        return 1;
    }

    if (command == "compress" && argc == 3)
    {
        try
        {
            std::filesystem::path path = argv[2];
            auto compressed_path = std::filesystem::path(path).concat(".compressed");
            FileIO::compressFile(path);
            std::cout << "Original:   " << std::filesystem::file_size(path) << " bytes\n";
            std::cout << "Compressed: " << std::filesystem::file_size(compressed_path) << " bytes\n";
            std::cout << "Ratio: " << (std::filesystem::file_size(compressed_path) * 100 / std::filesystem::file_size(path)) << "%\n\n";
        }
        catch (const std::runtime_error& er)
        {
            std::cerr << er.what() << std::endl;
            return 1;
        }
    }
    else if (command == "compress" && argc > 3)
    {
        try
        {
            std::filesystem::path path = argv[2];
            std::filesystem::path out_path = argv[3];
            FileIO::compressFile(path, out_path);
            std::cout << "Original:   " << std::filesystem::file_size(path) << " bytes\n";
            std::cout << "Compressed: " << std::filesystem::file_size(out_path) << " bytes\n";
            std::cout << "Ratio: " << (std::filesystem::file_size(out_path) * 100 / std::filesystem::file_size(path)) << "%\n\n";
        }
        catch (const std::runtime_error& er)
        {
            std::cerr << er.what() << std::endl;
            return 1;
        }
    }
    else if (command == "decompress" && argc == 3)
    {
        try
        {
            std::filesystem::path path = argv[2];
            FileIO::decompressFile(path);
            std::cout << "Decompressed: " << std::filesystem::file_size(path.replace_extension("")) << " bytes\n";
        }
        catch (const std::runtime_error& er)
        {
            std::cerr << er.what() << std::endl;
            return 1;
        }
    }
    else if (command == "decompress" && argc >= 4)
    {
        try
        {
            std::filesystem::path path = argv[2];
            std::filesystem::path out_path = argv[3];
            FileIO::decompressFile(path, out_path);
            std::cout << "Decompressed: " << std::filesystem::file_size(out_path) << " bytes\n";
    
        }
        catch (const std::runtime_error& er)
        {
            std::cerr << er.what() << std::endl;
            return 1;
        }
    }
    else if (command == "match" && argc >= 4)
    {
        std::filesystem::path path1 = argv[2];
        std::filesystem::path path2 = argv[3];

        std::cout << "Files match: " << (filesMatch(path1, path2) ? "YES" : "NO") << "\n";

    }
    else
    {
        std::cerr << "Unknown command" << std::endl;
    }

    return 0;
}