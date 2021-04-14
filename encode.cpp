#include <iostream>
#include <string>
#include <vector>
using namespace std;

//----------------------------------------------------//
// nodes for binary trees                             //                                   
//----------------------------------------------------//
struct Node;
typedef Node* node_pointer;
struct Node{
    node_pointer left;
    node_pointer right;
    unsigned char character;
    int freq;
    
    Node() {}
    Node(unsigned char c, int f, node_pointer l = nullptr, node_pointer r = nullptr) : character(c), left(l), right(r), freq(f) {}
};

// create the frequency array
void makeFreqArray(string text, vector<int> & alphabetFreq) {
    int len = text.length();
    // loop through text, increment at each character index
    for(int i = 0; i < len; i++) {
        unsigned char c = text[i];
        alphabetFreq[c]++;
    }
}

// performs modulus on type double
double mod(double x, double y) {
    return x - (int)(x/y) * y;
}

// performs floor on type double
double floor(double n) {
    return n - (mod(n, 1) >= 0 ? mod(n, 1) : (1 + mod(n, 1)));
}

// Performs fix-up for minHeap
// algorithm as in Module 2
void fixUp(vector<node_pointer> & minHeap) {
    double k = minHeap.size() - 1;
    double parentK = floor((k - 1)/2);

    // while parent exists and minHeap[parent] < minHeap[k]
    while (( parentK >= 0) && (minHeap[parentK]->freq > minHeap[k]->freq)) {
        auto tmp = minHeap[k];
        minHeap[k] = minHeap[parentK];
        minHeap[parentK] = tmp;
        k = parentK;
        parentK = floor((k - 1)/2);
    }
}

// Inserts a single node into the heap
void insertHeap(vector<node_pointer> & minHeap, node_pointer node) {
    minHeap.push_back(node);
    fixUp(minHeap);
}

// Inserts the two popped tries, T1 and T2 into the heap by creating a new combined trie
void insertHeap(vector<node_pointer> & minHeap, node_pointer T1, node_pointer T2) {
    node_pointer Trie = new Node(char(0), T1->freq + T2->freq, T1, T2);
    insertHeap(minHeap, Trie);
}

// Performs fixDown on heap
// algorithm as in Module 2
void fixDown(vector<node_pointer> & minHeap) {
    int i = 0;
    int size = minHeap.size();
    // while last(i) is less than size
    while (2*i + 1 < size) {
        // j equals left child
        int j = 2*i + 1;

        // if j is within the heap and minHeap at j+1 is less than j, increment j
        if ((j != size - 1) && (minHeap[j + 1]->freq < minHeap[j]->freq)) {
            j++;
        }
        if (minHeap[i]->freq <= minHeap[j]->freq) {
            break;
        }

        auto tmp = minHeap[j];
        minHeap[j] = minHeap[i];
        minHeap[i] = tmp;
        i = j;
    }
}

// Deletes the minimum value in heap
// algorithm as in Module 2
node_pointer deleteMin(vector<node_pointer> & minHeap) {
    auto tmp = minHeap[0];
    // swap with last leaf
    minHeap[0] = minHeap[minHeap.size() - 1];
    minHeap[minHeap.size() - 1] = tmp;

    node_pointer minNode = minHeap[minHeap.size() - 1];
    minHeap.pop_back();

    fixDown(minHeap);

    return minNode;
}

// Initialize the minHeap
vector<node_pointer> makeInitHeap(vector<int> & alphabetFreq) {
    vector<node_pointer> minHeap = vector<node_pointer>();
    int len = alphabetFreq.size();

    for (int i = 0; i < len; i++) {
        // skip characters not in text
        if (alphabetFreq[i] == 0) {
            continue;
        }

        // make a trie for the character
        node_pointer node = new Node(char(i), alphabetFreq[i], nullptr, nullptr);
        insertHeap(minHeap, node);
    }

    return minHeap;
}

// Print the character and depth as c_i d_i
void printTrie(node_pointer finalTrie, int depth, vector<string> & alphabetEncode, string encode = "") {
    // if not a character
    if (finalTrie->character == char(0)) {
        depth++;
        if (finalTrie->left != nullptr) {
            printTrie(finalTrie->left, depth, alphabetEncode, encode.append("0"));
        }

        if (finalTrie->right != nullptr) {
            encode.pop_back();
            printTrie(finalTrie->right, depth, alphabetEncode, encode.append("1"));
        }
    }

    else {
        cout << finalTrie->character << depth << " ";
        alphabetEncode[finalTrie->character] = encode;
    }
}


//----------------------------------------------------//
// main                                               //
//----------------------------------------------------//
int main(){ 
    string text;
    getline(cin, text);
    vector<int> alphabetFreq = vector<int>(256);
    makeFreqArray(text, alphabetFreq);
    vector<node_pointer> minHeap = makeInitHeap(alphabetFreq);

    // Perform repeated removal of minimum tries and insertions to get one final trie
    while (minHeap.size() > 1) {
        node_pointer T1 = deleteMin(minHeap);
        node_pointer T2 = deleteMin(minHeap);
        insertHeap(minHeap, T1, T2);
    }

    auto finalTrie = deleteMin(minHeap);

    // contains the encoding for each character
    vector<string> alphabetEncode = vector<string>(256, "");
    // Prints c_i d_i pairs
    printTrie(finalTrie, 0, alphabetEncode);
    cout << endl;

    // Encoded text
    int len = text.length();
    for (int i = 0; i < len; i++) {
        cout << alphabetEncode[text[i]];
    }
}