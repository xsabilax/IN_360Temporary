#include <map>
#include <iostream>
#include <string>
#include <queue>
#include <bitset>

// hello - salsabila abu

using namespace std;

class HuffmanTree {
    private:
        class Node {
            public:
                Node* pRight;
                Node* pLeft;

                char letter;
                int frequency;

                int seqno; // Its sequence number.

            public:

                Node(char letter, int frequency, int insertionNo) : pRight(nullptr), pLeft(nullptr), letter(letter), frequency(frequency), seqno(insertionNo) {};
                
                // Constructor to make my life easier
                Node(Node* left, Node* right, int insertionNo) : pRight(right), pLeft(left), letter(' '), frequency(left->frequency + right->frequency), seqno(insertionNo){
                    
                }

                struct HuffmanComparator{ // Comparator for creation of the tree.
                    bool operator() (const Node* first, const Node* second) {
                        if (first->frequency == second->frequency) {
                            return first->seqno > second->seqno;
                        } else {
                            return first->frequency > second->frequency;
                            // Min-heap logic.
                        }
                    }
                };
        };

        map<char,int> frequencyMap; // Map initially used for storing symbol frequency
        map<char,string> codeMap; // Map used for storing the symbol's converted code
        Node* root; // Root node for the actual tree

        string preEncoded; // Pre encoded string
        string postEncoded; // Post encoded string

        struct HuffmanCodeComparator { // Ensures the final codes are length ASC, symbol ASC
            bool operator() (const pair<char,string>& first, const pair<char,string>& second){
                if (first.second.length() == second.second.length()){
                    return first.first > second.first;
                } else {
                    return first.second.length() > second.second.length();
                }
            }
        };

    public:

        HuffmanTree() : frequencyMap(), codeMap(), root(nullptr), preEncoded(" ") {};
        ~HuffmanTree() {};
        void frequencyRecorder(const string&); // Records frequency
        void huffmanTreeGeneration(); // Generates the tree
        void valAssign(Node* root, string curr); // Assigns each symbol its code

        string textConversion(); // Converts the text into its compressed version.
        void printCodeMap(); 
        void printFirstFewBytes();

        void displayCompressionStats();

};

void HuffmanTree::frequencyRecorder(const string& text) {
    this->preEncoded = text;

    for (char letter : text) {
        frequencyMap[letter]++; // Implicitly adds any keys/values that do not already exist.
    }
}

void HuffmanTree::huffmanTreeGeneration() {
    priority_queue<Node*, vector<Node*>, Node::HuffmanComparator> huffTree;

    // Steps were outlined in the Trees01 pptx!! Thanks for the hints :)

    int insertionNo = 0;
    for (auto& letter: frequencyMap) { // Create a leaf node for each character...
        // The node should contain the character and its frequency. Dump all nodes into a min-heap priority queue.
        huffTree.push(new Node(letter.first, letter.second, insertionNo++)); // letter.first is the symbol (letter), letter.second is its frequency
    }

    while (huffTree.size() > 1) {
        Node* toleft = huffTree.top(); // Extract two nodes with the lowest frequencies
        huffTree.pop();
        Node* toright = huffTree.top();
        huffTree.pop();
        // Create a new internal node with frequency equal to the two nodes.
        huffTree.push(new Node(toleft, toright, insertionNo++));
    }

    // Repeated until there's only one node left, the root!

    this->root = huffTree.top();
    valAssign(root, ""); // Assigns the codes to the values
}

// Taking the root of the tree, assign each node a unique code based on its positioning.
void HuffmanTree::valAssign(Node* root, string curr) {
	if (root == nullptr) return;

    // Leaf node represents a character.
	if (root->pLeft == nullptr && root->pRight==nullptr) {
		codeMap[root->letter] = curr;
		return;
	}

	valAssign(root->pLeft, curr + "0");
	valAssign(root->pRight, curr + "1");
}

// Takes the pre encoded text and matches each character to its converted code
string HuffmanTree::textConversion() {
    string newString = "";

    for (char letter : this->preEncoded) {
        newString += codeMap[letter];
    }

    postEncoded = newString;
    return newString;
}

// Display the table of converted codes.
void HuffmanTree::printCodeMap(){
    // Use a priority queue with a unique code comparator.
    priority_queue<pair<char,string>, vector<pair<char,string>>, HuffmanCodeComparator> sortedCodes;

    for (auto& entry : codeMap) {
        sortedCodes.push(entry);
    }

    while (!sortedCodes.empty()) {
        pair<char,string> currentPair = sortedCodes.top();
        cout << currentPair.first << "  " << currentPair.second << endl;
        sortedCodes.pop();
    }

}

// Prints the first 8 bytes.
void HuffmanTree::printFirstFewBytes() {
    for (int i = 0; i < 64; i++) {
        if (i % 8 == 0 && i != 0) {
            cout << "  ";
        }
        cout << postEncoded[i];
    }
    cout << endl;
}

// Display comparisons between the packages
void HuffmanTree::displayCompressionStats() {
    int numBits = 0;
    int numBytes = 0;

    for (int i = 0; i < postEncoded.length(); i++) {
        numBits++;
        if (i % 8 == 0) {
            numBytes ++;
        }
    }

    // Use doubles to avoid integer division
    double preEncodedBits = preEncoded.length() * 8.0; // Each char is one byte, which is then 8 bits
    
    // Calculate the compression ratio (post compression / pre compression)
    double ratio = ((double) numBits / preEncodedBits) * 100.0;

    cout << "--- Compression Statistics ---" << endl;
    cout << "Original size:   " << preEncoded.length()*8 << " bits (" << preEncoded.length() << " bytes)" << endl;
    cout << "Compressed size: " << numBits << " bits (" << numBytes << " bytes)" << endl;
    cout << "Compression Ratio: " << ratio << "%" << endl;
}

void huffmanHelper(string text) {
    HuffmanTree newTree;

    cout << "Preencoded text:\n" << text << endl;
    cout << endl;

    newTree.frequencyRecorder(text);
    newTree.huffmanTreeGeneration();
    newTree.textConversion();

    cout << "Code table:" << endl;
    newTree.printCodeMap();
    cout << endl;

    cout << "Printing first 8 bytes:" << endl;
    newTree.printFirstFewBytes();
    cout << endl;

    newTree.displayCompressionStats();

}

int main(){

    huffmanHelper("Once there were brook trouts in the streams in the mountains. "
        "You could see them standing in the amber current where the white edges of their fins wimpled softly in the flow. " 
        "They smelled of moss in your hand. Polished and muscular and torsional. "
        "On their backs were vermiculate patterns that were maps of the world in its becoming. "
        "Maps and mazes. Of a thing which could not be put back. Not be made right again. "
        "In the deep glens where they lived all things were older than man and they hummed of mystery.");
}
