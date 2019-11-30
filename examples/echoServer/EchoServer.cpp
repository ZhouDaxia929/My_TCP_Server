#include "EchoServer.h"
#include "base/LogInterface.h"

#include <iostream>

using namespace My_TCP_Server::net;
using namespace std;

EchoServer::EchoServer(IOEventLoop *loop, SocketAddr &addr) : TcpServer(loop, addr){}

void EchoServer::connectCallback(std::shared_ptr<My_TCP_Server::net::TcpConnect> tcpConnect){
    cout << "new connect:" << tcpConnect->getAddr().toString() << "<count>" << getConnectCount() << endl;
}

void EchoServer::messageCallback(std::shared_ptr<My_TCP_Server::net::TcpConnect> tcpConnect, My_TCP_Server::net::Buffer &buffer){
    cout << "thread id:" << std::this_thread::get_id() << endl;
    string addr = tcpConnect->getAddr().toString();
    string data;
    buffer.readAllAsString(data);
    cout << "receive data from " << addr << ":" << data << endl;
    base::Log::OUT(base::Log::Info) << "receive data form " << addr << ":" << data;
    tcpConnect->write(data);
}

void EchoServer::writeCompleteCallback(std::shared_ptr<My_TCP_Server::net::TcpConnect> tcpConnect){
    cout << "thread id:" << std::this_thread::get_id() << endl;
    string addr = tcpConnect->getAddr().toString();
    cout << addr << ":" << "write complete" << endl;
    base::Log::OUT(base::Log::Info) << addr << ":" << "write complete";
}

void EchoServer::connectCloseCallback(std::shared_ptr<My_TCP_Server::net::TcpConnect> tcpConnect){
    string addr = tcpConnect->getAddr().toString();
    cout << "close connect: " << addr << endl;
    base::Log::OUT(base::Log::Info) << "close connect : " << addr;
}