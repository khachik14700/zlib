#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>

class FileIO
{
    public:
        static void compressFile(const std::filesystem::path& input_path, const std::filesystem::path& output_path = "");
        static void decompressFile(const std::filesystem::path& input_path, const std::filesystem::path& output_path = "");
};