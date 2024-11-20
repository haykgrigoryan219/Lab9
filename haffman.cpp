#include <iostream>
#include <queue>
#include <vector>
#include <unordered_map>
#include <string>
#include <bitset>

using namespace std;

// A tree node structure
struct Node {
    char ch;
    int freq;
    Node* left;
    Node* right;

    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

// Comparison function for the priority queue (min-heap)
struct Compare {
    bool operator()(Node* left, Node* right) {
        return left->freq > right->freq;
    }
};

// Function to generate the Huffman codes from the tree
void generateCodes(Node* root, const string& str, unordered_map<char, string>& codes) {
    if (!root) return;
    
    // If this is a leaf node, add its character and code
    if (!root->left && !root->right) {
        codes[root->ch] = str;
    }

    generateCodes(root->left, str + "0", codes);  // Go left with '0'
    generateCodes(root->right, str + "1", codes); // Go right with '1'
}

// Function to encode the input string using the Huffman codes
string encode(const string& text, const unordered_map<char, string>& codes) {
    string encoded = "";
    for (char ch : text) {
        encoded += codes.at(ch);
    }
    return encoded;
}

// Function to decode the encoded string using the Huffman tree
string decode(const string& encodedText, Node* root) {
    string decodedText = "";
    Node* currentNode = root;
    
    for (char bit : encodedText) {
        // Move left for '0' and right for '1'
        if (bit == '0') {
            currentNode = currentNode->left;
        } else {
            currentNode = currentNode->right;
        }
        
        // If a leaf node is reached, append the character to the result
        if (!currentNode->left && !currentNode->right) {
            decodedText += currentNode->ch;
            currentNode = root; // Start from the root for the next character
        }
    }

    return decodedText;
}

// Function to print the heap contents (for debugging)
void printHeap(priority_queue<Node*, vector<Node*>, Compare> minHeap) {
    // Create a temporary vector to copy the heap contents
    vector<Node*> temp;
    while (!minHeap.empty()) {
        temp.push_back(minHeap.top());
        minHeap.pop();
    }
    
    // Print the contents
    cout << "Heap contents (frequency and character):\n";
    for (Node* node : temp) {
        cout << "Character: " << node->ch << " | Frequency: " << node->freq << endl;
    }
}
// Main function for Huffman Encoding
void huffmanEncoding(const string& text) {
    // Step 1: Count frequency of each character
    unordered_map<char, int> frequency;
    for (char ch : text) {
        frequency[ch]++;
    }

    // Step 2: Build a priority queue (min-heap) for the nodes
    priority_queue<Node*, vector<Node*>, Compare> minHeap;

    // Create a leaf node for each character and insert it into the priority queue
    for (auto& pair : frequency) {
        minHeap.push(new Node(pair.first, pair.second));
    }
    printHeap(minHeap);
    
    // Step 3: Build the Huffman Tree
    while (minHeap.size() > 1) {
        // Extract the two nodes with the lowest frequency
        Node* left = minHeap.top();
        minHeap.pop();
        Node* right = minHeap.top();
        minHeap.pop();

        // Create a new internal node with a frequency equal to the sum of the two nodes
        Node* newNode = new Node('\0', left->freq + right->freq);
        newNode->left = left;
        newNode->right = right;

        // Insert the new node back into the priority queue
        minHeap.push(newNode);
    }

    // Step 4: Generate Huffman Codes
    unordered_map<char, string> codes;
    generateCodes(minHeap.top(), "", codes);
    
    // Step 5: Print the Huffman codes
    cout << "Character\tFrequency\tHuffman Code" << endl;
    for (auto& pair : codes) {
        cout << pair.first << "\t\t" << frequency[pair.first] << "\t\t" << pair.second << endl;
    }

    // Step 6: Encode the input text
    string encodedString = encode(text, codes);
    cout << "\nEncoded String: " << encodedString << endl;

    // Step 7: Decode the encoded string
    string decodedString = decode(encodedString, minHeap.top());
    cout << "\nDecoded String: " << decodedString << endl;

    cout << endl;
}

int main() {
    string inputText;
    cout << "Enter a string to encode: ";
    getline(cin, inputText);

    huffmanEncoding(inputText);
    
    return 0;
}
