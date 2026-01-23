#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <regex>

size_t char_len(char *buf);
void close_connection(int server,int client);
std::string extract_url(char *buffer);
void get_echo_str(std::string& URL,char* get_response);
void http_get(char* buffer,int client_fd);
