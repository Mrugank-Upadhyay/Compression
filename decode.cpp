#include <vector>    
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

struct Node;
struct LinkedNode;
typedef Node* node_pointer;
typedef LinkedNode* linked_node_pointer;

// Struct for the Trie Node
struct Node{
    node_pointer left;
    node_pointer right;
    unsigned char character;
    string trieName;
    
    Node(unsigned char c, node_pointer l = nullptr, node_pointer r = nullptr, string trieName = "") : character(c), left(l), right(r), /*priority(priority),*/ trieName(trieName) {}
};

// Struct for the Linked List Node
struct LinkedNode {
    linked_node_pointer next;
    linked_node_pointer prev;
    node_pointer trie;
    int depth;
    static int maxDepth;

    LinkedNode(node_pointer trie, linked_node_pointer next, linked_node_pointer prev, int depth) : trie(trie), next(next), prev(prev), depth(depth) {
    }

};

// set the value for the static member variable
int LinkedNode::maxDepth = 0;

// creates the initial linked list of individual tries
linked_node_pointer makeInitLinkedList(string & tree) {
    int len = tree.length();
    
    // create a trie for the character
    node_pointer node = new Node(tree[0], nullptr, nullptr, string(1, tree[0]));

    // each character had a depth, d
    // we don't know how many indicies the depth value takes up
    // e.g. a9 b20 c123
    // here depth for "a" takes up 1 index, while "c" takes up 3. 
    int depth;
    // we find the starting position of the depth
    int pos = 1;
    char depthLength = tree[pos];
    int count = 0;
    // count the number of indicies depth takes up
    while (depthLength != ' ') {
        pos++;
        depthLength = tree[pos];
        count++;
    } 
    // take the substring starting from the first position
    depth = stoi(tree.substr(1, count));

    // the rest of the for loop starts at this value.
    int start = count + 2;

    // create the first linked list node
    linked_node_pointer head = new LinkedNode(node, nullptr, nullptr, depth);

    // keep track of previous nodes
    linked_node_pointer prev = head;

    // set maxDepth to the first depth value we've seen
    int maxDepth = depth;

    // Do the same as above, for each character
    for (int i = start; i < len;) {
        node_pointer node = new Node(tree[i], nullptr, nullptr, string(1, tree[i]));
        pos = i+1;
        depthLength = tree[pos];
        count = 0;
        while (depthLength != ' ') {
            pos++;
            depthLength = tree[pos];
            count++;
        } 
        depth = stoi(tree.substr(i+1, count));

        linked_node_pointer linkedNode = new LinkedNode(node, nullptr, prev, depth);
        prev->next = linkedNode;
        prev = linkedNode;
        maxDepth = (depth > maxDepth) ? depth : maxDepth;
        i = i + count + 2;
    }

    head->maxDepth = maxDepth;
    return head;
}


// Creates the Huffman tree
linked_node_pointer makeHuffTree(linked_node_pointer linkedNode) {
    // sets current node to beginning of linked list
    linked_node_pointer currNode = linkedNode;

    // keeps track of the head node
    linked_node_pointer head = currNode;
    
    // keep looping until we have gone through all depths {1, ..., maxDepth}
    while (head->maxDepth > 0) {

        // while the current node isnt the last node of the linked list
        while (currNode->next != nullptr) {
            
            // if node n_i and n_i+1 have same depth, and depth = maxDepth
            if ((currNode->depth == currNode->next->depth) && (currNode->depth == currNode->maxDepth)) {
                
                // This is to correctly slot in our new node in the linked list
                // e.g. List = a -> b -> c -> d -> e
                // remove b,c
                // a -> bc -> d -> e
                auto previous = currNode->prev;
                auto next = currNode->next->next;
              
                // We combine the tries of n_i and n_i+1
                node_pointer newNode = new Node(char(0), currNode->trie, currNode->next->trie, 
                                                        currNode->trie->trieName + currNode->next->trie->trieName);
                
                // Then we make a new linked list node that takes in the combined tries
                // and connects to the previous and next nodes
                linked_node_pointer newLinkedNode = new LinkedNode(newNode, next, previous, currNode->depth - 1);
                
                // connect previous's next pointer to our new node
                if (previous != nullptr) {
                    previous->next = newLinkedNode;    
                }

                // connect next's previous pointer to our new node
                if (next != nullptr) {
                    next->prev = newLinkedNode;
                }

                // if our currnode used to be the head, make our new node the head
                if (head == currNode) {
                    head = newLinkedNode;
                }

                currNode = newLinkedNode;
            }
            
            // if our (new) current node isn't the final node of the list, move to next node
            if (currNode->next != nullptr) {
                currNode = currNode->next;
            }
        }

        // reset back to front of linked list and subtract the maxDepth
        currNode = head;
        head->maxDepth--;
    }

    return head;
}

// Decodes the coded text using the final huffman trie
void decode (node_pointer finalTrie, string text) {
    int len = text.length();

    // sets the current node instance to root of trie
    node_pointer currNode = finalTrie;

    // Loops through the coded text
    // each character decides whether to go left or right
    // once we hit a leaf, we print the character and reset our currnode to root
    // on the next index, we start again
    for (int i = 0; i < len; i++) {
        if (text[i] == '0') {
            currNode = currNode->left;
            // if leaf
            if (currNode->left == nullptr) {
                cout << currNode->character;
                // reset to root
                currNode = finalTrie;
            }
        }
        else if (text[i] == '1') {
            currNode = currNode->right;
            if (currNode->left == nullptr) {
                cout << currNode->character;
                currNode = finalTrie;
            }
        }
    }
}

//----------------------------------------------------//
// main                                               //
//----------------------------------------------------//
int main() { 
    string tree, code;
    getline(cin, tree);
    getline(cin, code);

    int len = tree.length();
    
    // Create initial linked list of tries
    linked_node_pointer headNode = makeInitLinkedList(tree);

    // We recreate the huffman tree
    headNode = makeHuffTree(headNode);

    node_pointer finalTrie = headNode->trie;

    // Now we will decode the code
    decode(finalTrie, code);
}