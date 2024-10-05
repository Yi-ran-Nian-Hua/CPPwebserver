//
// Created by 尹彦江 on 24-10-5.
//

#ifndef MYJSONSERVER_ASIOIOSERVICEPOOL_H
#define MYJSONSERVER_ASIOIOSERVICEPOOL_H

#include <vector>
#include <boost/asio.hpp>
#include "Signleton.h"

/**
 * @class AsioIOServicePool, 本质上是一个线程池, 基本功能就是根据构造函数传入的数量
 * 创建 n 个线程和 iocontext, 然后每一个线程跑一个 iocontext, 这样就可以并发处理不同的 iocontext
 * 读写事件
 */
class AsioIOServicePool: public  Signleton<AsioIOServicePool>{
    friend Signleton<AsioIOServicePool>;
public:
    using IOService = boost::asio::io_context;
    /**
     * @brief 在实际使用中, 我们通常会将一些异步操作交给 @c io_context 进行处理,
     * 然后该操作会被异步执行, 而不会立即返回结果. 如果没有其他任务需要执行, 那么@c io_context
     * 就会停止工作, 导致所有正在进行的异步操作都会被取消, 这时, 我们需要使用
     * @c boost::asio::io_context::work 对象来防止@c io_context来停止工作
     * @par @c boost::asio::io_context::work 的作用是持有一个指向@c io_context 的引用,
     * 并通过创建一个"工作"项来保证@c io_context 不会停止工作, 直到@c work 对象被销毁或者
     * 调用@c reset() 方法为止, 当所有异步操作完成之后, 程序可以使用@c work.reset() 方法来释放
     * @c io_context, 从而让其正常退出
     */
    using Work = boost::asio::io_context::work;
    using WorkPtr = std::unique_ptr<Work>;
    ~AsioIOServicePool();
    AsioIOServicePool(const AsioIOServicePool&) = delete;
    AsioIOServicePool& operator=(const AsioIOServicePool&) = delete;

    boost::asio::io_context& GetIOService();
    void Stop();
private:
    AsioIOServicePool(std::size_t size = std::thread::hardware_concurrency());
    std::vector<IOService> _ioServices; // 用来存储和初始化多个 IOService
    std::vector<WorkPtr> _works;
    std::vector<std::thread> _threads; // 线程 vector, 用来管理开辟的所有线程
    std::size_t _nextIOService; // 轮询索引, 用最简单的轮询算法为每个新创建的连接分配 io_context
};


#endif //MYJSONSERVER_ASIOIOSERVICEPOOL_H
