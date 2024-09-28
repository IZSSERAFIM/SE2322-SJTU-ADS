#include "radix_tree.hpp"

RadixTree::RadixTree()
{
    // To Be Implemented
    root = new Node();
    root->isLeaf = false;
}

RadixTree::~RadixTree()
{
    // To Be Implemented
    deleteNode(root);
}

void RadixTree::insert(int32_t value)
{
    // To Be Implemented
    Node* current = root;
    for (int i = 30; i >= 0; i -= 2) {
        int index = (value >> i) & 0x3;  // 获取两个比特
        if (current->children[index] == nullptr) {
            current->children[index] = new Node();
        } else {
            current->isLeaf = false;  // 设置父节点为非叶子节点
        }
        current = current->children[index];
    }
    current->isLeaf = true;  // 设置新插入的节点为叶子节点
}

void RadixTree::remove(int32_t value)
{
    // To Be Implemented
    Node* current = root;
    Node* nodes[16];  // 存储路径上的节点
    int indices[16];  // 存储路径上的索引
    int depth = 0;

    for (int i = 30; i >= 0; i -= 2) {
        int index = (value >> i) & 0x3;  // 获取两个比特
        if (current->children[index] == nullptr) {
            return;  // 整数不在树中
        }
        nodes[depth] = current;
        indices[depth] = index;
        depth++;
        current = current->children[index];
    }

    if (!current->isLeaf) {
        return;  // 整数不在树中
    }

    // 删除路径上的节点
    for (int i = depth - 1; i >= 0; --i) {
        delete nodes[i]->children[indices[i]];
        nodes[i]->children[indices[i]] = nullptr;

        // 检查是否需要删除父节点
        bool allNull = true;
        for (int j = 0; j < 4; ++j) {
            if (nodes[i]->children[j] != nullptr) {
                allNull = false;
                break;
            }
        }
        if (!allNull) {
            break;
        }
    }
}

bool RadixTree::find(int32_t value)
{
    // To Be Implemented
    Node* current = root;
    for (int i = 30; i >= 0; i -= 2) {
        int index = (value >> i) & 0x3;  // 获取两个比特
        if (current->children[index] == nullptr) {
            return false;  // 整数不在树中
        }
        current = current->children[index];
    }
    return current->isLeaf;
}

uint32_t RadixTree::size()
{
    // To Be Implemented
    return size(root);
}

uint32_t RadixTree::height()
{
    // To Be Implemented
    return height(root);
}
