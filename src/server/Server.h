#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include "ConnectionManager.h"

#define DEFAULT_PORT 11111
class Server
{
public:
    Server(int port = DEFAULT_PORT);
    ~Server();

    /**
     * Start io_service
     */
    void run();
private:

    /**
     * Waiting asynchronosuly for signal
     */
    void await_stop();

    /**
     * Starts accepting connections
     */
    void accept();
    boost::asio::io_service _ioService;
    boost::asio::signal_set _signals;
    boost::asio::ip::tcp::endpoint _endpoint;
    boost::asio::ip::tcp::acceptor _acceptor;
    boost::asio::ip::tcp::socket _socket;
    ConnectionManager _connectionManager;
};
#endif /* !SERVER_H */
