#ifndef ATOMIC_H
#define ATOMIC_H
#include <atomic>
#endif  // !ATOMIC_H

#ifndef THREAD_H
#define THREAD_H
#include <thread>
#endif  // !THREAD_H

#ifndef IOSTREAM_H
#define IOSTREAM_H
#include <iostream>
#endif  // !IOSTREAM_H

#ifndef FORMAT_H
#define FORMAT_H
#include <format>
#endif  // !FORMAT_H

#ifndef VECTOR_H
#define VECTOR_H
#include <vector>
#endif  // !VECTOR_H

#ifndef CSTDARG_H
#define CSTDARG_H
#include <cstdarg>
#endif  // !CSTDARG_H

struct FlagObject {
    bool flag;
    int operations;
    FlagObject(bool f = false, int ops = 0) : flag(f), operations(ops) {}
};

class Atomic4 {
   private:
    static std::atomic<FlagObject> flagObj;
    static std::atomic<int> readers_done;
    static std::atomic<int> writers_done;
    static constexpr int MAX_MESSAGES = 6;
    static constexpr int MESSAGE_SIZE = 100;
    static char messages[MAX_MESSAGES][MESSAGE_SIZE];
    static std::atomic<int> message_count;

    static void safe_sprintf(int id, const char *format, ...) {
        va_list args;
        va_start(args, format);
        int index = message_count.fetch_add(1, std::memory_order_relaxed);
        if (index < MAX_MESSAGES) {
            vsnprintf(messages[index], MESSAGE_SIZE, format, args);
        }
        va_end(args);
    }

    // Function to write to the atomic object
    static void writer(int id) {
        FlagObject oldObj, newObj;
        do {
            oldObj = flagObj.load(std::memory_order_relaxed);
            newObj = FlagObject(!oldObj.flag, oldObj.operations + 1);
        } while (!flagObj.compare_exchange_weak(oldObj, newObj,
                                                std::memory_order_release,
                                                std::memory_order_relaxed));

        safe_sprintf(id, "Writer %d set flag to %s, operations: %d", id,
                     newObj.flag ? "true" : "false", newObj.operations);
        writers_done.fetch_add(1, std::memory_order_relaxed);
    }

    // Function to read from the atomic object
    static void reader(int id) {
        FlagObject obj;
        do {
            obj = flagObj.load(std::memory_order_relaxed);
        } while (!obj.flag);

        safe_sprintf(id, "Reader %d detected flag is true, operations: %d", id,
                     obj.operations);
        readers_done.fetch_add(1, std::memory_order_relaxed);
    }

   public:
    static void main() {
        std::vector<std::thread> threads;

        // Create 3 reader threads
        for (int i = 0; i < 3; ++i) {
            threads.emplace_back(reader, i);
        }

        // Create 3 writer threads
        for (int i = 0; i < 3; ++i) {
            threads.emplace_back(writer, i);
        }

        // Wait for all threads to finish
        for (auto &t : threads) {
            t.join();
        }

        // Print all messages
        for (int i = 0; i < message_count; ++i) {
            std::cout << messages[i] << std::endl;
        }

        std::cout << "All threads have finished.\n";

        FlagObject finalObj = flagObj.load(std::memory_order_relaxed);
        std::cout << "Total operations on flag: " << finalObj.operations
                  << std::endl;
    }
};

std::atomic<FlagObject> Atomic4::flagObj{FlagObject(false, 0)};
std::atomic<int> Atomic4::readers_done(0);
std::atomic<int> Atomic4::writers_done(0);
char Atomic4::messages[Atomic4::MAX_MESSAGES][Atomic4::MESSAGE_SIZE];
std::atomic<int> Atomic4::message_count(0);