//
// Created by 尹彦江 on 24-10-5.
//

#include "LogicSystem.h"
using namespace std;

/**
 * @brief LogicSystem的构造函数, 构造函数将停止信息初始化为 false, 注册消息处理函数
 * 并且启动一个工作线程, 工作线程执行 DealMsg 逻辑
 */
LogicSystem::LogicSystem():
    _bStop{false}{
    RegisterCallBacks();
    _workerThread = std::thread(&LogicSystem::DealMsg, this);
}

LogicSystem::~LogicSystem() {
    _bStop = true;
    _consume.notify_one();
    _workerThread.join();
}

void LogicSystem::PostMsgToQueue(std::shared_ptr<LogicNode> msg) {
    std::unique_lock<std::mutex> uniqueLock(_mutex);
    this->_msgQueue.push(msg);

    // 由 0 变成 1 就发送通知信号
    if(_msgQueue.size() == 1){
        uniqueLock.unlock();
        _consume.notify_one();
    }
}

void LogicSystem::DealMsg() {
    while (true){
        std::unique_lock<std::mutex> uniqueLock(_mutex);
        // 判断队列为空则用条件变量阻塞等待, 并释放锁
        while(_msgQueue.empty() && !_bStop){
            _consume.wait(uniqueLock);
        }

        // 判断是否为关闭状态, 如果是, 就把所有逻辑执行完毕之后退出循环
        if(_bStop){
            while(!_msgQueue.empty()){
                auto msgNode = _msgQueue.front();
                std::cout << "Recv msg ID is: " << msgNode->_recvNode->messageID_ << std::endl;
                auto callbackIter = _funCallBack.find(msgNode->_recvNode->messageID_);
                if(callbackIter == _funCallBack.end()){
                    _msgQueue.pop();
                    continue;
                }
                callbackIter->second(msgNode->_session, msgNode->_recvNode->messageID_,
                                     std::string(msgNode->_recvNode->_data, msgNode->_recvNode->_currentLength));
                _msgQueue.pop();
            }
            break;
        }

        // 如果没有停服, 说明队列中有数据
        auto msgNode = _msgQueue.front();
        std::cout << "Recv message is: " << msgNode->_recvNode->messageID_ << std::endl;
        auto callbackIter = _funCallBack.find(msgNode->_recvNode->messageID_);
        if(callbackIter == _funCallBack.end()){
            _msgQueue.pop();
            continue;
        }
        callbackIter->second(msgNode->_session, msgNode->_recvNode->messageID_,
                             std::string(msgNode->_recvNode->_data, msgNode->_recvNode->_currentLength));
        _msgQueue.pop();
    }
}
void LogicSystem::RegisterCallBacks(){
    _funCallBack[MSG_HELLO_WORD] = std::bind(&LogicSystem::HelloWordCallBack, this,
                                             std::placeholders::_1, std::placeholders::_2,
                                             std::placeholders::_3);
}


void LogicSystem::HelloWordCallBack(std::shared_ptr<CSession> session,
                                    const short &msgID, const string &msgData){
    Json::Reader reader;
    Json::Value root;
    reader.parse(msgData, root);
    std::cout << "Recv msg id is: " << root["id"].asInt() << " msg data is: "
    <<root["data"].asString() << std::endl;
    root["data"] = "server has received msg, msg data is: "  + root["data"].asString();
    std::string returnStr = root.toStyledString();
    session->Send(returnStr, root["id"].asInt());
}

