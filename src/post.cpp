#ifndef DEFS
  #define DEFS
  #include "./include/defs.hpp"
#endif

int http_post(char *request, int client_fd,int argc, char** argv){
  if(argc<3){
    throw runtime_error("Directory not provided for processing POST request");
    return 1;
  }

  string dir = argv[2];
  string URL = extract_url(request);
  regex files_ep("/files/");

  string filename = URL.substr(7);
    if(regex_search(URL,files_ep)){
    string req_body;
    try{
      req_body = extract_request_body(request);
    } catch(const runtime_error &e){
      cerr<<e.what()<<endl;
    }
    ofstream post_out(dir+"/"+filename);
    post_out << req_body;

    char response_201[] = "HTTP/1.1 201 Created\r\n\r\n";       
    if(send(client_fd, response_201,strlen(response_201),0)<0){                    
      throw runtime_error("Failed to send 404 response");
      return 1;
    } 
  }

  return 0;
}