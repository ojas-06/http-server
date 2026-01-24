#ifndef DEFS
  #define DEFS
  #include "./include/defs.hpp"
#endif

size_t char_len(char *buf){
  size_t len = 1;
  while(buf[len-1] != '\0') len++;
  return len;
}

string extract_url(char *buffer){
  string URL;
  int loc=0;
  while(buffer[loc]!='/') loc++;
  
  while(buffer[loc]!=' ') {
    URL += buffer[loc];
    loc++;
  }
  return URL;
}

void get_echo_str(string& URL,char* get_response){
  int i=6;
  size_t l = strlen(get_response);
  while(i<URL.size()){
    get_response[l] = URL[i];
    i++;
    l++;
  }
  get_response[l] = '\0';
}

string extractHeader(char* request, string header){
  string content;
  string req = request;
  int i = req.find(header);
  if(i==string::npos) {
    return "void";
  }
  int j=i+1;
  while(req[j]!='\r') j++;
  string line = req.substr(i,j-i);

  int k = line.find(':');
  k++;
  while(line[k] == ' ') k++;
  content = line.substr(k);
  return content;
}

void nf_404(int client_fd){
  char response_404[] = "HTTP/1.1 404 Not Found\r\n\r\n";       
  if(send(client_fd, response_404,strlen(response_404),0)<0){   
    cerr<<"Failed to send 404 response\n";                 
  } 
}