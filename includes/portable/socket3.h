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

class SocketServer {
   public:
    static int main() {
#if defined(_WIN32)
        WSADATA d;
        if (WSAStartup(MAKEWORD(2, 2), &d)) {
            fprintf(stderr, "Failed to initialize.\n");
            return 1;
        }
#endif

        printf("Configuring local address...\n");
        struct addrinfo hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        struct addrinfo *bind_address;
        SOCKET_GETADDRINFO(0, "8080", &hints, &bind_address);

        printf("Creating socket...\n");
        SOCKET socket_listen;
        socket_listen = SOCKET_CREATESOCKET(bind_address->ai_family,
                                            bind_address->ai_socktype,
                                            bind_address->ai_protocol);
        if (!SOCKET_ISVALIDSOCKET(socket_listen)) {
            fprintf(stderr, "socket() failed. (%d)\n", SOCKET_GETSOCKETERRNO());
            SOCKET_FREEADDRINFO(bind_address);
            return 1;
        }

        printf("Binding socket to local address...\n");
        if (bind(socket_listen, bind_address->ai_addr,
                 static_cast<int>(bind_address->ai_addrlen))) {
            fprintf(stderr, "bind() failed. (%d)\n", SOCKET_GETSOCKETERRNO());
            SOCKET_FREEADDRINFO(bind_address);
            return 1;
        }
        // Retrieve the bound address info using getsockname()
        sockaddr_in local_address;
        socklen_t address_length = sizeof(local_address);
        if (getsockname(socket_listen, (sockaddr *)&local_address,
                        &address_length) == 0) {
            char address_buffer[100];
            char service_buffer[30];
            SOCKET_GETNAMEINFO((sockaddr *)&local_address, address_length,
                        address_buffer, sizeof(address_buffer), service_buffer,
                        sizeof(service_buffer),
                        NI_NUMERICHOST | NI_NUMERICSERV);

            printf("Bound to IP: %s, Port: %s\n", address_buffer,
                   service_buffer);
        } else {
            printf("Failed to get local address.\n");
        }
        SOCKET_FREEADDRINFO(bind_address);

        printf("Listening...\n");
        if (listen(socket_listen, 10) < 0) {
            fprintf(stderr, "listen() failed. (%d)\n", SOCKET_GETSOCKETERRNO());
            SOCKET_FREEADDRINFO(bind_address);
            return 1;
        }

        fd_set master;
        FD_ZERO(&master);
        FD_SET(socket_listen, &master);
        SOCKET max_socket = socket_listen;

        printf("Waiting for connections...\n");

        while (true) {
            fd_set reads = master;
            printf("Waiting for activity on %d sockets.\n", static_cast<int>(max_socket + 1));
            int activity = SOCKET_SELECT(max_socket + 1, &reads, 0, 0, NULL);
            if (activity < 0) {
                fprintf(stderr, "select() failed. (%d)\n",
                        SOCKET_GETSOCKETERRNO());
                SOCKET_FREEADDRINFO(bind_address);
                return 1;
            }
            printf("Select detected %d activity.\n", activity);

            SOCKET i;
            for (i = 0; i <= max_socket; ++i) {
                if (FD_ISSET(i, &reads)) {
                    if (i == socket_listen) {
                        struct sockaddr_storage client_address;
                        socklen_t client_len = sizeof(client_address);
                        SOCKET socket_client = accept(
                            socket_listen, (struct sockaddr *)&client_address,
                            &client_len);
                        if (!SOCKET_ISVALIDSOCKET(socket_client)) {
                            fprintf(stderr, "accept() failed. (%d)\n",
                                    SOCKET_GETSOCKETERRNO());
                            SOCKET_FREEADDRINFO(i);
                            return 1;
                        }

                        FD_SET(socket_client, &master);
                        if (socket_client > max_socket)
                            max_socket = socket_client;

                        char address_buffer[100];
                        SOCKET_GETNAMEINFO((struct sockaddr *)&client_address,
                                           client_len, address_buffer,
                                           sizeof(address_buffer), 0, 0,
                                           NI_NUMERICHOST);
                        printf("New connection from %s\n", address_buffer);

                    } else {
                        char read[1024];
                        int bytes_received = SOCKET_RECV(i, read, 1024, 0);
                        if (bytes_received < 1) {
                            FD_CLR(i, &master);
                            SOCKET_CLOSESOCKET(i);
                            continue;
                        }

                        int j;
                        for (j = 0; j < bytes_received; ++j)
                            read[j] = toupper(read[j]);
                        send(i, read, bytes_received, 0);
                    }

                }  // if FD_ISSET
            }      // for i to max_socket
        }          // while(1)

        printf("Closing listening socket...\n");
        SOCKET_CLOSESOCKET(socket_listen);

#if defined(_WIN32)
        WSACleanup();
#endif

        printf("Finished.\n");

        return 0;
    };
};