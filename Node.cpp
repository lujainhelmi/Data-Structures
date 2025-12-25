#include "Node.h"
#include <cstring>

// child constructor
child::child(char ch, Node* n) {
    firstChar = ch;
    node = n;
    next = nullptr;
}

// Node default constructor
Node::Node() {
    data[0] = '\0';
    ended = false;
    frequency = 0;
    timestamp = 0;
    children = nullptr;
}

// Node constructor with label
Node::Node(const char* label) {
    strncpy(data, label, 49);
    data[49] = '\0';
    ended = false;
    frequency = 0;
    timestamp = 0;
    children = nullptr;
}

bool Node::isLeaf() const {
    return children == nullptr;
}

int Node::getChildCount() const {
    int count = 0;
    child* ch = children;
    while (ch) {
        count++;
        ch = ch->next;
    }
    return count;
}



