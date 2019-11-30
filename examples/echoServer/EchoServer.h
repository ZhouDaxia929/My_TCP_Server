#ifndef MTS_DEMO_ECHOSERVER
#define MTS_DEMO_ECHOSERVER

#include "net/TcpServer.h"

class EchoServer : public My_TCP_Server::net::TcpServer{
public:
    EchoServer(My_TCP_Server::net::IOEventLoop *loop, My_TCP_Server::net::SocketAddr &addr);

    virtual void connectCallback(std::shared_ptr<My_TCP_Server::net::TcpConnect> tcpConnect);
    virtual void messageCallback(std::shared_ptr<My_TCP_Server::net::TcpConnect>, My_TCP_Server::net::Buffer &);
    virtual void writeCompleteCallback(std::shared_ptr<My_TCP_Server::net::TcpConnect> tcpConnect);
    virtual void connectCloseCallback(std::shared_ptr<My_TCP_Server::net::TcpConnect> tcpConnect);
};


#endif