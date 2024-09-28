#include <iostream>
#include "MurmurHash3.h"
#include <functional>
#include <vector>
#include <string>
#include <map>
#include <time.h>
#include <stdlib.h>
#include <algorithm>

class BloomFilter{
private:
    int m;//哈希数组的大小
    int k;//hash函数的个数
    uint64_t hash[2] = {0};
    uint64_t *array;//哈希数组
public:
    BloomFilter(int set_m, int set_k){
        m = set_m;
        k = set_k;
        array = new uint64_t[set_m];

        for(int i = 0; i < m; i++){
            array[i] = 0;
        }
    }
    ~BloomFilter(){
        if(hash != NULL)
            delete[] array;
    }
    // 插入一个元素
    void insertNum(uint64_t num){
        for(int i = 0; i < k; i++){
            MurmurHash3_x64_128(&num, sizeof(num), i, hash);
            array[hash[1]%(m-1)] = 1;
        }
    }
    // 返回是否存在某个元素
    bool findNum(uint64_t num){
        for(int i = 0; i < k; i++){
            MurmurHash3_x64_128(&num, sizeof(num), i, hash);
            if(array[hash[1]%(m-1)] == 0){
                return false;
            }
        }
        return true;
    }
};

double filterTest (int m, int n, int k, int t) {
    BloomFilter bloomFilter = BloomFilter(m, k);
    std::map<int, bool> map;
    for (int i = 0; i < n; i++) {
        int insertRandNum = rand();
        while (map.find(insertRandNum) != map.end()) {
            insertRandNum = rand();
        }
        map[insertRandNum] = true;
        bloomFilter.insertNum(insertRandNum);
    }
    std::vector<int> searchRandNum;
    for (int i = 0; i < t; i++) {
        int randNum = rand();
        while (map.find(randNum) != map.end()) {
            randNum = rand();
        }
        searchRandNum.push_back(randNum);
    }
    int wrongSearch = 0;
    for (int i = 0; i < t; i++) {
        if (bloomFilter.findNum(searchRandNum[i])) {
            wrongSearch++;
        }
    }
    return (double)wrongSearch / t;
}

int main() {
    srand(time(NULL));
    int m = 6000000;
    for (int i = 2; i <=5; i++) {
        for (int j = 1; j <= 5; j++) {
            std::cout << "m = " << m << ", n = " << m / i << ", k = " << j << ", m/n = " << i << ", wrong rate = " << filterTest(m, m / i, j, m / 10) << std::endl;
        }
    }
    /*
    uint64_t key = 522123456789;
    uint64_t hash[2] = {0};

    MurmurHash3_x64_128(&key, sizeof(key), 1, hash);

    std::cout << "Hash value: ";
    for(int i = 0; i < 2; i++) {
        std::cout << hash[i] << " ";
    }
    std::cout << std::endl;
    //mod m-1
    //only use hash[1] 
    int m = 100;
    std::cout << hash[1]%(m-1) << " ";
     */
    
    return 0;
}
