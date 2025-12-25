#pragma once

struct Node;

struct child {
    char firstChar;       // first character of node label
    Node* node;           // pointer to child node
    child* next;          // sibling pointer

    child(char ch, Node* n);
};

struct Node {
    char* data;           // <-- CRITICAL CHANGE: Node label is now a pointer to dynamic memory
    bool ended;           // marks end of valid word
    int frequency;        // usage frequency
    long long timestamp;  // last access time
    child* children;

    // DESTRUCTOR DECLARATION: Required to free the memory pointed to by 'data'
    ~Node(); 

    Node();
    Node(const char* label);

    bool isLeaf() const;      // no children
    int getChildCount() const; // count children
};
