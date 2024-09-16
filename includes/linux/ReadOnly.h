#include <algorithm>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>
class ReadOnlyAccess {
   public:
    static void Run() {
        const char* c = "Bob";
        const char* label = "Read=";
        unsigned long const num_threads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads(num_threads - 1);
        for (long i = 0; i < num_threads - 1; i++) {
            threads[i] = std::thread([&]() {
                const std::thread::id t_id = std::this_thread::get_id();
                char* txt = new char[strlen(label) + strlen(c) + 1];
                strcpy(txt, label);
                strcat(txt, c);
                std::cout << std::this_thread::get_id() << txt << std::endl;
            });
        }
        std::for_each(threads.begin(), threads.end(),
                      std::mem_fn(&std::thread::join));
    }
};
