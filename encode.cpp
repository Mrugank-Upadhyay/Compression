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
    // you can add attributes here
    node_pointer left;
    node_pointer right;
    unsigned char character;
    int freq;
    node_pointer parent;
    
    Node() {}
    Node(unsigned char c, int f, node_pointer l = nullptr, node_pointer r = nullptr) : character(c), left(l), right(r), freq(f) {}
    void addParent(node_pointer parentNode) {
        parent = parentNode;
    }
};

int comp(node_pointer a, node_pointer b){
    return -(a->freq - b->freq);
}


// add your code here

void makeFreqArray(string text, vector<int> & alphabetFreq) {
    int len = text.length();
    for(int i = 0; i < len; i++) {
        unsigned char c = text[i];
        alphabetFreq[c]++;
    }
}


void fixUp(vector<node_pointer> minHeap) {
    int i = minHeap.size() - 1;
    int parentK = (i - 1)/2;
    while (( parentK >= 0) && (minHeap[parentK] < minHeap[i])) {
        auto tmp = minHeap[i];
        minHeap[i] = minHeap[parentK];
        minHeap[parentK] = tmp;
        i = parentK;
        parentK = (i - 1)/2;
    }
}

void insertHeap(vector<node_pointer> minHeap, node_pointer node) {
    minHeap.push_back(node);
    fixUp(minHeap);
}

void insertHeap(vector<node_pointer> minHeap, node_pointer T1, node_pointer T2) {
    node_pointer Trie = new Node(char(0), T1->freq + T2->freq, T1, T2);
    insertHeap(minHeap, Trie);
}

void fixDown(vector<node_pointer> minHeap) {
    int i = 0;
    int size = minHeap.size();
    while (2*i + 1 < size) {
        int j = 2*i + 1;
        if ((j != size - 1) && (minHeap[j + 1] > minHeap[j])) {
            j++;
        }
        if (minHeap[i] >= minHeap[j]) {
            break;
        }

        auto tmp = minHeap[j];
        minHeap[j] = minHeap[i];
        minHeap[i] = tmp;
        i = j;
    }
}

node_pointer deleteMin(vector<node_pointer> minHeap) {
    auto tmp = minHeap[0];
    minHeap[0] = minHeap[minHeap.size() - 1];
    minHeap[minHeap.size() - 1] = tmp;

    node_pointer minNode = minHeap[minHeap.size() - 1];
    minHeap.pop_back();

    fixDown(minHeap);

    return minNode;
}

vector<node_pointer> makeInitHeap(vector<int> & alphabetFreq) {
    vector<node_pointer> minHeap = vector<node_pointer>();
    int len = alphabetFreq.size();
    for (int i = 0; i < len; i++) {
        if (alphabetFreq[i] == 0) {
            continue;
        }
        node_pointer node = new Node(char(i), alphabetFreq[i]);
        insertHeap(minHeap, node);
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
    // for(int i = 0; i < alphabetFreq.size(); i++) {
    //     if (alphabetFreq[i] != 0) {
    //         cout << char(i) << ":" << alphabetFreq[i] << ", ";
    //     }
    // }

    while (minHeap.size() > 1) {
        node_pointer T1 = deleteMin(minHeap);
        node_pointer T2 = deleteMin(minHeap);
        insertHeap(minHeap, T1, T2);
    }

    auto finalTrie = deleteMin(minHeap);

    // print out the encodings and coded text
}