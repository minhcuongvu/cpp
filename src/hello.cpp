#include "hello.h"

#include <iostream>
#include <string>
#include <vector>

void Greeting::SayHello() {
    std::vector<std::string> msg{"Hello", "World"};
    for (const std::string &word : msg) {
        std::cout << word << " ";
    }
    std::cout << std::endl;
}
