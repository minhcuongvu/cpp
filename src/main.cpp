#include "DODThreading.h"
// #include "IndentifyingThread.h"
// #include "NetworkedClock.h"
// #include "ReadOnly.h"
// #include "SquareRootException.h"
// #include "StringConcat.h"
// #include "Threading.h"
// #include "hello.h"
// #include "sock_init.h"
// #include "unix_list.h"
#include "Atomic4.h"

int main() {
    // Greeting::SayHello();
    // Unix::ListAdapters();
    // SocketInit::Init();
    // NetworkedClock::Init();
    // NetworkedClock::Main();
    // ThreadingTest::Init();
    // IndentifyingThread indentifyingThread(std::move(std::thread([]() {})));
    // ReadOnlyAccess::Run();
    // StringConcat::PrintConcat("Beep", "Boop");
    // SquareRootException::Main();
    // DODThreading::main();
    Atomic4::main();
    return 0;
}
