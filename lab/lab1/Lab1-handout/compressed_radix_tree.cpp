#include "compressed_radix_tree.hpp"

CompressedRadixTree::CompressedRadixTree()
{
    // To Be Implemented
    root = new Node();
    root->isLeaf = true;
}

CompressedRadixTree::~CompressedRadixTree()
{
    // To Be Implemented
    deleteNode(root);
}

void CompressedRadixTree::insert(int32_t value)
{
    // To Be Implemented
    if (find(value))
        return;
    Node * current = root;
    int bitIndex = 30;
    while(bitIndex >= 0)
    {
        if(current -> children[intercept(value, bitIndex, bitIndex + 2)])
            current = current -> children[intercept(value, bitIndex, bitIndex + 2)];
        else
        {
            int32_t newKey = intercept(value, 0, bitIndex + 2);
            int newLen = bitIndex + 2;
            Node * new_ptr = new Node();
            new_ptr -> key = newKey;
            new_ptr -> len = newLen;
            current = current -> children[intercept(value, bitIndex, bitIndex + 2)] = new_ptr;
        }
        if(intercept(value, bitIndex + 2 - current -> len, bitIndex + 2) == current -> key)
            bitIndex -= current -> len;
        else
        {
            int32_t diff = intercept(value, bitIndex + 2 - current -> len, bitIndex + 2) ^ current -> key;
            int diff_len = getHighestbit(diff);
            int same_len = current -> len - diff_len;
            current = split(current, diff_len, same_len);
            bitIndex -= same_len;
        }
    }
    current -> isLeaf = true;
}

void CompressedRadixTree::remove(int32_t value)
{
    // To Be Implemented
    if(!find(value))
        return;
    Node * current = root;
    int bitIndex = 30;
    std :: stack<std :: pair<Node *, int>> S;
    while(bitIndex >= 0)
    {
        current = current -> children[intercept(value, bitIndex, bitIndex + 2)];
        S.push(std::make_pair(current, intercept(value, bitIndex, bitIndex + 2)));
        if(intercept(value, bitIndex + 2 - current -> len, bitIndex + 2) != current -> key)
            return;
        bitIndex -= current -> len;
    }
    if(current -> isLeaf)
    {
        current -> isLeaf = false;
        while((!S.empty() && noChild(S.top().first)))
        {
            Node *top = S.top().first;
            int ch_id = S.top().second;
            S.pop();
            if(!S.empty()) S.top().first -> children[ch_id] = NULL;
            else root -> children[ch_id] = NULL;
            delete top;
        }
        if(!S.empty())
        {
            int num_ch = 0, ch_id;
            current = S.top().first;
            for(int i = 0; i < 4; i ++)
                if(current -> children[i])
                    num_ch ++, ch_id = i;
            if(num_ch == 1)
                merge(current, ch_id);
        }
    }
}

bool CompressedRadixTree::find(int32_t value)
{
    // To Be Implemented
    Node * current = root;
    int bitIndex = 30;
    while(bitIndex >= 0)
    {
        if(!current -> children[intercept(value, bitIndex, bitIndex + 2)])
            return false;
        current = current -> children[intercept(value, bitIndex, bitIndex + 2)];
        if(intercept(value, bitIndex + 2 - current -> len, bitIndex + 2) != current -> key)
            return false;
        bitIndex -= current -> len;
    }
    return current -> isLeaf;
}

uint32_t CompressedRadixTree::size()
{
    // To Be Implemented
    return size(root);
}

uint32_t CompressedRadixTree::height()
{
    // To Be Implemented
    return height(root);
}
