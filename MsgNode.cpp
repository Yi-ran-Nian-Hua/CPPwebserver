//
// Created by 尹彦江 on 24-10-3.
//

#include "MsgNode.h"

void MsgNode::Clear() {
    memset(_data, 0, _totalLength);
}

MsgNode::MsgNode(short maxLength):
        _currentLength{0}, _totalLength{maxLength}
{
    _data = new char[_totalLength + 1];
    memset(_data, 0, _totalLength + 1);
}

MsgNode::MsgNode(const char *msg, short maxLength):
    _totalLength(maxLength + HEAD_LENGTH),_currentLength{0}
{
    _data = new char[_totalLength + 1];
    memset(_data, 0,_totalLength + 1);
    // 转为网络字节序
    int maxLengthHost = boost::asio::detail::socket_ops::host_to_network_short(maxLength);
    memcpy(_data, &maxLengthHost, HEAD_LENGTH); // 头部内容复制

    memcpy(_data + HEAD_LENGTH, msg, maxLength); // 数据内容复制

    _data[_totalLength] = '\0';
}
MsgNode::~MsgNode(){
    delete[] _data;
}

