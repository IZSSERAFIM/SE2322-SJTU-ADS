#include <algorithm>
#include <cstdint>
#include "tree.hpp"
class RadixTree : public Tree
{
    // To Be Implemented
private:
    struct Node
    {
        Node *children[4];
        bool isLeaf;
        Node()
        {
            children[0] = nullptr;
            children[1] = nullptr;
            children[2] = nullptr;
            children[3] = nullptr;
            isLeaf = true;
        }
    };
    Node *root;
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
    uint32_t height(Node* node)
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
public:
    RadixTree();
    ~RadixTree();
    // basic operation
    void insert(int32_t value);
    void remove(int32_t value);
    bool find(int32_t value);
    // statistics
    uint32_t size();
    uint32_t height();
};

