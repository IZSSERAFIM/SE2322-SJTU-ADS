#include "skiplist.h"
#include <optional>
#include <ctime>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <climits>
#include <cassert>
#include <map>
#include <list>
namespace skiplist
{
    skiplist_type::Node::Node(key_type key, const value_type &val, int level) : key(key), val(val), level(level)
    {
        for (int i = 0; i < level; ++i)
            forward[i] = nullptr;
    }
    double skiplist_type::my_rand()
    {
        return rand() * 1.0 / INT_MAX;
    }
    int skiplist_type::random_level()
    {
        int level = 1;
        while (level < MAX_LEVEL && this->my_rand() <= Jump_Probaility)
        {
            level++;
        }
        return level;
    }

    skiplist_type::skiplist_type(double p)
    {
        Jump_Probaility = p;
        head = new Node(0, "", MAX_LEVEL);
        tail = new Node(0, "", MAX_LEVEL);
        for (int i = 0; i < MAX_LEVEL; ++i)
        {
            head->forward[i] = tail;
            tail->forward[i] = tail;
        }
    }
    skiplist_type::~skiplist_type()
    {
        Node *cur = head;
        while (cur != tail)
        {
            Node *tmp = cur;
            cur = cur->forward[0];
            delete tmp;
        }
        delete tail;
    }
    void skiplist_type::put(key_type key, const value_type &val)
    {
        Node *cur = head;
        Node *updated[MAX_LEVEL];
        for (int i = MAX_LEVEL - 1; i >= 0; --i)
        {
            while (cur->forward[i] != tail && cur->forward[i]->key < key)
                cur = cur->forward[i];
            updated[i] = cur;
        }
        // cur = cur->forward[0];
        // if (cur != NULL && cur->key != key)
        // {
        //     int rlevel = random_level();
        //     Node *n = new Node(key, val, rlevel);
        //     for (int i = 0; i < rlevel; ++i)
        //     {
        //         n->forward[i] = updated[i]->forward[i];
        //         updated[i]->forward[i] = n;
        //     }
        // }
        if (cur->forward[0] != tail && cur->forward[0]->key == key)
        {
            cur->forward[0]->val = val;
            return;
        }
        int rlevel = random_level();
        Node *n = new Node(key, val, rlevel);
        for (int i = 0; i < rlevel; ++i)
            n->forward[i] = tail;
        for (int i = rlevel; i >= 0; --i)
        {
            n->forward[i] = updated[i]->forward[i];
            updated[i]->forward[i] = n;
        }
    }
    std::string skiplist_type::get(key_type key) const
    {
        Node *cur = head;
        // for (int i = MAX_LEVEL - 1; i >= 0; --i)
        //     if (cur->forward[i] != NULL && cur->forward[i]->key == key)
        //         return cur->forward[0]->val;
        //     else{
        //     while (cur->forward[i] != NULL && cur->forward[i]->key < key)
        //         cur = cur->forward[i];
        //     }
        // if (cur->forward[0] != NULL && cur->forward[0]->key == key)
        //     return cur->forward[0]->val;
        // return "";
        for (int i = MAX_LEVEL - 1; i >= 0; --i){
            while (cur->forward[i] != tail && cur->forward[i]->key < key)
                cur = cur->forward[i];
            if (cur->forward[i] != tail && cur->forward[i]->key == key)
                return cur->forward[i]->val;
            }
        return "";
    }
    int skiplist_type::query_distance(key_type key) const
    {
        Node *cur = head;
        int path_length = 1;
        // for (int i = MAX_LEVEL - 1; i >= 0; --i)
        // {
        //     if (cur->forward[i] != NULL && cur->forward[i]->key == key)
        //         return path_length;
        //     else
        //     {
        //         while (cur->forward[i] != NULL && cur->forward[i]->key < key)
        //         {
        //             cur = cur->forward[i];
        //             path_length++;
        //         }
        //     }
        //     path_length++;
        // }

        // if (cur->forward[0] != NULL && cur->forward[0]->key == key)
        //     return path_length;
        // return 0;
        for (int i = MAX_LEVEL - 1; i >= 0; --i){
            while (cur->forward[i] != tail && cur->forward[i]->key < key)
            {
                cur = cur->forward[i];
                path_length++;
            }
            if (cur->forward[i] != tail && cur->forward[i]->key == key){
                return path_length++;
            }
            path_length++;
        }
        return path_length++;
    }

} // namespace skiplist
