#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#endif
#ifdef _WIN32
#ifndef SOCKET_ISVALIDSOCKET
#define SOCKET_ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
#endif
#ifndef SOCKET_CLOSESOCKET
#define SOCKET_CLOSESOCKET(s) closesocket(s)
#endif
#ifndef SOCKET_GETSOCKETERRNO
#define SOCKET_GETSOCKETERRNO() (WSAGetLastError())
#endif
#else
#ifndef SOCKET_ISVALIDSOCKET
#define SOCKET_ISVALIDSOCKET(s) ((s) >= 0)
#endif
#ifndef SOCKET_CLOSESOCKET
#define SOCKET_CLOSESOCKET(s) close(s)
#endif
#ifndef SOCKET
#define SOCKET int
#endif
#ifndef SOCKET_GETSOCKETERRNO
#define SOCKET_GETSOCKETERRNO() (errno)
#endif
#endif

#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <conio.h>
#endif

#ifndef SOCKET_GETADDRINFO
// Resolves a hostname and service to a socket address.
// Parameters:
// nodename:  The host name or IP address to be resolved (e.g., "example.com",
// or "192.168.1.1"). servname:  The service name or port number (e.g., "http"
// or "80"). hints:     Pointer to a struct addrinfo specifying criteria for
// selecting the socket address. res:       Pointer to a pointer where the
// result of the query is stored (linked list of addrinfo structures).
#define SOCKET_GETADDRINFO(nodename, servname, hints, res)          \
    getaddrinfo((const char *)(nodename), (const char *)(servname), \
                (const struct addrinfo *)(hints), (struct addrinfo **)(res))
#endif

#ifndef SOCKET_GETNAMEINFO
// Resolves a socket address to a hostname and service name.
// Parameters:
// -sa:       Pointer to a sockaddr structure (address information to be
// resolved). -salen:    Length of the sockaddr structure. -host:     Buffer to
// store the resolved hostname. -hostlen:  Size of the host buffer. -serv:
// Buffer to store the resolved service name (e.g., port number or protocol).
// -servlen:  Size of the serv buffer.
// -flags:    Flags to modify the behavior of getnameinfo (e.g., NI_NUMERICHOST
// for numeric IP). Returns: INT
#define SOCKET_GETNAMEINFO(sa, salen, host, hostlen, serv, servlen, flags) \
    getnameinfo((const struct sockaddr *)(sa), (socklen_t)(salen),         \
                (char *)(host), (size_t)(hostlen), (char *)(serv),         \
                (size_t)(servlen), (int)(flags))
#endif

#ifndef SOCKET_FREEADDRINFO
// Frees the memory allocated by getaddrinfo().
// Parameters:
// res:        A pointer to the addrinfo structure that was allocated by
// getaddrinfo(). Returns:    Nothing, the function only releases the allocated
// memory.
#define SOCKET_FREEADDRINFO(res) freeaddrinfo((struct addrinfo *)(res))
#endif

#ifndef SOCKET_CONNECT
// Attempts to establish a connection to a remote socket.
// Parameters:
// sockfd:     The file descriptor of the socket.
// addr:       A pointer to the sockaddr structure with the remote address.
// addrlen:    The size of the address structure.
// Returns:    0 if the connection is successfully established, -1 if an error
// occurs (with errno set).
#define SOCKET_CONNECT(sockfd, addr, addrlen) \
    connect((int)(sockfd), (struct sockaddr *)(addr), (socklen_t)(addrlen))
#endif

#ifndef SOCKET_SELECT
// Monitors file descriptors for readiness to read, write, or exceptions with a
// timeout. Parameters: nfds:       The highest-numbered file descriptor + 1.
// readfds:    Pointer to a set of file descriptors to be checked for
// readability. writefds:   Pointer to a set of file descriptors to be checked
// for writability. exceptfds:  Pointer to a set of file descriptors to be
// checked for exceptional conditions. timeout:    Pointer to a struct timeval
// specifying the timeout (NULL for blocking indefinitely).
#define SOCKET_SELECT(nfds, readfds, writefds, exceptfds, timeout) \
    select((int)(nfds), (fd_set *)(readfds), (fd_set *)(writefds), \
           (fd_set *)(exceptfds), (struct timeval *)(timeout))
#endif

