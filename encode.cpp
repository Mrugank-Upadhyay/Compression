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
    string trieName;
    
    Node() {}
    Node(unsigned char c, int f, node_pointer l = nullptr, node_pointer r = nullptr, string str = "") : character(c), left(l), right(r), freq(f), trieName(str) {}
    void addParent(node_pointer parentNode) {
        parent = parentNode;
    }
};

// int comp(node_pointer a, node_pointer b){
//     return -(a->freq - b->freq);
// }


void makeFreqArray(string text, vector<int> & alphabetFreq) {
    int len = text.length();
    for(int i = 0; i < len; i++) {
        unsigned char c = text[i];
        alphabetFreq[c]++;
    }
}

double mod(double x, double y) {
    return x - (int)(x/y) * y;
}

double floor(double n) {
    return n - (mod(n, 1) >= 0 ? mod(n, 1) : (1 + mod(n, 1)));
}

void fixUp(vector<node_pointer> & minHeap) {
    double i = minHeap.size() - 1;
    double parentK = floor((i - 1)/2);

    cout << "START ----> i: " << i << " parentK: " << parentK << endl;
    for (int i = 0; i < minHeap.size(); i++) {
        cout << minHeap[i]->trieName << ":" << minHeap[i]->freq << endl;
    }

    while (( parentK >= 0) && (minHeap[parentK]->freq > minHeap[i]->freq)) {
        auto tmp = minHeap[i];
        minHeap[i] = minHeap[parentK];
        minHeap[parentK] = tmp;
        i = parentK;
        parentK = floor((i - 1)/2);

        cout << "LOOP ---> i: " << i << " parentK: " << parentK << endl;

        for (int i = 0; i < minHeap.size(); i++) {
        cout << minHeap[i]->trieName << ":" << minHeap[i]->freq << endl;
        }
    }
    
    cout << "END" << endl;
    for (int i = 0; i < minHeap.size(); i++) {
        cout << minHeap[i]->trieName << ":" << minHeap[i]->freq << endl;
    }
}

void insertHeap(vector<node_pointer> & minHeap, node_pointer node) {
    minHeap.push_back(node);
    //cout << "minHeap size: " << minHeap.size() << endl;
    fixUp(minHeap);
}

void insertHeap(vector<node_pointer> & minHeap, node_pointer T1, node_pointer T2) {
    cout << "triename = " << T1->trieName + T2->trieName << endl;
    node_pointer Trie = new Node(char(0), T1->freq + T2->freq, T1, T2, T1->trieName + T2->trieName);
    insertHeap(minHeap, Trie);
}

void fixDown(vector<node_pointer> & minHeap) {
    int i = 0;
    int size = minHeap.size();
    while (2*i + 1 < size) {
        int j = 2*i + 1;
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

node_pointer deleteMin(vector<node_pointer> & minHeap) {
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
        node_pointer node = new Node(char(i), alphabetFreq[i], nullptr, nullptr, string(1,char(i)));
        insertHeap(minHeap, node);
    }

    return minHeap;
}


void printTrie(node_pointer finalTrie) {
    if (finalTrie->character == char(0)) {
        cout << "*" << endl;
        if (finalTrie->left != nullptr) {
            printTrie(finalTrie->left);
        }
        if (finalTrie->right != nullptr) {
            printTrie(finalTrie->right);
        }
    }

    else {
        cout << finalTrie->character << " ";
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

    // cout << endl;
    cout << "minHeap:" << endl;
    for (int i = 0; i < minHeap.size(); i++) {
        cout << minHeap[i]->trieName << ": " << minHeap[i]->freq << ", ";
    }
    cout << endl;

    while (minHeap.size() > 1) {
        node_pointer T1 = deleteMin(minHeap);
        node_pointer T2 = deleteMin(minHeap);
        insertHeap(minHeap, T1, T2);
       
        for (int i = 0; i < minHeap.size(); i++) {
            cout << minHeap[i]->trieName << ": " << minHeap[i]->freq << ", ";
        }
        cout << endl;
    }

    for (int i = 0; i < minHeap.size(); i++) {
        cout << minHeap[i]->trieName << ": " << minHeap[i]->freq << ", ";
    }
    cout << endl;

    auto finalTrie = deleteMin(minHeap);
    for (int i = 0; i < minHeap.size(); i++) {
        cout << minHeap[i]->trieName << ": " << minHeap[i]->freq << ", ";
    }
    cout << endl;

    printTrie(finalTrie);
    // print out the encodings and coded text
}