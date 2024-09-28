// implement a radix tree that support node compressino and store int32_t values
// each parent node has 4 children, representing 2 bits
#include <cstdint>
#include <algorithm>
#include <queue>
#include <stack>
#include <map>
#include <iostream>
#include "tree.hpp"
class CompressedRadixTree : public Tree
{
    // To Be Implemented
private:
    struct Node
    {
        Node *children[4];
        int32_t key;
        int len;
        bool isLeaf;
        Node()
        {
            children[0] = nullptr;
            children[1] = nullptr;
            children[2] = nullptr;
            children[3] = nullptr;
            key = 0;
            len = 0;
            isLeaf = true;
        }
    };
    Node * root;
    void deleteNode(Node* node)
    {
        if (node == nullptr) {
            return;
        }
        for (int i = 0; i < 4; ++i) {
            deleteNode(node->children[i]);
        }
        delete node;
    }
    int getHighestbit(int32_t value)
    {
        int l = 0, r = 32, m;
        while(l < r)
        {
            m = (l + r) / 2;
            if((value >> m) != 0)
                l = m + 1;
            else
                r = m;
        }
        return r & 0x1 ? (r + 1) : r;
    }
    int32_t intercept(int32_t value, int low, int up) // get bits from low to up
    {
        return ((((uint64_t)1 << up) - 1) & value) >> low;
    }
    Node* split(CompressedRadixTree::Node *current, int diff_len, int same_len)
    {
        Node * newN = new Node();
        for (int i = 0; i < 4; i++)
            newN -> children[i] = current -> children[i];
        newN -> key = intercept(current -> key, 0, diff_len);
        newN -> len = diff_len;
        newN -> isLeaf = current -> isLeaf;
        Node * new_ptr = newN;
        current -> children[0] = current -> children[1] = current -> children[2] = current -> children[3] = NULL;
        current -> children[intercept(current -> key, diff_len - 2, diff_len)] = new_ptr;
        current -> key = intercept(current -> key, diff_len, current -> len);
        current -> len = same_len;
        current -> isLeaf = false;
        return current;
    }
    Node * merge(CompressedRadixTree::Node *current, int ch_id)
    {
        Node * ch_ptr = current -> children[ch_id];
        for(int i = 0; i < 4; i ++)
            current -> children[i] = ch_ptr -> children[i];
        current -> len += ch_ptr -> len;
        current -> key = (current -> key << ch_ptr -> len) + ch_ptr -> key;
        current -> isLeaf = ch_ptr -> isLeaf;
        delete ch_ptr;
        return current;
    }
    uint32_t size(Node* node)
    {
        if (node == nullptr) {
            return 0;
        }
        uint32_t count = 1;  // 计算当前节点
        for (int i = 0; i < 4; ++i) {
            count += size(node->children[i]);  // 递归计算子节点
        }
        return count;
    }
    uint32_t height(Node *node)
    {
        if (node == nullptr) {
            return 0;
        }
        uint32_t maxDepth = 0;
        for (int i = 0; i < 4; ++i) {
            maxDepth = std::max(maxDepth, height(node->children[i]));
        }
        return maxDepth + 1;  // root node has depth 1
    }
    bool noChild(Node *current)
    {
        for(int i = 0; i < 4; i ++)
            if(current -> children[i])
                return false;
        return true;
    }

public:
    CompressedRadixTree();
    ~CompressedRadixTree();
    // basic operation
    void insert(int32_t value);
    void remove(int32_t value);
    bool find(int32_t value);
    // statistics
    uint32_t size();
    uint32_t height();
};
