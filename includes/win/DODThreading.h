#ifndef THREAD_H
#define THREAD_H
#include <thread>
#endif  // !THREAD_H

#ifndef IOSTREAM_H
#define IOSTREAM_H
#include <iostream>
#endif  // !IOSTREAM_H

#ifndef VECTOR_H
#define VECTOR_H
#include <vector>
#endif  // !VECTOR_H

#ifndef CHRONO_H
#define CHRONO_H
#include <chrono>
#endif  // !CHRONO_H

class DODThreading {
   private:
    // Data-oriented structure
    struct NumberArray {
        std::vector<int> numbers;
    };

    // Initialize the array with sequential numbers
    static void initializeArray(NumberArray &arr, int size) {
        arr.numbers.resize(size);
        for (int i = 0; i < size; ++i) {
            arr.numbers[i] = i + 1;
        }
    }

    // Process a portion of the array
    static void processNumbers(NumberArray &arr, int start, int end) {
        for (int i = start; i < end; ++i) {
            arr.numbers[i] *= 2;  // Double each number
        }
    }

    // Multi-threaded processing function
    static void processArrayMultiThreaded(NumberArray &arr, int numThreads) {
        std::vector<std::thread> threads;
        int chunkSize = static_cast<int>(arr.numbers.size()) / numThreads;

        for (int i = 0; i < numThreads; ++i) {
            int start = i * chunkSize;
            int end = (i == numThreads - 1)
                          ? static_cast<int>(arr.numbers.size())
                          : (i + 1) * chunkSize;
            threads.emplace_back(processNumbers, std::ref(arr), start, end);
        }

        for (auto &thread : threads) {
            thread.join();
        }
    }

   public:
    static int main() {
        const int arraySize = 1'000'000'000;
        const int numThreads = std::thread::hardware_concurrency();

        NumberArray myArray;
        initializeArray(myArray, arraySize);

        auto start = std::chrono::high_resolution_clock::now();

        processArrayMultiThreaded(myArray, numThreads);

        auto end = std::chrono::high_resolution_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "Processed " << arraySize << " numbers in "
                  << duration.count() << " ms using " << numThreads
                  << " threads." << std::endl;

        // Verify a few results
        std::cout << "First 5 numbers: ";
        for (int i = 0; i < 5; ++i) {
            std::cout << myArray.numbers[i] << " ";
        }
        std::cout << std::endl;

        return 0;
    }
};