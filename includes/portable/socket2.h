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

class Socket2 {
   public:
    static int main(int argc, char *argv[]) {
#ifdef _WIN32
        WSADATA d;
        if (WSAStartup(MAKEWORD(2, 2), &d)) return 1;
#endif
        if (argc < 3) return 1;

        addrinfo hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_socktype = SOCK_STREAM;
        addrinfo *peer_address;
        const char *host = argv[1];
        const char *port = argv[2];

        if (SOCKET_GETADDRINFO(host, port, &hints, &peer_address)) {
            fprintf(stderr, "getaddrinfo() failed. (%d)\n",
                    SOCKET_GETSOCKETERRNO());
            return 1;
        }

        SOCKET socket_peer = SOCKET_CREATESOCKET(peer_address->ai_family,
                                                 peer_address->ai_socktype,
                                                 peer_address->ai_protocol);
        if (!SOCKET_ISVALIDSOCKET(socket_peer)) return 1;

        if (SOCKET_CONNECT(socket_peer, peer_address->ai_addr,
                           peer_address->ai_addrlen)) {
            fprintf(stderr, "connect() failed. (%d)\n",
                    SOCKET_GETSOCKETERRNO());
            return 1;
        }
        SOCKET_FREEADDRINFO(peer_address);

        while (1) {
            fd_set fd_reads;
            FD_ZERO(&fd_reads);
            FD_SET(socket_peer, &fd_reads);
#ifndef _WIN32
            FD_SET(0, &fd_reads);
#endif
            timeval timeout = {0, 100000};
            if (SOCKET_SELECT(socket_peer + 1, &fd_reads, 0, 0, &timeout) < 0) {
                fprintf(stderr, "select() failed. (%d)\n",
                        SOCKET_GETSOCKETERRNO());
                return 1;
            }

            if (FD_ISSET(socket_peer, &fd_reads)) {
                char readBuffer[4096];
                int byteRecv =
                    SOCKET_RECV(socket_peer, readBuffer, sizeof(readBuffer), 0);
                if (byteRecv < 1) break;
                printf("Received (%d bytes): %.*s\n", byteRecv, byteRecv,
                       readBuffer);
            }

#ifdef _WIN32
            if (_kbhit()) {
#else
            if (FD_ISSET(0, &fd_reads)) {
#endif
                char readBuffer[4096];
                if (!fgets(readBuffer, sizeof(readBuffer), stdin)) break;
                printf("Sending: %.s\n", readBuffer);
                send(socket_peer, readBuffer, sizeof(readBuffer), 0);
            }
        }

        SOCKET_CLOSESOCKET(socket_peer);
#ifdef _WIN32
        WSACleanup();
#endif
        return 0;
    }
};