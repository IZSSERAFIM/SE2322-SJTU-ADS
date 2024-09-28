#include <iostream>
#include <chrono>
#include <random>
#include <unordered_set>
#include "rbtree.h"


int main() {
    RedBlackTree tree;

    tree.insert(7);
    tree.insert(3);
    tree.insert(18);
    tree.insert(10);
    tree.insert(22);
    tree.insert(8);
    tree.insert(11);
    tree.insert(26);

    std::cout << "Inorder traversal of the constructed tree: \n";
    tree.inorder();
    std::cout << std::endl;

//    // 顺序插入实验
//    {
//        for (int i = 1; i <= 10000; ++i) {
//            tree.insert(i);
//        }
//        std::cout << "Sequential insertion:" << std::endl;
//        std::cout << "fixcount: " << tree.fixcount << std::endl;
//        std::cout << "rotatecount: " << tree.rotatecount << std::endl;
//        std::cout << "dyecount: " << tree.dyecount << std::endl;
//    }

//    // 乱序插入实验
//    {
//        std::unordered_set<int> s;
//
//        std::default_random_engine e;
//        std::uniform_int_distribution<int> u(1, 10000);
//
//        for (int i = 1; i <= 10000; ++i) {
//            int x = u(e);
//            while (s.find(x) != s.end()) {
//                x = u(e);
//            }
//            s.insert(x);
//            tree.insert(x);
//        }
//
//        std::cout << "Disorderly insertion:" << std::endl;
//        std::cout << "fixcount: " << tree.fixcount << std::endl;
//        std::cout << "rotatecount: " << tree.rotatecount << std::endl;
//        std::cout << "dyecount: " << tree.dyecount << std::endl;
//
//    }

    return 0;
}