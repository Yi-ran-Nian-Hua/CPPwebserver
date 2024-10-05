//
// Created by 尹彦江 on 24-10-4.
//

#ifndef MYLOGICSERVER_CONST_H
#define MYLOGICSERVER_CONST_H

const int MAX_LENGTH = 1024 * 2; // 数据总长度
const int HEAD_TOTAL_LEN = 4; // 头部总长度
const int HEAD_ID_LEN = 2; // 头部 ID 部分总长度
const int HEAD_DATA_LEN = 2; // 头部数据部分总长度
const int MAX_RECVQUE = 10000; // 接收队列最大长度
const int MAX_SENDQUE = 10000; // 发送队列最大长度
enum  MSG_IDS{
    MSG_HELLO_WORD = 1001
};
#endif //MYLOGICSERVER_CONST_H
