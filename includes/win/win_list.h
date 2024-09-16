#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>


#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

class Win {
   public:
    static int ListAdapters() {
        // Initialize Winsock
        WSADATA d;
        if (WSAStartup(MAKEWORD(2, 2), &d)) {
            printf("Failed to initialize.\n");
            return -1;
        }

        DWORD asize = 20000;  // Initial size for adapter addresses buffer
        PIP_ADAPTER_ADDRESSES adapters;

        do {
            // Allocate memory for adapter addresses
            adapters = (PIP_ADAPTER_ADDRESSES)malloc(asize);
            if (!adapters) {
                printf("Couldn't allocate %ld bytes for adapters.\n", asize);
                WSACleanup();
                return -1;
            }

            // Get adapter addresses
            int r =
                GetAdaptersAddresses(AF_UNSPEC,  // Both IPv4 and IPv6
                                     GAA_FLAG_INCLUDE_PREFIX,  // Include prefix
                                     0,                        // Reserved
                                     adapters,                 // Buffer
                                     &asize);                  // Buffer size

            if (r == ERROR_BUFFER_OVERFLOW) {
                // Buffer too small, free memory and try again with suggested
                // size
                printf("GetAdaptersAddresses wants %ld bytes.\n", asize);
                free(adapters);
            } else if (r == ERROR_SUCCESS) {
                // Success, break out of the loop
                break;
            } else {
                // Other error occurred
                printf("Error from GetAdaptersAddresses: %d\n", r);
                free(adapters);
                WSACleanup();
                return -1;
            }
        } while (!adapters);  // Loop until successful or unrecoverable error

        // Iterate through each adapter
        PIP_ADAPTER_ADDRESSES adapter = adapters;
        while (adapter) {
            printf("\nAdapter name: %S\n", adapter->FriendlyName);

            // Iterate through each IP address for this adapter
            PIP_ADAPTER_UNICAST_ADDRESS address = adapter->FirstUnicastAddress;
            while (address) {
                // Determine if address is IPv4 or IPv6
                printf("\t%s", address->Address.lpSockaddr->sa_family == AF_INET
                                   ? "IPv4"
                                   : "IPv6");

                char ap[100];
                // Convert IP address to string representation
                getnameinfo(address->Address.lpSockaddr,
                            address->Address.iSockaddrLength, ap, sizeof(ap), 0,
                            0, NI_NUMERICHOST);
                printf("\t%s\n", ap);

                address = address->Next;  // Move to next address
            }

            adapter = adapter->Next;  // Move to next adapter
        }

        // Clean up
        free(adapters);
        WSACleanup();
        return 0;
    }
};