#if defined(_WIN32)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#elif defined(__linux__)
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#endif
#include <stdio.h>

// socket datatype
#if !defined(_WIN32)
#define SOCKET int
#endif

// valid socket
#if defined(_WIN32)
#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
#else
#define ISVALIDSOCKET(s) ((s) >= 0)
#endif

// close socket
#if defined(_WIN32)
#define CLOSESOCKET(s) closesocket(s)
#else
#define CLOSESOCKET(s) close(s)
#endif

// error handling
#if defined(_WIN32)
#define GETSOCKETERRNO() (WSAGetLastError())
#else
#define GETSOCKETERRNO() (errno)
#endif

class SocketInit
{
public:
  static int Init()
  {
#if defined(_WIN32)
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d))
    {
      fprintf(stderr, "Failed to initialize.\n");
      return 1;
    }
#elif defined(__linux__)
    printf("Ready to use socket API.\n");
#else // other platforms
    printf("? to use socket API.\n");
#endif
#if defined(_WIN32)
    WSACleanup();
#elif defined(__linux__)
    return 0;
#endif
    return 0;
  }
};