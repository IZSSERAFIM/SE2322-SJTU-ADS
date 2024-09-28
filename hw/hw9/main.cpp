#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <vector>

std::atomic<int> activeThreads(0);

int fibonacci(int n, int threadNum) {
    if (n <= 1) {
        return n;
    }

    int x, y;
    std::thread t1;

    if (activeThreads < threadNum) {
        t1 = std::thread([&]() {
            activeThreads++;
            x = fibonacci(n - 1, threadNum);
            activeThreads--;
        });

        y = fibonacci(n - 2, threadNum);
    } else {
        x = fibonacci(n - 1, threadNum);
        y = fibonacci(n - 2, threadNum);
    }

    if(t1.joinable()) {
        t1.join();
    }

    return x + y;
}

double test_fibonacci(int n, int threadNum) {
    auto start = std::chrono::high_resolution_clock::now();
    int result = fibonacci(n, threadNum);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    return diff.count();
}

int main() {
    int n = 40;
    int repeat = 5; // Repeat the test multiple times to get an average
    std::vector<double> singleThreadTimes(repeat);
    std::vector<double> multiThreadTimes(repeat);

    auto start = std::chrono::high_resolution_clock::now();
    int result = fibonacci(n, 1);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end-start;
    std::cout << "Time to calculate fibonacci(" << n << ") by 1 thread: " << diff.count() << " s\n";
    std::cout << "Result of fibonacci(" << n << "): " << result << std::endl;
    std::cout << std::endl;

    // Test single thread
    for (int i = 0; i < repeat; ++i) {
        singleThreadTimes[i] = test_fibonacci(n, 1);
    }

    double avgSingleThreadTime = 0;
    for (double t : singleThreadTimes) {
        avgSingleThreadTime += t;
    }
    avgSingleThreadTime /= repeat;

    // Test multi threads
    int threadNums[] = {2, 4, 8, 16};
    for (int threadNum : threadNums) {
        double totalMultiThreadTime = 0;
        for (int i = 0; i < repeat; ++i) {
            totalMultiThreadTime += test_fibonacci(n, threadNum);
        }
        double avgMultiThreadTime = totalMultiThreadTime / repeat;

        std::cout << "Time to calculate fibonacci(" << n << ") by " << threadNum << " threads: " << avgMultiThreadTime << " s\n";
        std::cout << "Speedup of fibonacci(" << n << ") by " << threadNum << " threads compared to 1 thread: " << avgSingleThreadTime / avgMultiThreadTime << "\n\n";
    }

    return 0;
}
