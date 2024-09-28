#include "src/hnsw.hpp"
#include "util/util.hpp"
#include "util/vecs_io.hpp"
#include "util/ground_truth.hpp"
#include "util/parameter.hpp"
#include <thread>

using namespace HNSWLab;

void query_thread(HNSW *hnsw, std::vector<std::vector<int>> *test_gnd, const int *query, int k, int start, int end)
{
    for (int i = start; i < end; i++) {
        (*test_gnd)[i] = hnsw -> query(query + i * 128, k);
    }
}

void createAndJoinThreads(HNSW& hnsw, std::vector<std::vector<int>>& test_gnd, const int* query, int gnd_vec_dim) {
    std::vector<std::thread> sub_threads;
    for(int i = 0; i < 4; i ++) {
        int start = i * 25;
        int end = (i + 1) * 25;
        sub_threads.push_back(std::thread(query_thread, &hnsw, &test_gnd, query, gnd_vec_dim, start, end));
    }
    for(auto& thread : sub_threads) {
        thread.join();
    }
}

int main() {

    std::printf("load ground truth\n");
    int gnd_n_vec = 100;
    int gnd_vec_dim = 10;
    char *path = "./data/siftsmall/gnd.ivecs";
    int *gnd = read_ivecs(gnd_n_vec, gnd_vec_dim, path);

    std::printf("load query\n");
    int query_n_vec = 100;
    int query_vec_dim = 128;
    path = "./data/siftsmall/query.bvecs";
    int *query = read_bvecs(query_n_vec, query_vec_dim, path);

    std::printf("load base\n");
    int base_n_vec = 10000;
    int base_vec_dim = 128;
    path = "./data/siftsmall/base.bvecs";
    int *base = read_bvecs(base_n_vec, base_vec_dim, path);

    HNSW hnsw;

    size_t report_every = 1000;
    TimeRecord insert_record;

    for (int i = 0; i < base_n_vec; i++) {
        hnsw.insert(base + base_vec_dim * i, i);

        if (i % report_every == 0) {
            insert_record.reset();
        }
    }

    printf("multithreading querying \n");
    std::vector <std::vector<int>> test_gnd_l(gnd_n_vec);
    TimeRecord query_record;

    createAndJoinThreads(hnsw, test_gnd_l, query, gnd_vec_dim);

    double single_query_time = query_record.get_elapsed_time_micro() / gnd_n_vec * 1e-3;

    double recall = count_recall(gnd_n_vec, gnd_vec_dim, test_gnd_l, gnd);
    printf("average recall: %.3f, single query time %.3f ms\n", recall, single_query_time);
    return 0;
}