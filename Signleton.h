//
// Created by 尹彦江 on 24-10-5.
//

#ifndef MYJSONSERVER_SIGNLETON_H
#define MYJSONSERVER_SIGNLETON_H
#include <memory>
#include <mutex>
#include <iostream>
using namespace std;

template<typename T>
class Signleton{
protected:
    Signleton() = default;
    Signleton(const Signleton<T>&) = delete; // 禁用拷贝构造函数
    Signleton& operator=(const Signleton<T>& st) = delete; // 禁用赋值构造函数

    static std::shared_ptr<T> _instance; // 必须确保只有一个实例生成, 因此声明为静态成员变量
public:
    static std::shared_ptr<T> GetInstance(){
        static std::once_flag Sflag; //
        /**
         * @func @c std::call_once 函数:确保某个函数仅被调用一次,即使在多线程环境下也能够保证这一特性
         * @param flag: 一个@c std::once_flag 的对象,
         * 用于标记某个特定的函数是否已经被调用过,这个对象在多个线程中共享,以协调各个线程对目标函数的调用
         * @param func: 要被调用的函数,可以是函数指针, 可以是成员函数, 或者其他可调用对象(lambda 表达式, 仿函数)
         * @param args...: 要传递给@c func 的参数
         */
        std::call_once(Sflag,[&](){
            _instance = shared_ptr<T>(new T);
        });

        return _instance;
    }

    void PrintAddress(){
        std::cout << this->GetInstance() << std::endl;
    }

    ~Signleton(){
        std::cout << "This is Signleton destruct" << std::endl;
    }
};

template <typename T>
std::shared_ptr<T> Signleton<T>::_instance = nullptr; // 初始化为空指针, 等待后面初始化

#endif //MYJSONSERVER_SIGNLETON_H
