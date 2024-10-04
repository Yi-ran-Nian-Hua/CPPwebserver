//
// Created by 尹彦江 on 24-10-4.
//

#ifndef MYLOGICSERVER_CONST_H
#define MYLOGICSERVER_CONST_H

const int MAX_LENGTH = 1024 * 2; // 定义一个节点包的数据长度最大 1024 * 2 字节
const short int HEAD_LENGTH = 4; // 定义一个节点包的头部长度为 4 字节
const int MAX_RECVQUE = 10000; // 定义接收队列最大数量
const int MAX_SENDQUE = 1000; // 定义发送队列最大数量
const short int HEAD_ID_LEN = 2; // 定义头部 id 长度为 2 字节
const short int HEAD_DATA_LEN = 2; // 定义头部数据长度为 2 字节
#endif //MYLOGICSERVER_CONST_H
