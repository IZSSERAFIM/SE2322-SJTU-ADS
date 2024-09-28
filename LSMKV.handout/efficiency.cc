#include "kvstore.h"
#include "utils.h"
#include <random>
#include <cstdio>
#include <map>
#include <vector>

std::mt19937_64 rng;
std::uniform_real_distribution<double> rand_double;
std::vector<std::pair<uint64_t, std::string>> put_data;
std::vector<uint64_t> get_data, del_data;

int test_size = 52224 * 2;

std::pair<uint64_t, std::string> random_kv(int min_key = 1, int max_key = 1024 * 48, int min_len = 1024, int max_len = 1024 * 4)
{
    uint64_t key, vlen;
    key = rand_double(rng) * (max_key - min_key) + min_key;
    vlen = rand_double(rng) * (max_len - min_len) + min_len;
    char buf[vlen + 1];
    for(int i = 0; i < vlen; i ++)
        buf[i] = 2 + rand_double(rng) * 100;
    buf[vlen] = 0;
    return std::make_pair(key, std::string(buf));
}

void routine_test(KVStore &kvstore)
{
    utils::TimeRecord timer;
    std::map<uint64_t, std::string> Map;
    
    printf("[ROUTINE TEST]\n");
    timer.reset();
    for(int i = 0; i < test_size; i ++)
        kvstore.put(put_data[i].first, put_data[i].second);
    double time = timer.get_elapsed_time_micro() / test_size;
    printf("  Phase 1:\nsingle put time %.3lfus\n", time);

    for(int i = 0; i < test_size; i ++)
        Map[put_data[i].first] = put_data[i].second;
    timer.reset();
    for(int i = 0; i < test_size; i ++)
        std::string val = kvstore.get(get_data[i]);
    time = timer.get_elapsed_time_micro() / test_size;
    int pass_size = 0;
   
   
    for(int i = 0; i < test_size; i ++)
        if(kvstore.get(get_data[i]) == Map[get_data[i]])
            pass_size ++;
    printf("  Phase 2: %d/ %d %s\n", pass_size, test_size, pass_size == test_size ? "[PASS]" : "[FAIL]");
    printf("single get time %.3lfus\n", time);

    for(int i = 0; i < test_size; i ++)
        if(Map.find(del_data[i]) != Map.end())
            Map.erase(Map.find(del_data[i]));
    timer.reset();
    for(int i = 0; i < test_size; i ++)
        kvstore.del(del_data[i]);
    time = timer.get_elapsed_time_micro() / test_size;
    pass_size = 0;
    for(int i = 0; i < test_size; i ++)
        if(kvstore.get(get_data[i]) == Map[get_data[i]])
            pass_size ++;
    printf("  Phase 3: %d/ %d %s\nsingle del time %.3lfus\n", pass_size, test_size, pass_size == test_size ? "[PASS]" : "[FAIL]", time);
}

void index_cache_bloom_test(KVStore &kvstore)
{
    utils::TimeRecord timer;
    std::map<uint64_t, std::string> Map;

    printf("[CACHE BLOOM TEST]\n");
    for(int i = 0; i < test_size; i ++)
    {
        kvstore.put(put_data[i].first, put_data[i].second);
        Map[put_data[i].first] = put_data[i].second;
    }

    timer.reset();
    for(int i = 0; i < test_size; i ++)
        std::string val = kvstore.get(get_data[i]);
    double time = timer.get_elapsed_time_micro() / test_size;
    int pass_size = 0;
    for(int i = 0; i < test_size; i ++)
        if(kvstore.get(get_data[i]) == Map[get_data[i]])
            pass_size ++;
    printf("  Phase 1: %d/ %d %s\n", pass_size, test_size, pass_size == test_size ? "[PASS]" : "[FAIL]");
    printf("normal: single get time %.3lfus\n", time);

    kvstore.test(1);

    timer.reset();
    for(int i = 0; i < test_size; i ++)
        std::string val = kvstore.get(get_data[i]);
    time = timer.get_elapsed_time_micro() / test_size;
    pass_size = 0;
    for(int i = 0; i < test_size; i ++)
        if(kvstore.get(get_data[i]) == Map[get_data[i]])
            pass_size ++;
    printf("  Phase 2: %d/ %d %s\n", pass_size, test_size, pass_size == test_size ? "[PASS]" : "[FAIL]");
    printf("no bloomfilter: single get time %.3lfus\n", time);

    kvstore.test(2);

    timer.reset();
    for(int i = 0; i < test_size; i ++)
        std::string val = kvstore.get(get_data[i]);
    time = timer.get_elapsed_time_micro() / test_size;
    pass_size = 0;
    for(int i = 0; i < test_size; i ++)
        if(kvstore.get(get_data[i]) == Map[get_data[i]])
            pass_size ++;
    printf("  Phase 3: %d/ %d %s\n", pass_size, test_size, pass_size == test_size ? "[PASS]" : "[FAIL]");
    printf("no cache: single get time %.3lfus\n", time);

    kvstore.test(0);
}

