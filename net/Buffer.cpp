#include "net/Buffer.h"
#include "net/SocketOperation.h"

#include <errno.h>
#include <sys/uio.h>

using namespace My_TCP_Server::net;
using namespace std;

const uint32_t Buffer::InitSize = 1024;

Buffer::Buffer(uint32_t size) : readIndex(0), writeIndex(0), buffer(size){}

Buffer::~Buffer(){}

uint32_t Buffer::writableBytes(){
    return buffer.size() - writeIndex;
}

uint32_t Buffer::readableBytes() const{
    return writeIndex - readIndex;
}

void Buffer::ensureWritableBytes(size_t len){
    if(writableBytes() < len){
        buffer.resize(writeIndex + len);
    }
}

void Buffer::append(const char *data, size_t len){
    ensureWritableBytes(len);
    std::copy(data, data + len, buffer.begin() + writeIndex);
    writeIndex += len;
}

void Buffer::append(const string &str){
    const char *data = &(*str.begin());
    append(data, str.size());
}

int Buffer::readFromIO(int fd, int &errorNo){
    //从fd中读取数据，如果buffer装不下，把剩余的数据放入extrabuf中
    char extrabuf[65536];
    struct iovec vec[2];
    uint32_t writable = writableBytes();
    vec[0].iov_base = &(*buffer.begin()) + writeIndex;
    vec[0].iov_len = writable;
    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof(extrabuf);

    uint32_t iovcnt = (writable < sizeof(extrabuf)) ? 2 : 1;
    int n = SocketOperation::readv(fd, vec, iovcnt);
    if(n < 0){
        errorNo = errno;
    } 
    else if((uint32_t)n <= writable){
        writeIndex += n;
    }
    //将extrabuf中保留的数据追加到buffer中
    else{
        writeIndex = buffer.size();
        append(extrabuf, n - writable);
    }
    return n;
}

void Buffer::swap(Buffer &rhs){
    buffer.swap(rhs.buffer);
    std::swap(readIndex, rhs.readIndex);
    std::swap(writeIndex, rhs.writeIndex);
}

