//
// Created by 尹彦江 on 24-10-5.
//

#ifndef MYJSONSERVER_LOGICSYSTEM_H
#define MYJSONSERVER_LOGICSYSTEM_H
#include "Signleton.h"
#include <queue>
#include <thread>
#include "CSession.h"
#include <map>
#include <functional>
#include "const.h"
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include "LogicNode.h"


class CSession;
class LogicNode;
typedef function<void(shared_ptr<CSession>, const short &msgID,
        const string &msgData)> FunCallBack; // 要注册的回调函数类型, 参数为会话类的智能指针, 消息 id 以及消息内容
class LogicSystem :public Signleton<LogicSystem>{
    friend class Signleton<LogicSystem>; // 将父类声明为友元,方便访问子类的私有构造函数
public:
    ~LogicSystem();
    void PostMsgToQueue(shared_ptr<LogicNode> msg);
private:
    LogicSystem();
    void DealMsg();
    void RegisterCallBacks();
    void HelloWordCallBack(std::shared_ptr<CSession>, const short &msgID, const string &msgData);
    std::thread _workerThread; // 工作线程, 用来从逻辑队列中取出数据并且执行回调函数
    std::queue<std::shared_ptr<LogicNode>> _msgQueue; // 逻辑队列
    std::mutex _mutex; // 保证逻辑队列安全的互斥量
    std::condition_variable _consume; // 消费者条件变量, 用来控制当逻辑队列为空的时候保证线程暂时挂起等待, 不要干扰其他线程
    bool _bStop; // 表示收到外部的停止信号, 逻辑类要终止工作线程并退出
    std::map<short, FunCallBack > _funCallBack; // 回调函数的 map, 根据 id 查找对应的逻辑处理函数
};


#endif //MYJSONSERVER_LOGICSYSTEM_H
