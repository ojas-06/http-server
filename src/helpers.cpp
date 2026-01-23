#ifndef DEFS
  #define DEFS
  #include "./include/defs.hpp"
#endif

size_t char_len(char *buf){
  size_t len = 1;
  while(buf[len-1] != '\0') len++;
  return len;
}

void close_connection(int server,int client){
  close(client);
  close(server);
}

std::string extract_url(char *buffer){
  std::string URL;
  int loc=0;
  while(buffer[loc]!='/') loc++;
  
  while(buffer[loc]!=' ') {
    URL += buffer[loc];
    loc++;
  }
  return URL;
}

void get_echo_str(std::string& URL,char* get_response){
  int i=6;
  size_t l = strlen(get_response);
  while(i<URL.size()){
    get_response[l] = URL[i];
    i++;
    l++;
  }
  get_response[l] = '\0';
}