//
// Created by 尹彦江 on 24-10-3.
//

#include "CServer.h"
#include <iostream>

CServer::CServer(boost::asio::io_context &io_context, short port):
_io_context{io_context}, _port(port),
_acceptor(io_context, tcp::endpoint (tcp::v4(), port)){
    std::cout << "Server start success, listen on port: " << _port << std::endl;
    StartAccept();
}

void CServer::HandleAccept(std::shared_ptr<CSession> newSession, const boost::system::error_code &error) {
    if(!error){
        newSession->Start();
        _sessions.insert(std::make_pair(newSession->getUuid(), newSession));
    }
    else{
        std::cout << "Session accept failed, error is: " << error.what() << std::endl;
    }
    StartAccept();
}

void CServer::StartAccept() {
    std::shared_ptr<CSession> newSession = std::make_shared<CSession>(_io_context, this);
    _acceptor.async_accept(newSession->getSocket(), std::bind(&CServer::HandleAccept,
                                                              this, newSession, std::placeholders::_1));
}

void CServer::ClearSession(std::string uuid) {
    _sessions.erase(uuid);
}