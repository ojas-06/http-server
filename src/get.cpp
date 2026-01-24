#ifndef DEFS
  #define DEFS
  #include "./include/defs.hpp"
#endif

void http_get(char* request,int client_fd){
  string URL = extract_url(request);
  regex echo("^/echo/");
  regex userAgent("^/user-agent");

  if(regex_search(URL,echo)){
    size_t c_len = URL.size() - 6;
    string c_len_str = to_string(c_len)+"\r\n\r\n";
    char echo_response[1024] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: ";
    size_t l=strlen(echo_response);
    for(char c:c_len_str){
      echo_response[l] = c;
      l++;
    }
    echo_response[l] = '\0';
    get_echo_str(URL,echo_response);
    if(send(client_fd, echo_response,strlen(echo_response),0)<0){
      cerr<<"Failed to send GET response\n";
    }
  } 
  else if(regex_search(URL,userAgent)){
    string headerContent = extractHeader(request, "User-Agent");
    if(headerContent == "void"){
      cerr<<"No 'User-Agent' header present in HTTP request\n";
      return;
    }
    char ua_response[1024] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: ";
    size_t i = strlen(ua_response);
    string contentSize = to_string(headerContent.size())+"\r\n\r\n";
    for(char c:contentSize){
      ua_response[i] = c;
      i++;
    }
    for(char c:headerContent) ua_response[i++] = c;
    if(send(client_fd, ua_response,strlen(ua_response),0)<0){   
      cerr<<"Failed to send 200 response\n";                 
    }  
  }
  else if(URL == "/") {                                         
    char response_200[] = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n";              
    if(send(client_fd, response_200,strlen(response_200),0)<0){   
      cerr<<"Failed to send 200 response\n";                 
    }                                                             
  } else {                                                        
    char response_404[] = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";       
    if(send(client_fd, response_404,strlen(response_404),0)<0){   
      cerr<<"Failed to send 404 response\n";                 
    } 
  }
}
