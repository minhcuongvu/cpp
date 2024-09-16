/*
Threading, https://en.cppreference.com/w/cpp/thread/async
*/
#include <cstring>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>

class Obj {
   private:
    char *mName;
    mutable std::mutex mtx;  // Mutex for thread-safe access

   public:
    // Constructor
    Obj(const char *_name) {
        mName = new char[std::strlen(_name) + 1];
        std::strcpy(mName, _name);
    }

    // Destructor
    ~Obj() { delete[] mName; }

    // Copy Constructor
    Obj(const Obj &other) {
        mName = new char[std::strlen(other.mName) + 1];
        std::strcpy(mName, other.mName);
    }

    // Copy Assignment Operator
    Obj &operator=(const Obj &other) {
        if (this != &other) {
            delete[] mName;
            mName = new char[std::strlen(other.mName) + 1];
            std::strcpy(mName, other.mName);
        }
        return *this;
    }

    // Getter
    const char *GetName() const {
        std::lock_guard<std::mutex> lock(mtx);
        return mName;
    }

    // Setter
    void SetName(const char *_name) {
        std::lock_guard<std::mutex> lock(mtx);
        if (std::strcmp(mName, _name) != 0) {
            delete[] mName;
            mName = new char[std::strlen(_name) + 1];
            std::strcpy(mName, _name);
        }
    }
};

class ThreadingTest {
   private:
    static void Say(const char *str) {
        std::cout << "\nsay: " << str << std::endl;
    }
    static void Say(const char &str) {
        std::cout << "\nsay: " << str << std::endl;
    }

    static void Say(const Obj &obj) {
        std::cout << "\nsay: " << obj.GetName() << std::endl;
    }

   public:
    static int Init() {
        Obj obj("Bob");
        const char *LOL = "LOL";
        char *s1 = new char[std::strlen(LOL) + 1];
        std::strcpy(s1, LOL);

        // Create threads using std::reference_wrapper to pass by reference
        std::thread t1(
            [&](const char *str) {
                Say(str);
                std::cout << "Object name=" << obj.GetName() << std::endl;
                Say(s1);

                const char *LMAO = "LMAO";
                std::strcpy(s1, LMAO);
            },
            "Bop");

        // Pass the object by reference
        std::thread t2(
            [&](const char *str) {
                Say(obj);
                obj.SetName("Beep");
                Say(str);
            },
            "123");

        t1.join();
        t2.join();

        std::cout << obj.GetName() << std::endl;
        std::cout << s1 << std::endl;

        std::cout << "Hardware concurrency="
                  << std::thread::hardware_concurrency() << std::endl;
        return 0;
    }
};
