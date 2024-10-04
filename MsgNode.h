//
// Created by 尹彦江 on 24-10-3.
//

#ifndef MYLOGICSERVER_MSGNODE_H
#define MYLOGICSERVER_MSGNODE_H
#include "CSession.h"

class MsgNode {
    friend class CSession;
public:
    MsgNode(const char* msg, short maxLength);
    MsgNode(short maxLength);
    ~MsgNode();
    void Clear();
private:
    short _currentLength;
    short _totalLength;
    char* _data;

};


#endif //MYLOGICSERVER_MSGNODE_H
