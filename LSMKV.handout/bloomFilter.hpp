#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H
#include "MurmurHash3.h"

class bloomFilter
{
//friend class diskLevel;
private:
	int m, k;
	bool *a;
public:
	bloomFilter(int m, int k);
	~bloomFilter();
	bool count(const uint64_t x);
	void insert(const uint64_t x);
	bool *getArray() const;
    int get_m() {return m;}
	int size();
};

#endif
