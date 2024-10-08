//
// Created by 尹彦江 on 24-10-3.
//

#include "CSession.h"
#include <iostream>
#include <sstream>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include "CServer.h"
#include "MsgNode.h"

CSession::~CSession() {

}

CSession::CSession(boost::asio::io_context &io_context, CServer *server):
        _socket(io_context), _server(server), _bClose(false), _bHeadParsed(false){
    boost::uuids::uuid aUUid = boost::uuids::random_generator()();
    _uuid = boost::uuids::to_string(aUUid);
    _recvHeadNode = std::make_shared<MsgNode>(HEAD_LENGTH);

}

tcp::socket &CSession::getSocket() {
    return this->_socket;
}

std::string &CSession::getUuid() {
    return this->_uuid;
}

void CSession::Start() {
    memset(_data, 0, MAX_LENGTH);
    boost::asio::async_read(_socket, boost::asio::buffer(_recvHeadNode->_data, HEAD_LENGTH),
                            std::bind(&CSession::HandleReadHead, this,
                                      std::placeholders::_1, std::placeholders::_2,sharedSelf()));
}

void CSession::Send(char *msg, int maxLength) {
    std::lock_guard<std::mutex> lock(_sendBlock);
    int sendQueueSize = _sendQue.size();
    if (sendQueueSize > MAX_SENDQUE){
        std::cerr << "Session: " << _uuid << "send queue failed, size is " << MAX_SENDQUE << std::endl;
        return;
    }

    _sendQue.push(std::make_shared<MsgNode>(msg, maxLength));
    if(sendQueueSize > 0){
        return;
    }
    auto &msgNode = _sendQue.front();
    boost::asio::async_write(_socket, boost::asio::buffer(msgNode->_data, msgNode->_totalLength),
                             std::bind(&CSession::HandleWrite, this, std::placeholders::_1, sharedSelf()));
}

void CSession::Send(std::string msg) {
    std::lock_guard<std::mutex> lock(_sendBlock);
    int sendQueueSize = _sendQue.size();
    if (sendQueueSize > MAX_SENDQUE){
        std::cerr << "Session: " << _uuid << "send queue failed, size is " << MAX_SENDQUE << std::endl;
        return;
    }

    _sendQue.push(std::make_shared<MsgNode>(msg.c_str(), msg.length()));
    if(sendQueueSize > 0){
        return;
    }
    auto &msgNode = _sendQue.front();
    boost::asio::async_write(_socket, boost::asio::buffer(msgNode->_data, msgNode->_totalLength),
                             std::bind(&CSession::HandleWrite, this, std::placeholders::_1, sharedSelf()));

}

void CSession::Close() {
    _socket.close();
    _bClose = true;
}

std::shared_ptr<CSession> CSession::sharedSelf() {
    return shared_from_this();
}


void CSession::HandleWrite(const boost::system::error_code &error, std::shared_ptr<CSession> shareSelf) {
    // 异常处理
    try{
        if(!error){
            std::lock_guard<std::mutex> lock(_sendBlock);
            _sendQue.pop();
            if(!_sendQue.empty()){
                auto &msgNode = _sendQue.front();
                boost::asio::async_write(_socket, boost::asio::buffer(msgNode->_data, msgNode->_totalLength),
                                         std::bind(&CSession::HandleWrite, this, std::placeholders::_1,shareSelf));
            }
        }else{
            std::cerr << "Handle write failed, error is: " << error.what() << std::endl;
            Close();
            _server->ClearSession(_uuid);
        }
    }catch(std::exception& e){
        std::cerr << "Exception code: " << e.what() << std::endl;
    }
}

void CSession::HandleReadHead(const boost::system::error_code &error, size_t bytes_transferred,
                              std::shared_ptr<CSession> shareSelf) {
    if(!error){
        if(bytes_transferred < HEAD_LENGTH){
            std::cerr << "Read Head length error" << std::endl;
            Close();
            _server->ClearSession(this->getUuid());
            return;
        }

        // 头部接收完毕, 解析头部
        short dataLength = 0;
        memcpy(&dataLength, _recvHeadNode->_data, HEAD_LENGTH);
        std::cout << "Data length is: " << dataLength << std::endl;

        // 字节序转换, 将网络字节序转换为本地字节序
        int trueDataLength = boost::asio::detail::socket_ops::network_to_host_short(dataLength);

        // 如果头部长度非法
        if(trueDataLength > MAX_LENGTH){
            std::cerr << "Invalid data length: " << trueDataLength << std::endl;
            _server->ClearSession(this->getUuid());
            return;
        }

        _recvMsgNode = std::make_shared<MsgNode>(trueDataLength);
        boost::asio::async_read(_socket, boost::asio::buffer(_recvMsgNode->_data, _recvMsgNode->_totalLength),
                                std::bind(&CSession::HandleReadMsg, this, std::placeholders::_1, std::placeholders::_2,
                                          shareSelf));
    }
    else{
        std::cerr << "Handle read failed: " << error.what() << std::endl;
        Close();
        _server->ClearSession(this->getUuid());
    }
}

void CSession::HandleReadMsg(const boost::system::error_code &error, size_t bytes_transferred,
                             std::shared_ptr<CSession> sharedSelf) {
    if(!error){
        _recvMsgNode->_data[_recvMsgNode->_totalLength] = '\0';
        std::cout << "Receive data is: " << _recvMsgNode->_data << std::endl;
        Send(_recvMsgNode->_data, _recvMsgNode->_totalLength);
        //再次接收头部数据
        _recvHeadNode->Clear();
        boost::asio::async_read(_socket, boost::asio::buffer(_recvHeadNode->_data, HEAD_LENGTH),
                                std::bind(&CSession::HandleReadHead, this, std::placeholders::_1,
                                          std::placeholders::_2,sharedSelf));
    }else{
        std::cout << "Handle read message failed, error code is: " << error.what() << std::endl;
        Close();
        _server->ClearSession(this->getUuid());
    }
}
