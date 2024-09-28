#ifndef SSTABLE_H
#define SSTABLE_H
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>
#include "bloomFilter.hpp"
#include "utils.h"

#define VLOGPRE 15
template<typename key_type, typename value_type>
class skiplist; 

typedef struct
{
    uint64_t step, num_kv, max_k, min_k;
}head_type;

template <typename key_type, typename value_type>
class SSTable
{
private:
    head_type head;
	int level, id;
    bloomFilter *bloomfilter;
    std::string dir_path, vlog_path;
    std::vector <key_type> keys;
    std::vector <uint64_t> offsets;
    std::vector <uint64_t> valueLens;
	void write_sst() const;
public:
    SSTable(head_type, int, int, bloomFilter *, std::vector<key_type>, std::vector<uint64_t>, std::vector<uint64_t>, std::string dir_path, std::string vlog_path);
    SSTable(int level, int id, std::string, std::string, std::string, uint64_t);
    ~SSTable();

    value_type get(key_type key) const;
	value_type get_fromdisk(key_type key) const;
	uint64_t get_off(key_type key) const;
    std::vector<std::pair<key_type, value_type>> scan(key_type key1, key_type key2);
    bool mayCount(key_type);
	void write_disk() const;
	void delete_disk() const;
	void set_id(int new_id);

	uint64_t get_numkv() const {return head.num_kv;}
    uint64_t getStep() const {return head.step;}
	uint64_t get_bloomSize() const {return bloomfilter -> get_m();}
	key_type get_maxk() const {return head.max_k;}
	key_type get_mink() const {return head.min_k;}
	std::vector <key_type> get_keys() const {return keys;}
	std::vector <uint64_t> get_offsets() const {return offsets;}
	std::vector <uint64_t> get_valueLens() const {return valueLens;}
};

template <typename key_type, typename value_type>
SSTable<key_type, value_type> :: SSTable(head_type head, int level, int id, bloomFilter *bloomFilter, std::vector<key_type> keys, std::vector<uint64_t> offsets, std::vector<uint64_t> valueLens, std::string dir_path, std::string vlog_path) 
	: head(head), level(level), id(id), bloomfilter(bloomFilter), keys(keys), offsets(offsets), valueLens(valueLens), dir_path(dir_path), vlog_path(vlog_path)
{
    
}

template <typename key_type, typename value_type>
SSTable<key_type, value_type> :: SSTable(int level, int id, std::string sst, std::string dir_path, std::string vlog_path, uint64_t bloomSize)
 : level(level), id(id), dir_path(dir_path), vlog_path(vlog_path)
{
	sst = dir_path + "/" + sst;
	char buf[64] = {0};
	int fd = open(sst.c_str(), O_RDWR, 0644);
	utils::read_file(fd, -1, 32, buf);
	head.step = *(uint64_t*)buf;
	head.num_kv = *(uint64_t*)(buf + 8);
	head.min_k = *(uint64_t*)(buf + 16);
	head.max_k = *(uint64_t*)(buf + 24);
	bloomfilter = new bloomFilter(bloomSize, 3);
	utils::read_file(fd, -1, get_bloomSize(), bloomfilter -> getArray());
	key_type key;
	uint64_t offset, valueLen;
	for(int i = 0; i < head.num_kv; i ++)
	{
		utils :: read_file(fd, -1, 20, buf);
		key = *(uint64_t*)buf;
		offset = *(uint64_t*)(buf + 8);
		valueLen = *(uint32_t*)(buf + 16);
		keys.push_back(key);
		offsets.push_back(offset);
		valueLens.push_back(valueLen);
	}
	close(fd);
}

template <class key_type, class value_type>
SSTable<key_type, value_type> :: ~SSTable()
{
    delete bloomfilter;
}

template <typename key_type, typename value_type>
value_type SSTable<key_type, value_type> :: get(key_type key) const
{
	typename std::vector<key_type>::const_iterator iter = lower_bound(keys.begin(), keys.end(), key);
    if(iter != keys.end() && *iter == key)
	{
		int idx = int(iter - keys.begin());
		off_t offset = offsets[idx];
		size_t size = valueLens[idx];
		if(size)
		{
			char buf[VLOGPRE + size + 5] = {0};
			utils::read_file(vlog_path, offset, size + VLOGPRE, buf);
			return std::string(buf + VLOGPRE);
		}
		else return std::string("~DELETED~");
	}
	//printf("get%ld\n", (long long)key);
    return std::string("");
}

