#ifndef DEFS
  #define DEFS
  #include "./include/defs.hpp"
#endif

int http_post(string request, int client_fd,int argc , char** argv,string compression){
  if(argc<3){
    throw runtime_error("Directory not provided for processing POST request");
    return 1;
  }

  string dir = argv[2];
  string URL = extractURL(request);

  string filename = URL.substr(7);
    if(URL.substr(0,7) == "/files/"){
    string req_body;
    try{
      req_body = extract_request_body(request);
    } catch(const runtime_error &e){
      cerr<<e.what()<<endl;
    }
    ofstream post_out(dir+"/"+filename);
    post_out << req_body;

    string response_201 = "HTTP/1.1 201 Created\r\n\r\n";       
    if(send(client_fd, response_201.data(),response_201.size(),0)<0){                    
      throw runtime_error("Failed to send 404 response");
      return 1;
    } 
  }

  return 0;
}