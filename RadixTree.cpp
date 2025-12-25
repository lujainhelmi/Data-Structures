#include "RadixTree.h"
#include <iostream>
#include <chrono>
#include <cstring>
using namespace std;

// ================= Constructors & Destructor =================

RadixTree::RadixTree() {
    myRoot = nullptr;
}

RadixTree::~RadixTree() {
    destroy(myRoot);
}

void RadixTree::destroy(Node* node) {
    if (!node) return;
    child* ch = node->children;
    while (ch) {
        destroy(ch->node);
        child* old = ch;
        ch = ch->next;
        delete old;
    }
    delete node;
}

bool RadixTree::empty() {
    return myRoot == nullptr;
}

// ================= Helper Functions =================

int RadixTree::matchPrefix(const char* nodePrefix, const char* keySegment) {
    int i = 0;
    while (nodePrefix[i] != '\0' && keySegment[i] != '\0' && nodePrefix[i] == keySegment[i])
        i++;
    return i;
}

int RadixTree::SearchPrefix(const char* word, Node* currentNode) {
    return matchPrefix(word, currentNode->data);
}

void RadixTree::addchild(Node* parent, Node* childnode) {
    child* newchild = new child(childnode->data[0], childnode);
    newchild->next = parent->children;
    parent->children = newchild;
}

// ================= INSERT =================

void RadixTree::insert(const char* word) {
    if (empty()) {
        myRoot = new Node(word);
        myRoot->ended = true;
        myRoot->frequency = 1;
        myRoot->timestamp = getCurrentTimestamp();
        return;
    }

    Node* parent = nullptr;
    Node* current = myRoot;
    const char* w = word;

    while (current) {
        int p = matchPrefix(current->data, w);
        int nLen = strlen(current->data);
        int wLen = strlen(w);

        // 1️⃣ SPLIT if mismatch inside current label
        if (p < nLen) {
            Node* suffix = new Node(current->data + p);
            suffix->ended = current->ended;
            suffix->frequency = current->frequency;
            suffix->timestamp = current->timestamp;
            suffix->children = current->children;

            current->data[p] = '\0';
            current->ended = false;
            current->children = nullptr;
            addchild(current, suffix);

            if (p == wLen) {
                current->ended = true;
                current->frequency++;
                current->timestamp = getCurrentTimestamp();
                return;
            }

            Node* nw = new Node(w + p);
            nw->ended = true;
            nw->frequency = 1;
            nw->timestamp = getCurrentTimestamp();
            addchild(current, nw);
            return;
        }

        // 2️⃣ EXACT MATCH → mark as full word
        if (p == wLen && p == nLen) {
            current->ended = true;
            current->frequency++;
            current->timestamp = getCurrentTimestamp();
            return;
        }

        // 3️⃣ Continue search among children
        child* ch = current->children;
        parent = current;
        current = nullptr;
        while (ch) {
            if (ch->firstChar == *(w + p)) {
                current = ch->node;
                break;
            }
            ch = ch->next;
        }

        if (!current) {
            Node* nw = new Node(w + p);
            nw->ended = true;
            nw->frequency = 1;
            nw->timestamp = getCurrentTimestamp();
            addchild(parent, nw);
            return;
        }

        w += p;
    }
}

// ================= SEARCH =================

bool RadixTree::search(const char* key) {
    if (empty()) return false;

    Node* current = myRoot;
    const char* k = key;

    while (current) {
        int p = matchPrefix(current->data, k);
        int nLen = strlen(current->data);

        if (p < nLen) return false;
        if (p == strlen(k)) return current->ended;

        child* ch = current->children;
        current = nullptr;

        while (ch) {
            if (ch->firstChar == *(k + p)) {
                current = ch->node;
                break;
            }
            ch = ch->next;
        }

        if (!current) return false;
        k += p;
    }
    return false;
}

// ================= DELETE (FULL FIXED VERSION) =================

bool RadixTree::deleteWord(const char* word) {
    if (!myRoot) return false;
    return deleteRec(myRoot, word);
}

bool RadixTree::deleteRec(Node*& current, const char* word) {
    if (!current) return false;

    const char* w = word;
    int p = matchPrefix(current->data, w);
    int nLen = strlen(current->data);
    int wLen = strlen(w);

    if (p == 0) return false;

    // FULL match at this node
    if (p == nLen && p == wLen) {
        if (!current->ended) return false;
        current->ended = false;

        if (!current->children) {
            delete current;
            current = nullptr;
            return true;
        }
        return true;
    }

    // Traverse deeper
    if (p == nLen && p < wLen) {
        w += p;
        char nextChar = *w;

        child* prev = nullptr;
        child* ch = current->children;
        while (ch && ch->firstChar != nextChar) {
            prev = ch;
            ch = ch->next;
        }
        if (!ch) return false;

        bool removed = deleteRec(ch->node, w);
        if (!removed) return false;

        if (!ch->node) {
            if (!prev) current->children = ch->next;
            else prev->next = ch->next;
            delete ch;
        }

        // MERGE UPWARD if one child left & not a word
        if (!current->ended && current->children && current->children->next == nullptr) {
            Node* childNode = current->children->node;
            strcat(current->data, childNode->data);
            current->ended = childNode->ended;
            current->children = childNode->children;
            delete childNode;
        }
        return true;
    }

    return false;
}

// ================= AUTOCOMPLETE (Direct Print) =================

void RadixTree::collectWords(Node* node, const char* prefix) {
    if (!node) return;

    char newPrefix[100];
    strcpy(newPrefix, prefix);
    strcat(newPrefix, node->data);

    if (node->ended) cout << "- " << newPrefix << endl;

    child* ch = node->children;
    while (ch) {
        collectWords(ch->node, newPrefix);
        ch = ch->next;
    }
}

void RadixTree::getAutocompletions(const char* prefix) {
    if (empty()) {
        cout << "Tree empty.\n";
        return;
    }

    Node* current = myRoot;
    const char* p = prefix;
    int pre = matchPrefix(current->data, p);

    if (pre == 0 && strlen(current->data) > 0) {
        cout << "No suggestions found.\n";
        return;
    }

    p += pre;
    while (strlen(p) > 0) {
        child* ch = current->children;
        current = nullptr;
        while (ch) {
            int m = matchPrefix(ch->node->data, p);
            if (m > 0) {
                current = ch->node;
                p += m;
                break;
            }
            ch = ch->next;
        }
        if (!current) {
            cout << "No suggestions found.\n";
            return;
        }
    }

    collectWords(current, prefix);
}

// ================= Timestamp =================

long long RadixTree::getCurrentTimestamp() {
    return chrono::duration_cast<chrono::milliseconds>(
        chrono::system_clock::now().time_since_epoch()).count();
}

void RadixTree::updateWordFrequency(Node* node) {
    if (node && node->ended) {
        node->frequency++;
        node->timestamp = getCurrentTimestamp();
    }
}

void RadixTree::incrementFrequency(const char* word) {
    if (!myRoot) return;
    search(word);
}