template <typename key_type, typename value_type>
value_type SSTable<key_type, value_type> :: get_fromdisk(key_type key) const
{
	std::string sst = dir_path + "/" + std::to_string(level) + "-" + std::to_string(id) + ".sst";
	char buf[64] = {0};
	//if(!utils::fileExists(sst)) write_disk();
	int fd = open(sst.c_str(), O_RDWR, 0644);
	utils::read_file(fd, 0, 32, buf);
	uint64_t num_kv = *(uint64_t*)(buf + 8);

	lseek(fd, get_bloomSize() + 32, SEEK_SET);
	key_type now_key;
	uint64_t offset, valueLen;
	for(int i = 0; i < head.num_kv; i ++)
	{
		utils :: read_file(fd, -1, 20, buf);
		now_key = *(uint64_t*)buf;
		offset = *(uint64_t*)(buf + 8);
		valueLen = *(uint32_t*)(buf + 16);
		if(now_key == key)
		{
			close(fd);
			if(valueLen)
			{
				char buf[VLOGPRE + valueLen + 5] = {0};
				utils::read_file(vlog_path, offset, valueLen + VLOGPRE, buf);
				return std::string(buf + VLOGPRE);
			}
			else return std::string("~DELETED~");
		} 
	}
	close(fd);
    return std::string("");
}

template <typename key_type, typename value_type>
uint64_t SSTable<key_type, value_type> :: get_off(key_type key) const
{
	typename std::vector<key_type>::const_iterator iter = lower_bound(keys.begin(), keys.end(), key);
    if(iter != keys.end() && *iter == key)
	{
		int idx = int(iter - keys.begin());
		if(!valueLens[idx]) return 2; 
		else return offsets[idx];
	}
    return 1;
}

template <typename key_type, typename value_type>
std::vector<std::pair<key_type, value_type>> SSTable<key_type, value_type> :: scan(key_type key1, key_type key2)
{
	std::vector<std::pair<key_type, value_type>> list;
	if(key1 > key2) return list;
	typename std::vector<key_type>::iterator iter1 = lower_bound(keys.begin(), keys.end(), key1);
	typename std::vector<key_type>::iterator iter2 = lower_bound(keys.begin(), keys.end(), key2);
	if(iter2 != keys.end() && *iter2 == key2) iter2 ++;
	int idx1 = int(iter1 - keys.begin()), idx2 = int(iter2 - keys.begin());
    int fd = open(vlog_path.c_str(), O_RDWR, 0644);
	for(int i = idx1; i < idx2; i ++)
	{
		off_t offset = offsets[i];
		size_t size = valueLens[i];
		if(size)
		{
			char buf[VLOGPRE + size + 5] = {0};
			utils::read_file(fd, offset, size + VLOGPRE, buf);
			list.push_back(std::make_pair(keys[i], std::string(buf + VLOGPRE)));
		}
		else
			list.push_back(std::make_pair(keys[i], std::string("~DELETED~")));
	}
	close(fd);
	return list;
}

template <typename key_type, typename value_type>
bool SSTable<key_type, value_type> :: mayCount(key_type key)
{
	return bloomfilter -> count(key);
}

template <typename key_type, typename value_type>
void SSTable<key_type, value_type> :: write_disk() const
{
	write_sst();	
}

template <typename key_type, typename value_type>
void SSTable<key_type, value_type> :: delete_disk() const
{
	std::string sst = dir_path + "/" + std::to_string(level) + "-" + std::to_string(id) + ".sst";
	assert(utils::fileExists(sst));
	utils :: rmfile(sst);
}

template <typename key_type, typename value_type>
void SSTable<key_type, value_type>::set_id(int new_id)
{
	std::string old_sst = dir_path + "/" + std::to_string(level) + "-" + std::to_string(id) + ".sst";
	std::string new_sst = dir_path + "/" + std::to_string(level) + "-" + std::to_string(new_id) + ".sst";
	id = new_id;
	assert(utils::fileExists(old_sst));
	std::rename(old_sst.c_str(), new_sst.c_str());
}

template<typename key_type, typename value_type>
void SSTable<key_type, value_type> :: write_sst() const
{
    std::string sst = dir_path + std::string("/") + std::to_string(level) + std::string("-") + std::to_string(id) + std::string(".sst");
    //printf("%s\n", sst.c_str());
	char buffer[33] = {0};
	*(uint64_t*)buffer = head.step;
	*(uint64_t*)(buffer + 8) = head.num_kv;
	*(uint64_t*)(buffer + 16) = head.min_k;
	*(uint64_t*)(buffer + 24) = head.max_k;
	utils :: write_file(sst, -1, 32, buffer);
	utils :: write_file(sst, -1, bloomfilter -> get_m(), bloomfilter -> getArray());
	for(int i = 0; i < keys.size(); i ++)
	{
		*(uint64_t*)buffer = keys[i];
		*(uint64_t*)(buffer + 8) = offsets[i];
		*(uint32_t*)(buffer + 16) = valueLens[i];
		utils :: write_file(sst, -1, 20, buffer);
	}
}

#endif