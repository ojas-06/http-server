#ifndef DEFS
  #define DEFS
  #include "./include/defs.hpp"
#endif

int http_get(string request,int client_fd ,int argc, char **argv,string compression){
  string URL = extractURL(request);
  regex echo("^/echo/");
  regex userAgent("^/user-agent");
  regex files("^/files/");

  if(regex_search(URL,echo)){
    string echo_response;
    bool compress = false;
    if(compression.find("gzip") != string::npos) compress = true;
    ssize_t responseSize;
    if(compress) {
      responseSize = echoResponse(URL,echo_response,1); //1 -> gzip
    }
    else{
      responseSize = echoResponse(URL,echo_response,0); //0 -> no compression
    }

    if(responseSize<0){
      throw runtime_error("Failed to generate echo response");
    }; 

    if(send(client_fd, echo_response.data(),responseSize,0)<0){
      throw runtime_error("Failed to send GET response");
    }
  } 
  else if(regex_search(URL,files)){
    if(argc < 3) {
      throw runtime_error("Directory not provided for processing GET request");  
      return 1;
    }
    string dir = argv[2];
    string filename = URL.substr(7);
    if(std::filesystem::exists(dir+"/"+filename)){
      char response[1024] = "HTTP/1.1 200 OK\r\nContent-Type: application/octet-stream\r\nContent-Length: ";
      ifstream files(dir+"/"+filename,std::ios::binary);
      if(!files.is_open()){
        throw runtime_error("File exists but could not open");
        return 1;
      }

    std::ostringstream file_content;
    file_content << files.rdbuf();
    string content_str = file_content.str();

    int i=strlen(response);
    size_t c_size = content_str.size();
    for(char c: to_string(c_size) + "\r\n\r\n"){
      response[i] = c;
      i++;
    }
    for(char c:content_str){
      response[i] = c;
      i++;
    }

    if(send(client_fd,response,strlen(response),0)<0){
      throw runtime_error("Unable to send to client");
      return 1;
    }
  }
  else{
    nf_404(client_fd);
  }

  }
  else if(regex_search(URL,userAgent)){
    string headerContent;
    try{
      headerContent = extractHeader(request, "User-Agent");
    } catch(const runtime_error &e){
      cerr<<e.what()<<endl;
    }
    if(!headerContent.size()){
      throw runtime_error("No 'User-Agent' header present in HTTP request");
      return 1;
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
    nf_404(client_fd);
  }
  return 0;
}