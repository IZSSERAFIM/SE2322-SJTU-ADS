#include "rbtree.hpp"
#include <iostream>
#include <cstdint>

RBTree::RBTree() {}

Node::Node(int data) : data(data), color(RED), parent(nullptr), left(nullptr), right(nullptr) {}

void RBTree::rotateLeft(Node* x) {
    // Left rotation logic
    Node* y = x->right;
    x->right = y->left;
    if (y->left != nullptr)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == nullptr)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
    rotatecount++;
}

void RBTree::rotateRight(Node* x) {
    // Right rotation logic
    Node* y = x->left;
    x->left = y->right;
    if (y->right != nullptr)
        y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == nullptr)
        root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
    y->right = x;
    x->parent = y;
    rotatecount++;
}

void RBTree::fixViolation(Node* pt) {
    // Fix violation logic
    Node* parent_pt = nullptr;
    Node* grand_parent_pt = nullptr;


    while ((pt != root) && (pt->color == RED) && (pt->parent->color == RED)) {
        parent_pt = pt->parent;
        grand_parent_pt = pt->parent->parent;
        fixcount++;

        if (parent_pt == grand_parent_pt->left) {
            Node* uncle_pt = grand_parent_pt->right;

            // Uncle is red
            if (uncle_pt != nullptr && uncle_pt->color == RED) {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
                dyecount+=3;
            } else {// Uncle is black
                // LL-Case and LR-Case, write your code here
                if (pt == parent_pt->right) {
                    rotateLeft(parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }
                rotateRight(grand_parent_pt);
                std::swap(parent_pt->color, grand_parent_pt->color);
                dyecount+=2;
                pt = parent_pt;
            }
        } else {
            Node* uncle_pt = grand_parent_pt->left;

            if ((uncle_pt != nullptr) && (uncle_pt->color == RED)) {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
                dyecount+=3;
            } else {
                // RR-Case and RL-Case, write your code here
                if (pt == parent_pt->left) {
                    rotateRight(parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }
                rotateLeft(grand_parent_pt);
                std::swap(parent_pt->color, grand_parent_pt->color);
                dyecount+=2;
                pt = parent_pt;
            }
        }
    }

    root->color = BLACK;
    dyecount++;
}

Node* RBTree::BSTInsert(Node* root, Node* pt) {
    // Binary search tree insert logic
    if (root == nullptr)
        return pt;

    if (pt->data < root->data) {
        root->left = BSTInsert(root->left, pt);
        root->left->parent = root;
    } else if (pt->data > root->data) {
        root->right = BSTInsert(root->right, pt);
        root->right->parent = root;
    }

    return root;
}

void RBTree::inorderUtil(Node* root) {
    // Inorder traversal logic
    if (root == nullptr)
        return;
    inorderUtil(root->left);
//    std::cout << root->data << " " << root->color << std::endl;
    switch (root->color) {
        case RED:
            std::cout << root->data << " RED" << std::endl;
            break;
        case BLACK:
            std::cout << root->data << " BLACK" << std::endl;
            break;
        default:
            std::cout << root->data << " Unknown color" << std::endl;
    }

    inorderUtil(root->right);
}

void RBTree::inorder() {
    inorderUtil(root);
}

uint32_t RBTree::size()
{
    return M.size();
}

uint32_t RBTree::height()
{
    return 0;
}

bool RBTree::find(const int data){
    return M.find(data) != M.end();
}


void RBTree::insert(const int data) {
    M[data] = 1;
}

void RBTree::remove(const int data){
    std :: map<uint32_t, bool> :: iterator iter = M.find(data);
    if(iter != M.end()) M.erase(iter);
}
