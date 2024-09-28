#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
#include "tree.hpp"
#include "util.hpp"
#include "compressed_radix_tree.hpp"
#include "radix_tree.hpp"
#include "rbtree.hpp"

using namespace std::chrono;

struct TestResult {
    double average_time;
    double p50;
    double p90;
    double p99;
};

std::mt19937 gen(time(NULL));
std::uniform_real_distribution<double> randf(0.0, 1.0);
double test_time = 60000, time_p[1000000] = {0};

void perform_operations(Tree *tree, double *work, int batch_size, int &oper) {
    for (int i = 0; i < batch_size; i++) {
        double p = randf(gen);
        uint32_t x = zipf();
        if (p < work[0]) {
            tree->find(x);
        } else if (p < work[1]) {
            tree->insert(x);
        } else if (p < work[2]) {
            tree->remove(x);
        }
    }
    oper += batch_size;
}

TestResult calculate_test_result(double total_time, int oper, int batch_size) {
    TestResult result;
    result.average_time = (double)total_time / oper;
    std::sort(time_p, time_p + oper / batch_size + 1);
    result.p50 = time_p[int(oper * 0.5 / batch_size)];
    result.p90 = time_p[int(oper * 0.9 / batch_size)];
    result.p99 = time_p[int(oper * 0.99 / batch_size)];
    return result;
}

TestResult test_tree_work(Tree *tree, double *work, int batch_size) {
    int oper = 0;
    double total_time = 0;
    time_t t0 = time(0);
    while (time(0) - t0 < test_time / 1000) {
        auto start = high_resolution_clock::now();
        perform_operations(tree, work, batch_size, oper);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        total_time += duration.count();
    }
    return calculate_test_result(total_time, oper, batch_size);
}

void test_and_print_results(Tree *tree, const char *tree_name, double (*works)[3], int num_workloads, int batch_size) {
    std::cout << tree_name << std::endl;
    for (int work_id = 0; work_id < num_workloads; work_id++) {
        std::cout << "workload-" << (work_id + 1) << std::endl;
        TestResult result = test_tree_work(tree, works[work_id], batch_size);
        std::cout << "average: " << result.average_time << " P50: " << result.p50 << " P90: " << result.p90 << " P99: " << result.p99 << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    TreeType trees[3] = {TreeType::RadixTree, TreeType::CompressedRadixTree, TreeType::RBTree};
    double works[3][3] = {{0.5, 0.5, 0}, {1.0, 0, 0}, {0.5, 0.25, 0.25}};
    const char *tree_names[3] = {"RadixTree", "CompressedRadixTree", "RBTree"};
    int batch_size = 1000;
    for (int tree_id = 0; tree_id < 3; tree_id++) {
        Tree *tree = nullptr;
        switch (trees[tree_id]) {
            case TreeType::RadixTree:
                tree = new class RadixTree();
                break;
            case TreeType::CompressedRadixTree:
                tree = new class CompressedRadixTree();
                break;
            case TreeType::RBTree:
                tree = new class RBTree();
                break;
            default:
                break;
        }
        test_and_print_results(tree, tree_names[tree_id], works, 3, batch_size);
        delete tree;
    }
    return 0;
}
