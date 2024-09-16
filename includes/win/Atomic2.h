#ifndef ATOMIC_H
#define ATOMIC_H
#include <atomic>
#endif // !ATOMIC_H

#ifndef THREAD_H
#define THREAD_H
#include <thread>
#endif // !THREAD_H

#ifndef IOSTREAM_H
#define IOSTREAM_H
#include <iostream>
#endif // !IOSTREAM_H

#ifndef FORMAT_H
#define FORMAT_H
#include <format>
#endif // !FORMAT_H

#ifndef VECTOR_H
#define VECTOR_H
#include <vector>
#endif // !VECTOR_H

class Atomic2
{
private:
  static std::atomic<bool> flag;
  static std::atomic<int> readers_done;
  static std::atomic<int> writers_done;

  // Function to write to the atomic boolean
  static void writer(int id)
  {
    flag.store(true, std::memory_order_release);
    std::cout << "Writer " << id << " set flag to true\n";
    writers_done.fetch_add(1, std::memory_order_relaxed);
  }

  // Function to read from the atomic boolean
  static void reader(int id)
  {
    // Spin-wait until the flag becomes true
    // load() reads the value atomically
    while (!flag.load(std::memory_order_relaxed))
    {
      // Keep checking the flag until it's true
    }

    // Once the flag is true, do something, ie. print a message
    std::cout << std::format("Reader {} detected flag is true\n", id) << std::endl;
  }

public:
  static void main()
  {
    std::vector<std::thread> threads;

    // Create 3 reader threads
    for (int i = 0; i < 3; ++i)
    {
      threads.emplace_back(reader, i);
    }

    // Create 3 writer threads
    for (int i = 0; i < 3; ++i)
    {
      threads.emplace_back(writer, i);
    }

    // Wait for all threads to finish
    for (auto &t : threads)
    {
      t.join();
    }
    std::cout << "All threads have finished.\n";
  }
};

std::atomic<bool> Atomic2::flag{false};
std::atomic<int> Atomic2::readers_done(0);
std::atomic<int> Atomic2::writers_done(0);