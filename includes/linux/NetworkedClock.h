// bind address port
#define __PORT "42069"
// number of connections per socket listen allowed to be queued up
#define NUM_CONN 10
// include platform headers
#if defined(_WIN32)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
/*
Socket
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#endif

// custom macros for win and unix sockets
#if defined(_WIN32)
#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
#define CLOSESOCKET(s) closesocket(s)
#define GETSOCKETERRNO() (WSAGetLastError())
#else
#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int
#define GETSOCKETERRNO() (errno)
#endif

// standard headers
#include <stdio.h>
#include <string.h>
#include <time.h>

class NetworkedClock
{
public:
  static int Init()
  {
    time_t timer;
    time(&timer);
    printf("Local time is: %s", ctime(&timer));
    return 0;
  }

  static int Main()
  {
#if defined(_WIN32)
    // Windows requires WinSock initialization
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d))
    {
      fprintf(stderr, "Failed to initialize.\n");
      return 1;
    }
#endif
    // bind to local address and port
    printf("Configuring local address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;       // specify IPv4
    hints.ai_socktype = SOCK_STREAM; // use TCP, UDP would be SOCK_DGRAM
    hints.ai_flags = AI_PASSIVE;     // bind to wildcard address, listen to any network interface
    struct addrinfo *bind_address;
    /*
    getaddrinfo() generates and assigns address to bind
    in order to do this
    - first param is null/0
    - ai_flags = AI_PASSIVE
    getaddrinfo() makes it easy to convert from IPv4 to IPv6,
    as we only need to use ai_family = AF.INET6 instead

    Remarks
    https://learn.microsoft.com/en-us/windows/win32/api/ws2tcpip/nf-ws2tcpip-getaddrinfo#remarks
    */
    getaddrinfo(0, __PORT, &hints, &bind_address);

    printf("Creating socket...\n");
    SOCKET socket_listen; // typeof WinSock, or int on other platforms
    socket_listen = socket(bind_address->ai_family,
                           bind_address->ai_socktype, bind_address->ai_protocol);
    if (!ISVALIDSOCKET(socket_listen))
    {
      fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
      return 1;
    }

    printf("Binding socket to local address...\n");
    if (bind(socket_listen,
             bind_address->ai_addr, bind_address->ai_addrlen))
    {
      fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
      return 1;
    }
    freeaddrinfo(bind_address);

    printf("Listening...\n");
    if (listen(socket_listen, NUM_CONN) < 0)
    {
      fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
      return 1;
    }

    printf("Waiting for connection...\n");
    struct sockaddr_storage client_address;
    socklen_t client_len = sizeof(client_address);
    /*
    accept() first blocks the program until a new connection is made
    accept() returns sockaddr_storage and socklen_t
    */
    SOCKET socket_client = accept(socket_listen,
                                  (struct sockaddr *)&client_address, &client_len);
    if (!ISVALIDSOCKET(socket_client))
    {
      fprintf(stderr, "accept() failed. (%d)\n", GETSOCKETERRNO());
      return 1;
    }

    printf("Client is connected... ");
    char address_buffer[100];
    getnameinfo((struct sockaddr *)&client_address,
                client_len,
                address_buffer,
                sizeof(address_buffer),
                0,             // ignore
                0,             // ignore
                NI_NUMERICHOST // Don't try to look up hostname. Show IP
    );
    printf("%s\n", address_buffer);

    /*
    expecting HTTP requests
    */
    printf("Reading request...\n");
    char request[1024]; // allocate buffer for the request
    /*
    recv() blocks waiting for request and returns bytes received
    if connection terminated by client, returns 0 or -1
    check for recv() > 0 before proceed
    */
    int bytes_received = recv(socket_client, request, 1024, 0);
    printf("Received %d bytes.\n", bytes_received);

    printf("%.*s", bytes_received, request);

    printf("Sending response...\n");
    const char *response =
        "HTTP/1.1 200 OK\r\n"              // header
        "Connection: close\r\n"            // header
        "Content-Type: text/plain\r\n\r\n" // header end
        "Local time is: ";
    int bytes_sent = send(socket_client, response, strlen(response), 0);
    printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(response));

    time_t timer;
    time(&timer);
    char *time_msg = ctime(&timer);

    bytes_sent = send(socket_client, time_msg, strlen(time_msg), 0);
    printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(time_msg));

    // close connection after all data is sent
    printf("Closing connection...\n");
    CLOSESOCKET(socket_client);
#if defined(_WIN32)
    WSACleanup();
#endif
    printf("Finished.\n");
    return 0;
  }
};