# Compressor — File Compression Library

A file compression library implemented in C++ from scratch, inspired by zlib.
Implements **RLE**, **LZ77**, **Huffman Coding**, and **DEFLATE** algorithms.

---

## Algorithms

### RLE — Run-Length Encoding
The simplest compression algorithm. Replaces consecutive repeated bytes with a count and value pair.
```
AAAABBBCCDD → 4A 3B 2C 2D
```
Works well for data with long runs of repeated bytes (e.g. bitmap images).

### LZ77 — Lempel-Ziv 1977
Finds repeated sequences in a sliding window and replaces them with references `(offset, length)`.
```
ABCABCABC → 'A' 'B' 'C' (offset=3, length=6)
```
Works well for text and structured data with repeated patterns.

### Huffman Coding
Assigns shorter bit codes to more frequent bytes and longer codes to rare ones.
```
'A' (freq=5) → 0
'B' (freq=2) → 10
'C' (freq=1) → 11
```
Reduces the total number of bits needed to represent the data.

### DEFLATE
Combines LZ77 and Huffman together — the same approach used in zlib and gzip.
```
Input → LZ77 (remove repeated sequences) → Huffman (encode with fewer bits) → Output
```

---

## Project Structure

```
.
├── Compressor/
│   ├── main.cpp
│   └── src/
│       ├── fileio.h
│       └── fileio.cpp
├── Deflate/
│   ├── main.cpp
│   └── src/
│       ├── deflate.h
│       └── deflate.cpp
├── Huffman/
│   ├── main.cpp
│   └── src/
│       ├── huffman.h
│       └── huffman.cpp
├── LZ77/
│   ├── main.cpp
│   └── src/
│       ├── lz77.h
│       └── lz77.cpp
├── RLE/
│   ├── main.cpp
│   └── src/
│       ├── rle.h
│       └── rle.cpp
└── Makefile
```

---

## Build

Requirements:
- g++ with C++17 support
- make

```bash
make
```

Clean build artifacts:
```bash
make fclean
```

Rebuild from scratch:
```bash
make re
```

---

## Usage

### Compress a file
```bash
./compressor compress file.txt
# output: file.txt.compressed
```

```bash
./compressor compress file.txt output.bin
# output: output.bin
```

### Decompress a file
```bash
./compressor decompress file.txt.compressed
# output: file.txt
```

```bash
./compressor decompress file.txt.compressed restored.txt
# output: restored.txt
```

### Compare two files
```bash
./compressor match file1.txt file2.txt
# output: Files match: YES
```

### Help
```bash
./compressor help
```

---

## Example

```bash
$ ./compressor compress test.txt
Original:   82319 bytes
Compressed: 57032 bytes
Ratio:      69%

$ ./compressor decompress test.txt.compressed restored.txt
Decompressed: 82319 bytes

$ ./compressor match test.txt restored.txt
Files match: YES
```

---

## Compressed File Format

The `.compressed` file format is structured as follows:

```
[1 byte]  padding        — number of unused bits in the last byte
[2 bytes] symbol count   — number of unique symbols in frequency table
[5 bytes] per symbol     — 1 byte symbol + 4 bytes frequency (repeated)
[N bytes] compressed data — Huffman-encoded LZ77 token stream
```

---

## API

### RLE
```cpp
#include "RLE/src/rle.h"

std::vector<uint8_t> compressed   = RLE::compress(data);
std::vector<uint8_t> decompressed = RLE::decompress(compressed);

std::vector<uint8_t> compressed   = RLE::compress("AAAABBB");
std::string          decompressed = RLE::decompressToString(compressed);
```

### LZ77
```cpp
#include "LZ77/src/lz77.h"

std::vector<Token>   tokens       = LZ77::compress(data);
std::vector<uint8_t> decompressed = LZ77::decompress(tokens);
```

### Huffman
```cpp
#include "Huffman/src/huffman.h"

std::vector<uint8_t> compressed   = Huffman::compress(data);
std::vector<uint8_t> decompressed = Huffman::decompress(compressed);
```

### DEFLATE
```cpp
#include "Deflate/src/deflate.h"

std::vector<uint8_t> compressed   = Deflate::compress(data);
std::vector<uint8_t> decompressed = Deflate::decompress(compressed);
std::string          decompressed = Deflate::decompressToString(compressed);
```

### FileIO
```cpp
#include "Compressor/src/fileio.h"

FileIO::compressFile("file.txt");
FileIO::compressFile("file.txt", "output.bin");

FileIO::decompressFile("file.txt.compressed");
FileIO::decompressFile("file.txt.compressed", "restored.txt");
```

---