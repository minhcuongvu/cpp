#include "hello.h"

void Greeting::SayHello() {
    vector<string> msg {"Hello", "World"};
    for (const string& word : msg)
    {
        cout << word << " ";
    }
    cout << endl;
}