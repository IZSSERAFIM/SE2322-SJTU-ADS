#include "MurmurHash3.h"
#include "bloomFilter.hpp"

bloomFilter::bloomFilter(int m, int k) : m(m), k(k)
{
    a = new bool [m];
    for(int i = 0; i < m; i ++)
        a[i] = 0;
}

bloomFilter::~bloomFilter()
{
    delete [] a;
}

void bloomFilter::insert(const uint64_t item) 
{
    uint32_t hash[4] = {0};
    for (int i = 0; i < k; i++) 
    {
       MurmurHash3_x64_128(&item, sizeof(item), i, hash);
       a[hash[i % k] % m] = true;
    }
}

bool bloomFilter::count(const uint64_t item) 
{
    uint32_t hash[4] = {0};
    for (int i = 0; i < k; i++) 
    {
        MurmurHash3_x64_128(&item, sizeof(item), i, hash);
        if (!a[hash[i % k] % m]) 
        {
            return false;
        }
    }
    return true;
} 

bool* bloomFilter::getArray() const
{
	return a;
}

int bloomFilter :: size()
{
    return m;
}