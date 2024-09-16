/*
Threading, https://en.cppreference.com/w/cpp/thread/async
*/
#include <future>
#include <iostream>
#include <mutex>
#include <thread>

class Obj {
   private:
    const char *mName;

   public:
    Obj(const char *_name) : mName(_name) {}
    const char *GetName() const { return this->mName; }
};

class ThreadingTest {
   private:
    static std::mutex mtx;
    static void Say(const char *str) {
        std::scoped_lock<std::mutex> lock(mtx);
        std::cout << "\nsay: " << str << std::endl;
    }

    static void Say(const Obj &obj) {
        std::scoped_lock<std::mutex> lock(mtx);
        std::cout << "\nsay: " << obj.GetName() << std::endl;
    }

   public:
    static int Init() {
        Obj obj("Bob");
        std::thread t1([](const char *str) { Say(str); }, "Bop");
        std::thread t2([](const Obj &obj) { Say(obj); }, obj);
        t1.join();
        t2.join();
        return 0;
    }
};

std::mutex ThreadingTest::mtx;