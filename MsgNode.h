//
// Created by 尹彦江 on 24-10-3.
//

#ifndef MYLOGICSERVER_MSGNODE_H
#define MYLOGICSERVER_MSGNODE_H

#include <string>
#include "const.h"
#include <iostream>
#include <boost/asio.hpp>
#include "LogicSystem.h"

class MsgNode {
    friend class CSession;
public:
    MsgNode(const char* msg, short maxLength);
    MsgNode(short maxLength);
    ~MsgNode();
    void Clear();

    short _currentLength;
    short _totalLength;
    char* _data;

};

class RecvNode : public MsgNode{
    friend class LogicSystem;
public:
    RecvNode(short maxLength, short messageID);
private:
    short messageID_;
};

class SendNode: public MsgNode{
    friend class LogicSystem;
public:
    SendNode(const char* message, short maxLength, short messageID);
private:
    short messageID_;
};


#endif //MYLOGICSERVER_MSGNODE_H
