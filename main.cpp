#include <iostream>
#include "CServer.h"
#include "Signleton.h"
#include "LogicSystem.h"
#include <csignal>
#include <thread>
#include <mutex>
using namespace std;
bool stop = false;
std::condition_variable conditionQuit;
std::mutex mutexQuit;
int main()
{
    try{
        boost::asio::io_context io_context;
        boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
        signals.async_wait([&io_context](auto, auto){
            io_context.stop();
        });
        CServer s(io_context, 10086);
        io_context.run();

    }catch (std::exception& e){
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    boost::asio::io_context io_context;
}