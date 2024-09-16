#include <iostream>
#include <thread>
class IndentifyingThread {
    std::thread t;

   public:
    explicit IndentifyingThread(std::thread t_) : t(std::move(t_)) {
        if (!t.joinable()) throw std::logic_error("no thread");
        std::cout << "This thread id=" << std::this_thread::get_id()
                  << std::endl;
    }
    ~IndentifyingThread() { t.join(); }
};