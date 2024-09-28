#ifndef RBTREE_H
#define RBTREE_H
#include "tree.hpp"
#include <cstdint>
#include <map>

enum Color { RED, BLACK };
struct Node {
    int data;
    Color color;
    Node *parent, *left, *right;

    Node(int data);
};

class RBTree : public Tree {
private:
    std :: map<uint32_t, bool> M;
    Node* root;
    void rotateLeft(Node* x);
    void rotateRight(Node* x);
    void fixViolation(Node* pt);
    Node* BSTInsert(Node* root, Node* pt);
    void inorderUtil(Node* root);

public:
    RBTree();
    void insert(const int data);
    void remove(const int data);
    bool find(const int data);
    uint32_t size();
    uint32_t height();
    void inorder();
    int dyecount = 0;
    int rotatecount = 0;
    int fixcount = 0;
};

#endif // RBTREE_H
