#include<iostream>
//#include <ctime>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>

using namespace std;

template<typename T>
class node {
public:
    T key;
    int height;
    node *left;
    node *right;

    node(T k) {
        height = 1;
        key = k;
        left = NULL;
        right = NULL;
    }
};

template<typename T>
class AVL {
public:
    node<T> *root = NULL;
    int num;

    void insert(T x) {
        root = insertUtil(root, x);
    }

    void remove(T x) {
        root = removeUtil(root, x);
    }

    node<T> *search(T x) {
        return searchUtil(root, x);
    }

    void inorder() {
        inorderUtil(root);
        cout << endl;
    }

    int getHeight() {
        return height(root);
    }


private:
    int height(node<T> *head) {
        if (head == NULL) return 0;
        return head->height;
    }

    node<T> *rightRotation(node<T> *head) {
        node<T> *newhead = head->left;
        head->left = newhead->right;
        newhead->right = head;
        head->height = 1 + max(height(head->left), height(head->right));
        newhead->height = 1 + max(height(newhead->left), height(newhead->right));
        return newhead;
    }

    node<T> *leftRotation(node<T> *head) {
        node<T> *newhead = head->right;
        head->right = newhead->left;
        newhead->left = head;
        head->height = 1 + max(height(head->left), height(head->right));
        newhead->height = 1 + max(height(newhead->left), height(newhead->right));
        return newhead;
    }

    // Traverse and print AVL tree
    void inorderUtil(node<T> *head) {
        if (head == NULL) return;
        inorderUtil(head->left);
        cout << head->key << " ";
        inorderUtil(head->right);
    }

    node<T> *insertUtil(node<T> *head, T x) {
        // TODO: finish insertUtil
        if (head == NULL) head = new node<T>(x);
        else if (searchUtil(root, x) != NULL) return (searchUtil(root, x));
        else {
            if (x < head->key) head->left = insertUtil(head->left, x);
            else head->right = insertUtil(head->right, x);
        }
        return balanceTree(head);
    }

    node<T> *removeUtil(node<T> *head, T x) {
        // TODO: finish removeUtil
        if (searchUtil(head, x) == NULL) return NULL;
        if (head == NULL) return NULL;
        if (x < head->key) head->left = removeUtil(head->left, x);
        else if (x > head->key) head->right = removeUtil(head->right, x);
        else {
            if (head->left == NULL || head->right == NULL) {
                head = (head->left == NULL) ? head->right : head->left;
            } else {
                node<T> *temp = head->right;
                while (temp->left != NULL) temp = temp->left;
                head->key = temp->key;
                head->right = removeUtil(head->right, temp->key);
            }
        }
        return balanceTree(head);
    }

    node<T> *searchUtil(node<T> *head, T x) {
        if (head == NULL) return NULL;
        T k = head->key;
        if (k == x) return head;
        if (k > x) return searchUtil(head->left, x);
        if (k < x) return searchUtil(head->right, x);
    }

    node<T> *balanceTree(node<T> *head) {
        if (head == NULL) return NULL;
        int balance = height(head->left) - height(head->right);
        if (balance > 1) {
            if (height(head->left->left) >= height(head->left->right)) {
                head = rightRotation(head);
            } else {
                head->left = leftRotation(head->left);
                head = rightRotation(head);
            }
        } else if (balance < -1) {
            if (height(head->right->right) >= height(head->right->left)) {
                head = leftRotation(head);
            } else {
                head->right = rightRotation(head->right);
                head = leftRotation(head);
            }
        }
        head->height = 1 + max(height(head->left), height(head->right));
        return head;
    }
};


template<typename T>
class BST {
    // TODO: finish BST according to AVL
public:
    node<T> *root = NULL;
    int num;

    void insert(T x) {
        root = insertUtil(root, x);
    }

    void remove(T x) {
        root = removeUtil(root, x);
    }

    node<T> *search(T x) {
        return searchUtil(root, x);
    }

    void inorder() {
        inorderUtil(root);
        cout << endl;
    }

    int getHeight() {
        return height(root);
    }


private:
    int height(node<T> *head) {
        if (head == NULL) return 0;
        return head->height;
    }

    int updateHeight(node<T> *head) {
        if (head == NULL) return 0;
        else {
            int leftHeight = updateHeight(head->left);
            int rightHeight = updateHeight(head->right);
            head->height = 1 + max(leftHeight, rightHeight);
            return head->height;
        }
    }

    void inorderUtil(node<T> *head) {
        if (head == NULL) return;
        inorderUtil(head->left);
        cout << head->key << " ";
        inorderUtil(head->right);
    }

    node<T> *insertUtil(node<T> *head, T x) {
        if (head == NULL) head = new node<T>(x);
        else if (searchUtil(root, x) != NULL) return (searchUtil(root, x));
        else {
            if (x < head->key) head->left = insertUtil(head->left, x);
            else head->right = insertUtil(head->right, x);
        }
        updateHeight(head);
        return head;
    }

