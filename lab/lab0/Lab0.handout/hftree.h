#ifndef HFTREE_H
#define HFTREE_H

#include <string>
#include <map>

class hfTree
{

private:
    // TODO: Your code here
    struct Node
    {
        std::string data;
        int frequency;
        Node *left;
        Node *right;
        Node(std::string data, int frequency, Node *left, Node *right): data(data), frequency(frequency), left(left), right(right) {}
        std::string getMinData() const {
            if (this->left == nullptr && this->right == nullptr) {
                return data;  // 如果是叶子节点，直接返回节点的数据
            }
            std::string minData = this->data;  // 初始化最小字典序为当前节点的数据
            if (this->left != nullptr) {
                std::string leftMinData = this->left->getMinData();  // 递归获取左子树的最小字典序
                if (leftMinData < minData) {
                    minData = leftMinData;  // 更新最小字典序
                }
            }
            if (this->right != nullptr) {
                std::string rightMinData = this->right->getMinData();  // 递归获取右子树的最小字典序
                if (rightMinData < minData) {
                    minData = rightMinData;  // 更新最小字典序
                }
            }
            return minData;
        }
    };

    static bool compare(const Node* a, const Node* b) {
        if (a->frequency == b->frequency) {
            std::string minDataA = a->getMinData();  // 获取树a中所有节点的最小字典序
            std::string minDataB = b->getMinData();  // 获取树b中所有节点的最小字典序
            return minDataA < minDataB;  // 比较最小字典序
        }
        return a->frequency < b->frequency;
    }

    Node *ptr;

public:
    enum class Option
    {
        SingleChar,
        MultiChar
    };
    hfTree(const std::string &text, const Option op);
    std::map<std::string, std::string> getCodingTable();
};

#endif