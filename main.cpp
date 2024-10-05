#include <iostream>
#include "CServer.h"
#include "Signleton.h"
#include "LogicSystem.h"
#include <csignal>
#include <thread>
#include <mutex>
#include "AsioIoServicePool.h"
using namespace std;
bool stop = false;
std::condition_variable conditionQuit;
std::mutex mutexQuit;
int main()
{
    try{
        auto pool = AsioIOServicePool::GetInstance();

        boost::asio::io_context io_context;
        boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
        signals.async_wait([&io_context, pool](auto, auto){
            io_context.stop();
            pool->Stop();
        });
        CServer s(io_context, 10086);
        io_context.run();

    }catch (std::exception& e){
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}