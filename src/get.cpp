#ifndef DEFS
  #define DEFS
  #include <./include/defs.hpp>
#endif

void http_get(char* buffer,int client_fd){
  std::string URL = extract_url(buffer);
  std::regex echo("^/echo/");
  if(regex_search(URL,echo)){
    size_t c_len = URL.size() - 6;
    std::string c_len_str = std::to_string(c_len)+"\r\n\r\n";
    char get_response[1024] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: ";
    size_t l=strlen(get_response);
    for(char c:c_len_str){
      get_response[l] = c;
      l++;
    }
    get_response[l] = '\0';
    get_echo_str(URL,get_response);
    if(send(client_fd, get_response,sizeof(get_response),0)<0){
      std::cerr<<"Failed to send GET response\n";
    }
  } else if(URL == "/") {                                         
    char response_200[] = "HTTP/1.1 200 OK\r\n\r\n";              
    if(send(client_fd, response_200,strlen(response_200),0)<0){   
      std::cerr<<"Failed to send 200 response\n";                 
    }                                                             
  } else {                                                        
    char response_404[] = "HTTP/1.1 404 Not Found\r\n\r\n";       
    if(send(client_fd, response_404,strlen(response_404),0)<0){   
      std::cerr<<"Failed to send 404 response\n";                 
    } 
  }
}
