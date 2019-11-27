#ifndef MTS_SOCKET
#define MTS_SOCKET

#include "net/SocketOperation.h"
#include "net/SocketAddr.h"

namespace My_TCP_Server{

namespace net{

class Socket{
public:
    Socket(int fd);
    ~Socket();

    void bind(SocketAddr &addr);
    int accept(SocketAddr &addr);
    void listen();
    
    int getFd();
    void setTcpNoDelay(bool enable);
    int shutdownWrite();
private:
    int socketFd;
};

}
}

#endif