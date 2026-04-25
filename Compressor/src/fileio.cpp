#include "fileio.h"
#include "../../Deflate/src/deflate.h"
#include <stdexcept>

void FileIO::compressFile(const std::filesystem::path& input_path, const std::filesystem::path& output_path)
{
    if (!std::filesystem::exists(input_path))
        throw std::runtime_error("File not found: " + input_path.string());
    std::ifstream file(input_path, std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file: " + input_path.string());
    }

    std::vector<uint8_t> data(
        (std::istreambuf_iterator<char>(file)),
        (std::istreambuf_iterator<char>())
    );

    std::vector<uint8_t> output = Deflate::compress(data);

    std::filesystem::path out_path = output_path.empty() 
        ? std::filesystem::path(input_path).concat(".compressed")
        : output_path;
    std::ofstream out(out_path, std::ios::binary);
    if (!out.is_open())
    {
        throw std::runtime_error("Cannot write file: " + out_path.string());
    }
    out.write(reinterpret_cast<const char*>(output.data()), output.size());
    if (!out.good())
    {
        throw std::runtime_error("FileIO: failed to write decompressed file: " + out_path.string());
    }
}

void FileIO::decompressFile(const std::filesystem::path& input_path, const std::filesystem::path& output_path)
{
    if (!std::filesystem::exists(input_path))
        throw std::runtime_error("File not found: " + input_path.string());
    std::ifstream file(input_path, std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file: " + input_path.string());
    }

    std::vector<uint8_t> data(
        (std::istreambuf_iterator<char>(file)),
        (std::istreambuf_iterator<char>())
    );

    std::vector<uint8_t> output = Deflate::decompress(data);

    std::filesystem::path out_path = output_path.empty()
        ? std::filesystem::path(input_path).replace_extension("")
        : output_path;

    std::ofstream out(out_path, std::ios::binary);
    if (!out.is_open())
    {
        throw std::runtime_error("Cannot write file: " + out_path.string());
    }
    out.write(reinterpret_cast<const char*>(output.data()), output.size());
    if (!out.good())
    {
        throw std::runtime_error("FileIO: failed to write decompressed file: " + out_path.string());
    }
}