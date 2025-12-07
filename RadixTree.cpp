// RadixTree.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <cstring>
#include "RadixTree.h"


RadixTree::RadixTree() {
	myRoot = nullptr;
}
RadixTree::~RadixTree() {
	//deletion 
}

bool RadixTree::empty() {
	return myRoot == nullptr;

}
int RadixTree::SearchPrefix(const char* word, Node* currentNode) {
	int i = 0;
	while (currentNode->data[i] != '\0' && word[i] != '\0' && word[i] == currentNode->data[i])
		i++;
	return i;
}
void RadixTree::addchild(Node* parent, Node* childnode) {
	child* newchild = new child(childnode->data[0], childnode);//saved first char, and childnode is the node of child itselt
	newchild->next = parent->children;//siblings of new child is now the chilren of the parent
	parent->children = newchild; //children of parent is now the new child, so when saying children the pointer now points to the new child
}
void RadixTree::insert(const char* word) {
	//case1:empty tree
	if (empty()) {
		myRoot = new Node(word);
		myRoot->ended = true;
		return;
	}
	//case 3:common  prefix found
	//child* cur = myRoot->children;
	bool isPrefix = false;
	//Node* current = myRoot;
	Node* parentNode = myRoot;
	child* ch = parentNode->children;
	while (ch != nullptr) {
		int nodeLen = 0;
		while (ch->node->data[nodeLen] != '\0') nodeLen++;
		int wordLen = 0;
		while (word[wordLen] != '\0') wordLen++;
		int prefix = SearchPrefix(word, ch->node);
		if (prefix > 0) {// case exact match
			isPrefix = true;
			if (prefix == nodeLen && prefix == wordLen) {// this works becaus ethe serachprefix counts only equal prefixes so if they are same number as well then its the same word
				ch->node->ended = true;
				return;
			}
			if (prefix == wordLen && prefix < nodeLen) {
				Node* oldnode = ch->node;//let oldnode pointer point at the node with the whole word
				Node* newnode = new Node(word); //node created for our word 
				newnode->ended = true;
				/*char temp[50]
				strcpy(temp, oldnode->data+pref);
				strcpy(oldnode->data,temp);
				or */
				Node* suffix = new Node(oldnode->data + prefix);
				suffix->ended = oldnode->ended;
				suffix->children = oldnode->children;
				newnode->children = nullptr;
				addchild(newnode, suffix);
				ch->node = newnode;
				delete oldnode;
				return;

			}
				//case 2:common  prefix found
		    /*if (wordLen > nodeLen) {
				isPrefix = true;
				for (int i = 0; i < nodeLen; i++) {
					if (ch->node->data[i] != word[i]) {
						isPrefix = false;
						break;
					}
				}
			if (isPrefix) {
					int difference = wordLen - nodeLen;
					Node* newNode = new Node();
					for (int i = 0; i < difference; i++) {
						newNode->data[i] = word[nodeLen + i];
					}
				    newNode->data[difference] = '\0';
					newNode->ended = true;
					newNode->children = nullptr;
					child* newChild = new child(newNode->data[0], newNode);
					newChild->next = ch->node->children;
					ch->node->children = newChild;
					addchild(ch->node, newNode);
					return;
				}
			}*/
			if (prefix == nodeLen && wordLen > nodeLen) {

				Node* newNode = new Node(word + nodeLen);
				newNode->ended = true;
				newNode->children = nullptr;

				addchild(ch->node, newNode);
				return;
			}
			//else {//case 3:partial overlap condition
				//int prefix = SearchPrefix(word, ch->node->data);
			if (prefix > 0 && prefix < nodeLen) {
				Node* newTop = new Node();
				strncpy_s(newTop->data, ch->node->data, prefix);
				newTop->data[prefix] = '\0';
				newTop->ended = false;
				Node* node1 = new Node();
				Node* node2 = new Node();
				strncpy_s(node1->data, ch->node->data + prefix, nodeLen - prefix);
				node1->data[nodeLen - prefix] = '\0';
				node1->ended = ch->node->ended;
				node1->children = ch->node->children;
				strncpy_s(node2->data, word + prefix, wordLen - prefix);
				node2->data[wordLen - prefix] = '\0';
				node2->ended = true;
				node2->children = nullptr;
				child* child1 = new child(node1->data[0], node1);
				child* child2 = new child(node2->data[0], node2);
				child1->next = child2;
				newTop->children = child1;
				//child* newChild= new child(newTop->data[0],newTop);
				//newChild->next = ch->next;
				//parentNode->children = newChild;
				addchild(parentNode, newTop);
				return;
			}
		}
		parentNode = ch->node;
		ch = ch->next;

	}

	//case 2:no common prefix found
		Node* newnode = new Node(word);// created a newnode pointer of type node, then made it point to a node that has the word
		newnode->ended = true;
		newnode->children = nullptr;
		addchild(parentNode, newnode);
	}

