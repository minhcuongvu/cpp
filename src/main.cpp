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
#include "unix_list.h"
#endif

#pragma region PORTABLE
#include "hello.h"
#include "socket1.h"
#include "socket2.h"
#include "socket3.h"
#include "socket_init.h"
#pragma endregion

int main(int argc, char* argv[]) {
    // Greeting::SayHello();
    // Unix::ListAdapters();
    // Win::ListAdapters();
    // SocketInit::Init();
    // NetworkedClock::Init();
    // NetworkedClock::Main();
    // ThreadingTest::Init();
    // IdentifyingThread identifyingThread(std::move(std::thread([]() {})));
    // ReadOnlyAccess::Run();
    // StringConcat::PrintConcat("Beep", "Boop");
    // SquareRootException::Main();
    // DODThreading::main();
    Atomic4::main();
    // Socket1::main(argc, argv);
    // SocketClient::main(argc, argv);
    return 0;
}