#ifndef SOCKET_CREATESOCKET
// Create a socket.
// Parameters:
// domain:   The communication domain (e.g., AF_INET for IPv4, AF_INET6 for
// IPv6). type:     The socket type (e.g., SOCK_STREAM for TCP, SOCK_DGRAM for
// UDP). protocol: The protocol to be used with the socket. Pass 0 to choose the
// default protocol for the type.
#define SOCKET_CREATESOCKET(domain, type, protocol) \
    socket((int)(domain), (int)(type), (int)(protocol))
#endif

#ifndef SOCKET_RECV
// Receives data from a connected socket.
// Parameters:
// sockfd:     The file descriptor of the socket.
// buf:        A pointer to the buffer where the received data will be stored.
// len:        The maximum number of bytes to be received.
// flags:      Flags to modify the behavior of the recv() function.
// Returns:    The number of bytes received on success, or -1 on error (with
// errno set).
#define SOCKET_RECV(sockfd, buf, len, flags) \
    recv((int)(sockfd), (char *)(buf), (size_t)(len), (int)(flags))
#endif

class SocketClient {
   public:
    static int main(int argc, char *argv[]) {
#if defined(_WIN32)
        WSADATA d;
        if (WSAStartup(MAKEWORD(2, 2), &d)) {
            fprintf(stderr, "Failed to initialize.\n");
            return 1;
        }
#endif
        if (argc < 3) {
            fprintf(stderr, "usage: client.exe <hostname> <port>\n");
            return 1;
        }

        printf("Configuring remote address...\n");
        struct addrinfo hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_socktype = SOCK_STREAM;
        struct addrinfo *peer_address;
        if (SOCKET_GETADDRINFO(argv[1], argv[2], &hints, &peer_address)) {
            fprintf(stderr, "getaddrinfo() failed. (%d)\n",
                    SOCKET_GETSOCKETERRNO());
            return 1;
        }

        printf("Remote address is: ");
        char address_buffer[100];
        char service_buffer[100];
        SOCKET_GETNAMEINFO(peer_address->ai_addr, peer_address->ai_addrlen,
                           address_buffer, sizeof(address_buffer),
                           service_buffer, sizeof(service_buffer),
                           NI_NUMERICHOST);
        printf("%s %s\n", address_buffer, service_buffer);

        printf("Creating socket...\n");
        SOCKET socket_peer;
        socket_peer = SOCKET_CREATESOCKET(peer_address->ai_family,
                                          peer_address->ai_socktype,
                                          peer_address->ai_protocol);
        if (!SOCKET_ISVALIDSOCKET(socket_peer)) {
            fprintf(stderr, "socket() failed. (%d)\n", SOCKET_GETSOCKETERRNO());
            return 1;
        }

        printf("Connecting...\n");
        if (SOCKET_CONNECT(socket_peer, peer_address->ai_addr,
                           peer_address->ai_addrlen)) {
            fprintf(stderr, "connect() failed. (%d)\n",
                    SOCKET_GETSOCKETERRNO());
            return 1;
        }
        SOCKET_FREEADDRINFO(peer_address);

        printf("Connected.\n");
        printf("To send data, enter text followed by enter.\n");

        while (1) {
            fd_set reads;
            FD_ZERO(&reads);
            FD_SET(socket_peer, &reads);
#if !defined(_WIN32)
            FD_SET(0, &reads);
#endif

            struct timeval timeout;
            timeout.tv_sec = 0;
            timeout.tv_usec = 100000;

            if (SOCKET_SELECT(socket_peer + 1, &reads, 0, 0, &timeout) < 0) {
                fprintf(stderr, "select() failed. (%d)\n",
                        SOCKET_GETSOCKETERRNO());
                return 1;
            }

            if (FD_ISSET(socket_peer, &reads)) {
                char read[4096];
                int bytes_received = SOCKET_RECV(socket_peer, read, 4096, 0);
                if (bytes_received < 1) {
                    printf("Connection closed by peer.\n");
                    break;
                }
                printf("Received (%d bytes): %.*s", bytes_received,
                       bytes_received, read);
            }

#if defined(_WIN32)
            if (_kbhit()) {
#else
            if (FD_ISSET(0, &reads)) {
#endif
                char read[4096];
                if (!fgets(read, 4096, stdin)) break;
                printf("Sending: %s", read);
                int bytes_sent = send(socket_peer, read, static_cast<int>(strlen(read)), 0);
                printf("Sent %d bytes.\n", bytes_sent);
            }
        }  // end while(1)

        printf("Closing socket...\n");
        SOCKET_CLOSESOCKET(socket_peer);

#if defined(_WIN32)
        WSACleanup();
#endif
        printf("Finished.\n");
        return 0;
    }
};