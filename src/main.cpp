#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

size_t char_len(char *buf);

int main(int argc, char **argv) {
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  
  std::cout << "Logs from your program will appear here!\n";
  
  /*
    AF_INET	    -> IPv4
    SOCK_STREAM -> TCP
    0           -> Default protocol for TCP
  */

  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
   std::cerr << "Failed to create server socket\n";
   return 1;
  }
  
  // Since the tester restarts your program quite often, setting SO_REUSEADDR
  // ensures that we don't run into 'Address already in use' errors
  /*
    What ACTUALLY happens when a server closes
    When your server stops:
    The process dies
    BUT the OS keeps the port reserved for a short time
    This tells the OS:

    “I know what I’m doing.
    If this port is in TIME_WAIT, let me reuse it anyway.”
  */
  int reuse = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
    std::cerr << "setsockopt failed\n";
    return 1;
  }
  
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  /*
    Network byte order = Big-endian
    When a number needs more than 1 byte, which byte first?
    Big-endian - big byte first, Little-endian

    htons -> host to network short
    htonl -> long
    ||ly ntohs,ntohl

    (IP address,port ) pair should be unique... this case client and server are on the same machine so 
    server -> fixed port(4221)
    client -> random aval port no (OS assigns)
  */
  server_addr.sin_port = htons(4221);
  
  if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr))!=0) {
    std::cerr << "Failed to bind to port 4221\n";
    return 1;
  }
  
  int connection_backlog = 5;
  //turns a socket into listening socket
  // details skipped(backlg?)
  if (listen(server_fd, connection_backlog) != 0) {
    std::cerr << "listen failed\n";
    return 1;
  }
  
  struct sockaddr_in client_addr;
  int client_addr_len = sizeof(client_addr);
  
  std::cout << "Waiting for a client to connect...\n";
  
  int client_fd = accept(server_fd, (struct sockaddr *) &client_addr, (socklen_t *) &client_addr_len);
  if(client_fd < 0) {
    std::cerr<<"Client-side connection failed\n";
    return 1;
  }
  std::cout << "Client connected\n";
  
  char response_ok[] = "HTTP/1.1 200 OK\r\n\r\n";
  size_t len_res = char_len(response_ok);
  /*
    ssize_t send(int socket, const void *buffer, size_t length, int flags);
    size_t -> unsigned int based on archi(auto 32/64bit assign)
    ssize_t -> signed version of size_t
  */

  send( client_fd , response_ok , len_res ,MSG_DONTROUTE);


  /*
    Normally, Keep server_fd open
    Close client_fd after handling request
  */
  close(server_fd);

  return 0;
}

size_t char_len(char *buf){
  size_t len = 1;
  while(buf[len-1] != '\0') len++;
  return len;
}


/*
int socket(int, int, int);

int bind(int, const sockaddr*, socklen_t);

int listen(int, int);

int accept(int, sockaddr*, socklen_t*);

ssize_t recv(int, void*, size_t, int);

ssize_t send(int, const void*, size_t, int);

int close(int);
*/
