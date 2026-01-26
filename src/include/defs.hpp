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
#include<filesystem>
#include<fstream>
#include <system_error>


using namespace std;

size_t char_len(char *buf);
string extract_url(char *buffer);
void get_echo_str(string& URL,char* get_response);
string extractHeader(char* request, string header);
void nf_404(int client_fd);
string extract_request_body(char * request);

int http_get(char*,int,int, char **,string);
int http_post(char *,int,int,char **,string);