//
// Created by 尹彦江 on 24-10-3.
//

#ifndef MYLOGICSERVER_CSERVER_H
#define MYLOGICSERVER_CSERVER_H
#include <boost/asio.hpp>
#include <memory>
#include <map>
#include "CSession.h"

using boost::asio::ip::tcp;

class CServer {
public:
    CServer(boost::asio::io_context& io_context, short port);
    void ClearSession(std::string uuid);

private:
    void HandleAccept(std::shared_ptr<CSession> newSession, const boost::system::error_code& error);
    void StartAccept();
    boost::asio::io_context& _io_context;
    short _port;
    boost::asio::ip::tcp::acceptor _acceptor;
    std::map<std::string, std::shared_ptr<CSession>> _sessions;
};


#endif //MYLOGICSERVER_CSERVER_H
