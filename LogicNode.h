//
// Created by 尹彦江 on 24-10-5.
//

#ifndef MYJSONSERVER_LOGICNODE_H
#define MYJSONSERVER_LOGICNODE_H
#include "CSession.h"
#include "MsgNode.h"

class CSession;
class RecvNode;
class LogicNode {
    friend class LogicSystem;
public:
    LogicNode(std::shared_ptr<CSession> session, std::shared_ptr<RecvNode> recvNode);
private:
    std::shared_ptr<CSession> _session;
    std::shared_ptr<RecvNode> _recvNode;

};


#endif //MYJSONSERVER_LOGICNODE_H
