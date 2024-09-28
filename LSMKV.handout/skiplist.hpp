#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <cstdint>
#include <cassert>
#include <vector>
#include <string>
#include <random>
#include <time.h>
#include <list>
#include <iostream>
#include "SSTable.hpp"
#include "utils.h"

#define VLOGPRE 15
#define MAGIC 0xff

template<typename key_type, typename value_type>
class skiplist
{
private:
	double p;
    uint64_t bloomSize;
	int max_layer, num_kv;
	struct Node
	{
		key_type key;
		value_type value;
		Node *down, *next;
		Node(key_type k, value_type v, Node* d, Node* n):key(k), value(v), down(d), next(n){}
	};
	std::vector<Node*> head;
	std::mt19937_64 rng;
	std::uniform_real_distribution<double> rand_double;
	void write_vlog(Node *p, off_t&, int);
	int getlayer();
	
public:
	explicit skiplist(double, uint64_t);
	~skiplist();
	void put(key_type key, const value_type &val);
	bool del(key_type key);
	std::vector<std::pair<key_type, value_type>> scan(key_type key1, key_type key2) const;
	value_type get(key_type key) const;
	int size();
    int get_numkv() {return num_kv;}
	void print_self();
	SSTable<key_type, value_type>* convertSSTable(int, uint64_t, const std::string&, const std::string&);
};

template<typename key_type, typename value_type>
skiplist<key_type, value_type>::skiplist(double p, uint64_t bloomSize) : num_kv(0), p(p), max_layer(1), rand_double(0, 1), bloomSize(bloomSize)
{
    skiplist :: Node *new_head = new skiplist :: Node(-0x7ffffff, std::string("~None~"), NULL, NULL);
    head.push_back(new_head);
    rng.seed(time(0));
}

template<typename key_type, typename value_type>
int skiplist<key_type, value_type> :: getlayer()
{
    int layer = 1;
    while(rand_double(rng) < p)
    {
	    layer ++;
    }
    return layer;
}

template<typename key_type, typename value_type>
void skiplist<key_type, value_type>::put(key_type key, const value_type &val) 
{
    skiplist :: Node *ptr = head[max_layer - 1], *former[max_layer];
    for(int layer = max_layer; layer; layer --)
    {
        while(ptr -> next && ptr -> next -> key <= key)
            ptr = ptr -> next;
        former[layer - 1] = ptr;
        if(ptr -> key == key)
        {
            while(ptr) ptr -> value = val, ptr = ptr -> down;
            return;
        }
        else if(ptr -> down) ptr = ptr -> down;
    }
    num_kv ++;
    int new_layer = getlayer();
    ptr = NULL;
    for(int layer = 1; layer <= std::min(max_layer, new_layer); layer ++)
    {
	    ptr = former[layer - 1] -> next = new skiplist :: Node(key, val, ptr, former[layer - 1] -> next);
    }
    for(int layer = max_layer + 1; layer <= new_layer; layer ++)
    {
        skiplist :: Node *new_head = new skiplist :: Node(-0x7ffffff, std::string("~None~"), head.back(), NULL);
        ptr = new_head -> next = new skiplist :: Node(key, val, ptr, NULL);
        head.push_back(new_head);
    }
    max_layer = std::max(max_layer, new_layer);	
}

template<typename key_type, typename value_type>
bool skiplist<key_type, value_type>::del(key_type key)
{
    std :: string val = get(key);
    if(val == "~DELETED~" || val == "")
    	return false;
    put(key, std::string("~DELETED~"));
    return true;
}

template<typename key_type, typename value_type>
value_type skiplist<key_type, value_type>::get(key_type key) const 
{
    skiplist :: Node *ptr = head[max_layer - 1];
    for(int layer = max_layer; layer; layer --)
    {
	    while(ptr -> next && ptr -> next -> key <= key)
	        ptr = ptr -> next;
	    if(ptr -> key == key)
	    {
	        return ptr -> value;
	    }
	    else if(ptr -> down) ptr = ptr -> down;
    }
    return std::string("");
}

