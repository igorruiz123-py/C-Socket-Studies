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

    int status;

    struct addrinfo hints, *serverinfo, *p;
    char ipstr[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;


    if ((status = getaddrinfo("google.com", "2100", &hints, &serverinfo)) != 0) {
        fprintf(stderr, "gai error: %s\n", gai_strerror(status));
        exit(1);
    }

    printf("Address returned by the OS: \n\n");

    for (p = serverinfo; p != NULL; p = p->ai_next){

        void * addr;
        char *ip;

        if (p->ai_family == AF_INET){
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ip = "IPv4";
        }
        
        else{

            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ip = "IPv6";
        }

        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        printf("  %s: %s\n", ip, ipstr);
    }

    freeaddrinfo(serverinfo);
    return 0;

}