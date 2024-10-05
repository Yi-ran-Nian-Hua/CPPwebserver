//
// Created by 尹彦江 on 24-10-5.
//

#include "LogicNode.h"
LogicNode::LogicNode(std::shared_ptr<CSession> session, std::shared_ptr<RecvNode> recvNode):
    _session(session), _recvNode(recvNode){}