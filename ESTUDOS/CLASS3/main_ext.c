// Difference between structs "addrinfo" and "sockaddr_in"

/*

*sockaddr_in* (low level)

It describe a specific IPv4 address

struct sockaddr_in (It represents a specific IPv4 ready to be used straight in the kernel)

struct sockaddr_in {
    sa_family_t    sin_family; // AF_INET
    in_port_t      sin_port;   // port (network byte order)
    struct in_addr sin_addr;   // IP IPv4
};

It is used when we want only to work with IPv4, set the port and IP manually and call directlly bind() and listen()

Example:

struct sockaddr_in addr;

addr.sin_family = AF_INET;
addr.sin_port = htons(2100);
inet_pton(AF_INET, "192.168.1.10", &addr.sin_addr);

bind(sockfd, (struct sockaddr *)&addr, sizeof addr);


*/

/*

*addrinfo* (high level)

It represents a complete description on how to create and use a socket.

It contains:

1. Family (AF_INET, AF_INET6)
2. Type (SOCK_STREAM, SOCK_DGRAM)
3. Protocol (TCP, UDP)
4. Generic pointer 
5. Pointer to next

struct addrinfo {
    int              ai_family;
    int              ai_socktype;
    int              ai_protocol;
    size_t           ai_addrlen;
    struct sockaddr *ai_addr;
    struct addrinfo *ai_next;
};


*/