#include "hftree.h"
#include <iostream>
#include <queue>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <deque>

hfTree::hfTree(const std::string &text, const Option op)
{
    // TODO: Your code here
    std::deque<Node *> forest;
    if (op == Option::SingleChar)
    {
        std::map<char, int> charCount;
        for (char c : text) {
            charCount[c]++;
        }
        for (auto it = charCount.begin(); it != charCount.end(); it++) {
            Node *node = new Node(std::string(1, it->first), it->second, nullptr, nullptr);
            forest.push_back(node);
        }
        int j = forest.size();
        for (int i = 0; i < j - 1; i++) {
            std::sort(forest.begin(), forest.end(), compare);
            ptr = new Node(forest[0]->data + forest[1]->data, forest[0]->frequency + forest[1]->frequency, forest[0], forest[1]);
            forest.pop_front();
            forest.pop_front();
            forest.push_back(ptr);
        }
        ptr = forest.front();
    }
    else if (op == Option::MultiChar)
    {
        std::map<std::string, int> charCount;
        for (int i = 0; i < text.size(); i ++) {
            std::string temp = text.substr(i, 1);
            charCount[temp] = 0;
        }
        std::map<std::string, int> tempCount;
        for (int i = 0; i < text.size() - 1; i ++) {
            std::string temp = text.substr(i, 2);
            tempCount[temp]++;
        }
        std::vector<std::pair<std::string, int>> topCombinations;
        for (auto it = tempCount.begin(); it != tempCount.end(); it++) {
            topCombinations.push_back(*it);
        }
        std::sort(topCombinations.begin(), topCombinations.end(), [](const auto& a, const auto& b) {
            if (a.second == b.second) {
                return a.first < b.first;
            }
            return a.second > b.second;
        });
        for (int i = 0; i < 3; i++) {
            if (i >= topCombinations.size()) break;
            std::string temp = topCombinations[i].first;
            charCount[temp] = 0;
        }
        for (int i = 0; i < text.size(); i ++) {
            std::string currentTwoChars = text.substr(i, 2);
            if (charCount.find(currentTwoChars) != charCount.end()) {
                charCount[currentTwoChars]++;
                i++;
            }
            else {
                std::string currentOneChar = text.substr(i, 1);
                if (charCount.find(currentOneChar) != charCount.end()) {
                    charCount[currentOneChar]++;  // 匹配到单个字符的字符串，计数加一
                }
            }
        }
        auto it = charCount.begin();
        while (it != charCount.end()) {
            if (it->second == 0) {
                it = charCount.erase(it);
            } else {
                ++it;
            }
        }
        for (auto it = charCount.begin(); it != charCount.end(); it++) {
            Node *node = new Node(it->first, it->second, nullptr, nullptr);
            forest.push_back(node);
        }
        int j = forest.size();
        for (int i = 0; i < j - 1; i++) {
            std::sort(forest.begin(), forest.end(), compare);
            ptr = new Node(forest[0]->data + forest[1]->data, forest[0]->frequency + forest[1]->frequency, forest[0], forest[1]);
            forest.pop_front();
            forest.pop_front();
            forest.push_back(ptr);
        }
        ptr = forest.front();
    }
}

std::map<std::string, std::string> hfTree::getCodingTable()
{
    // TODO: Your code here
    if (ptr != nullptr) {
        std::map<std::string, std::string> codingTable;
        std::string code;
        std::queue<std::pair<Node *, std::string>> q;
        q.push(std::make_pair(ptr, ""));
        while (!q.empty()) {
            Node *node = q.front().first;
            code = q.front().second;
            q.pop();
            if (node->left == nullptr && node->right == nullptr) {
                codingTable[node->data] = code;
            }
            if (node->left != nullptr) {
                q.push(std::make_pair(node->left, code + "0"));
            }
            if (node->right != nullptr) {
                q.push(std::make_pair(node->right, code + "1"));
            }
        }
//        std::cout << "codingTable:" << std::endl;
//        for (auto it = codingTable.begin(); it != codingTable.end(); it++)
//        {
//            std::cout << it->first << ": " << it->second << std::endl;
//        }
        return codingTable;
    }
    return std::map<std::string, std::string>();
}

