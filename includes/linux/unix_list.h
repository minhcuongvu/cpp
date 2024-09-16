#ifdef _WIN32
#else
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#endif

class Unix
{
public:
  static int ListAdapters()
  {
    struct ifaddrs *addresses;

    // Get the list of network interfaces
    if (getifaddrs(&addresses) == -1)
    {
      printf("getifaddrs call failed\n");
      return -1;
    }

    // Iterate through the linked list of interfaces
    struct ifaddrs *address = addresses;
    while (address)
    {
      int family = address->ifa_addr->sa_family;

      // Check if the address family is IPv4 or IPv6
      if (family == AF_INET || family == AF_INET6)
      {
        // Print the interface name
        printf("%s\t", address->ifa_name);

        // Print whether it's IPv4 or IPv6
        printf("%s\t", family == AF_INET ? "IPv4" : "IPv6");

        char ap[100];
        // Determine the size of the address structure based on the family
        const int family_size = family == AF_INET ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);

        // Convert the IP address to a string
        getnameinfo(address->ifa_addr,
                    family_size, ap, sizeof(ap), 0, 0, NI_NUMERICHOST);

        // Print the IP address
        printf("\t%s\n", ap);
      }

      // Move to the next address in the linked list
      address = address->ifa_next;
    }

    // Free the memory allocated by getifaddrs
    freeifaddrs(addresses);
    return 0;
  }
};