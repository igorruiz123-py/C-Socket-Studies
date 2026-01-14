/*Learning on how to program SOCKETS*/

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

// SERVER SIDE:

/*
CREATING THE SOCKET USING socket():

(This step involves the creation of the socket using the socket() function.)

#include <arpa/inet.h>

parameters: (domain: IPv4/IPv6, type: TCP/UDP, protocol: 0);

1. AF_NET = IPv4
2. AF_NET6 = IPv6
3. SOCK_STREAM = TCP
4. SOCK_DGRAM = UDP
5. PROTOCOL = 0
*/

////////////////////////////////////////////////////////////////////////////////////////////////////

/*
ASSOCIATE IP + PORT USING bind():

After the creation of the socket, the bind() function binds the socket
to the address and port number specified in addr(custom data structure). 

parameters: (sockfd, sockaaddr *addr, socklen_t addrlen);

1. sockfd: socket file descriptor created using the socket() function.
2. addr: pointer to a struct sockaddr that contains the IP address and port number to bind the socket.
3. addrlen: length of the addr structure.

struct used:
struct sockaddr_in addr:
addr.sin_family = AF_INET;
addr.sin_port = htons(8080);
addr.sin_addr.s_addr = INADDR_ANY;
*/

////////////////////////////////////////////////////////////////////////////////////////////////////

/*
LISTEN CONNECTIONS USING listen():

In this step the server uses the listen() 
function that puts the server socket in a passive mode, 
where it waits for the client to approach the server to make a connection.

parameters: (sockfd, backlog)

1. sockfd: socket file descriptor created using the socket() function.
2. backlog: number representing the size of the queue holding the pending connections while the server is waiting to accept a connection.
*/

////////////////////////////////////////////////////////////////////////////////////////////////////

/*
ACCEPT CONNECTION USING accept():

In this step the server extracts the first connection request from the queue of pending connections for the listening socket,
sockfd, creates a new connected socket using the accept() function, 
and returns a new file descriptor referring to that socket. At this point, 
the connection is established between client and server, and they are ready to transfer data.

parameters: (sockfd, sockaddr *addr, socklen_t *addrlen)

1. sockfd: socket file descriptor returned by socket() and bind().
2. addr: pointer to a struct sockaddr that will be hold the client's IP address and port number.
3. addrlen: pointer to a variable that specifies the length of the address structure.
*/

////////////////////////////////////////////////////////////////////////////////////////////////////

/*
SENDING AND RECEIVING DATA TO THE CLIENT USING send() AND recv():

Send(): to send data to the client 

parameters: (sockfd, *buf, len, flags)

1. sockfd: socket file descriptor returned by the socket() function.
2. buf: pointer to the buffer containing the data to be sent.
3. len: number of bytes of data to be sent.
4. flags: integer specifying various options for how the data is sent, typically 0 is used for default behavior.

Recv(): to recieve the data from the client.

parameters: (sockfd, *buf, len, flags)

1. sockfd: socket file descriptor returned by the socket() function.
2. buf: pointer to the buffer containing the data to be sent.
3. len: number of bytes of data to be sent.
4. flags: integer specifying various options for how the data is sent, typically 0 is used for default behavior.
*/

////////////////////////////////////////////////////////////////////////////////////////////////////

/*
CLOSING THE CONNECTION USING close():

After the exchange of information is complete, the server closes the socket using the close() 
function and releases the system resources.

parameters: (sockefd)

1. sockfd: socket file descriptor returned by the socket() function.
*/

//-------------------------------------------------------------------------------------------------------------------------//

// CLIENT SIDE:

/*
SOCKET CREATION USING socket():

This step involves the creation of the socket which is done in the same way as that of server’s socket creation

parameters: (domain, type, protocol)
*/

////////////////////////////////////////////////////////////////////////////////////////////////////

/*
SOCKET CONNECTION USING connect():

The connect() system call connects the socket referred to by the file
descriptor sockfd to the address specified by addr. Server’s address and port is specified in addr.

parameters: (sockfd, *addr, socklen_t addrlen)

1. sockfd: socket file descriptor returned by the socket() function.
2. addr: pointer to struct sockaddr containing the server's IP address and port number.
3. addrlen: size of the addr.
*/

////////////////////////////////////////////////////////////////////////////////////////////////////

/*
SEND AND RECEIVE DATA FROM AND TO THE SERVER USING send() AND recv():

In this step the client can send or receive data from the server which
is done using the send() and recieve() functions similar to how the server sends/recieves data from the client. 
*/

////////////////////////////////////////////////////////////////////////////////////////////////////

/*
CLOSING THE CONNECTION USING close():

After the exchange of information is complete, the server closes the socket using the close() 
function and releases the system resources.

parameters: (sockefd)

1. sockfd: socket file descriptor returned by the socket() function.
*/


int main(){

    //*************SERVER SIDE***************************//

    // Creating the socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Defining bind 
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(2100);
    addr.sin_addr.s_addr = INADDR_ANY;

    // Using bind function
    bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));

    // Listening connections
    listen(sockfd, 1);

    printf("Server listening for connections...\n");

    int client_fd = accept(sockfd, NULL, NULL);

    char buffer[64];

    recv(client_fd, buffer, sizeof(buffer), 0);

    printf("Data package received: %s \n", buffer);

    send(client_fd, "Data package received!", strlen("Data package received!"), 0);

    close(client_fd);
    close(sockfd);
}
