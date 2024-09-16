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

class Atomic1
{
private:
  // Declare an atomic boolean flag, initialized to false
  static std::atomic<bool> flag;

  // Function to write to the atomic boolean
  static void writer()
  {
    // Set the flag to true atomically
    // This operation is guaranteed to be atomic
    flag.store(true, std::memory_order_relaxed);
    std::cout << "Writer set flag to true\n"
              << std::endl;
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
    std::thread t1(writer);
    std::thread t2(reader, 1);
    std::thread t3(reader, 2);
    t1.join();
    t2.join();
    t3.join();
  }
};

std::atomic<bool> Atomic1::flag{false};