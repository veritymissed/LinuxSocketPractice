#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <time.h>
#include <string>

#include <iostream>
#define PORT "8889"

using namespace std;
int client_connect(const char* host,const char* port);

int main(int argc,const char* argv[]) {
  int socketFd;
  char targetHost[20];

  cout<<"Input the hostname or IP address!"<<endl;
  cin>>targetHost;
  if ( ( socketFd = client_connect(targetHost,PORT) )<0 ){
    cout<<"Connect error to "<<targetHost<<endl;
    return 0;
  }
  cout<<"Connect established!"<<endl;

  int sizeRecv = 10000;
  int sizeSend = 100;
  char recvBuf[sizeRecv];
  char sendBuf[sizeSend];
  ///Send and recv loop
  while (1) {
    if( recv(socketFd,recvBuf,sizeRecv,0) < 0 ){
      cout<<"Receive message error"<<endl;
      return 0;
    }
    cout<<"Server message: "<<recvBuf<<endl;

    cout<<"Input:"<<endl;
    cin>>sendBuf;

    if( send(socketFd,sendBuf,sizeSend,0) < 0 ){
      cout<<"Send sendBuf error"<<endl;
      return 0;
    }
  }
  ///
  return 0;
}

int client_connect(const char* host,const char* port){
  struct addrinfo hints,*res;
  int sd;

  memset(&hints,0,sizeof(hints));
  hints.ai_family=AF_UNSPEC;
  hints.ai_socktype=SOCK_STREAM;

  getaddrinfo(host,port,&hints,&res);

  char ipStr[20];
  void *addr;
  struct sockaddr_in *ipv4 = (struct sockaddr_in*)res->ai_addr;
  addr = &(ipv4->sin_addr);
  inet_ntop(res->ai_family,addr,ipStr,sizeof(ipStr));

  if( ( sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol) ) ==-1 ){
    cout<<"Error occured when open socket"<<endl;
    return -1;
  }

  if(connect(sd, res->ai_addr, res->ai_addrlen)==-1){
    cout<<"Error occured when connect "<<host<<endl;
    return -1;
  };

  return sd;
};
