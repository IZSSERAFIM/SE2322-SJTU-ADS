#pragma once

#include "kvstore_api.h"
#include "skiplist.hpp"

#define SSTABLESIZE (1 << 14)
using key_type = uint64_t;
using value_type = std::string;

class KVStore : public KVStoreAPI
{
	// You can add your implementation here
private:
	uint64_t step, head, tail, bloomSize;
	int test_mode;
	skiplist<key_type, value_type> *memTable;
	std::vector<std::vector<SSTable<key_type, value_type>*>> layers;
	std::string dir_path, vlog_path;

	void compaction(int level);
public:
	KVStore(const std::string &dir, const std::string &vlog);

	KVStore(uint64_t, const std::string &dir, const std::string &vlog);

	~KVStore();

	void put(uint64_t key, const std::string &s) override;

	std::string get(uint64_t key) override;

	bool del(uint64_t key) override;

	void reset() override;

	void scan(uint64_t key1, uint64_t key2, std::list<std::pair<uint64_t, std::string>> &list) override;

	void gc(uint64_t chunk_size) override;

	void test(int test_mode);
};
