//
// Created by 尹彦江 on 24-10-3.
//

#ifndef MYLOGICSERVER_CSESSION_H
#define MYLOGICSERVER_CSESSION_H
#include <boost/asio.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <queue>
#include <mutex>
#include <memory>
#include "const.h"

using boost::asio::ip::tcp;
class CServer;
class MsgNode;

class CSession :public std::enable_shared_from_this<CSession>{
public:
    CSession(boost::asio::io_context& io_context, CServer* server);
    ~CSession();
    tcp::socket & getSocket();
    std::string& getUuid();
    void Start();
    void Send(char* msg, int maxLength);
    void Send(std::string msg);
    void Close();
    std::shared_ptr<CSession> sharedSelf();
private:
    void HandleReadHead(const boost::system::error_code& error, size_t bytes_transferred,
                    std::shared_ptr<CSession> shareSelf);
    void HandleReadMsg(const boost::system::error_code& error, size_t bytes_transferred,
                       std::shared_ptr<CSession> sharedSelf);
    void HandleWrite(const boost::system::error_code& error,
                     std::shared_ptr<CSession> shareSelf);
    boost::asio::ip::tcp::socket _socket;
    std::string _uuid;
    char _data[MAX_LENGTH];
    CServer* _server;
    bool _bClose;
    std::queue<std::shared_ptr<MsgNode>> _sendQue;
    std::mutex _sendBlock;

    // 收到的消息结构
    std::shared_ptr<MsgNode> _recvMsgNode;
    bool _bHeadParsed;
    // 收到的头部结构
    std::shared_ptr<MsgNode> _recvHeadNode;

};


#endif //MYLOGICSERVER_CSESSION_H
