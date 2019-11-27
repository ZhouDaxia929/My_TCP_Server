#ifndef MTS_NET_BUFFER
#define MTS_NET_BUFFER

#include <vector>
#include <stdint.h>
#include <iostream>
#include <algorithm>

namespace My_TCP_Server{

namespace net{

class Buffer{
public:
    static const uint32_t InitSize;
    
    Buffer(uint32_t size = InitSize);
    ~Buffer();

    int readFromIO(int fd, int &errorNo);

    uint32_t writableBytes();
    uint32_t readableBytes() const;

    bool isReadble(){
        return readableBytes() > 0 ? true : false;
    }

    bool isEmpty(){
        return !isReadble();
    }

    void append(const char *data, std::size_t len);
    void append(const std::string &str);

    void ensureWritableBytes(std::size_t len);

    const uint8_t *readIndexPtr() const{
        return bufferPtr() + readIndex;
    }

    void resetIndex(){
        readIndex = 0;
        writeIndex = 0;
    }

    //根据实际读的长度，去更新readIndex的值。
    void clearReadIndex(uint32_t len){
        if(len < readableBytes()){
            readIndex += len;
        }
        else{
            resetIndex();
        }
    }

    int readAsString(uint32_t len, std::string &readBuf){
        if(len > readableBytes()){
            len = readableBytes();
        }
        readBuf.assign((const char *)readIndexPtr(), len);
        clearReadIndex(len);
        return len;
    }

    int readAllAsString(std::string &readBuf){
        return readAsString(readableBytes(), readBuf);
    }

    int readBytes(std::vector<uint8_t> &data, uint32_t len){
        if(len > readableBytes()){
            len = readableBytes();
        }
        data.assign(readIndexPtr(), readIndexPtr() + len);
        clearReadIndex(len);
        return len;
    }

    int readAllAsByte(std::vector<uint8_t> &data){
        return readBytes(data, readableBytes());
    }

    void swap(Buffer &rhs);

    const char *findCRLF() const{
        char kCRLF[] = "\r\n";
        const char *crlf = std::search((char *)readIndexPtr(), (char *)bufferPtr() + writeIndex, kCRLF, kCRLF + 2);
        return crlf == ((char *)bufferPtr() + writeIndex) ? nullptr : crlf;
    }

    void retrieveUntil(const char *end){
        if(!(readIndexPtr() <= (uint8_t *)end)){
            std::cout << "error retrieveUntil" << std::endl;
            return;
        }
        if(!(end <= ((char *)bufferPtr() + writeIndex))){
            std::cout << "error retrieveUntil" << std::endl;
            return;
        }
        clearReadIndex((uint8_t *)end - readIndexPtr());
    }


private:
    uint32_t readIndex;
    uint32_t writeIndex;

    std::vector<uint8_t> buffer;

    const uint8_t* bufferPtr() const{
        return &(*buffer.begin());
    }
};

}
}


#endif