#include <vector>    
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

struct Node;
typedef Node* node_pointer;
struct Node{
    node_pointer left;
    node_pointer right;
    unsigned char character;
    // [depth, position]
    vector<int> priority;
    
    Node() {}
    Node(unsigned char c, vector<int> priority, node_pointer l = nullptr, node_pointer r = nullptr) : character(c), left(l), right(r), priority(priority) {}
};

double mod(double x, double y) {
    return x - (int)(x/y) * y;
}

double floor(double n) {
    return n - (mod(n, 1) >= 0 ? mod(n, 1) : (1 + mod(n, 1)));
}

// True if a < b
bool compareLessThanPriority(node_pointer a, node_pointer b) {
    if (a->priority[0] < b->priority[0]) {
        return true;
    }
    else if (a->priority[0] > b->priority[0]){
        return false;
    }
    else {
        if (a->priority[1] < b->priority[1]) {
            return false;
        }
        else {
            return true;
        }
    }
}

void fixUp(vector<node_pointer> & maxHeap) {
    double k = maxHeap.size() - 1;
    double parentK = floor((k - 1)/2);

    while (( parentK >= 0) && (compareLessThanPriority(maxHeap[parentK], maxHeap[k]))) {
        auto tmp = maxHeap[k];
        maxHeap[k] = maxHeap[parentK];
        maxHeap[parentK] = tmp;
        k = parentK;
        parentK = floor((k - 1)/2);
    }
}

void insertHeap(vector<node_pointer> & maxHeap, node_pointer node) {
    maxHeap.push_back(node);
    fixUp(maxHeap);
}

// void insertHeap(vector<node_pointer> & maxHeap, node_pointer T1, node_pointer T2) {
//     node_pointer Trie = new Node(char(0), T1->freq + T2->freq, T1, T2);
//     insertHeap(maxHeap, Trie);
// }

// void fixDown(vector<node_pointer> & maxHeap) {
//     int i = 0;
//     int size = maxHeap.size();
//     while (2*i + 1 < size) {
//         int j = 2*i + 1;
//         if ((j != size - 1) && (maxHeap[j + 1]->freq < maxHeap[j]->freq)) {
//             j++;
//         }
//         if (maxHeap[i]->freq <= maxHeap[j]->freq) {
//             break;
//         }

//         auto tmp = maxHeap[j];
//         maxHeap[j] = maxHeap[i];
//         maxHeap[i] = tmp;
//         i = j;
//     }
// }

// node_pointer deleteMin(vector<node_pointer> & maxHeap) {
//     auto tmp = maxHeap[0];
//     maxHeap[0] = maxHeap[maxHeap.size() - 1];
//     maxHeap[maxHeap.size() - 1] = tmp;

//     node_pointer minNode = maxHeap[maxHeap.size() - 1];
//     maxHeap.pop_back();

//     fixDown(maxHeap);

//     return minNode;
// }

// vector<node_pointer> makeInitHeap(vector<int> & alphabetFreq) {
//     vector<node_pointer> maxHeap = vector<node_pointer>();
//     int len = alphabetFreq.size();
//     for (int i = 0; i < len; i++) {
//         if (alphabetFreq[i] == 0) {
//             continue;
//         }
//         node_pointer node = new Node(char(i), alphabetFreq[i], nullptr, nullptr);
//         insertHeap(maxHeap, node);
//     }

//     return maxHeap;
// }

vector<node_pointer> makeInitHeap(vector<int> positionAtDepth, string tree) {
    vector<node_pointer> maxHeap = vector<node_pointer>();
    int len = tree.length();

    for (int i = 0; i < len; i += 3) {
        int depth = tree[i+1];
        positionAtDepth[depth]++;
        vector<int> priority = {depth, positionAtDepth[depth]};
        node_pointer node = new Node(char(i), priority);
        insertHeap(maxHeap, node);
    }
}

//----------------------------------------------------//
// main                                               //
//----------------------------------------------------//
int main(){ 
    string tree, code;
    getline(cin, tree);
    getline(cin, code);

    int maxDepth = 0;
    int len = tree.length();
    for (int i = 1; i < len; i += 3) {
            maxDepth = (maxDepth < tree[i] - '0') ? tree[i] - '0' : maxDepth;
    }
    cout << maxDepth << endl;
    vector<int> positionAtDepth = vector<int>(maxDepth + 1, 0);

    vector<node_pointer> maxHeap = makeInitHeap(positionAtDepth, tree);

    for (int i = 0; i < maxHeap.size(); i++) {
        cout << maxHeap[i]->character << ": (" << maxHeap[i]->priority[0] << ", " << maxHeap[i]->priority[1] << ")  ";
    }

}