#pragma once
#include <cstring>

// ---------------- Node Structure ----------------
struct Node {
    char prefix[50];
    bool isTerminal;
    Node* children[26];

    Node() {
        prefix[0] = '\0';
        isTerminal = false;
        for (int i = 0; i < 26; i++) children[i] = nullptr;
    }
};

// --------------- Helper Functions ---------------

// Count number of non-null children
int countChildren(Node* node) {
    int count = 0;
    for (int i = 0; i < 26; i++)
        if (node->children[i] != nullptr)
            count++;
    return count;
}

// Returns the index of the single child, or -1 if none or more than 1
int getOnlyChild(Node* node) {
    int idx = -1;
    for (int i = 0; i < 26; i++) {
        if (node->children[i] != nullptr) {
            if (idx != -1) return -1; // more than one child
            idx = i;
        }
    }
    return idx;
}

// Removes a prefix from the front of a C-string
void removePrefix(char* str, int prefixLen) {
    int len = strlen(str);
    for (int i = prefixLen; i <= len; i++)
        str[i - prefixLen] = str[i];
}

// --------------- Core Deletion Function ---------------

bool deleteRec(Node*& node, const char* key) {
    if (!node) return false;

    int prefixLen = strlen(node->prefix);
    int keyLen = strlen(key);

    // Prefix mismatch
    if (prefixLen > 0) {
        if (strncmp(node->prefix, key, prefixLen) != 0)
            return false;
    }

    // Case 1: We reached the node storing the key
    if (prefixLen == keyLen) {
        if (!node->isTerminal) return false; // key not stored
        
        node->isTerminal = false; // delete key value

        // If leaf → remove node
        if (countChildren(node) == 0) {
            delete node;
            node = nullptr;
            return true;
        }

        // If exactly one child left → merge
        if (countChildren(node) == 1) {
            int idx = getOnlyChild(node);
            Node* child = node->children[idx];

            char newPrefix[50];
            strcpy(newPrefix, node->prefix);
            strcat(newPrefix, child->prefix);

            strcpy(node->prefix, newPrefix);
            node->isTerminal = child->isTerminal;

            // move grandchildren up
            for (int i = 0; i < 26; i++) {
                node->children[i] = child->children[i];
            }

            delete child;
        }

        return true;
    }

    // Case 2: Need to continue deeper
    int nextIndex = key[prefixLen] - 'a';
    if (nextIndex < 0 || nextIndex >= 26) return false;

    Node* child = node->children[nextIndex];
    if (!child) return false;

    bool removed = deleteRec(child, key + prefixLen);

    if (!removed) return false;

    // After deletion, check if child should be removed
    if (child == nullptr) {
        node->children[nextIndex] = nullptr;
    } else {
        // If child becomes removable
        if (!child->isTerminal && countChildren(child) == 0) {
            delete child;
            node->children[nextIndex] = nullptr;
        }
        // If child can be merged upward
        else if (!child->isTerminal && countChildren(child) == 1) {
            int idx = getOnlyChild(child);

            char newPrefix[50];
            strcpy(newPrefix, node->children[nextIndex]->prefix);
            strcat(newPrefix, child->children[idx]->prefix);

            Node* grand = child->children[idx];

            strcpy(node->children[nextIndex]->prefix, newPrefix);
            node->children[nextIndex]->isTerminal = grand->isTerminal;

            for (int i = 0; i < 26; i++)
                node->children[nextIndex]->children[i] = grand->children[i];

            delete grand;
        }
    }

    // Merge this node upward if possible
    if (!node->isTerminal && countChildren(node) == 1) {
        int only = getOnlyChild(node);
        if (only != -1) {
            Node* child2 = node->children[only];

            char newPrefix[50];
            strcpy(newPrefix, node->prefix);
            strcat(newPrefix, child2->prefix);

            strcpy(node->prefix, newPrefix);
            node->isTerminal = child2->isTerminal;

            for (int i = 0; i < 26; i++)
                node->children[i] = child2->children[i];

            delete child2;
        }
    }

    return true;
}





// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file