void compaction_test(KVStore &kvstore)
{
    utils::TimeRecord timer;
    int fd = open("./compaction_time.txt", O_RDWR | O_CREAT, 0644);
    char buf[124];
    for(int i = 0; i < test_size; i ++)
    {
        timer.reset();
        std::string val = kvstore.get(get_data[i]);
        double time = timer.get_elapsed_time_micro();
        sprintf(buf, "%.3lf ", time);
        write(fd, buf, strlen(buf));
    }
    close(fd);
    printf("[COMPACTION TEST]\n  Phase 1:\n");
    printf("save in ./compaction_time.txt\n");
}

void bloomsize_test()
{
    std::vector<double> put_time, get_time;
    uint64_t bloomSizes[10];
    utils::TimeRecord timer;
    std::map<uint64_t, std::string> Map;
    for(int i = 0; i < 10; i ++)
        bloomSizes[i] = 8192 + (i - 5) * 1500;
    printf("[BLOOMSIZE TEST]\n");
    for(int i = 0; i < test_size; i ++)
        Map[put_data[i].first] = put_data[i].second;

    for(int i = 0; i < 10; i ++)
    {
        KVStore kvstore(bloomSizes[i], "./data", "./data/vlog");
        timer.reset();
        for(int j = 0; j < test_size; j ++)
            kvstore.put(put_data[j].first, put_data[j].second);
        double time = timer.get_elapsed_time_micro() / test_size;
        put_time.push_back(time);
        printf("  Phase: %d\nsingle put time %.3lfus\n", i + 1, time);

        timer.reset();
        for(int j = 0; j < test_size; j ++)
            std::string val = kvstore.get(get_data[j]);
        time = timer.get_elapsed_time_micro() / test_size;
        get_time.push_back(time);
        printf(" single get time %.3lfus\n", time);

        int pass_size = 0;
        for(int j = 0; j < test_size; j ++)
            if(kvstore.get(get_data[j]) == Map[get_data[j]])
                pass_size ++;
            else
            {
                printf("EXPECT|%s|\n GOT|%s|\n", Map[get_data[j]], kvstore.get(get_data[j]));
                printf("%ld: ", (long long)get_data[j]);
                for(int k = 0; k < put_data.size(); k ++)
                    if(put_data[k].first == get_data[j])
                        printf("%s, ", put_data[k].second);
                printf("\n");
            } 
        printf("size:%d %d/ %d %s\n", (int)bloomSizes[i], pass_size, test_size, pass_size == test_size ? "[PASS]" : "[FAIL]");
        kvstore.reset();
    }

    for(int i = 0; i < 10; i ++)
        printf("%d ", (int)bloomSizes[i]);
    printf("\n");
    for(int i = 0; i < 10; i ++)
        printf("%.2lf ", put_time[i]);
    printf("\n");
    for(int i = 0; i < 10; i ++)
        printf("%.3lf ", get_time[i]);
}

int main()
{
    printf("KVStore Efficiency Test\n");
    KVStore kvstore("./data", "./data/vlog");

    for(int i = 0; i < test_size; i ++)
    {
        put_data.push_back(random_kv());
        del_data.push_back(put_data.back().first);
    }
    for(int i = 0; i < test_size; i ++)
    {
        double p = rand_double(rng);
        if(p < 0.5)
            get_data.push_back(put_data[(int)(p * test_size)].first);
        else
            get_data.push_back(random_kv().first);
    }

    //routine_test(kvstore);

    //kvstore.reset();

    //index_cache_bloom_test(kvstore);

    //kvstore.reset();

    //compaction_test(kvstore);

    //kvstore.reset();

    bloomsize_test();

    return 0;
}