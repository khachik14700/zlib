#include "huffman.h"
#include <stdexcept>
#include <map>
#include <queue>

struct Node
{
    uint8_t byte;
    int freq;
    Node* left;
    Node* right;

    Node(uint8_t b, int f): 
        byte(b), freq(f), left(nullptr), right(nullptr) {}
    
    Node(int f, Node* l, Node* r):
        byte(0), freq(f), left(l), right(r) {}
};

struct Compare
{
    bool operator() (Node* a, Node* b)
    {
        return a->freq > b->freq;
    }
};

Node* buildTree(const std::map<uint8_t, uint32_t>& freq)
{
    if (freq.empty())
    {
        throw std::runtime_error("Huffman: empty frequency table.");
    }

    std::priority_queue<Node*, std::vector<Node*>, Compare> pq;
    for (const auto& pair: freq)
    {
        pq.push(new Node(pair.first, pair.second));
    }

    while (pq.size() > 1)
    {
        Node* a = pq.top(); pq.pop();
        Node* b = pq.top(); pq.pop();
        int new_freq = a->freq + b->freq;
        Node* new_nod = new Node(new_freq, a, b);
        pq.push(new_nod);
    }

    return pq.top();
}

void deleteTree(Node* node)
{
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

void buildCodes(Node* node, std::string code, std::map<uint8_t, std::string>& codes)
{
    if (!node->left && !node->right)
    {
        codes[node->byte] = code.empty() ? "0" : code;
    }
    else
    {
        if (node->left)
        {
            buildCodes(node->left, code + '0', codes);
        }
        if (node->right)
        {
            buildCodes(node->right, code + '1', codes);
        }
    }
}

std::vector<uint8_t> Huffman::compress(const std::vector<uint8_t>& input)
{
    std::map<uint8_t, uint32_t> freq;
    std::vector<uint8_t> output;
    if (input.empty())
    {
        return output;
    }

    for (uint8_t ch : input)
    {
        freq[ch]++;
    }

    Node* root = buildTree(freq);
    std::map<uint8_t, std::string> codes;
    buildCodes(root, "", codes);

    uint16_t num = freq.size();
    output.emplace_back((num >> 8) & 0xFF);
    output.emplace_back((num >> 0) & 0xFF);
    for (const auto& pair: freq)
    {
        output.emplace_back(pair.first);
        uint32_t f = pair.second;
        output.emplace_back((f>>24) & 0xFF);
        output.emplace_back((f>>16) & 0xFF);
        output.emplace_back((f>>8) & 0xFF);
        output.emplace_back((f>>0) & 0xFF);
    }

    uint8_t buffer = 0;
    int bites_count = 0;

    for (uint8_t ch : input)
    {
        std::string code = codes[ch];
        for(auto bit: code)
        {
            buffer = buffer << 1;
            if (bit == '1')
            {
                buffer = buffer | 1;
            }
            bites_count++;
            if (bites_count == 8)
            {
                output.emplace_back(buffer);
                buffer = 0;
                bites_count = 0;
            }
        }
    }
    uint8_t padding = 0;
    if (bites_count > 0)
    {
        buffer = buffer << (8 - bites_count);
        output.emplace_back(buffer);
        padding = 8 - bites_count;
    }
    output.insert(output.begin(), padding);

    deleteTree(root);
    return output;
}

std::vector<uint8_t> Huffman::decompress(const std::vector<uint8_t>& input)
{
    std::vector<uint8_t> output;
    std::map<uint8_t, uint32_t> freq;

    if (input.empty())
    {
        return output;
    }

    size_t pos = 0;
    if (input.size() < 2)
        throw std::runtime_error("Huffman decompress: input too short.");
    int padding = input[pos++];
    uint16_t num_symbol = (input[pos] << 8) | input[pos + 1];
    pos += 2;
    for(uint16_t i = 0; i < num_symbol; i++)
    {
        if (pos + 5 > input.size())
            throw std::runtime_error("Huffman decompress: corrupted frequency table.");
        uint8_t symbol = input[pos++];
        uint8_t byte0 = input[pos++];
        uint8_t byte1 = input[pos++];
        uint8_t byte2 = input[pos++];
        uint8_t byte3 = input[pos++];
        freq[symbol] = byte0 << 24 | byte1 << 16 | byte2 << 8 | byte3;
    }
    Node* root = buildTree(freq);
    int total_bits = (input.size() - pos) * 8 - padding;
    int read_bits = 0;

    Node* current = root;

    if (!root->left && !root->right)
    {
        uint32_t count = root->freq;

        for (uint32_t i = 0; i < count; i++)
        {
            output.emplace_back(root->byte);
        }

        deleteTree(root);
        return output;
    }

    while (read_bits < total_bits)
    {
        for (int bit = 7; bit >= 0; bit--)
        {
            if (read_bits >= total_bits) break;
            
            bool is_one = (input[pos] >> bit) & 1;
            if (is_one)
            {
                current = current->right;
            }
            else
            {
                current = current->left;
            }
            if (!current)
                throw std::runtime_error("Huffman decompress: corrupted data, tree traversal failed.");
            read_bits++;
            if (!current->left && !current->right)
            {
                output.emplace_back(current->byte);
                current = root;
            }
        }
        pos++;
    }

    deleteTree(root);
    return output;
}

std::vector<uint8_t> Huffman::compress(const std::string& input)
{
    std::vector<uint8_t> original(input.begin(), input.end());
    return Huffman::compress(original);
}

std::string Huffman::decompressToString(const std::vector<uint8_t>& input)
{
    std::vector<uint8_t> decompressed = Huffman::decompress(input);
    std::string result(decompressed.begin(), decompressed.end());
    return result;
}