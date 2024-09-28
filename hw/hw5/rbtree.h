#ifndef RBTREE_H
#define RBTREE_H

enum Color { RED, BLACK };

struct Node {
    int data;
    Color color;
    Node *parent, *left, *right;

    Node(int data);
};

class RedBlackTree {
private:
    Node* root;

    void rotateLeft(Node* x);
    void rotateRight(Node* x);
    void fixViolation(Node* pt);
    Node* BSTInsert(Node* root, Node* pt);
    void inorderUtil(Node* root);

public:
    RedBlackTree();
    void insert(const int data);
    void inorder();
    int dyecount = 0;
    int rotatecount = 0;
    int fixcount = 0;
};

#endif // RBTREE_H
