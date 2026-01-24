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
#include<thread>
using namespace std;

size_t char_len(char *buf);
string extract_url(char *buffer);
void get_echo_str(string& URL,char* get_response);
void http_get(char* buffer,int client_fd);
string extractHeader(char* request, string header);
