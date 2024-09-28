#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <cstdint>
// #include <optional>
// #include <vector>
#include <string>
#include <cstdint>
#include <list>
#include <random>

namespace skiplist
{
    using key_type = uint64_t;
    // using value_type = std::vector<char>;
    using value_type = std::string;
    const int MAX_LEVEL = 8;
    class skiplist_type
    {
        // add something here
    private:
        struct Node
        {
            key_type key;
            value_type val;
            int level;
            Node *forward[MAX_LEVEL];
            Node(key_type key, const value_type &s, int level);
        } *head, *tail;
        int random_level();
        double my_rand();
        double Jump_Probaility;

    public:
        explicit skiplist_type(double p = 0.5);
        void put(key_type key, const value_type &val);
        // std::optional<value_type> get(key_type key) const;
        std::string get(key_type key) const;

        // for hw1 only
        int query_distance(key_type key) const;
        ~skiplist_type();
    };

} // namespace skiplist

#endif // SKIPLIST_H
