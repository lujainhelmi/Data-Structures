#pragma once
#include "Node.h"

class RadixTree {
public:
    Node* myRoot;

    RadixTree();
    ~RadixTree();

    bool empty();
    void insert(const char* word);
    void addchild(Node* parent, Node* childnode);
    int matchPrefix(const char* nodePrefix, const char* keySegment);
    int SearchPrefix(const char* word, Node* currectNode);

    // search
    bool search(const char* key);
    Node* traverseEdge(Node* node, const char* keySegment, int& matchedLen);
    bool isKeyFullyMatched(Node* node, const char* key, int keyLen);
    void handleSearchFailure(const char* reason);

    // deletion
    bool deleteWord(const char* word);
    bool deleteRec(Node*& current, const char* word);

    // autocomplete
    void collectWords(Node* node, const char* prefix);
    void getAutocompletions(const char* prefix);

    // extras
    long long getCurrentTimestamp();
    void updateWordFrequency(Node* node);
    void incrementFrequency(const char* word);

    // internal memory cleanup
    void destroy(Node* node);
};
