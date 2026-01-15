#include <sys/types.h> // basic system types (needed in sockets)
#include <sys/socket.h> // sockets definition (AF_INET, SOCK_STREAM)
#include <netdb.h> // struct getaddrinfo()
#include <netinet/in.h> // struct sockaddr_in and sockaddr_in6
#include <string.h> // for using memset()

#define MYPORT "3490"  // the port users will be connecting to
#define BACKLOG 10     // how many pending connections queue holds


// socket() / bind() / connect() / listen() / accept() / send() and recv()

// socket() — Get the File Descriptor!

/*

Sockets are endpoints of communication that allow two processes to communicate,
it is like a special file that can send and receive data through the internet 

Function signature: int socket(int domain, int type, int protocol); 

1. Domain -> IPv4 (AF_INET) or IPv6 (AF_INET6)
2. type -> TCP (SOCT_STREAM) or UDP (SOCK_DGRAM)
3. protocol -> by default 0 for letting the OS decide the type of protocol that it is being used

This function returns a "File Descriptor (fd)" if fd >= 0 (success), else if fd = -1 (faillure)

*/

// bind() — What port am I on?

/*

Once you have a socket, you might have to associate that socket with a port on your local machine. 
The port number is used by the kernel to match an incoming packet to a certain process’s socket descriptor.

Function signature: int bind(int sockfd, struct sockaddr *my_addr, int addrlen);

1. sockfd is the socket file descriptor returned by socket()
2. my_addr is a pointer to a struct sockaddr that contains information about your address, namely, port and IP address
3. addrlen is the length in bytes of that address.


*/

// connect() — Hey, you!

/*

you’re now perusing the section on connect()—how to connect to a remote host.

Function signature: int connect(int sockfd, struct sockaddr *serv_addr, int addrlen); 

1. sockfd is our friendly neighborhood socket file descriptor, as returned by the socket()
2. serv_addr is a struct sockaddr containing the destination port and IP address
3. ddrlen is the length in bytes of the server address structure


*/

// accept() —  "Thank you for calling port 3490.”

/*

What’s going to happen is this: 
someone far far away will try to connect() to your machine on a port that you are listen()ing on. 
Their connection will be queued up waiting to be accept()ed. You call accept() and you tell it to get the pending connection. 
It’ll return to you a brand new socket file descriptor to use for this single connection! 

Function signature: int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen); 

1. sockfd is the listen()ing socket descriptor
2. ddr will usually be a pointer to a local struct sockaddr_storage
3. addrlen is a local integer variable that should be set to sizeof(struct sockaddr_storage)

*/

// send() and recv() — Talk to me, baby!

/*

These two functions are for communicating over stream sockets or connected datagram sockets. 

Function signature: int send(int sockfd, const void *msg, int len, int flags); 

1. sockfd is the listen()ing socket descriptor
2. msg is a pointer to the data you want to send
3. en is the length of that data in bytes
4. Just set flags to 0. 

*/

void server(void){


    // Generic struct to clients IP addresses, big enough for IPv4 and IPv6
    struct sockaddr_storage their_addr;

    // Variable that holds the size of the address struct
    socklen_t addr_size;

    // status variable. It will hold 0 for success
    int status;

    // hints: (IPv4 or IPv6) | serverinfo: (linked list returned by the OS) | p: (pointer to iterate the list) 
    struct addrinfo hints, *serverinfo, *p;

    // Message
    char *msg = "Beej was here!";
    int len, bytes_sent;

    // Freeing hints to avoid memory trash
    memset(&hints, 0, sizeof hints);

    // Configuring filters (AF_UNSPEC -> IPv4 or IPv6)
    hints.ai_family = AF_UNSPEC;
    // We want TCP
    hints.ai_socktype = SOCK_STREAM;
    // Fill my IP for me
    hints.ai_flags = AI_PASSIVE;

    // This block does:
    //  1. Resolve DNS (google.com)
    //  2. Checks IPV4 / IPv6 if available
    //  3. Consider type TCP
    //  4. Return a linked list of address
    if ((status = getaddrinfo(NULL, MYPORT, &hints, &serverinfo)) != 0) {
        fprintf(stderr, "gai error: %s\n", gai_strerror(status));
        exit(1);
    }

    int sockfd, new_fd;

    for (p = serverinfo; p != NULL; p = p->ai_next){

        // Make a socket
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

        if (sockfd == -1){
            perror("socket");
            continue;
        }

        // bind it to the port we passed in to getaddrinfo():
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1){
            close(sockfd);
            continue;
        }

        break;

    }


    if (p == NULL) {
        fprintf(stderr, "Failed to bind\n");
        freeaddrinfo(serverinfo);
        return;
    }

    freeaddrinfo(serverinfo);

    listen(sockfd, 1);

    // now accept an incoming connection:
    addr_size = sizeof their_addr;
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);

    if (new_fd == -1) {
        perror("accept");
        close(sockfd);
        return;
    }

    len = strlen(msg);
    bytes_sent = send(new_fd, msg, len, 0);

    close(new_fd);
    close(sockfd);
}

void client(void){

    // status variable. It will hold 0 for success
    int status;

    // hints: (IPv4 or IPv6) | serverinfo: (linked list returned by the OS) | p: (pointer to iterate the list) 
    struct addrinfo hints, *serverinfo, *p;

    // Freeing hints to avoid memory trash
    memset(&hints, 0, sizeof hints);

    // Configuring filters (AF_UNSPEC -> IPv4 or IPv6)
    hints.ai_family = AF_UNSPEC;
    // We want TCP
    hints.ai_socktype = SOCK_STREAM;
    // Fill my IP for me
    hints.ai_flags = AI_PASSIVE;

    // This block does:
    //  1. Resolve DNS (google.com)
    //  2. Checks IPV4 / IPv6 if available
    //  3. Consider type TCP
    //  4. Return a linked list of address
    if ((status = getaddrinfo("192.168.100.249", "3490", &hints, &serverinfo)) != 0) {
        fprintf(stderr, "gai error: %s\n", gai_strerror(status));
        exit(1);
    }

    int sockfd;

    for (p = serverinfo; p != NULL; p = p->ai_next){

        // Make a socket
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

        if (sockfd == -1){
            perror("socket");
            continue;
        }

        // On client side, instead of bind() we use connect() to connect to the server
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            continue;
        }

        break;

    }

    freeaddrinfo(serverinfo);

    char *msg = "hello server!";
    send(sockfd, msg, strlen(msg), 0);

    char buf[100];
    int numbytes = recv(sockfd, buf, sizeof buf - 1, 0);
    if (numbytes > 0) {
        buf[numbytes] = '\0';
        printf("Client received: %s\n", buf);
    }

    close(sockfd);
}