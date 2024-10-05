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
    _totalLength(maxLength + HEAD_TOTAL_LEN),_currentLength{0}
{
    _data = new char[_totalLength + 1];
    memset(_data, 0,_totalLength + 1);
    // 转为网络字节序
    int maxLengthHost = boost::asio::detail::socket_ops::host_to_network_short(maxLength);
    memcpy(_data, &maxLengthHost, HEAD_TOTAL_LEN); // 头部内容复制

    memcpy(_data + HEAD_TOTAL_LEN, msg, maxLength); // 数据内容复制

    _data[_totalLength] = '\0';
}
MsgNode::~MsgNode(){
    delete[] _data;
}

RecvNode::RecvNode(short maxLength, short messageID):
        MsgNode(maxLength),messageID_{messageID}{

}

SendNode::SendNode(const char *message, short maxLength, short messageID):
        MsgNode(maxLength + HEAD_TOTAL_LEN), messageID_{messageID}{
    // 先发送 ID, 转换成网络字节序
    short trueMessageID = boost::asio::detail::socket_ops::host_to_network_short(messageID);
    memcpy(_data, &trueMessageID, HEAD_ID_LEN);

    // 转换成网络字节序
    short maxLengthHost = boost::asio::detail::socket_ops::host_to_network_short(maxLength);
    memcpy(_data + HEAD_ID_LEN, &maxLengthHost, HEAD_DATA_LEN);
    memcpy(_data + HEAD_TOTAL_LEN,message, maxLength);
}


