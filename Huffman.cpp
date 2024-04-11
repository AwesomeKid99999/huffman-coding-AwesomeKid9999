#include "Huffman.h"


Huffman::Huffman() {
    root = nullptr;
}

void Huffman::compress(std::string input, std::string output) {
    // checks if the maps and queue are empty
    while (!charQueue.empty()) {
        charQueue.pop();
    }
    if (!charCount.empty()) {
        charCount.clear();
    }
    if (!charCodes.empty()) {
        charCodes.clear();
    }

    // create input and output streams
    std::ifstream inputFile;
    std::ofstream outputFile;

    // open input file
    inputFile.open(input);


    char character; // variable to read the characters in the input file
    int numChars = 0; // variable to store how many characters were in the text file

    while (inputFile.get(character)) { // reads one letter at a time until the end of the file
        charCount[character]++; // store how many times each character was seen
        numChars++; // increment the total number of characters found in the file
    }
    // close file after its done reading
    inputFile.close();

    countCharacters(); // count how many times each character was seen
    buildHuffmanTree(); // builds huffman tree

    // recursively traverse the huffman tree and build the code table
    buildTable(root, "");


    std::string header = ""; // make an empty header string

    // store the total number of characters in the text file
    header += std::to_string(numChars) + static_cast<char>(30);

    for (const auto& pair : charCodes) { // loops through code table to build header from the codes
        // appends the character (pair.first) and its huffman code (pair.second) to the header
        header += std::string(1, pair.first) + pair.second + static_cast<char>(30);
    }

    storage.open(output, "write"); // open the output with storage in write mode
    storage.setHeader(header); // set the header from the string we appended

    // compresses the text and saving into binary
    inputFile.open(input); // open the input file again
    while (inputFile.get(character)) { // read every character
        // inserting the binary string associated with characters of the huffman table
        storage.insert(charCodes[character]);
    }
    // close storage and input file
    storage.close();
    inputFile.close();

}

void Huffman::decompress(std::string input, std::string output) {
    // checks if the maps and queue are empty
    while (!charQueue.empty()) {
        charQueue.pop();
    }
    if (!charCount.empty()) {
        charCount.clear();
    }
    if (!charCodes.empty()) {
        charCodes.clear();
    }

    // create input and output streams
    std::ifstream inputFile;
    std::ofstream outputFile(output);

    inputFile.open(input); // open compressed file with IO
    storage.open(input, "read"); // open compressed file with storage
    std::string header = storage.getHeader(); // get the header
    std::stringstream ss(header); // create a string stream with header

    // make a delimiter and set it to record separator (the character we used to separate the codes)
    char delimiter = static_cast<char>(30);
    std::string string; // make a string variable to get the string content up to the delimiter
    char currentCharacter; // make a character variable to store the current character in the string


    Node *root = new Node('\0', 1); // make a new root node
    Node *current = root; // make a current node and set it with root

    // get the total number of characters in the text file from the header
    std::getline(ss, string, delimiter);
    int size = std::stoi(string); // convert the string number to an integer

    // rebuilding the huffman tree
    while (std::getline(ss, string, delimiter)) { // reads one character and its code at a time
        currentCharacter = string[0]; // get the character from the first character in the string
        for (int i = 1; i < string.length(); i++) { // loops through the character code followed by the character
            if (string[i] == '0') { // if current character is 0
                if (current->zero == nullptr) { // checking for null node
                    current->zero = new Node('\0', 1); // make a new node if null
                }
                // traverse left
                current = current->zero;
            } else if (string[i] == '1') { // if current character is 1
                if (current->one == nullptr) { // checking for null node
                    current->one = new Node('\0', 1); // make a new node if null
                }
                // traverse right
                current = current->one;
            }
        }
        // set the current node's letter to the first character found in the string
        current->letter = currentCharacter;
        // set the current node back to the root
        current = root;
    }

    std::string binary = ""; // make an empty binary string

    // reading the binary file then writing to the output file
    while (storage.extract(binary)) { // extract the binary string from the input file
        for (int i = 0; i < binary.length(); i++) { // loops through the binary string
            if (binary[i] == '0') { // if the current character is 0
                current = current->zero; // traverse left
            } else { // if current character is 1
                current = current->one; // traverse right
            }

            if (current->letter != '\0') { // if current node has a character (leaf node)
                outputFile << current->letter; // write the character into the output
                current = root; // make the current node go back to the root
                size--; // decrement the total number of characters found in the string
            }

            if (size == 0) { // if we don't need to decode any more new characters
                break;
            }
        }
    }
    inputFile.close();
    storage.close();
}


void Huffman::countCharacters() {
    // auto iterator to iterate counting the characters to create a priority queue
    for (const auto& pair : charCount) { // loop through the map
        // create nodes to store each letter and how many times it appeared
        Node *charNode = new Node(pair.first, pair.second);
        charQueue.push(charNode); // push char nodes into queue
    }
}

void Huffman::buildHuffmanTree() {
    // building the huffman tree
    while (charQueue.size() > 1) {
        Node *left_child = charQueue.top(); // make left node top of queue
        charQueue.pop();
        Node *right_child = charQueue.top(); // make right node top of queue
        charQueue.pop();

        // making the parent node
        int sum = left_child->weight + right_child->weight; // get the sum of the left and right child weights
        // make a parent node, set its letter to null, its weight to the sum of left and right children
        Node *parent = new Node('\0', sum, left_child, right_child);
        charQueue.push(parent); // push parent back into queue
    }
    // when loop is done set the root node to queue's first value
    root = charQueue.top();
}

void Huffman::buildTable(Node* node, std::string code) {
    if (node == nullptr) {
        return;
    }

    // base case
    if (node->letter != '\0') { // if letter was found
        charCodes[node->letter] = code; // store the code into the map
    } else { // recursive case
        buildTable(node->zero, code + "0"); // traverse left
        buildTable(node->one, code + "1"); // traverse right
    }

}

