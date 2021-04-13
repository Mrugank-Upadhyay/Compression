#include <iostream>
#include <string>
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
    int freq;
    
    Node() {}
    Node(unsigned char c, int f, node_pointer l = nullptr, node_pointer r = nullptr) : left(l), right(r) {}
};

int comp(node_pointer a, node_pointer b){
    return -(a->freq - b->freq);
}


// add your code here


//----------------------------------------------------//
// main                                               //
//----------------------------------------------------//
int main(){ 
    string text;
    getline(cin, text);
    // add your code here
}