#ifndef DEFS
  #define DEFS
  #include "./include/defs.hpp"
#endif

int main(int argc, char **argv) {
  cout << unitbuf;
  cerr << unitbuf;
  
  cout << "Logs from your program will appear here!\n";

  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
   cerr << "Failed to create server socket\n";
   return 1;
  }

  int reuse = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
    cerr << "setsockopt failed\n";
    return 1;
  }
  
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;

  server_addr.sin_port = htons(4221);
  
  if (::bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr))!=0) {
    cerr << "Failed to bind to port 4221\n";
    close(server_fd);
    return 1;
  }
  
  int connection_backlog = 5;
  if (listen(server_fd, connection_backlog) != 0) {
    cerr << "listen failed\n";
    close(server_fd);
    return 1;
  }
  
  struct sockaddr_in client_addr;
  int client_addr_len = sizeof(client_addr);
  
  cout << "Waiting for a client to connect...\n";
  
  while(true){
    int client_fd = accept(server_fd, (struct sockaddr *) &client_addr, (socklen_t *) &client_addr_len);
    if(client_fd < 0) {
      cerr<<"Client-side connection failed\n";
      close(client_fd);
      continue;
    }
    cout << "Client connected\n";


    std::thread([client_fd,argc,argv]() {
      string req;
      char buf[1024];
      while (req.find("\r\n\r\n") == string::npos) {
        ssize_t n = recv(client_fd, buf, sizeof(buf), 0);
        if (n <= 0) {
          cerr<<"Recv failed or connection closed\n";
          close(client_fd);
          return;
        }
        req.append(buf, n);
        if (req.size() > 8192) {
          cerr<<"Request headers too large\n";
          close(client_fd);
          return;
        }
      }

      string comp_scheme = extractHeader(req, "Accept-Encoding");
      regex get("^GET "); 
      regex post("^POST ");
      if( req.substr(0,3) == "GET" ){
        try{
          http_get(req,client_fd,argc,argv,comp_scheme);
        } catch(const runtime_error &e){
          cerr<<"Runtime error: "<<e.what()<<endl;
        } 
      }
      else if( req.substr(0,3) == "POST" ){
        try{
          http_post(req,client_fd,argc,argv,comp_scheme);
        } catch(const runtime_error &e){
          cerr<<e.what()<<endl;
        }
      }
      close(client_fd);
    }).detach();

  }
  close(server_fd);

  return 0;
}
