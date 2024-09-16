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

#ifndef CSTDARG_H
#define CSTDARG_H
#include <cstdarg>
#endif // !CSTDARG_H

/*
  * @brief
  Synchronization Mechanism: Uses lock-free atomic operations for synchronization.

  Performance: Generally faster, especially under low contention, as it avoids the overhead of locking and unlocking.

  Scalability: Scales better with increasing number of threads, as there's no blocking. The atomic approach has a fixed message limit, while the mutex approach can handle any number of messages.

  Complexity: Can be more complex to implement correctly, especially for complex data structures.

  Granularity: Provides fine-grained control, allowing operations on individual variables.

  Deadlock Risk: No risk of deadlock in this simple use case.

  Memory Usage: Fixed-size array in this example, which might waste space if underutilized.

  Flexibility: Limited to operations that can be done atomically.
*/
class AtomicApproach
{
private:
  static std::atomic<int> flag;
  static constexpr int MAX_MESSAGES = 6;
  static constexpr int MESSAGE_SIZE = 100;
  static char messages[MAX_MESSAGES][MESSAGE_SIZE];
  static std::atomic<int> message_count;

  static void safe_sprintf(const char *format, ...)
  {
    va_list args;
    va_start(args, format);
    int index = message_count.fetch_add(1, std::memory_order_relaxed);
    if (index < MAX_MESSAGES)
    {
      vsnprintf(messages[index], MESSAGE_SIZE, format, args);
    }
    va_end(args);
  }

  static void worker(int id)
  {
    int old_value = flag.fetch_add(1, std::memory_order_relaxed);
    safe_sprintf("Worker %d changed flag from %d to %d", id, old_value, old_value + 1);
  }

public:
  static void run()
  {
    std::vector<std::thread> threads;
    for (int i = 0; i < 6; ++i)
    {
      threads.emplace_back(worker, i);
    }
    for (auto &t : threads)
    {
      t.join();
    }
    for (int i = 0; i < message_count; ++i)
    {
      std::cout << messages[i] << std::endl;
    }
  }
};

std::atomic<int> AtomicApproach::flag(0);
char AtomicApproach::messages[AtomicApproach::MAX_MESSAGES][AtomicApproach::MESSAGE_SIZE];
std::atomic<int> AtomicApproach::message_count(0);

/*
  * @brief
  Synchronization Mechanism: Uses a mutex to create a critical section for shared resource access.

  Performance: Can be slower due to the overhead of lock acquisition and release, especially under high contention.

  Scalability: May not scale as well, as threads might block waiting for the lock. The atomic approach has a fixed message limit, while the mutex approach can handle any number of messages.

  Complexity: Generally simpler to understand and implement correctly.

  Granularity: Typically used for coarse-grained locking of larger critical sections.

  Deadlock Risk: Potential for deadlock if multiple mutexes are used incorrectly.

  Memory Usage: Uses a dynamic vector, only consuming memory as needed.

  Flexibility: Can protect more complex operations and data structures.
*/
class MutexApproach
{
private:
  static int flag;
  static std::vector<std::string> messages;
  static std::mutex mtx;

  static void worker(int id)
  {
    int old_value, new_value;
    {
      std::lock_guard<std::mutex> lock(mtx);
      old_value = flag;
      flag++;
      new_value = flag;
      messages.push_back("Worker " + std::to_string(id) +
                         " changed flag from " + std::to_string(old_value) +
                         " to " + std::to_string(new_value));
    }
  }

public:
  static void run()
  {
    std::vector<std::thread> threads;
    for (int i = 0; i < 6; ++i)
    {
      threads.emplace_back(worker, i);
    }
    for (auto &t : threads)
    {
      t.join();
    }
    for (const auto &msg : messages)
    {
      std::cout << msg << std::endl;
    }
  }
};

int MutexApproach::flag = 0;
std::vector<std::string> MutexApproach::messages;
std::mutex MutexApproach::mtx;

class MutexVSAtomicLogging
{
public:
  static void main()
  {
    std::cout << "Atomic Approach:" << std::endl;
    AtomicApproach::run();

    std::cout << "\nMutex Approach:" << std::endl;
    MutexApproach::run();
  }
};