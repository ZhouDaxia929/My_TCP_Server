#ifndef MTS_NET_TCPCONNECT
#define MTS_NET_TCPCONNECT

#include "net/IOEventLoop.h"
#include "net/Socket.h"
#include "net/Buffer.h"

#include <memory>
#include <functional>

namespace My_TCP_Server{

namespace net{

class TcpConnect : public std::enable_shared_from_this<TcpConnect>{
public:
    using OnMessageCallback = std::function<void (std::shared_ptr<TcpConnect>, Buffer&)>;
    using OnCloseCallback = std::function<void (std::shared_ptr<TcpConnect>)>;
    using OnWriteCompleteCallback = std::function<void (std::shared_ptr<TcpConnect>)>;

    TcpConnect(IOEventLoop *l, struct sockaddr_in addr, int fd);
    ~TcpConnect();
    
    void setMessageCallback(const OnMessageCallback callback);
    void setCloseCallback(const OnCloseCallback callback);
    void setWriteCompleteCallback(const OnWriteCompleteCallback callback);

    const SocketAddr &getAddr() const;
    std::string getName() const;

    void write(const char *data);
    void write(const std::string &data);
    void write(const void *data, uint32_t len);
    void writeInLoop(const void *data, uint32_t len);

    void connectedHandle();

    void setNoDelay(bool enable);
    void shutdownWrite();

private:
    IOEventLoop *loop;
    SocketAddr socketAddr;
    std::string name;
    std::shared_ptr<Socket> socket;
    std::shared_ptr<IOEvent> event;

    void readEvent();
    void closeEvent();
    void writeEvent();
    void errorEvent();
    
    OnMessageCallback messageCallback;
    OnCloseCallback closeCallback;
    OnWriteCompleteCallback writeCompleteCallback;

    Buffer readBuf;
    Buffer writeBuf;
    TcpConnect &getRefer();

    int state;
    enum ConnectState{
        Disconnected, 
        Connecting, 
        Connected, 
        Disconnecting
    };
};

}
}

#endif