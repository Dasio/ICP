#include "Server.h"

Server::Server(int port) : _ioService(),_signals(_ioService),_endpoint(boost::asio::ip::tcp::v4(), port), _acceptor(_ioService,_endpoint), _socket(_ioService)
{
    _signals.add(SIGINT);
    _signals.add(SIGTERM);
    await_stop();
    accept();
}

Server::~Server()
{

}

void Server::run()
{
    _ioService.run();
}

void Server::accept()
{
    _acceptor.async_accept(_socket,
        [this](boost::system::error_code ec)
        {
            if(!_acceptor.is_open())
                return;
            if(!ec)
            {
                std::cout << "Client connected" << std::endl;
                _connectionManager.start(std::make_shared<Connection>(std::move(_socket),_connectionManager));
            }
            // Accept next connection
            accept();
        });
}
void Server::await_stop()
{
    _signals.async_wait(
        [this](boost::system::error_code,int)
        {
            _acceptor.close();
            _connectionManager.stop_all();
            _ioService.stop();
        });
}
