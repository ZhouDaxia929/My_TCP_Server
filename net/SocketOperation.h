#ifndef MTS_SOCKET_OPERATION
#define MTS_SOCKET_OPERATION

#include <sys/types.h>
#include <string>
#include <netinet/in.h>
#include <linux/tcp.h>

namespace My_TCP_Server{

namespace net{

class SocketOperation{
public:
    static const int32_t Ipv4AddrAny;
    static int createNonblockingSocket();
    static int bind(int sockfd, const struct sockaddr_in *addr); 
    static int listen(int sockfd);
    static int connect(int sockfd, const struct sockaddr *addr);
    static int accept(int sockfd, const struct sockaddr_in *addr);
    static ssize_t read(int sockfd, void *buf, size_t count);
    static ssize_t readv(int sockfd, const struct iovec *iov, int iovcnt);
    static ssize_t write(int sockfd, const void *buf, size_t count);
    static void close(int sockfd);
    static void getAddrAnyIpv4(struct sockaddr_in &addrIn, uint16_t port);
    /*
     * 对于整个的IP:port的socket字符串，将其拆分并配置成一个sockaddr_in
     */
    static bool toAddrIpv4(const std::string &addrIp, struct sockaddr_in &addrIn);
    /*
     * 将参数给出的点分十进制的ip和端口号配置成 sockaddr_in
     */
    static bool toAddrIpv4(const std::string &addr, uint16_t port, struct sockaddr_in &addrIn);
    static bool toAddrIpv4(uint16_t port, struct sockaddr_in &addrIn);
    static std::string ipToString(struct sockaddr_in addr);
    static std::string toString(struct sockaddr_in addr);

    template<typename T>
    static bool stringToInt(const std::string &str, T &num);

    static int shutdownWrite(int sockfd);
    static void setTcpNoDelay(int fd, bool isEnable);
};


}
}


#endif