    node<T> *searchUtil(node<T> *head, T x) {
        if (head == NULL) return NULL;
        T k = head->key;
        if (k == x) return head;
        if (k > x) return searchUtil(head->left, x);
        if (k < x) return searchUtil(head->right, x);
    }

    node<T> *removeUtil(node<T> *head, T x) {
        if (searchUtil(head, x) == NULL) return NULL;
        if (head == NULL) return NULL;
        if (head->key == x) {
            if (head->left == NULL || head->right == NULL) {
                head = (head->left == NULL) ? head->right : head->left;
                return head;
            } else {
                node<T> *pre = head->left;
                while (pre->right != NULL) pre = pre->right;
                pre->right = head->right;
                return head->left;
            }
        } else if (x < head->key) {
            head->left = removeUtil(head->left, x);
            return head;
        } else {
            head->right = removeUtil(head->right, x);
            return head;
        }
    }
};

void test(AVL<int> avl, BST<int> bst) {
    cout << "AVL Tree Function Test:" << endl;
    avl.insert(47);
    avl.insert(83);
    avl.insert(13);
    avl.insert(92);
    avl.insert(5);
    avl.insert(71);
    avl.insert(34);
    avl.insert(61);
    avl.insert(95);
    avl.insert(28);
    avl.insert(76);
    avl.insert(40);
    avl.insert(10);
    avl.insert(89);
    avl.insert(54);
    avl.inorder();
    if (avl.search(10) != NULL) cout << "10 is in the AVL tree" << endl;
    else cout << "10 is not in the AVL tree" << endl;
    avl.remove(10);
    cout << "After removing 10: " << endl;
    avl.inorder();
    if (avl.search(10) != NULL) cout << "10 is in the AVL tree" << endl;
    else cout << "10 is not in the AVL tree" << endl;
    avl.insert(10);
    cout << "----------------------------------------------------" << endl;
    cout << "BST Tree Function Test:" << endl;
    bst.insert(47);
    bst.insert(83);
    bst.insert(13);
    bst.insert(92);
    bst.insert(5);
    bst.insert(71);
    bst.insert(34);
    bst.insert(61);
    bst.insert(95);
    bst.insert(28);
    bst.insert(76);
    bst.insert(40);
    bst.insert(10);
    bst.insert(89);
    bst.insert(54);
    bst.inorder();
    if (bst.search(10) != NULL) cout << "10 is in the BST tree" << endl;
    else cout << "10 is not in the BST tree" << endl;
    bst.remove(10);
    cout << "After removing 10: " << endl;
    bst.inorder();
    if (bst.search(10) != NULL) cout << "10 is in the BST tree" << endl;
    else cout << "10 is not in the BST tree" << endl;
    bst.insert(10);
    cout << "----------------------------------------------------" << endl;
}

void compare(AVL<int> avl, BST<int> bst) {
    srand(time(0));
    for (int t = 1; t <= 5; t++) {
        int size = 100 * pow(10, t);
        //    int size = 10000000;
        int *sequence = new int[size];
        for (int i = 0; i < size; i++) {
            sequence[i] = rand() % 1000;
        }
        clock_t avlStartBTime = clock();
        for (int i = 0; i < size; i++) {
            avl.insert(sequence[i]);
        }
        clock_t avlEndBTime = clock();

        clock_t bstStartBTime = clock();
        for (int i = 0; i < size; i++) {
            bst.insert(sequence[i]);
        }
        clock_t bstEndBTime = clock();

        double avlBTime = double(avlEndBTime - avlStartBTime) / CLOCKS_PER_SEC;
        double bstBTime = double(bstEndBTime - bstStartBTime) / CLOCKS_PER_SEC;
        int avlHeight = avl.getHeight();
        int bstHeight = bst.getHeight();
        cout << "Sequence size: " << size << endl;
        cout << "AVL Tree height: " << avlHeight << endl;
        cout << "BST height: " << bstHeight << endl;
        cout << "AVL Tree build time: " << fixed <<setprecision(9) << avlBTime << " seconds" << endl;
        cout << "BST build time: " << fixed <<setprecision(9) << bstBTime << " seconds" << endl;

        clock_t avlStartSTime = clock();
        for (int j = 0; j < size; j++) {
            avl.search(sequence[j]);
        }
        clock_t avlEndSTime = clock();

        clock_t bstStartSTime = clock();
        for (int j = 0; j < size; j++) {
            bst.search(sequence[j]);
        }
        clock_t bstEndSTime = clock();

        double avlSTime = double(avlEndSTime - avlStartSTime) / CLOCKS_PER_SEC;
        double bstSTime = double(bstEndSTime - bstStartSTime) / CLOCKS_PER_SEC;
        cout << "AVL Tree search time: " << fixed <<setprecision(9) << avlSTime << " seconds" << endl;
        cout << "BST search time: " << fixed <<setprecision(9) << bstSTime << " seconds" << endl;
        cout << "----------------------------------------------------" << endl;

        delete[] sequence;
    }

}

int main() {
    AVL<int> avl;
    BST<int> bst;

    // Design your program
    test(avl, bst);
    compare(avl, bst);
}

