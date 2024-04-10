#include <string>
#include <sstream>
#include <queue>
#include <fstream>
#include <map>
#include "Node.h"
#include "Storage/Storage.h"

#ifndef HUFFMAN_H
#define HUFFMAN_H

/**
 * @class Huffman
 * @brief Represents a Huffman class. Text files can be compressed and decompressed using this tool.
 */

class Huffman {
private:
    std::unordered_map<char, int> charCount; // an unordered map to store how many times each character was seen
    std::priority_queue<Node*, std::vector<Node*>, compareWeights> charQueue; // used to store nodes to build tree
    std::unordered_map<char, std::string> charCodes; // an unordered map to store each character and its code
    Node* root;
    Storage storage;
    /**
     * helper function for compress that reads every character from a text file
     * and stores how many times each character was seen into the charCount map
     */
    void countCharacters();
    /**
     * recursive helper function for compress that traverses through the huffman tree
     * and stores the node's character and its code into the charCodes map
     * @param node the node to go to find the character
     * @param code the huffman code to insert
     */
    void buildTable(Node* node, std::string code);
    /**
     * recursive helper function for compress that builds the huffman tree
     * using the help of a priority queue
     */
    void buildHuffmanTree();
public:
    /**
     * Default constructor. Sets root to null
     */
    Huffman();
    /**
     * Function to compress a file into binary
     * @param input the input file
     * @param output the output file
     */
    void compress(std::string input, std::string output);
    /**
     * Function to decompress a binary file
     * @param input the input file
     * @param output the output file
     */
    void decompress(std::string input, std::string output);

};

#endif //HUFFMAN_H
