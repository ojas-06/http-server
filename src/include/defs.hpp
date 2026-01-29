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
#include <zlib.h>

using namespace std;

size_t char_len(char *buf);
string extract_url(char *buffer);
ssize_t echoResponse(string& URL,char* get_response, int compress);
string extractHeader(char* request, string header);
void nf_404(int client_fd);
string extract_request_body(char * request);
void str_to_bytef(string in_buf,vector<Bytef> &out_buf) ;
int compressBuffer(const std::vector<Bytef>& input, vector<Bytef>& compressed);

int http_get(char*,int,int, char **,string);
int http_post(char *,int,int,char **,string);