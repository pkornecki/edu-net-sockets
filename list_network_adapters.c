// list network adapters

#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    // get the adapters
    struct ifaddrs * addresses;
    if (-1 == getifaddrs(&addresses)) {
        printf("getifaddrs call failed\n");
        return -1;
    }

    // iterate through the (linked) list of adapters
    struct ifaddrs * address = addresses;
    while (address) {
        // we are only interested in the IP protocol
        int family = address->ifa_addr->sa_family;
        if (AF_INET == family || AF_INET6 == family) {
            // print the name
            printf("%s\t", address->ifa_name);
            // print the protocol family
            printf("%s\t", AF_INET == family ? "ipv4" : "ipv6");

            // print the address
            char buffer[100];
            const int family_size = AF_INET == family ?
                sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);
            getnameinfo(address->ifa_addr, family_size, buffer, sizeof(buffer), 0, 0, NI_NUMERICHOST);
            printf("%s\n", buffer);
        }
        // pick the next address
        address = address->ifa_next;
    }

    // free the memory back
    freeifaddrs(addresses);

    return 0;
}