template<typename key_type, typename value_type>
std::vector<std::pair<key_type, value_type>> skiplist<key_type, value_type>::scan(key_type key1, key_type key2) const
{
    skiplist :: Node *ptr = head[max_layer - 1];
    std::vector<std::pair<key_type, value_type>> result;
    while(true)
    {
        while(ptr -> next && ptr -> next -> key < key1)
            ptr = ptr -> next;
        if(ptr -> down) ptr = ptr -> down;
        else break;
    }
    ptr = ptr -> next;
    while(ptr && ptr -> key >= key1 && ptr -> key <= key2)
    {
        result.push_back(std::make_pair(ptr -> key, ptr -> value));
        ptr = ptr -> next;
    }
    return result;
}

template<typename key_type, typename value_type>
void skiplist<key_type, value_type> :: print_self()
{
    if(num_kv > 5) return;
    std::cout << "max_layer " << max_layer << std::endl;
    for(int layer = max_layer; layer; layer --)
    {
	for(skiplist::Node *ptr = head[layer - 1]; ptr; ptr = ptr -> next)
	    std::cout << '[' << ptr -> key << ", " << ptr -> value << "] ";
	std::cout << std::endl;
    }
}

template<typename key_type, typename value_type>
void skiplist<key_type, value_type> :: write_vlog(Node *p, off_t &offset, int fd)
{
    size_t vlog_len = p -> value.length() + VLOGPRE;
    char buf[vlog_len + 5];
    strcpy(buf + VLOGPRE, p -> value.c_str());
    buf[0] = MAGIC; 
    buf[vlog_len] = 0;
    *(uint16_t*)(buf + 1) = utils::generate_checksum(p -> key, p -> value.length(), p -> value);
    *(uint64_t*)(buf + 3) = p -> key;
    *(uint32_t*)(buf + 11) = (uint32_t)p -> value.length();
    utils :: write_file(fd, vlog_len, buf);
    offset += vlog_len;
}

template<typename key_type, typename value_type>
SSTable<key_type, value_type> *skiplist<key_type, value_type> :: convertSSTable(int id, uint64_t step, const std::string &dir, const std::string &vlog)
{
    off_t offset = (off_t)utils :: get_end_offset(vlog);
    key_type max_k = 0, min_k = 0x7fffffff;
    std::vector<key_type> keys;
    std::vector<uint64_t> offsets, valueLens;
    bloomFilter * bloom_p = new bloomFilter(bloomSize, 3);
    skiplist :: Node *ptr = head[0];
    int fd = open(vlog.c_str(), O_RDWR | O_CREAT | O_APPEND, 0644);
    lseek(fd, offset, SEEK_SET);
    while(ptr -> next)
    {
        skiplist :: Node * p = ptr -> next;
        bloom_p -> insert(p -> key);
        keys.push_back(p -> key);
        offsets.push_back(offset);
    	if(p -> value != "~DELETED~")
    	{
    	    if(p -> key > max_k) max_k = p -> key;
    	    if(p -> key < min_k) min_k = p -> key;
    	    valueLens.push_back(p -> value.length());
            write_vlog(p, offset, fd);
    	}
        else valueLens.push_back(0);
    	ptr = ptr -> next;
    }
    close(fd);
    SSTable<key_type, value_type> *sst = new SSTable<key_type, value_type>({step, num_kv, max_k, min_k}, 0, id, bloom_p, keys, offsets, valueLens, dir, vlog);
    sst -> write_disk();
    return sst;
}

template<typename key_type, typename value_type>
skiplist<key_type, value_type> :: ~ skiplist()
{
    skiplist :: Node * ptr, * now_p;
    for(int layer = max_layer; layer; layer --)
    {
    	skiplist :: Node *ptr = head[layer - 1] -> next;
        while(ptr)
        {
            now_p = ptr;
            ptr = ptr -> next;
            delete now_p;
        }
    }
}

template<typename key_type, typename value_type>
int skiplist<key_type, value_type> :: size()
{
    return num_kv * 20 + bloomSize + 32;
}

#endif
