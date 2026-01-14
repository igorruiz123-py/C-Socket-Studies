// getaddrinfo() — Prepare to launch!

/*
Central ideia: Given a name (google.com/IP address) and a service (Port 80), return me all the possible information 
on how to create a valid socket to connect

Function signature:

int getaddrinfo(
    const char *node,      // hostname | NULL for using local IP
    const char *service,   // port or serice name
    const struct addrinfo *hints, // preferences (IPv4 IPv6, TCP/UDP)
    struct addrinfo **res // results list linked by pointers
);


Here’s a sample call if you’re a server who wants to listen
on your host’s IP address, port 3490. Note that this doesn’t
actually do any listening or network setup; it merely sets up structures we’ll use later:

    int status; -> holds the return value if status = 0 (success) else (failure)
    struct addrinfo hints; -> structure used as a filter or preferences
    struct addrinfo *servinfo;  // will point to the results -> it will receive a list of final results 

    memset(&hints, 0, sizeof hints); // make sure the struct is empty -> avoid memory trash
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    if ((status = getaddrinfo(NULL, "3490", &hints, &servinfo)) != 0) {
        fprintf(stderr, "gai error: %s\n", gai_strerror(status));
        exit(1);
    }

NULL -> uses local IP
"3490" -> TCP Port 
&hints -> preferences
&serverinfo -> where to hold results

*/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>


int main(){

    // status variable. It will hold 0 for success
    int status;

    // hints: (IPv4 or IPv6) | serverinfo: (linked list returned by the OS) | p: (pointer to iterate the list) 
    struct addrinfo hints, *serverinfo, *p;

    // Buffer to keep the IP (big enough for IPv6)
    char ipstr[INET6_ADDRSTRLEN];

    // Freeing hints to avoid memory trash
    memset(&hints, 0, sizeof hints);

    // Configuring filters (AF_UNSPEC -> IPv4 or IPv6)
    hints.ai_family = AF_UNSPEC;
    // We wanto TCP
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // This block does:
    //  1. Resolve DNS (google.com)
    //  2. Checks IPV4 / IPv6 if available
    //  3. Consider type TCP
    //  4. Return a linked list of address
    if ((status = getaddrinfo("google.com", "http", &hints, &serverinfo)) != 0) {
        fprintf(stderr, "gai error: %s\n", gai_strerror(status));
        exit(1);
    }

    // Loop to iterate the linked list
    for (p = serverinfo; p != NULL; p = p->ai_next){

        // Auxiliar pointers
        void * addr; // Generic IP for IPv4 and IPv6
        char *ip; // String for IPv4 and IPv6

        // If addreess be IPv4
        if (p->ai_family == AF_INET){
            // Convert generic sockaddr to sockaddr_in  
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            // Takes only IPv4 addresses
            addr = &(ipv4->sin_addr);
            ip = "IPv4";
        }
        
        // Same logic
        else{

            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ip = "IPv6";
        }

        // Convert binary IP to string
        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        // Print output
        printf("  %s: %s\n", ip, ipstr);
    }

    freeaddrinfo(serverinfo);
    return 0;

}