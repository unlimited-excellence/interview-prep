#include <iostream>
#include <vector>
#include <list>
#include <chrono>
#include <random>
#include <numeric>
#include <algorithm>
#include <iomanip>

// Use a volatile sink to prevent the compiler from optimizing away the loops
volatile long long sink = 0;

// High-precision timer using std::chrono
class Timer {
public:
    Timer() : start_time(std::chrono::high_resolution_clock::now()) {
    }

    double elapsed_ms() {
        auto end_time = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end_time - start_time).count();
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
};

void benchmark_sequential_traversal(const std::vector<int> &vec, const std::list<int> &li) {
    std::cout << "\n--- Benchmark 1: Sequential Traversal ---\n";

    // Vector traversal
    Timer vec_timer;
    long long vec_sum = 0;
    for (int x: vec) {
        vec_sum += x;
    }
    sink = vec_sum; // Use the result
    double vec_duration = vec_timer.elapsed_ms();
    std::cout << "std::vector traversal time: " << std::fixed << std::setprecision(2) << vec_duration << " ms\n";

    // List traversal
    Timer list_timer;
    long long list_sum = 0;
    for (int x: li) {
        list_sum += x;
    }
    sink = list_sum; // Use the result
    double list_duration = list_timer.elapsed_ms();
    std::cout << "std::list traversal time:   " << std::fixed << std::setprecision(2) << list_duration << " ms\n";
}

void benchmark_random_access(const std::vector<int> &vec, const std::list<int> &li,
                             const std::vector<size_t> &indices) {
    std::cout << "\n--- Benchmark 2: Random Access ---\n";

    // Vector random access
    Timer vec_timer;
    long long vec_sum = 0;
    for (size_t index: indices) {
        vec_sum += vec[index];
    }
    sink = vec_sum;
    double vec_duration = vec_timer.elapsed_ms();
    std::cout << "std::vector random access time: " << std::fixed << std::setprecision(2) << vec_duration << " ms\n";

    // List random access (simulated by advancing an iterator)
    Timer list_timer;
    long long list_sum = 0;
    for (size_t index: indices) {
        auto it = li.begin();
        std::advance(it, index);
        list_sum += *it;
    }
    sink = list_sum;
    double list_duration = list_timer.elapsed_ms();
    std::cout << "std::list random access time:   " << std::fixed << std::setprecision(2) << list_duration << " ms\n";
}

void benchmark_insertions_at_front() {
    std::cout << "\n--- Benchmark 3: Insertion at Front ---\n";
    const int INSERTION_COUNT = 100000;

    // Vector insertion at front
    Timer vec_timer;
    std::vector<int> vec_insert;
    for (int i = 0; i < INSERTION_COUNT; ++i) {
        vec_insert.insert(vec_insert.begin(), i);
    }
    double vec_duration = vec_timer.elapsed_ms();
    std::cout << "std::vector insertion time: " << std::fixed << std::setprecision(2) << vec_duration << " ms\n";

    // List insertion at front
    Timer list_timer;
    std::list<int> list_insert;
    for (int i = 0; i < INSERTION_COUNT; ++i) {
        list_insert.push_front(i);
    }
    double list_duration = list_timer.elapsed_ms();
    std::cout << "std::list insertion time:   " << std::fixed << std::setprecision(2) << list_duration << " ms\n";
}

int main() {
    const size_t ELEMENT_COUNT = 10000000;
    const size_t RANDOM_ACCESS_COUNT = 100000;

    std::cout << "Setting up benchmark data with " << ELEMENT_COUNT << " elements...\n";

    // Setup main data structures
    std::vector<int> main_vector(ELEMENT_COUNT);
    std::iota(main_vector.begin(), main_vector.end(), 0);
    std::list<int> main_list;
    main_list.assign(main_vector.begin(), main_vector.end());

    // Setup random indices for access test
    std::vector<size_t> random_indices(RANDOM_ACCESS_COUNT);
    std::mt19937 gen(1337); // Fixed seed for reproducible results
    std::uniform_int_distribution<size_t> distrib(0, ELEMENT_COUNT - 1);
    for (size_t i = 0; i < RANDOM_ACCESS_COUNT; ++i) {
        random_indices[i] = distrib(gen);
    }

    std::cout << "Setup complete. Running benchmarks...\n";

    benchmark_sequential_traversal(main_vector, main_list);
    benchmark_random_access(main_vector, main_list, random_indices);
    benchmark_insertions_at_front();

    return 0;
}
