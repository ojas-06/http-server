#ifndef DEFS
  #define DEFS
  #include "./include/defs.hpp"
#endif

string extractURL(string& buffer){
  string URL;
  int loc=0;
  while(buffer[loc]!='/') loc++;
  
  while(buffer[loc]!=' ') {
    URL += buffer[loc];
    loc++;
  }
  return URL;
}

ssize_t echoResponse(string& URL,string& response,int compress){
  string uncomp = URL.substr(6);
  if(!compress){
    response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: ";
    for(char c: to_string(uncomp.size())+"\r\n\r\n"){
      response += c;
    }
    for(char c:uncomp){
      response += c;
    }
  } else if(compress == 1){
    response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Encoding: gzip\r\nContent-Length: ";
    vector<Bytef> uncomp_bytef;
    str_to_bytef(uncomp, uncomp_bytef);
    vector<Bytef> compressed;
    int status = compressBuffer(uncomp_bytef,compressed);

    if(status) {
      return -1;
    }
    for(char c: to_string(compressed.size())+"\r\n\r\n"){
      response += c;
    }
    for(auto c:compressed){
      response += c;
    }
  }
  return response.size();
}

string extractHeader(string& req, string header){
  string content;
  int i = req.find(header);
  if(i==string::npos) {
    return "";
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

string extract_request_body(string& req_str){
  int index = req_str.find("\r\n\r\n");
  if(index == string::npos){
    throw runtime_error("Failed to extract request body");
    return "";
  }
  index += 4;

  return req_str.substr(index);
}

void str_to_bytef(string& in_buf,vector<Bytef> &out_buf) {
  size_t n = in_buf.size();
  for(size_t i=0;i<n;i++){
    out_buf.push_back(in_buf[i]);
  }
}

int compressBuffer(const std::vector<Bytef>& input,vector<Bytef>& compressed) {
  z_stream zs{};
  if (deflateInit2(&zs, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15 + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK) return 1;

  Bytef outbuf[32768];
  zs.next_in = const_cast<Bytef*>(input.data());
  zs.avail_in = input.size();

  int ret;
  do {
    zs.next_out = outbuf;
    zs.avail_out = sizeof(outbuf);
    ret = deflate(&zs, Z_FINISH);
    compressed.insert(compressed.end(), outbuf, outbuf + (sizeof(outbuf) - zs.avail_out));
  } while (ret == Z_OK);

  deflateEnd(&zs);
  return (ret == Z_STREAM_END) ? 0 : 1;
}