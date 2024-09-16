#ifdef _WIN32
#include "Atomic4.h"
#include "DODThreading.h"
#include "win_list.h"
#endif
#ifdef __linux__
#include "IndentifyingThread.h"
#include "NetworkedClock.h"
#include "ReadOnly.h"
#include "SquareRootException.h"
#include "StringConcat.h"
#include "Threading.h"
#include "hello.h"
#include "sock_init.h"
#include "unix_list.h"
#endif

#include "socket1.h"
#include "socket2.h"

int main(int argc, char* argv[]) {
    // Greeting::SayHello();
    // Unix::ListAdapters();
    // Win::ListAdapters();
    // SocketInit::Init();
    // NetworkedClock::Init();
    // NetworkedClock::Main();
    // ThreadingTest::Init();
    // IndentifyingThread indentifyingThread(std::move(std::thread([]() {})));
    // ReadOnlyAccess::Run();
    // StringConcat::PrintConcat("Beep", "Boop");
    // SquareRootException::Main();
    // DODThreading::main();
    // Atomic4::main();
    // Socket1::main(argc, argv);
    Socket2::main(argc, argv);
    return 0;
}
